#include "HEM3_ASO.h"

HEM3_ASO::HEM3_ASO(int type)
{
    numSub = 0;
    numDimension = atts;
    buckStep = (valDom - 1) / buks + 1;
    numBucket = (valDom - 1) / buckStep + 1;

    numAttrGroup = attrGroup;
    attrGroupSize = (numDimension + numAttrGroup - 1) / numAttrGroup; // 最后一组可能不满
    attrGroupBits.resize(numAttrGroup);

    (be == -1) ? numBits = be2 : numBits = pow(2, be) + 1; // 每个维度上 bits 数组个数

    data[0].resize(numDimension, vector<vector<Combo>>(numBucket));
    data[1].resize(numDimension, vector<vector<Combo>>(numBucket));

    bits.resize(numDimension, vector<bitset<subs>>(numBits));

    endBucket[0] = new int *[numDimension];
    endBucket[1] = new int *[numDimension];
    bitsID = new int *[numDimension];
    _for(i, 0, numDimension)
    {
        endBucket[0][i] = new int[numBucket];
        endBucket[1][i] = new int[numBucket];
        bitsID[i] = new int[numBucket];
    }

    fix[0].resize(numDimension, vector<int>(numBucket + 1));
    fix[1].resize(numDimension, vector<int>(numBucket + 1));
    string TYPE;
    if (type == HEM3_D_VASO)
        TYPE = "HEM3_D_VASO)";
    else if (type == HEM3_D_RASO)
        TYPE = "HEM3_D_RASO";
    else if (type == HEM3_D_RASO_AVXOR_PARALLEL)
    {
        TYPE = "HEM3_D_RASO_AVXOR" + to_string(blockSize) + "_PARALLEL";
        threadPool.initThreadPool(parallelDegree);
    }
    cout << "ExpID = " << expID << ". " + TYPE + ": bitset number = " << numBits << ", bucketStep = " << buckStep
         << ", numBucket = " << numBucket << ", attrGroupNum = " << numAttrGroup << ", attGroupSize = " << attrGroupSize
         << endl;
}

HEM3_ASO::~HEM3_ASO()
{
    _for(i, 0, numDimension)
    {
        delete[] endBucket[0][i], endBucket[1][i], bitsID[i];
    }
    delete[] endBucket[0], endBucket[1], bitsID;
}

void HEM3_ASO::insert_VASO(IntervalSub sub)
{
    Combo c;
    c.subID = sub.id;
    for (auto &&iCnt : sub.constraints)
    {
        attrGroupBits[iCnt.att / attrGroupSize][sub.id] = 1;
        c.val = iCnt.lowValue;
        data[0][iCnt.att][iCnt.lowValue / buckStep].emplace_back(c);
        c.val = iCnt.highValue;
        data[1][iCnt.att][iCnt.highValue / buckStep].emplace_back(c);
    }
    numSub++;
}

void HEM3_ASO::insert_RASO(IntervalSub sub)
{
    for (int attGroupNo = sub.constraints[0].att / attrGroupSize, gi = 0; gi < numAttrGroup; gi++)
    {
        if (gi != attGroupNo)
            attrGroupBits[gi][sub.id] = 1;
    }
    Combo c;
    c.subID = sub.id;
    for (auto &&iCnt : sub.constraints)
    {
        c.val = iCnt.lowValue;
        data[0][iCnt.att][iCnt.lowValue / buckStep].emplace_back(c);
        c.val = iCnt.highValue;
        data[1][iCnt.att][iCnt.highValue / buckStep].emplace_back(c);
    }
    numSub++;
}

// 在线插入，已经构建好索引结构了，插入谓词、标记订阅
void HEM3_ASO::insert_online_VASO(IntervalSub sub)
{
    int b, bucketID;
    Combo c;
    c.subID = sub.id;

    for (auto &&iCnt : sub.constraints)
    {
        bits[iCnt.att][0][sub.id] = 1; // 默认 0 号位集存储所有订阅，即原来的 fullBits
        attrGroupBits[iCnt.att / attrGroupSize][sub.id] = 1;
        b = bitsID[iCnt.att][bucketID]; // = 这个桶所属的组号（从 LVE看）

        bucketID = iCnt.lowValue / buckStep;
        c.val = iCnt.lowValue;
        data[0][iCnt.att][bucketID].emplace_back(c);
        _for(q, b + 1, numBits) bits[iCnt.att][q][sub.id] = 1;

        bucketID = iCnt.highValue / buckStep;
        c.val = iCnt.highValue;
        data[1][iCnt.att][bucketID].emplace_back(c);
        _for(q, 1, b) bits[iCnt.att][q][sub.id] = 1;
    }
    numSub++;
}

