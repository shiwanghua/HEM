#include "AdaRein.h"


void AdaRein::insert(IntervalSub sub)
{
    for (int i = 0; i < sub.size; i++)
    {
        IntervalCnt cnt = sub.constraints[i];
        Combo c;
        c.val = cnt.lowValue;
        c.subID = sub.id;
        data[cnt.att][0][c.val / buckStep].push_back(c);
        c.val = cnt.highValue;
        data[cnt.att][1][c.val / buckStep].push_back(c);
    }
    numSub++;
}

bool AdaRein::deleteSubscription(IntervalSub sub)
{
    bool find = false;
    IntervalCnt cnt;
    int bucketID, id = sub.id;

    _for(i, 0, sub.size)
    {
        cnt = sub.constraints[i];

        bucketID = cnt.lowValue / buckStep;
        vector<Combo>::iterator it;
        for (it = data[0][cnt.att][bucketID].begin(); it != data[0][cnt.att][bucketID].end(); it++)
            if (it->subID == id) {
                data[0][cnt.att][bucketID].erase(it); // it = 
                find = true;
                break;
            }

        bucketID = cnt.highValue / buckStep;
        for (it = data[1][cnt.att][bucketID].begin(); it != data[1][cnt.att][bucketID].end(); it++)
            if (it->subID == id) {
                data[1][cnt.att][bucketID].erase(it); // it = 
                break;
            }
    }
    if (find)
        numSub--;
    return find;
}

void AdaRein::select_skipped_atts(const vector<IntervalSub>& subList, double falsePositive)
{
    for (int i = 0; i < atts; i++)
    {
        attsCounts[i].att = i;
        attsCounts[i].count = 0;
    }
    int countSum = 0, currentSum = 0;
    for (int i = 0; i < subList.size(); i++)
        for (int j = 0; j < subList[i].size; j++)
        {
            ++attsCounts[subList[i].constraints[j].att].count;
            ++countSum;
        }

    // faster version, need to be verified.
    /*_for(i, 0, atts) {
        _for(j, 0, bucks) 
            attsCounts[i].count += data[i][0][j].size();
        countSum = attsCounts[i].count;
    }*/

    sort(attsCounts, attsCounts + atts);

    memset(skipped, 0, sizeof(skipped));
    cout << "Skip attribute: ";
    for (int i = 0; i < atts; i++)
    {
        currentSum += attsCounts[i].count;
        // µÈÐ§°æ±¾:
        // if((double)(countSum - currentSum)/(double)numSub > cons+ log(falsePositive + 1)/log(w))
        if ((double)(countSum - currentSum) / (double)subList.size() > subList[0].constraints.size() + log(falsePositive + 1) / log((subList[0].constraints[0].highValue - subList[0].constraints[0].lowValue) / (double)valDom)) {
            skipped[attsCounts[i].att] = true;
            cout << " " << attsCounts[i].att;
        }
        else
            break;
    }
    cout << endl;
}

void AdaRein::accurate_match(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList)
{
    memset(bits, 0, sizeof(bits));
    vector<bool> attExist(atts, false);

    for (int i = 0; i < pub.size; i++)
    {
        int value = pub.pairs[i].value, att = pub.pairs[i].att, buck = value / buckStep;
        attExist[att] = true;
        for (int k = 0; k < data[att][0][buck].size(); k++)
            if (data[att][0][buck][k].val > value)
                bits[data[att][0][buck][k].subID] = true;
        for (int j = buck + 1; j < numBucket; j++)
            for (int k = 0; k < data[att][0][j].size(); k++)
                bits[data[att][0][j][k].subID] = true;

        for (int k = 0; k < data[att][1][buck].size(); k++)
            if (data[att][1][buck][k].val < value)
                bits[data[att][1][buck][k].subID] = true;
        for (int j = buck - 1; j >= 0; j--)
            for (int k = 0; k < data[att][1][j].size(); k++)
                bits[data[att][1][j][k].subID] = true;
    }

    for (int i = 0; i < atts; i++)
        if (!attExist[i])
            for (int j = 0; j < numBucket; j++)
                for (int k = 0; k < data[0][i][j].size(); k++)
                    bits[data[i][0][j][k].subID] = true;

    for (int i = 0; i < subList.size(); i++)
        if (!bits[i])
            ++matchSubs;
}

void AdaRein::approx_match(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList)
{
    memset(bits, 0, sizeof(bits));
    vector<bool> attExist(atts, false);
    for (int i = 0; i < pub.size; i++)
    {
        int att = pub.pairs[i].att;
        attExist[att] = true;
        if (skipped[att])
            continue;
        int value = pub.pairs[i].value, buck = value / buckStep;
        for (int k = 0; k < data[att][0][buck].size(); k++)
            if (data[att][0][buck][k].val > value)
                bits[data[att][0][buck][k].subID] = true;
        for (int j = buck + 1; j < numBucket; j++)
            for (int k = 0; k < data[att][0][j].size(); k++)
                bits[data[att][0][j][k].subID] = true;

        for (int k = 0; k < data[att][1][buck].size(); k++)
            if (data[att][1][buck][k].val < value)
                bits[data[att][1][buck][k].subID] = true;
        for (int j = buck - 1; j >= 0; j--)
            for (int k = 0; k < data[att][1][j].size(); k++)
                bits[data[att][1][j][k].subID] = true;
    }

    for (int i = 0; i < atts; i++)
        if (!attExist[i]&& !skipped[i])
            for (int j = 0; j < numBucket; j++)
                for (int k = 0; k < data[0][i][j].size(); k++)
                    bits[data[i][0][j][k].subID] = true;

    for (int i = 0; i < subList.size(); i++)
        if (!bits[i])
            ++matchSubs;
}

int AdaRein::calMemory() {
    long long  size = 0; // Byte
    _for(i, 0, atts)
        _for(j, 0, numBucket)
        size += sizeof(Combo) * (data[i][0][j].size() + data[i][1][j].size());
    size += sizeof(bool) * atts + sizeof(attAndCount) * atts;
    //cout << "attAndCount size = " << sizeof(attAndCount) << endl; // 8
    size = size / 1024 / 1024; // MB
    return (int)size;
}