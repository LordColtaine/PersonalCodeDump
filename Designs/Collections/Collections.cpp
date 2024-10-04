// Collections.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<vector>
using namespace std;
struct Ishape
{
    virtual float area() = 0;
};

struct SQ :Ishape
{
    int s;
    SQ(int i)
    {
        s = i;
    }
    float area()
    {
        return s * s;
    }
};

struct CI :Ishape
{
    int r;
    CI(int i)
    {
        r = i;
    }
    float area()
    {
        return 22 * r * r/7;
    }
};

struct TR :Ishape
{
    int b, h;
    TR(int i, int j)
    {
        b = i;
        h = j;
    }
    float area()
    {
        return b * h / 2;
    }
};

struct Collection :Ishape
{
    vector<Ishape*> v;
    void addtocollection(Ishape* shape)
    {
        v.push_back(shape);
    }
    void RemoveFromcollection(Ishape* shape)
    {
        for (int i=0;i<v.size();i++)
        {
            if (shape == v[i])
            {
                v.erase(v.begin()+i);
            }
        }
    }

    float area()
    {
        float tot = 0;
        for (auto e : v)
        {
            tot += e->area();
        }
        return tot;
    }
};
int main()
{
    TR* tr1 = new TR(5, 4);
    CI* ci1 = new CI(7);
    SQ* sq1 = new SQ(4);
    Collection* c =new Collection;
    c->addtocollection(tr1);
    c->addtocollection(ci1);
    c->addtocollection(sq1);
    float a = c->area();
    cout << a << endl;
    c->RemoveFromcollection(tr1);
    cout << c->area() << endl;

    Collection *c2 = new Collection;
    c2->addtocollection(c);
    c2->addtocollection(new TR(5, 6));
    cout << c2->area() << endl;
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
