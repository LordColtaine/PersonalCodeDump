#pragma once
#include "Subject.h"
#include<vector>
#include <iostream>

using namespace std;
#include "Student.h"
struct Student;

struct Course : public enable_shared_from_this<Course>
{
    int m_CourseID;
    string m_CourseName;
    vector<shared_ptr<Subject>> m_pSubjectList;
    vector<shared_ptr<Student>> m_pStudentList;
    vector<int> m_SubjectIDs;
    vector<int> m_StudentIDs;

    Course() { m_CourseID = 0; }
    Course(int Course_ID) { m_CourseID = Course_ID; }
    Course(int Course_ID, string Name) { m_CourseID = Course_ID; m_CourseName = Name; }
    ~Course()
    {
        m_pStudentList.clear();
        m_pSubjectList.clear();
    }
    bool AddSubject(shared_ptr<Subject> subject);
    
    bool RemoveSubject(int SubjectID);

    bool AddStudent(shared_ptr<Student> student);

    bool RemoveStudent(int StudentID);

    void DisplayCourseDetails(bool first = true);


    template<typename T>
    void Serialize(T& stream)
    {
        stream << endl << m_CourseID << " " << m_CourseName << " < ";
        for (auto subject : m_pSubjectList)
            stream << subject->m_SubjectID << " ";
        stream << "> ( ";
        for (auto student : m_pStudentList)
            stream << student->m_StudentID<<" ";
        stream << ")";
    }

    template<typename T>
    static shared_ptr<Course> Deserialize(T& stream)
    {
        int ID;
        string Name;
        string ch;
        vector<int> subjectIDs, studentIDs;
        stream >> ID >> Name >> ch >> ch;

        while (ch.compare(">"))
        {
            subjectIDs.push_back(stoi(ch));
            stream >> ch;
        }
        stream >> ch >> ch;
        while (ch.compare(")"))
        {
            studentIDs.push_back(stoi(ch));
            stream >> ch;
        }
        shared_ptr<Course> course = make_shared<Course>(ID, Name);
        course->m_SubjectIDs = subjectIDs;
        course->m_StudentIDs = studentIDs;
        return course;
    }
};