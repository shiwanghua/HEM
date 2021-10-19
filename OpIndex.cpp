#include "OpIndex.h"

void OpIndex::insert(Sub x)
{
    int att = getMinFre(x);
    isPivot[att] = true;
    for (int i = 0; i < x.size; i++)
    {
        ConElement e;
        e.subID = x.id;
        e.att = x.constraints[i].att;
        e.val = x.constraints[i].value;

        if (x.constraints[i].op == 0)
        {
            data[att][x.constraints[i].op][e.att % SEGMENTS][signatureHash1(e.att, e.val)].push_back(e);
            sig[att][x.constraints[i].op][e.att % SEGMENTS][signatureHash1(e.att, e.val)] = true;
        }
        else
        {
            data[att][x.constraints[i].op][e.att % SEGMENTS][signatureHash2(e.att)].push_back(e);
            sig[att][x.constraints[i].op][e.att % SEGMENTS][signatureHash2(e.att)] = true;
        }
    }
    numSub++;
}

void OpIndex::insert(IntervalSub x)
{
    int att = getMinFre(x);
    isPivot[att] = true;
    for (int i = 0; i < x.size; i++)
    {
        ConElement e;
        e.subID = x.id;
        e.att = x.constraints[i].att;
        e.val = x.constraints[i].lowValue;
        data[att][1][e.att % SEGMENTS][signatureHash2(e.att)].push_back(e);
        sig[att][1][e.att % SEGMENTS][signatureHash2(e.att)] = true;
        e.val = x.constraints[i].highValue;
        data[att][2][e.att % SEGMENTS][signatureHash2(e.att)].push_back(e);
        sig[att][2][e.att % SEGMENTS][signatureHash2(e.att)] = true;
    }
    numSub++;
}

bool OpIndex::deleteSubscription(IntervalSub sub)
{
    int find = 0;
    int id = sub.id;
    int pivotAtt = getMinFre(sub);
    //isPivot[pivotAtt] = false; // 未实现

    for (int i = 0; i < sub.size; i++)
    {
        int att = sub.constraints[i].att;
        int seg = att % SEGMENTS;
        int hash = signatureHash2(att);
        vector<ConElement>::iterator it;
        for (it = data[pivotAtt][1][seg][hash].begin(); it != data[pivotAtt][1][seg][hash].end(); it++)
            if (it->subID == id) {
                data[pivotAtt][1][seg][hash].erase(it); 
                find++;
                break;
            }
        for (it = data[pivotAtt][2][seg][hash].begin(); it != data[pivotAtt][2][seg][hash].end(); it++)
            if (it->subID == id) {
                data[pivotAtt][2][seg][hash].erase(it);
                find++;
                break;
            }
        //sig[pivotAtt][1][seg][hash] = false; // 未实现
        //sig[pivotAtt][2][seg][hash] = false; // 未实现
    }

    if (find == 2 * sub.size)
        numSub--;
    return find == 2 * sub.size;
}

void OpIndex::match(Pub pub, int& matchSubs, const vector<Sub>& subList)
{
    initCounter(subList);
    for (int i = 0; i < pub.size; i++)
    {
        int piv_att = pub.pairs[i].att;

        if (!isPivot[piv_att])
            continue;


        for (int j = 0; j < pub.size; j++)
        {
            int att = pub.pairs[j].att % SEGMENTS, value = pub.pairs[j].value;
            int hashValue = signatureHash1(pub.pairs[j].att, value);
            if (sig[piv_att][0][att][hashValue])
                for (int k = 0; k < data[piv_att][0][att][hashValue].size(); k++)
                {
                    ConElement ce = data[piv_att][0][att][hashValue][k];
                    if (ce.val == value && ce.att == pub.pairs[j].att)
                    {
                        --counter[ce.subID];
                        if (counter[ce.subID] == 0)
                            ++matchSubs;
                    }
                }
            hashValue = signatureHash2(pub.pairs[j].att);
            if (sig[piv_att][1][att][hashValue])
                for (int k = 0; k < data[piv_att][1][att][hashValue].size(); k++)
                {
                    ConElement ce = data[piv_att][1][att][hashValue][k];
                    if (ce.att == pub.pairs[j].att && ce.val <= value)
                    {
                        --counter[ce.subID];
                        if (counter[ce.subID] == 0)
                            ++matchSubs;
                    }
                }
            if (sig[piv_att][2][att][hashValue])
                for (int k = 0; k < data[piv_att][2][att][hashValue].size(); k++)
                {
                    ConElement ce = data[piv_att][2][att][hashValue][k];
                    if (ce.att == pub.pairs[j].att && ce.val >= value)
                    {
                        --counter[ce.subID];
                        if (counter[ce.subID] == 0)
                            ++matchSubs;
                    }
                }
        }
    }
}

