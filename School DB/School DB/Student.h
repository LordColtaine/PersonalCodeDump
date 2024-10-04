#pragma once

#include<string>
#include <iostream>
#include <vector>
#include<chrono>
#include"Courses.h"
using namespace std;
struct Course;

struct Student : public enable_shared_from_this<Student>
{
    int m_StudentID;
    string m_StudentName;
    time_t m_DateOfJoining;
    vector<shared_ptr<Course>> m_pCourseList;
    vector<int> m_CourseIDs;
    
    Student(int ID, string Name, long int Date)
    {
        m_StudentID = ID;
        m_StudentName = Name;
        m_DateOfJoining = Date;
    }
    Student(int ID, string Name, tm* Date)
    {
        m_StudentID = ID;
        m_StudentName = Name;
        m_DateOfJoining = mktime(Date);

    }
    ~Student()
    {
        m_pCourseList.clear();
    }


    //To add a course but not increase the number of course objects with a particular id, 
    //the shared pointer is used to link to the existing course
    //But in order to allow sending a reference of this object to add in the course's student list as a shared ptr,
    //The course class inherits from "enable_shared_from_this" which allows to create shared_ptr of the current instance
    bool AddCourse(shared_ptr<Course> course);

    bool RemoveCourse(int CourseID);

    void DisplayStudentInfo(bool first = true);

    template<typename T>
    void Serialize(T& stream)
    {
        stream << endl << m_StudentID << " " << m_StudentName << " " << m_DateOfJoining << " < ";
        for (auto course : m_pCourseList)
        {
            stream << course->m_CourseID << " ";
        }
        stream << ">";
    }

    template<typename T>
    static shared_ptr<Student> Deserialize(T& stream)
    {
        int ID;
        long int Date;
        string Name, ch;
        vector<int> courseIDs;
        stream >> ID >> Name >> Date >> ch >> ch;
        while (ch.compare(">"))
        {
            courseIDs.push_back(stoi(ch));
            stream >> ch;
        }
        shared_ptr<Student> student = make_shared<Student>(ID, Name, Date);
        student->m_CourseIDs = courseIDs;
        return student;
    }
};