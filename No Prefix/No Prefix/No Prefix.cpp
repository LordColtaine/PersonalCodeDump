// No Prefix.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <string>
using namespace std;
#define ALPHABET_SIZE 10
struct TrieNode
{
    TrieNode* children[ALPHABET_SIZE];
    bool isEndOfWord;
};

TrieNode* CreateNode()
{
    TrieNode* node = new TrieNode();
    node->isEndOfWord = false;
    for (auto child : node->children)
    {
        child = nullptr;
    }
    return node;
}
bool CLEAR = false;
bool Insert(TrieNode* root, const string str)
{
    struct TrieNode* pCrawl = root;
    bool new_branch = false, part_of_word = false;
    for (int i = 0; i < str.length(); i++)
    {
        int index = str[i] - 'a';
        if (pCrawl->isEndOfWord)
        {
            part_of_word = true;
        }
        if (!pCrawl->children[index])
        {
            if (!part_of_word)
            {
                new_branch = true;
            }
            pCrawl->children[index] = CreateNode();
        }
        pCrawl = pCrawl->children[index];
    }

    // mark last node as leaf
    pCrawl->isEndOfWord = true;
    return !part_of_word && new_branch;
}

void noPrefix(vector<string> words) 
{
    TrieNode* root = new TrieNode();
    for (auto word : words)
    {
        CLEAR = false;
        Insert(root, word);
        if (!CLEAR)
        {
            cout << "BAD SET\n" << word << endl;
            return;
        }
    }
    cout << "GOOD SET\n";
    return;
}

int main()
{
    
    unsigned int n;
    cin >> n;
    vector<string> words(n);
    for (unsigned int i = 0; i < n; i++)
    {
        cin >> words[i];
    }
    cout << "\n\n\n\n\n";
    noPrefix(words);
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
