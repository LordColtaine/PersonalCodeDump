// STL Datastructures.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<set>
#include <unordered_set>
#include<map>
#include<unordered_map>
using namespace std;


struct Data {
    int x;
    Data()
    {
        ;
    }
    Data(int ele)
    {
        x = ele;
    }
    bool operator<(const Data& d) const
    {
        return x < d.x;
    }
    bool operator ==(const Data& d)const
    {
        return x == d.x;
    }
};
ostream& operator<<(ostream& os, const Data& d)
{
    cout << d.x;
    return os;
}
//bool operator<(const Data& d1, const Data& d2)
//{
//    return d1.x < d2.x;
//}
namespace std {
    template<>
    struct hash<Data>
    {
        size_t operator()(const Data& data) const
        {
            return data.x % 20;
        }
    };
}
int main()
{
    /*set<Data> Set;
    for (int i = 0; i < 5; i++)
        Set.insert(Data(10 - i + 1));
    Set.insert(Data(10));
    for (auto i : Set)
        cout << i << " ";
    cout << endl;
    if (Set.find(Data(10)) == Set.end())cout << "Elememt absent\n";
    else cout << "element present\n";*/

    /*multiset<Data> ms;
    for (int i = 0; i < 5; i++)
        ms.insert(Data(10 - i + 1));
    ms.insert(Data(10));
    for (auto i : ms)
        cout << i << " ";
    cout << endl;
    if (ms.find(Data(10)) == ms.end())cout << "Elememt absent\n";
    else cout << "element present\n";*/

    unordered_set<Data> us;
    for (int i = 0; i < 5; i++)
        us.insert(Data(10 - i + 1));
    us.insert(Data(10));
    for (auto i : us)
        cout << i << " ";
    cout << endl;
    if (us.find(Data(10)) == us.end())cout << "Elememt absent\n";
    else cout << "element present\n";

    /*map<Data, int> us;
    for (int i = 0; i < 5; i++)
        us.insert(pair<Data, int>(Data(10 - i + 1), i));
    us.insert(pair<Data, int>(Data(10), 10));
    for (auto i : us)
        cout << i.first << " "<<i.second<<endl;
    cout << endl;
    if (us.find(Data(10)) == us.end())cout << "Elememt absent\n";
    else cout << "element present\n";

    cout << us[(Data(7))] << endl;*/

    //multimap<Data, int> us;
    //for (int i = 0; i < 5; i++)
    //    us.insert(pair<Data, int>(Data(10 - i + 1), i));
    //us.insert(pair<Data, int>(Data(10), 10));
    //for (auto i : us)
    //    cout << i.first << " " << i.second << endl;
    //cout << endl;
    //
    ////[] is not possible

    //if (us.find(Data(10)) == us.end())cout << "Elememt absent\n";
    //else cout << "element present\n";

    //unordered_map<Data, int> us;
    //for (int i = 0; i < 5; i++)
    //    us.insert(pair<Data, int>(Data(10 - i + 1), i));
    //us.insert(pair<Data, int>(Data(10), 10));
    //for (auto i : us)
    //    cout << i.first << " " << i.second << endl;
    //cout << endl;

    ////[] is not possible

    //if (us.find(Data(10)) == us.end())cout << "Elememt absent\n";
    //else cout << "element present\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
