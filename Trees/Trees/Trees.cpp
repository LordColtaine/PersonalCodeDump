// Trees.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <map>
using namespace std;

#define BUF_SIZE 32

int counter = 0;
struct Data {
    int member = -1;
    Data() { counter++; }
    ~Data() { counter--; }
    Data operator=(const Data d1)
    {
        this->member = d1.member;
        return *this;
    }
    Data* operator=(const Data* d1)
    {
        this->member = d1->member;
        return this;
    }
};

struct Node {
    Data *m_data;
    Node* m_left, * m_right;
    int m_dist = 0;
    Node()
    {
        m_left = m_right = nullptr;
        m_data = new Data;
    }
    Node(int ele)
    {
        m_data = new Data;
        m_left = m_right = nullptr;
        m_data->member = ele;
    }
    ~Node()
    {
        delete m_data;
    }
};

struct stack
{
    Node** arr;
    int top;
    stack()
    {
        arr = NULL;
        top = -1;
    }
    stack(const stack& s)
    {

        delete[] arr;
        arr = new Node*[s.top + 1];
        memcpy(arr, s.arr, sizeof(Node*) * (s.top + 1));
        top = s.top;
    }
    stack& operator =(const stack& s)
    {
        if (this != &s)
        {
            delete[] arr;
            arr = new Node*[s.top + 1];
            memcpy(arr, s.arr, sizeof(Node*) * (s.top + 1));
            top = s.top;
        }
        return *this;
    }
    ~stack()
    {
        delete[] arr;
    }
    bool empty()
    {
        return (top == -1);
    }
    void push(Node *a)
    {
        if (empty())
        {
            arr = new Node*;
            arr[++top] = a;
            return;
        }
        top++;
        Node** t = new Node*[(top + 1)];
        memcpy(t, arr, top * sizeof(Node*));
        delete[] arr;
        t[top] = a;
        arr = t;
        t = NULL;
    }

    void printstack()
    {
        if (empty())
        {
            cout << "Empty stack\n";
            return;
        }
        for (int i = 0; i <= top; i++)
        {
            cout << arr[i]->m_data->member << " ";
        }
        cout << endl;
    }
    void pop()
    {
        if (empty())
        {
            cout << "Stack is empty\n";
            return;
        }
        Node** t = new Node*[top];
        memcpy(t, arr, top * sizeof(Node*));
        delete[] arr;
        top--;
        arr = t;
        t = NULL;
    }
    Node* topOfStack()
    {
        if (!empty())
        {
            return arr[top];
        }
        return nullptr;
    }
};

struct queue
{
    Node** arr;
    int f, r;
    queue()
    {
        f = r = -1;
        arr = NULL;
    }
    ~queue()
    {
        delete[] arr;
    }
    bool empty()
    {
        return (-1 == r);
    }
    void enqueue(Node* a)
    {
        if (empty())
        {
            arr = new Node*;
            arr[++r] = a;
            return;
        }
        r++;
        Node** t = new Node*[(r + 1)];
        memcpy(t, arr, r * sizeof(Node*));
        delete[] arr;
        t[r] = a;
        arr = t;
        t = NULL;
    }
    void printqueue()
    {
        if (empty())
        {
            cout << "Empty queue\n";
            return;
        }
        for (int i = 0; i <= r; i++)
        {
            cout << arr[i]->m_data->member << " ";
        }
        cout << endl;
    }
    void dequeue()
    {
        if (empty())
        {
            cout << "Queue is empty\n";
            return;
        }
        Node** t = new Node*[r];
        memcpy(t, arr + 1, r * sizeof(Node*));
        delete[] arr;
        r--;
        arr = t;
        t = NULL;
    }
    Node* front()
    {
        if (empty())
        {
            return nullptr;
        }
        return arr[0];
    }
};



void addNodeRecursion(Node*& root, int ele)
{
    if (!root)
    {
        root = new Node(ele);
        return;
    }
    if (root->m_data->member<ele)
        addNodeRecursion(root->m_right, ele);
    else
        addNodeRecursion(root->m_left, ele);
}

void printTreeInorder(Node* root)
{
    if (root)
    {
        printTreeInorder(root->m_left);
        cout << root->m_data->member << " ";
        printTreeInorder(root->m_right);
    }
}