void HEM3_ASO::insert_online_RASO(IntervalSub sub)
{
    int b, bucketID;
    Combo c;
    c.subID = sub.id;

    for (int attGroupNo = sub.constraints[0].att / attrGroupSize, i = 0; i < numAttrGroup; i++)
    {
        if (i != attGroupNo)
            attrGroupBits[i][sub.id] = 1;
    }

    for (auto &&iCnt : sub.constraints)
    {
        bits[iCnt.att][0][sub.id] = 1;
        b = bitsID[iCnt.att][bucketID]; // = 这个桶所属的组号（从 LVE看）

        bucketID = iCnt.lowValue / buckStep;
        c.val = iCnt.lowValue;
        data[0][iCnt.att][bucketID].emplace_back(c);
        _for(q, b + 1, numBits) bits[iCnt.att][q][sub.id] = 1;

        bucketID = iCnt.highValue / buckStep;
        c.val = iCnt.highValue;
        data[1][iCnt.att][bucketID].emplace_back(c);
        _for(q, 1, b) bits[iCnt.att][q][sub.id] = 1;
    }
    numSub++;
}

bool HEM3_ASO::deleteSubscription_VASO(IntervalSub sub)
{
    int find = 0, b, bucketID, id = sub.id;
    vector<Combo>::const_iterator it;

    for (auto &&iCnt : sub.constraints)
    {
        bits[iCnt.att][0][sub.id] = 0;
        attrGroupBits[iCnt.att / attrGroupSize][sub.id] = 0;
        b = bitsID[iCnt.att][bucketID];

        bucketID = iCnt.lowValue / buckStep;
        for (it = data[0][iCnt.att][bucketID].cbegin(); it != data[0][iCnt.att][bucketID].cend(); it++)
            if (it->subID == id)
            {
                data[0][iCnt.att][bucketID].erase(it);
                find++;
                break;
            }
        _for(q, b + 1, numBits) bits[iCnt.att][q][sub.id] = 0;

        bucketID = iCnt.highValue / buckStep;
        for (it = data[1][iCnt.att][bucketID].cbegin(); it != data[1][iCnt.att][bucketID].cend(); it++)
            if (it->subID == id)
            {
                data[1][iCnt.att][bucketID].erase(it);
                find++;
                break;
            }
        _for(q, 1, b) bits[iCnt.att][q][sub.id] = 0;
    }
    if (find == sub.size << 1)
    {
        numSub--;
        return true;
    }
    return false;
}

bool HEM3_ASO::deleteSubscription_RASO(IntervalSub sub)
{
    int find = 0, b, bucketID, id = sub.id;

    // int attGroupNo = sub.constraints[0].att / attrGroupSize;
    _for(i, 0, numAttrGroup)
    {
        // if (i != attGroupNo)
        attrGroupBits[i][sub.id] = 0;
    }

    for (auto &&iCnt : sub.constraints)
    {
        bits[iCnt.att][0][sub.id] = 0;
        b = bitsID[iCnt.att][bucketID];

        bucketID = iCnt.lowValue / buckStep;
        vector<Combo>::const_iterator it;
        for (it = data[0][iCnt.att][bucketID].cbegin(); it != data[0][iCnt.att][bucketID].cend(); it++)
            if (it->subID == id)
            {
                data[0][iCnt.att][bucketID].erase(it);
                find++;
                break;
            }
        _for(q, b + 1, numBits) bits[iCnt.att][q][sub.id] = 0;

        bucketID = iCnt.highValue / buckStep;
        for (it = data[1][iCnt.att][bucketID].cbegin(); it != data[1][iCnt.att][bucketID].cend(); it++)
            if (it->subID == id)
            {
                data[1][iCnt.att][bucketID].erase(it);
                find++;
                break;
            }
        _for(q, 1, b) bits[iCnt.att][q][sub.id] = 0;
    }
    if (find == sub.size << 1)
    {
        numSub--;
        return true;
    }
    return false;
}