void OpIndex::match(Pub pub, int& matchSubs, const vector<IntervalSub>& subList)
{
    initCounter(subList);
    for (int i = 0; i < pub.size; i++)
    {
        int piv_att = pub.pairs[i].att;
        if (!isPivot[piv_att])
            continue;

        for (int j = 0; j < pub.size; j++)
        {
            int att = pub.pairs[j].att % SEGMENTS, value = pub.pairs[j].value;

            int hashValue = signatureHash2(pub.pairs[j].att);
            if (sig[piv_att][1][att][hashValue])
                for (int k = 0; k < data[piv_att][1][att][hashValue].size(); k++)
                {
                    ConElement ce = data[piv_att][1][att][hashValue][k];
                    if (ce.att == pub.pairs[j].att && ce.val <= value)
                    {
                        --counter[ce.subID];
                        if (counter[ce.subID] == 0)
                            ++matchSubs;
                    }
                }
            if (sig[piv_att][2][att][hashValue])
                for (int k = 0; k < data[piv_att][2][att][hashValue].size(); k++)
                {
                    ConElement ce = data[piv_att][2][att][hashValue][k];
                    if (ce.att == pub.pairs[j].att && ce.val >= value)
                    {
                        --counter[ce.subID];
                        if (counter[ce.subID] == 0)
                            ++matchSubs;
                    }
                }
        }
    }
}

void OpIndex::initCounter(const vector<Sub>& subList)
{
    for (int i = 0; i < subList.size(); i++)
        counter[i] = subList[i].size;
}

void OpIndex::initCounter(const vector<IntervalSub>& subList)
{
    for (int i = 0; i < subList.size(); i++)
        counter[i] = subList[i].size << 1;
}

int OpIndex::getMinFre(Sub x)
{
    int tmp = x.constraints.at(0).att;
    for (int i = 1; i < x.size; i++)
        if (fre[x.constraints[i].att] < fre[tmp])
            tmp = x.constraints[i].att;
    return tmp;
}

int OpIndex::getMinFre(IntervalSub x)
{
    int tmp = x.constraints.at(0).att;
    for (int i = 1; i < x.size; i++)
        if (fre[x.constraints[i].att] < fre[tmp])
            tmp = x.constraints[i].att;
    return tmp;
}

void OpIndex::calcFrequency(const vector<Sub>& subList)
{
    memset(fre, 0, sizeof(fre));
    for (int i = 0; i < subList.size(); i++)
        for (int j = 0; j < subList[i].size; j++)
            ++fre[subList[i].constraints[j].att];
}

void OpIndex::calcFrequency(const vector<IntervalSub>& subList)
{
    memset(fre, 0, sizeof(fre));
    for (int i = 0; i < subList.size(); i++)
        for (int j = 0; j < subList[i].size; j++)
            ++fre[subList[i].constraints[j].att];
}

int OpIndex::signatureHash1(int att, int val)
{
    return att * val % MAX_SIGNATURE;
}

int OpIndex::signatureHash2(int att)
{
    return att * att % MAX_SIGNATURE;
}

int OpIndex::calMemory()
{
    long long size = 0; // Byte
    _for(i, 0, atts) {
        _for(j, 0, 3) {
            _for(k, 0, SEGMENTS) {
                _for(q, 0, MAX_SIGNATURE) {
                    size += data[i][j][k][q].size() * sizeof(ConElement) + sizeof(bool); // sig 数组
                }
            }
        }
    }
    size += MAX_SUBS * sizeof(int) + MAX_ATTS * sizeof(bool) + MAX_ATTS * sizeof(int); // counter, isPivot, fre
    size = size / 1024 / 1024; // MB
    return (int)size;
}