void addNodeIteration(Node*& root, int ele)
{
    if (!root)
    {
        root = new Node(ele);
        return;
    }
    Node* temp = root, * parent = nullptr;
    while (temp)
    {
        parent = temp;
        if (temp->m_data->member <ele)
        {
            temp = temp->m_right;
        }
        else temp = temp->m_left;
    }
    temp = new Node(ele);
    if (parent->m_data->member <ele)parent->m_right = temp;
    else parent->m_left = temp;

}
void rotateLeft(Node*& root)
{
    if (!root)return;
    Node* left = root->m_left, * right = root->m_right;
    if (nullptr != right)
    {
        root->m_right = right->m_left;
        right->m_left = root;
        root = right;
    }
}
void rotateRight(Node*& root)
{
    if (!root)return;
    Node* left = root->m_left, * right = root->m_right;
    if (nullptr != left)
    {
        root->m_left = left->m_right;
        left->m_right = root;
        root = left;
    }
}

void DFS(Node *root)
{
    stack s;
    s.push(root);
    while (!s.empty())
    {
        Node* temp = s.topOfStack();
        s.pop();
        if (temp->m_right)s.push(temp->m_right);
        if (temp->m_left)s.push(temp->m_left);
        cout << temp->m_data->member << " ";
    }
    s.printstack();
}

void BFS(Node* root)
{
    queue q;
    q.enqueue(root);
    while (!q.empty())
    {
        Node* temp = q.front();
        q.dequeue();
        if (temp->m_left)q.enqueue(temp->m_left);
        if (temp->m_right)q.enqueue(temp->m_right);
        cout << temp->m_data->member << " ";
    }
    cout << endl;
    q.printqueue();
    
}

void printTreeForm(Node* root, int pos = 0)
{
    if (!root)return;
    pos += 5;
    printTreeForm(root->m_right, pos);
    cout << endl;
    for (int i = 5; i < pos; i++)cout << " ";
    cout << root->m_data->member;
    printTreeForm(root->m_left, pos);
}

void printTreeUsing2D(Node* root, int arr[BUF_SIZE][BUF_SIZE], int y = 0, int LeftBound = 0, int RightBound = BUF_SIZE-1)
{
    if (!root)return;
    int x = (LeftBound + RightBound) / 2;
    arr[y][x] = root->m_data->member;
    if (root->m_left)
    {
        printTreeUsing2D(root->m_left, arr, y+1, LeftBound, x);
    }
    if (root->m_right)
    {
        printTreeUsing2D(root->m_right, arr, y+1, x, RightBound);
    }
}

void deleteLeft(Node*& root)
{
    if (nullptr == root->m_left->m_right)
    {
        root->m_data->member = root->m_left->m_data->member;
        Node* temp = root->m_left;
        root->m_left = root->m_left->m_left;
        delete temp;
        return;
    }
    Node* cur = root->m_left, * prev = root;
    while (cur->m_right)
    {
        prev = cur;
        cur = cur->m_right;
    }
    root->m_data->member = cur->m_data->member;
    prev->m_right = cur->m_left;
    delete cur;
}

void deleteRight(Node*& root)
{
    if (nullptr == root->m_right->m_left)
    {
        root->m_data->member = root->m_right->m_data->member;
        Node* temp = root->m_right;
        root->m_right = root->m_right->m_right;
        delete temp;
        return;
    }
    Node* cur = root->m_right, * prev = root;
    while (cur->m_left)
    {
        prev = cur;
        cur = cur->m_left;
    }
    root->m_data->member = cur->m_data->member;
    prev->m_left = cur->m_right;
    delete cur;
}

void deleteNode(Node*& root, int ele)
{
    if (!root)return;
    if (root->m_data->member == ele)
    {
        if (nullptr == root->m_left && nullptr == root->m_right)
        {
            delete root;
            root = nullptr;
            return;
        }
        if (root->m_left)
        {
            deleteLeft(root);
        }
        else
        {
            deleteRight(root);
        }
        return;
    }
    if (root->m_data->member > ele)deleteNode(root->m_left, ele);
    else
        deleteNode(root->m_right, ele);

}