void HEM3_ASO::initBits()
{
    // 考虑多次初始化
    _for(i, 0, numDimension) delete[] endBucket[0][i], endBucket[1][i], bitsID[i], bitsID[i];
    delete[] endBucket[0], endBucket[1], bitsID;
    endBucket[0] = new int *[numDimension];
    endBucket[1] = new int *[numDimension];
    bitsID = new int *[numDimension];
    _for(i, 0, numDimension)
    {
        endBucket[0][i] = new int[numBucket];
        endBucket[1][i] = new int[numBucket];
        bitsID[i] = new int[numBucket];
    }
    bits[0].clear(), bits[1].clear();
    bits.resize(numDimension, vector<bitset<subs>>(numBits));

    //// 前缀和、后缀和数组, 不包括本身
    //_for(i, 0, numDimension) {
    //	_for(j, 1, numBucket) {
    //		fix[0][i][numBucket - 1 - j] = fix[0][i][numBucket - j] + data[0][i][numBucket - j].size();
    //		fix[1][i][j] = fix[1][i][j - 1] + data[1][i][j - 1].size();
    //	}
    //	// 整个数组的和存在最后一个元素上
    //	fix[0][i][numBucket] = fix[0][i][0] + data[0][i][0].size();
    //	fix[1][i][numBucket] = fix[1][i][numBucket - 1] + data[1][i][numBucket - 1].size(); // Bug: 少了-1!!!
    //}

    // 前缀和数组(不包括本身)、后缀和数组(包括本身)
    _for(i, 0, numDimension)
    {
        fix[0][i][numBucket] = 0; // 后缀和
        fix[0][i][numBucket - 1] = data[0][i][numBucket - 1].size();
        fix[1][i][0] = 0;
        _for(j, 1, numBucket)
        {
            fix[0][i][numBucket - j - 1] = fix[0][i][numBucket - j] + data[0][i][numBucket - j - 1].size();
            fix[1][i][j] = fix[1][i][j - 1] + data[1][i][j - 1].size();
        }
        // 整个数组的和存在最后一个元素上
        // fix[0][i][numBucket] = fix[0][i][0];  // Bug: 导致后面刚开始算映射关系时fix[0][i][lowBktId]不合理
        fix[1][i][numBucket] = fix[1][i][numBucket - 1] + data[1][i][numBucket - 1].size(); // Bug: 少了-1!!!
    }

    // 当前应该映射到的bitId, 桶id, 下一个临界负载点
    int lowBid, highBid, lowBktId, highBktId, lowSubWorkLoad, highSubWorkLoad;
    int subWorkLoadStep; // 每个维度上的subWorkLoadStep都不同, 但同一个维度上的low/high subWorkLoadStep是一样的
    _for(i, 0, numDimension)
    {
        // 基本不会出现
        if (fix[0][i][0] == 0) // 空维度
        {
            _for(j, 0, numBucket)
            {
                bitsID[i][j] = 0;
                endBucket[0][i][j] = j; // 遍历到等于endBucket[0][i][j]-1
                endBucket[1][i][j] = j; // 遍历到大于等于endBucket[1][i][j]
            }
            continue;
        }

        subWorkLoadStep = (fix[0][i][0] + numBits - 1) / numBits; // fix[1][i][numBucket]

        // 由于是low/high都是动态的, 基本不可能共用同一套partition/cell,
        // 但这里low还是从左边开始数一个subWorkLoadStep的量, 保持一致
        // 或者(但是要改成)从右边数 剩余负载量 开始累加subWorkLoadStep, 否则不清楚endBucket!
        // 0号low桶一定可以用到以 (numBits - 2) 为下标的bitset
        // 最后一个桶一定用不到bitset
        // 举例: numBits=15(不是16), fix[0][i][0]=1000000, subWorkLoadStep=66667 (low上的后14个多1, high上的前14个多1)
        // fix[0][i][numBucket] / subWorkLoadStep=14, lowSubWorkLoad=66662
        lowSubWorkLoad = fix[0][i][0] - (fix[0][i][0] - 1) / subWorkLoadStep * subWorkLoadStep;
        highSubWorkLoad = subWorkLoadStep;

        // lowContain[i]=从右数(第一个覆盖)lowSubWorkLoad+(i-1)*subWorkLoadStep个订阅所到的桶号(i>0时)
        vector<int> lowContain(numBits + 1, numBucket);
        // highContain[i]=左数 i*subWorkLoadStep 个订阅所到的桶号
        vector<int> highContain(numBits + 1, 0);
        int li = 1, hi = 1; // lowContain和highContain的下标
        _for(j, 0, numBucket)
        {
            if (fix[1][i][j] >= highSubWorkLoad)
            {                          // fix[1][i][numBucket]才包括全部, 最后不一定能进入if
                highContain[hi++] = j; // numBits=1时highContain[1]<=numBucket-1(右数第一个非空桶位置+1)
                highSubWorkLoad += subWorkLoadStep;
            }
            // 举例: fix[0][i][0]=1M, subWorkLoadStep=100000, numBits=10
            // li,lowSubWorkLoad = 1,100000; 2,200000; ... ; 9,900000; 10,1000000; 11,1100000
            if (fix[0][i][numBucket - j - 1] >= lowSubWorkLoad)
            {                                         // fix[0][i][0]就包括全部, 所以一定进入if
                lowContain[li++] = numBucket - j - 1; // numBits=1时lowContain[1]>=0 (左数第一个非空桶位置)
                lowSubWorkLoad += subWorkLoadStep;
            }
        }
        // lowContain[li] = 0; // 为啥不会越界??? li==numBits+1了
        if (hi == numBits)               // Bug: 最后几个桶为空时hi会在for循环里增加到numBits+1; 最后一个桶非空时highContain[numBits]还没赋值
            highContain[hi] = numBucket; // numBits=1时highContain[1]=numBucket

        li = hi = 1; // 双重反向遍历时所对应的另一端的桶号在contain数组中的下标, 其实 li=lowBid+2, hi=highBid+2
        lowSubWorkLoad = fix[0][i][0] - (fix[0][i][0] - 1) / subWorkLoadStep * subWorkLoadStep;
        highSubWorkLoad = subWorkLoadStep;
        lowBid = -1;
        highBid = -1;
        lowBktId = numBucket;
        highBktId = 0;
        for (int lj = 0, hj = numBucket - 1; lj < numBucket; lj++, hj--)
        {
            // 此时大于等于highSubWorkLoad了, 可以用bits, 因为bits覆盖到j-1桶
            if (fix[1][i][lj] >= highSubWorkLoad)
            { // 第一个大于等于临界点的桶(j-1号, 前缀和不包含本身)作为bitset覆盖的终点桶
                highSubWorkLoad += subWorkLoadStep;
                hi++;
                highBid++;
                highBktId = lj;
            }

            // Bug: 提前满了, 最后几个桶为空, 此时highBid=numBits-1, hi=numBits+1, 越界了, 直接用fullBL
            if (fix[1][i][lj] == fix[1][i][numBucket])
            {
                bitsID[1][i][lj] = numBits - 1;
                endBucket[1][i][lj] = lj; // bkt lj does not need to be marked
                doubleReverse[1][i][lj] = true;
            }
            else if (fix[1][i][lj] - fix[1][i][highBktId] <
                     fix[1][i][highContain[hi]] - fix[1][i][lj + 1])
            {                                    // Bug: 没有减highBktId
                bitsID[1][i][lj] = highBid;      // hi - 2
                endBucket[1][i][lj] = highBktId; // 遍历到大于等于endBucket[1][i][j]
                doubleReverse[1][i][lj] = false;
            }
            else
            {
                bitsID[1][i][lj] = hi - 1;             // highBid+1
                endBucket[1][i][lj] = highContain[hi]; // 从j往右遍历到小于endBucket[1][i][j]
                doubleReverse[1][i][lj] = true;
            }

            // 后缀数组求和时包括本身(如果不包括本身, 则在两个lj、lowBktId和lowContain[li]后再减一，而lowContain[li]有可能为0); -1+1省去了
            // fix[0][i][j][numBucket]需要是0, 使fix[0][i][j][lowBktId]刚开始为0
            // Bug: 提前满了, 序号小的几个桶为空, 单独考虑, 直接用二重反向
            if (fix[0][i][hj] == fix[0][i][0])
            {
                bitsID[0][i][hj] = numBits - 1;
                endBucket[0][i][hj] = hj + 1;
                doubleReverse[0][i][hj] = true;
            }
            else if (fix[0][i][hj + 1] - fix[0][i][lowBktId] <
                     fix[0][i][lowContain[li]] - fix[0][i][hj])
            {
                bitsID[0][i][hj] = lowBid;
                endBucket[0][i][hj] = lowBktId;
                doubleReverse[0][i][hj] = false;
            }
            else
            {
                bitsID[0][i][hj] = li - 1; // lowBid+1
                endBucket[0][i][hj] = lowContain[li];
                doubleReverse[0][i][hj] = true;
            }

            // 此时虽然大于等于lowSubWorkLoad了, 但仍不可以用bits, 因为bits要覆盖到hj号桶
            if (fix[0][i][hj] >= lowSubWorkLoad)
            {
                lowSubWorkLoad += subWorkLoadStep;
                li++;
                lowBid++;
                lowBktId = hj;
            }
        }
    }

    int subID, b; // 起始标记数组的下标
    _for(i, 0, numDimension)
    { // 每个维度
        _for(j, 0, numBucket)
        { // 每个桶
            if (doubleReverse[0][i][j])
                b = bitsID[0][i][j]; // 最小的需要插入的bits数组的ID
            else
                b = bitsID[0][i][j] + 1;
            _for(k, 0, data[0][i][j].size())
            {
                subID = data[0][i][j][k].subID;
                fullBits[i][subID] = 1; // 0号bits每次必须标记
                _for(q, b, numBits - 1) // Bug: bits都是是从高位(覆盖广)往低位遍历！
                    bits[0][i][q][subID] = 1;
            }

            if (doubleReverse[1][i][j])
                b = bitsID[1][i][j];
            else
                b = bitsID[1][i][j] + 1; // 最小的需要插入的bits数组的ID
            _for(k, 0, data[1][i][j].size())
            { // 桶里每个订阅
                subID = data[1][i][j][k].subID;
                _for(q, b, numBits - 1) bits[1][i][q][subID] = 1;
            }
        }
    }
    // cout << "HEM5_AGDD Stop.\n";
}

