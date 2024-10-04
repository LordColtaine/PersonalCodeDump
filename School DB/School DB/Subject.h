#pragma once


#include <string>
#include<iostream>
using namespace std;
struct Subject
{
    int m_SubjectID;
    string m_SubjectName;
    Subject(int ID, string name)
    {
        m_SubjectID = ID;
        m_SubjectName = name;
    }

    void DisplaySubjectDetails()
    {
        cout << "SubjectID : " << m_SubjectID << "\nSubject Name : " << m_SubjectName << "\n\n";
    }
    template<typename T>
    void Serialize(T &stream)
    {
        stream <<endl<< m_SubjectID << " " << m_SubjectName;
    }
    template<typename T>
    static shared_ptr<Subject> Deserialize(T &stream)
    {
        int ID;
        string Name;
        stream >> ID >> Name;
        shared_ptr<Subject> temp = make_shared<Subject>(ID,Name);
        return temp;
    }
};