int numNodes(Node* root)
{
    if (!root)return 0;
    return 1 + numNodes(root->m_left) + numNodes(root->m_right);
}

int height(Node* root)
{
    if (!root)return 0;
    return 1 + max(height(root->m_left), height(root->m_right));
}

int kthSmallest(Node* root, int k)
{
    stack s;
    Node* temp = root;
    while (!s.empty() || temp)
    {
        while (temp)
        {
            s.push(temp);
            temp = temp->m_left;
        }
        temp = s.topOfStack();
        s.pop();
        k--;
        if (0 == k)break;
        temp = temp->m_right;
    }
    if (temp)return temp->m_data->member;
    return -1;
}

int kthLargest(Node* root, int k)
{
    stack s;
    Node* temp = root;
    while (!s.empty() || temp)
    {
        while (temp)
        {
            s.push(temp);
            temp = temp->m_right;
        }
        temp = s.topOfStack();
        s.pop();
        k--;
        if (0 == k)break;
        temp = temp->m_left;
    }
    if (temp)return temp->m_data->member;
    return -1;
}

void bottomView(Node* root)
{
    if (!root)return;
    queue q;
    int dist;
    map<int, int> m;
    root->m_dist = 0;
    q.enqueue(root);
    Node* temp = nullptr;
    while (!q.empty())
    {
        temp = q.front();
        q.dequeue();
        dist = temp->m_dist;
        m[dist] = temp->m_data->member;
        if (temp->m_left)
        {
            temp->m_left->m_dist = dist - 1;
            q.enqueue(temp->m_left);
        }
        if (temp->m_right)
        {
            temp->m_right->m_dist = dist + 1;
            q.enqueue(temp->m_right);
        }
    }
    for (auto i = m.begin(); i != m.end(); i++)
        cout << i->second << " ";
    cout << endl;
}

void topView(Node* root)
{
    if (!root)return;
    queue q;
    int dist;
    map<int, int> m;
    root->m_dist = 0;
    q.enqueue(root);
    Node* temp = nullptr;
    while (!q.empty())
    {
        temp = q.front();
        q.dequeue();
        dist = temp->m_dist;
        if(m.count(dist)==0)
        m[dist] = temp->m_data->member;
        if (temp->m_left)
        {
            temp->m_left->m_dist = dist - 1;
            q.enqueue(temp->m_left);
        }
        if (temp->m_right)
        {
            temp->m_right->m_dist = dist + 1;
            q.enqueue(temp->m_right);
        }
    }
    for (auto i = m.begin(); i != m.end(); i++)
        cout << i->second << " ";
    cout << endl;
}

int main()
{
    Node* root = nullptr;
    addNodeRecursion(root, 15);
    addNodeRecursion(root, 20);
    addNodeIteration(root, 19);
    addNodeIteration(root, 30);
    addNodeRecursion(root, 10);
    addNodeIteration(root, 5);
    //cout << "Count : " << counter << endl;
    printTreeForm(root);
    //cout << root->m_data.member << endl;
    cout << "\n\n\n";
    //rotateLeft(root->m_right);
    //printTreeForm(root);
    //cout << root->m_data.member << endl;
    //DFS(root);
    //cout << "\n\n\n";
    //BFS(root);
    //printTreeForm(root);
    //cout << "\n\n\n\n";
    /*
    int arr[BUF_SIZE][BUF_SIZE];
    for (int i = 0; i < BUF_SIZE; i++)
    {
        for (int j = 0; j < BUF_SIZE; j++)
        {
            arr[i][j] = ' ';
        }
    }
    printTreeUsing2D(root, arr);
    cout << "\n\n\n\n";
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < BUF_SIZE; j++)
        {
            if (arr[i][j] == 32)cout << " ";
            else cout <<arr[i][j];
        }
        cout << endl;
    }
    */
    deleteNode(root, 20);
    printTreeForm(root);
    //cout << "Count : " << counter << endl;
    //cout << height(root) <<numNodes(root)<< endl;
    //cout << kthSmallest(root, 2) << endl;
    //cout << kthLargest(root, 2) << endl;

    //topView(root);
    //bottomView(root);
    return 0;
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