void HEM3_ASO::match_VASO(const Pub &pub, int &matchSubs)
{
    bitset<subs> b; // register
    bitset<subs> bLocal;
    vector<bool> attExist(numDimension, false);
    vector<bool> attGroupExist(numAttrGroup, false);
    int value, att, buck;

    _for(i, 0, pub.size)
    {

        value = pub.pairs[i].value, att = pub.pairs[i].att, buck = value / buckStep;
        attExist[att] = true;
        attGroupExist[att / attrGroupSize] = true;

        if (doubleReverse[0][att][buck])
        {
#ifdef DEBUG
            Timer markStart;
#endif                                               // DEBUG
            if (bitsID[0][att][buck] == numBits - 1) // 只有1个bitset时建到fullBits上，去掉: && numBits > 1
                bLocal = fullBits[att];
            else
                bLocal = bits[0][att][bitsID[0][att][buck]];
            _for(j, endBucket[0][att][buck], buck) for (auto &&iCob : data[0][att][j])
                bLocal[iCob.subID] = 0;
#ifdef DEBUG
            markTime += (double)markStart.elapsed_nano();
            Timer compareStart;
#endif // DEBUG
            for (auto &&iCob : data[0][att][buck])
                if (iCob.val <= value)
                    bLocal[iCob.subID] = 0;
#ifdef DEBUG
            compareTime += (double)compareStart.elapsed_nano();
            Timer orStart;
#endif // DEBUG
            b = b | bLocal;
#ifdef DEBUG
            orTime += (double)orStart.elapsed_nano();
#endif // DEBUG
        }
        else
        {
#ifdef DEBUG
            Timer markStart;
#endif // DEBUG
            _for(j, buck + 1, endBucket[0][att][buck]) for (auto &&iCob : data[0][att][j])
                b[iCob.subID] = 1;
#ifdef DEBUG
            markTime += (double)markStart.elapsed_nano();
            Timer compareStart;
#endif // DEBUG
            for (auto &&iCob : data[0][att][buck])
                if (iCob.val > value)
                    b[iCob.subID] = 1;
#ifdef DEBUG
            compareTime += (double)compareStart.elapsed_nano();
            Timer orStart;
#endif // DEBUG
            if (bitsID[0][att][buck] != -1)
                b = b | bits[0][att][bitsID[0][att][buck]];
#ifdef DEBUG
            orTime += (double)orStart.elapsed_nano();
#endif // DEBUG
        }

        if (doubleReverse[1][att][buck])
        {
#ifdef DEBUG
            Timer markStart;
#endif                                               // DEBUG
            if (bitsID[1][att][buck] == numBits - 1) // 只有1个bitset时建到fullBits上，去掉: && numBits > 1
                bLocal = fullBits[att];
            else
                bLocal = bits[1][att][bitsID[1][att][buck]];
            _for(j, buck + 1, endBucket[1][att][buck]) for (auto &&iCob : data[1][att][j])
                bLocal[iCob.subID] = 0;
#ifdef DEBUG
            markTime += (double)markStart.elapsed_nano();
            Timer compareStart;
#endif // DEBUG
            for (auto &&iCob : data[1][att][buck])
                if (iCob.val >= value)
                    bLocal[iCob.subID] = 0;
#ifdef DEBUG
            compareTime += (double)compareStart.elapsed_nano();
            Timer orStart;
#endif // DEBUG
            b = b | bLocal;
#ifdef DEBUG
            orTime += (double)orStart.elapsed_nano();
#endif // DEBUG
        }
        else
        {
#ifdef DEBUG
            Timer markStart;
#endif // DEBUG
            _for(j, endBucket[1][att][buck], buck) for (auto &&iCob : data[1][att][j])
                b[iCob.subID] = 1;
#ifdef DEBUG
            markTime += (double)markStart.elapsed_nano();
            Timer compareStart;
#endif // DEBUG
            for (auto &&iCob : data[1][att][buck])
                if (iCob.val < value)
                    b[iCob.subID] = 1;
#ifdef DEBUG
            compareTime += (double)compareStart.elapsed_nano();
            Timer orStart;
#endif // DEBUG
            if (bitsID[1][att][buck] != -1)
                b = b | bits[1][att][bitsID[1][att][buck]]; // Bug: 是att不是i
#ifdef DEBUG
            orTime += (double)orStart.elapsed_nano();
#endif // DEBUG
        }
    }

    // 处理空维度情况
    /*if (numBits > 1) // 如果只有一个bitset时bitset是设置为负责一半的桶而不是全部桶就要用if区分
    {*/
    /*_for(i, 0, numDimension) if (!attExist[i])
        b = b | fullBits[i];*/
    /*}
    else
    {
        _for(i, 0, numDimension) if (!attExist[i])
            _for(j, 0, endBucket[0][i][0])
            _for(k, 0, data[0][i][j].size())
            b[data[0][i][j][k].subID] = 1;

        _for(i, 0, numDimension) if (!attExist[i])
            b = b | bits[0][i][0];
    }*/
#ifdef DEBUG
    Timer orStart;
#endif // DEBUG
    for (int AGi = 0, base; AGi < numAttrGroup; AGi++)
    {
        if (attGroupExist[AGi])
        {
            base = AGi * attrGroupSize;
            _for(aj, base, base + attrGroupSize)
            {
                if (!attExist[aj])
                    b = b | fullBits[aj];
            }
        }
        else
        {
            b = b | attrGroupBits[AGi];
        }
    }
#ifdef DEBUG
    orTime += (double)orStart.elapsed_nano();
    Timer bitStart;
#endif // DEBUG
    //	_for(i, 0, subs) if (!b[i]) {
    //			++matchSubs;
    //			//cout << "HEM5_VAG matches sub: " << i << endl;
    //		}
    matchSubs = subs - b.count();
#ifdef DEBUG
    bitTime += (double)bitStart.elapsed_nano();
#endif // DEBUG
}

