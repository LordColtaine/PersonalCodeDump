// School DB.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Headers.h"
using namespace std;

shared_ptr<Subject> GetSubject(vector<shared_ptr<Subject>> m_pSubjectList, int ID)
{
    for (auto subject : m_pSubjectList)
        if (ID == subject->m_SubjectID)
            return subject;
    return nullptr;
}

int main()
{
    /*
    vector<shared_ptr<Subject>> subjects;
    subjects.push_back(make_shared<Subject>(1, "Sub1"));
    subjects.push_back(make_shared<Subject>(2, "Sub2"));
    subjects.push_back(make_shared<Subject>(3, "Sub3"));
    //for (auto sub : subjects)
     //   sub.DisplaySubjectDetails();

    shared_ptr<Course> c1 = make_shared<Course>(1, "Course1"), c2 = make_shared<Course>(2, "Course2");
    c1->AddSubject(subjects[0]);
    c1->AddSubject(subjects[1]);

    c2->AddSubject(subjects[0]);
    c2->AddSubject(subjects[2]);
    //c1->DisplayCourseDetails(false);
    //c2->DisplayCourseDetails(false);
    //cout << &(c2->m_pSubjectList[0]->m_SubjectID) << " "<< &(c1->m_pSubjectList[0]->m_SubjectID)<<endl;
    struct tm* date = new struct tm();
    time_t curtime;
    time(&curtime);
    localtime_s(date, &curtime);
    date->tm_year = 2012 - 1900;
    date->tm_mday = 25;
    date->tm_mon = 10;

    shared_ptr<Student> s1 = make_shared<Student>(1, "Ravi", date ), s2 = make_shared<Student>(2, "Raj", date);
    s1->AddCourse(c1);
    
    s1->AddCourse(c2);
    s2->AddCourse(c1);
    //c1->DisplayCourseDetails();
    //s1->DisplayStudentInfo();

    //s1->RemoveCourse(1);
    
    c1->DisplayCourseDetails();
    s1->DisplayStudentInfo();
    subjects.clear();
    */
    /*
    ofstream SubjectStream, StudentStream, CourseStream;

    SubjectStream.open("subjects.txt");
    StudentStream.open("students.txt");
    CourseStream.open("courses.txt");

    
    
    struct tm* date = new struct tm();
    time_t curtime;
    time(&curtime);
    localtime_s(date, &curtime);
    date->tm_year = 2012 - 1900;
    date->tm_mday = 25;
    date->tm_mon = 10;

    School S;
    S.AddSubject(1, "Sub1");
    S.AddSubject(2, "Sub2");
    
    S.AddCourse(1, "C1");
    S.AddCourse(2, "C2");

    S.AddStudent(1, "S1", date);
    S.AddStudent(2, "S2", date);

    S.AddSubjectsToCourse("C1", vector<int>({ 1, 2 }));
    S.AddSubjectsToCourse("C2", vector<int>({ 2 }));

    S.AddCoursesToStudent(1, vector<int>({1, 2}));
    S.AddCoursesToStudent(2, vector<int>({ 1 }));
    //cout << &(S.m_pSubjectList[1]->m_SubjectID) << endl;;
    //cout<<&(S.m_pCourseList[0]->m_pSubjectList[1]->m_SubjectID)<<endl;
    //cout << &(S.m_pStudentList[0]->m_pCourseList[1]->m_pSubjectList[0]->m_SubjectID) << endl;
    
    S.Serialize(SubjectStream, CourseStream, StudentStream);
    SubjectStream.close();
    StudentStream.close();
    CourseStream.close();
    */
    
    School S2;
    ifstream SubjectStream, StudentStream, CourseStream;
    
    SubjectStream.open("subjects.txt");
    StudentStream.open("students.txt");
    CourseStream.open("courses.txt");
    //char ch;
    //SubjectStream >> ch;
    //StudentStream >> ch;
    //CourseStream >> ch;
    S2 = School::Deserialize(SubjectStream, CourseStream, StudentStream);
    S2.DisplayAll();

    SubjectStream.close();
    StudentStream.close();
    CourseStream.close();
    
    /*
    struct tm* date = new struct tm();
    time_t curtime;
    time(&curtime);
    localtime_s(date, &curtime);
    date->tm_year = 2012-1900;
    date->tm_mday = 25;
    date->tm_mon = 10;
    cout<<mktime(date)<<endl;
    
    //Student S(1, "Me", date);
    //S.DisplayStudentInfo();
    */
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
