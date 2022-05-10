#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <numeric>
#include <random>
#include <cmath>
#include <queue>
using namespace std;
int search1(string start, string end)
{
    int count = 0;
    for (int i = 0; i < start.length(); i++)
    {
        if (start[i] != end[i])
            count++;
    }
    if (count == 1)
        return 1;
    return 0;
}
int minMutation(string start, string end, vector<string> &bank)
{
    queue<string> q;
    q.push(start);
    int count = 0;
    while (!q.empty())
    {
        int size = q.size();
        for (vector<string>::iterator it = bank.begin(); it < bank.end(); it++)
            cout<<*it<<" ";
        cout<<endl;
        while (size--)
        {
            string cur = q.front();
            q.pop();
            if (cur == end)
                return count;
            vector<string>::iterator it = bank.begin();
            for (; it !=bank.end(); it++)
            {
                cout<<*it<<endl;
                if (search1(cur, *it))
                {
                    q.push(*it);
                    bank.erase(it);
                    it--;
                }
            }
            cout<<"//"<<endl;
        }
        count++;
    }
    return -1;
}

int main()
{
    vector<string> bank;
    //cout<<search1("AACCGGTT","AACCGGTT")<<endl;
    string start="AACCGGTT";
    string end="AAACGGTA";
    bank.push_back("AACCGGTA");
    bank.push_back("AACCGCTA");
    bank.push_back(end);
    cout<<minMutation(start,end,bank)<<endl;
    system("pause");
    return 0;
}