void HEM3_ASO::match_RASO(const Pub &pub, int &matchSubs)
{
    bitset<subs> b; // register
    bitset<subs> bLocal;
    vector<bool> attExist(numDimension, false);
    int value, att, buck;

    _for(i, 0, pub.size)
    {

        value = pub.pairs[i].value, att = pub.pairs[i].att, buck = value / buckStep;
        attExist[att] = true;

        if (doubleReverse[0][att][buck])
        {
#ifdef DEBUG
            Timer markStart;
#endif                                               // DEBUG
            if (bitsID[0][att][buck] == numBits - 1) // 只有1个bitset时建到fullBits上，去掉: && numBits > 1
                bLocal = fullBits[att];
            else
                bLocal = bits[0][att][bitsID[0][att][buck]];
            _for(j, endBucket[0][att][buck], buck) for (auto &&iCob : data[0][att][j])
                bLocal[iCob.subID] = 0;
#ifdef DEBUG
            markTime += (double)markStart.elapsed_nano();
            Timer compareStart;
#endif // DEBUG
            for (auto &&iCob : data[0][att][buck])
                if (iCob.val <= value)
                    bLocal[iCob.subID] = 0;
#ifdef DEBUG
            compareTime += (double)compareStart.elapsed_nano();
            Timer orStart;
#endif // DEBUG
            b = b | bLocal;
#ifdef DEBUG
            orTime += (double)orStart.elapsed_nano();
#endif // DEBUG
        }
        else
        {
#ifdef DEBUG
            Timer markStart;
#endif // DEBUG
            _for(j, buck + 1, endBucket[0][att][buck]) for (auto &&iCob : data[0][att][j])
                b[iCob.subID] = 1;
#ifdef DEBUG
            markTime += (double)markStart.elapsed_nano();
            Timer compareStart;
#endif // DEBUG
            for (auto &&iCob : data[0][att][buck])
                if (iCob.val > value)
                    b[iCob.subID] = 1;
#ifdef DEBUG
            compareTime += (double)compareStart.elapsed_nano();
            Timer orStart;
#endif // DEBUG
            if (bitsID[0][att][buck] != -1)
                b = b | bits[0][att][bitsID[0][att][buck]];
#ifdef DEBUG
            orTime += (double)orStart.elapsed_nano();
#endif // DEBUG
        }

        if (doubleReverse[1][att][buck])
        {
#ifdef DEBUG
            Timer markStart;
#endif                                               // DEBUG
            if (bitsID[1][att][buck] == numBits - 1) // 只有1个bitset时建到fullBits上，去掉: && numBits > 1
                bLocal = fullBits[att];
            else
                bLocal = bits[1][att][bitsID[1][att][buck]];
            _for(j, buck + 1, endBucket[1][att][buck]) for (auto &&iCob : data[1][att][j])
                bLocal[iCob.subID] = 0;
#ifdef DEBUG
            markTime += (double)markStart.elapsed_nano();
            Timer compareStart;
#endif // DEBUG
            for (auto &&iCob : data[1][att][buck])
                if (iCob.val >= value)
                    bLocal[iCob.subID] = 0;
#ifdef DEBUG
            compareTime += (double)compareStart.elapsed_nano();
            Timer orStart;
#endif // DEBUG
            b = b | bLocal;
#ifdef DEBUG
            orTime += (double)orStart.elapsed_nano();
#endif // DEBUG
        }
        else
        {
#ifdef DEBUG
            Timer markStart;
#endif // DEBUG
            _for(j, endBucket[1][att][buck], buck) for (auto &&iCob : data[1][att][j])
                b[iCob.subID] = 1;
#ifdef DEBUG
            markTime += (double)markStart.elapsed_nano();
            Timer compareStart;
#endif // DEBUG
            for (auto &&iCob : data[1][att][buck])
                if (iCob.val < value)
                    b[iCob.subID] = 1;
#ifdef DEBUG
            compareTime += (double)compareStart.elapsed_nano();
            Timer orStart;
#endif // DEBUG
            if (bitsID[1][att][buck] != -1)
                b = b | bits[1][att][bitsID[1][att][buck]]; // Bug: 是att不是i
#ifdef DEBUG
            orTime += (double)orStart.elapsed_nano();
#endif // DEBUG
        }
    }

    // 处理空维度情况
    /*if (numBits > 1) // 如果只有一个bitset时bitset是设置为负责一半的桶而不是全部桶就要用if区分
    {*/
    /*_for(i, 0, numDimension) if (!attExist[i])
        b = b | fullBits[i];*/
    /*}
    else
    {
        _for(i, 0, numDimension) if (!attExist[i])
            _for(j, 0, endBucket[0][i][0])
            _for(k, 0, data[0][i][j].size())
            b[data[0][i][j][k].subID] = 1;

        _for(i, 0, numDimension) if (!attExist[i])
            b = b | bits[0][i][0];
    }*/
#ifdef DEBUG
    Timer orStart;
#endif // DEBUG
    int attGroupNo = att / attrGroupSize;
    b = b | attrGroupBits[attGroupNo];
    int base = attGroupNo * attrGroupSize;
    _for(ai, base, base + attrGroupSize)
    {
        if (!attExist[ai])
            b = b | fullBits[ai];
    }
#ifdef DEBUG
    orTime += (double)orStart.elapsed_nano();
    Timer bitStart;
#endif // DEBUG
    //	_for(i, 0, subs) if (!b[i]) {
    //			++matchSubs;
    //			//cout << "HEM5_VAG matches sub: " << i << endl;
    //		}
    matchSubs = subs - b.count();
#ifdef DEBUG
    bitTime += (double)bitStart.elapsed_nano();
#endif // DEBUG
}

