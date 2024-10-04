// Pointers.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
using namespace std;
int counter = 0;
struct B
{
    int i;
    B()
    {
        counter++;
    }
    ~B()
    {
        counter--;
    }
};
struct A
{
    int i;
    shared_ptr<A> p;
    weak_ptr<A> p1;
    A()
    {
        cout << "Constructor called\n";
    }
    ~A()
    {
        cout << "Destructor called\n";
    }

};
struct Node
{
    B obj;
    Node* next;
};

struct DoubleNode
{
    B obj;
    DoubleNode* prev, * next;
};
Node* addNode(Node*& head, int ele, int index = -1)
{
    Node* temp = new Node, * prev = nullptr, * cur = head;
    temp->obj.i = ele;
    temp->next = nullptr;
    if (!head)
    {
        head = temp;
        return temp;
    }
    if (index == -1)
    {
        while (cur)
        {
            prev = cur;
            cur = cur->next;
        }
        prev->next = temp;
    }
    else
    {
        if (index == 0) {
            temp->next = head;
            head = temp;
        }
        else {
            while (cur)
            {
                if (0 == index)break;
                index--;
                prev = cur;
                cur = cur->next;
            }
            if (cur)
            {
                prev->next = temp;
                temp->next = cur;
            }
        }
    }
    return head;
}

Node* deleteNode(Node*& head, int index)
{
    if (!head)
    {
        return nullptr;
    }
    if (index == 0)
    {
        Node* temp = head;
        head = head->next;
        delete temp;
        return head;
    }
    Node* prev = nullptr, * cur = head;
    int i = 0;
    while (cur)
    {
        if (i == index)break;
        i++;
        prev = cur;
        cur = cur->next;
    }
    if (i == index)
    {
        prev->next = cur->next;
        cur->next = nullptr;
        delete cur;
        cur = nullptr;
    }
    else
    {
        cout << "Index out of bounds\n";
    }
    return head;
}

void reverseList(Node*& head)
{
    if (!head)return;
    Node* prev = nullptr, * cur = nullptr, * next = nullptr;
    cur = head;
    next = head->next;
    while (cur)
    {
        cur->next = prev;
        prev = cur;
        cur = next;
        if (next)
            next = next->next;
    }
    head = prev;
}
void printList(Node* head)
{
    Node* temp = head;
    while (temp)
    {
        cout << temp->obj.i <<" ";
        temp = temp->next;
    }
    cout << endl;
}
bool findEle(Node* head, int ele)
{
    if (!head)return false;
    Node* temp = head;
    while (temp)
    {
        if (ele == temp->obj.i)
        {
            return true;
        }
        temp = temp->next;
    }
    return false;
}
DoubleNode* addNode(DoubleNode*& head, int ele, int index = -1)
{
    DoubleNode* temp = new DoubleNode, * prevnode = nullptr, * cur = head;
    temp->obj.i = ele;
    temp->next = temp->prev = nullptr;
    if (!head)
    {
        head = temp;
        return temp;
    }
    if (-1 == index)
    {
        while (cur)
        {
            prevnode = cur;
            cur = cur->next;
        }
        prevnode->next = temp;
        temp->prev = prevnode;
    }
    else {
        
        if (index == 0) {
            temp->next = head;
            head->prev = temp;
            head = temp;
        }
        else {
            while (cur)
            {
                if (0 == index)break;
                index--;
                prevnode = cur;
                cur = cur->next;
            }
            if (cur)
            {
                prevnode->next = temp;
                temp->prev = prevnode;
                temp->next = cur;
                cur->prev = temp;
            }
        }
    }
    return head;
}

DoubleNode* deleteNode(DoubleNode*& head, int index)
{
    if (!head)
    {
        return nullptr;
    }
    if (index == 0)
    {
        DoubleNode* temp = head;
        head = head->next;
        delete temp;
        return head;
    }
    DoubleNode* cur = head;
    int i = 0;
    while (cur)
    {
        if (i == index)break;
        i++;
        cur = cur->next;
    }
    if (!cur)return head;
    if (i == index)
    {
        DoubleNode* prevnode = cur->prev, * nextnode = cur->next;
        prevnode->next = nextnode;
        if(nextnode)
            nextnode->prev = prevnode;
        cur->prev = cur->next = nullptr;
        delete cur;
    }
    else
    {
        cout << "Index out of bounds\n";
    }
    return head;
}
void printList(DoubleNode* head)
{
    DoubleNode* temp = head;
    while (temp)
    {
        cout << temp->obj.i << " ";
        temp = temp->next;
    }
    cout << endl;
}

void reverseList(DoubleNode*& head)
{
    if (!head)return;
    DoubleNode* temp = nullptr, * cur = nullptr;
    cur = head;
    while (cur)
    {
        temp = cur->prev;
        cur->prev = cur->next;
        cur->next = temp;
        cur = cur->prev;
    }
    head = temp->prev;
    
}
bool findEle(DoubleNode* head, int ele)
{
    if (!head)return false;
    DoubleNode* temp = head;
    while (temp)
    {
        if (ele == temp->obj.i)
        {
            return true;
        }
        temp = temp->next;
    }
    return false;
}
int main()
{
    /*shared_ptr<A> a (new A());
    shared_ptr<A> b (new A());
    shared_ptr<A> c (new A());

    a->p = b;
    b->p = c;
    c->p1 = a;
    */
    
    //Node* head = nullptr;
    Node *head = nullptr;
    head = addNode(head, 5);
    head = addNode(head, 10);
    head = addNode(head, 15);
    head = addNode(head, 20);
    head = addNode(head, 25);
    head = addNode(head, 30);
    head = addNode(head, 35);
    head = addNode(head, 40);
    printList(head);
    reverseList(head);
    printList(head);
    head = deleteNode(head, 0);
    printList(head);
    head = deleteNode(head, 6);
    printList(head);
    head = deleteNode(head, 3);
    printList(head);
    if (findEle(head, 25))cout << "25 is present\n";
    else cout << "25 is not present\n";
    if (findEle(head, 40))cout << "40 is present\n";
    else cout << "40 is not present\n";
    //head = deleteNode(head, 0);
    //head = deleteNode(head, 0);
    //head = deleteNode(head, 0);
    //head = deleteNode(head, 0);
    //head = deleteNode(head, 0);
    cout << "Counter : "<<counter << endl;
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