void HEM3_ASO::match_RASO_avxOR_parallel(const Pub &pub, int &matchSubs)
{
    vector<future<bitset<subs>>> threadResult;
    int seg = pub.size / parallelDegree;
    int remainder = pub.size % parallelDegree;
    int tId = 0, end;
    for (int begin = 0; begin < pub.size; begin = end, tId++)
    {
        if (tId < remainder)
            end = begin + seg + 1;
        else
            end = begin + seg;
        threadResult.emplace_back(threadPool.enqueue([this, &pub, begin, end]
                                                     {
// 局部变量存栈里
			bitset<subs> b; // register
			bitset<subs> bLocal;
			
			int value, att, buck;
			for (int i = begin; i < end; i++) {
				value = pub.pairs[i].value, att = pub.pairs[i].att, buck = value / buckStep;

				if (doubleReverse[0][att][buck]) {
					if (bitsID[0][att][buck] == numBits - 1) // 只有1个bitset时建到fullBits上，去掉: && numBits > 1
						bLocal = fullBits[att];
					else
						bLocal = bits[0][att][bitsID[0][att][buck]];
					_for(j, endBucket[0][att][buck], buck)
					for (auto &&iCob: data[0][att][j])
							bLocal[iCob.subID] = 0;
					_for(k, 0, data[0][att][buck].size()) if (data[0][att][buck][k].val <= value)
							bLocal[data[0][att][buck][k].subID] = 0;
					Util::bitsetOr(b, bLocal);
				} else {
					_for(j, buck + 1, endBucket[0][att][buck]) 
					for (auto &&iCob: data[0][att][j])
							b[iCob.subID] = 1;
					_for(k, 0, data[0][att][buck].size()) if (data[0][att][buck][k].val > value)
							b[data[0][att][buck][k].subID] = 1;
					if (bitsID[0][att][buck] != -1)
						Util::bitsetOr(b,bits[0][att][bitsID[0][att][buck]]);
				}

				if (doubleReverse[1][att][buck]) {
					if (bitsID[1][att][buck] == numBits - 1) // 只有1个bitset时建到fullBits上，去掉: && numBits > 1
						bLocal = fullBits[att];
					else
						bLocal = bits[1][att][bitsID[1][att][buck]];
					_for(j, buck+1, endBucket[1][att][buck])
					for (auto &&iCob: data[1][att][j])
							bLocal[iCob.subID] = 0;
					_for(k, 0, data[1][att][buck].size()) if (data[1][att][buck][k].val >= value)
							bLocal[data[1][att][buck][k].subID] = 0;
					Util::bitsetOr(b, bLocal);
				} else {
					_for(j, endBucket[1][att][buck], buck) 
					for (auto &&iCob: data[1][att][j])
							b[iCob.subID] = 1;
					_for(k, 0, data[1][att][buck].size()) if (data[1][att][buck][k].val < value)
							b[data[1][att][buck][k].subID] = 1;
					if (bitsID[1][att][buck] != -1)
						Util::bitsetOr(b, bits[1][att][bitsID[1][att][buck]]);//b = b | bits[1][att][bitsID[1][att][buck]]; // Bug: 是att不是i
				}
			}
			return b; }));
    }

    // 处理空维度情况
    /*if (numBits > 1) // 如果只有一个bitset时bitset是设置为负责一半的桶而不是全部桶就要用if区分
    {*/
    /*_for(i, 0, numDimension) if (!attExist[i])
        b = b | fullBits[i];*/
    /*}
    else
    {
        _for(i, 0, numDimension) if (!attExist[i])
            _for(j, 0, endBucket[0][i][0])
            _for(k, 0, data[0][i][j].size())
            b[data[0][i][j][k].subID] = 1;

        _for(i, 0, numDimension) if (!attExist[i])
            b = b | bits[0][i][0];
    }*/
    int attGroupNo = pub.pairs[0].att / attrGroupSize;
    bitset<subs> gb = attrGroupBits[attGroupNo];
    if (pub.size < attrGroupSize)
    {
        vector<bool> attExist(numDimension, false);
        for (auto &item : pub.pairs)
            attExist[item.att] = true;
        int base = attGroupNo * attrGroupSize;
        _for(ai, base, base + attrGroupSize)
        {
            if (!attExist[ai])
                gb = gb | fullBits[ai];
        }
    }
#ifdef DEBUG
    Timer mergeStart;
#endif // DEBUG
    for (int i = 0; i < threadResult.size(); i++)
        Util::bitsetOr(gb, threadResult[i].get());
#ifdef DEBUG
    mergeTime += (double)mergeStart.elapsed_nano();
    Timer bitStart;
#endif // DEBUG
    //	_for(i, 0, subs) if (!b[i]) {
    //			++matchSubs;
    //			//cout << "HEM5_VAG matches sub: " << i << endl;
    //		}
    matchSubs = subs - gb.count();
#ifdef DEBUG
    bitTime += (double)bitStart.elapsed_nano();
#endif // DEBUG
}

int HEM3_ASO::calMemory()
{
    long long size = 0; // Byte
    size += sizeof(bits) + sizeof(bits[0]) * 2 + sizeof(data) + sizeof(data[0]) + sizeof(data[1]);
    // cout << sizeof(bits[0]) << " " << sizeof(bits[1]) <<" " << sizeof(data) << " " << sizeof(data[0]) << " " << sizeof(data[1]) << "\n";
    _for(i, 0, numDimension)
    {
        // 若每个维度上bits数组个数一样就是 2*sizeof(bitset<subs>)*numDimension*numBits
        size += sizeof(bitset<subs>) * (bits[0][i].size() + bits[1][i].size());
        size += (sizeof(bits[0][i]) + sizeof(data[0][i])) * 2;
        // cout << i << ": " << sizeof(bits[0][i]) << " " << sizeof(data[0][i]) << " ";
        _for(j, 0, numBucket)
        {
            // cout << sizeof(data[0][i][j]) << " " << sizeof(data[1][i][j]) << " ";
            size += sizeof(data[0][i][j]) + sizeof(data[1][i][j]);
            size += sizeof(Combo) * (data[0][i][j].size() + data[1][i][j].size());
        }
        // cout << "\n";
    }

    // fullBits
    size += sizeof(bitset<subs>) * fullBits.size(); // fullBits.size()即numDimension
    size += sizeof(fullBits);                       // 24
    // cout << "fullBits: " << sizeof(fullBits) << " " << sizeof(fullBits[0]) << "\n";

    // attrGroupBits
    size += sizeof(bitset<subs>) * attrGroupBits.size();

    // 两个fix
    // cout << "fix: " << sizeof(fix) << " " << sizeof(fix[0]) << " " << sizeof(fix[0][10]) << sizeof(fix[1][7][20]) << "\n";
    size += sizeof(fix) + sizeof(fix[0]) * 2 + sizeof(fix[0][0]) * numDimension +
            sizeof(fix[0][0][0]) * numDimension * (numBucket + 1) * 2;

    // 两个endBucket、两个bitsID、两个doubleReverse
    size += (4 * sizeof(int) + 2 * sizeof(bool)) * numDimension * numBucket;
    size += sizeof(endBucket[0]) * 4 + sizeof(endBucket[0][0]) * numDimension * 4;
    // cout << sizeof(endBucket) << " " << sizeof(endBucket[0]) << " " << sizeof(endBucket[0][0]) << " " << sizeof(endBucket[0][0][0]) << "\n";
    size = size / 1024 / 1024; // MB
    return (int)size;
}

void HEM3_ASO::printRelation(int dimension_i)
{
    cout << "\n\nHEM5_AGDDMap\n";
    if (dimension_i == -1)
        _for(i, 0, numDimension)
        {
            cout << "\nDimension " << i << "    LowBucket Predicates: " << fix[0][i][0] << "   ----------------\n";
            _for(j, 0, numBucket)
            {
                cout << "lBkt" << j << ": bID=" << bitsID[0][i][j] << ", eBkt=" << endBucket[0][i][j] << ", dRvs="
                     << doubleReverse[0][i][j] << "; ";
                if (j % 5 == 0 && j > 0)
                    cout << "\n";
            }
            cout << "\n\nDimension " << i << "    HighBucket Predicates: " << fix[1][i][numBucket]
                 << "   ----------------\n";
            _for(j, 0, numBucket)
            {
                cout << "hBkt" << j << ": bID=" << bitsID[1][i][j] << ", eBkt=" << endBucket[1][i][j] << ", dRvs="
                     << doubleReverse[1][i][j] << "; ";
                if (j % 5 == 0 && j > 0)
                    cout << "\n";
            }
        }
    else
    {
        cout << "\nDimension: " << dimension_i << "    LowBucket Predicates: " << fix[0][dimension_i][0]
             << "   ----------------\n";
        _for(i, 0, numBucket)
        {
            cout << "lBkt" << i << ": bID=" << bitsID[0][dimension_i][i] << ", eBkt=" << endBucket[0][dimension_i][i]
                 << ", dRvs=" << doubleReverse[0][dimension_i][i] << "; ";
            if (i % 5 == 0 && i > 0)
                cout << "\n";
        }
        cout << "\n\nDimension: " << dimension_i << "    HighBucket Predicates: " << fix[1][dimension_i][numBucket]
             << "   ----------------\n";
        _for(i, 0, numBucket)
        {
            cout << "hBkt" << i << ": bID=" << bitsID[1][dimension_i][i] << ", eBkt=" << endBucket[1][dimension_i][i]
                 << ", dRvs=" << doubleReverse[1][dimension_i][i] << "; ";
            if (i % 5 == 0 && i > 0)
                cout << "\n";
        }
    }
    cout << "\n\n";
}

vector<int> HEM3_ASO::calMarkNumForBuckets()
{
    vector<int> numMarking(numBucket, 0);
    _for(i, 0, numBucket)
    {
        _for(j, 0, numDimension)
        {
            numMarking[i] += data[0][j][i].size() + data[1][j][i].size(); // 比较

            if (doubleReverse[0][j][i])
            {
                _for(k, endBucket[0][j][i], i + 1) numMarking[i] += data[0][j][k].size();
            }
            else
            {
                _for(k, i + 1, endBucket[0][j][i]) numMarking[i] += data[0][j][k].size();
            }

            if (doubleReverse[1][j][i])
            {
                _for(k, i, endBucket[1][j][i]) numMarking[i] += data[0][j][k].size();
            }
            else
            {
                _for(k, endBucket[1][j][i], i) numMarking[i] += data[1][j][k].size();
            }
        }
    }
    return numMarking;
}