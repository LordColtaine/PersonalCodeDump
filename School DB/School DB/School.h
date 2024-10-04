#pragma once
#include"Student.h"
#include"Courses.h"

struct School
{
    vector<shared_ptr<Subject>> m_pSubjectList;
    vector<shared_ptr<Course>> m_pCourseList;
    vector<shared_ptr<Student>> m_pStudentList;


    bool AddSubject(int ID, string Name);
    shared_ptr<Subject> GetSubject(int ID);
    shared_ptr<Subject> GetSubject(string Name);
    bool RemoveSubject(int ID);
    bool RemoveSubject(string Name);
    void DisplaySubjects();


    
    bool AddStudent(int ID, string Name, long int Date);
    bool AddStudent(int ID, string Name, struct tm* Date);
    shared_ptr<Student> GetStudent(int ID);
    shared_ptr<Student> GetStudent(string Name);
    bool RemoveStudent(int ID);
    bool RemoveStudent(string Name);
    void DisplayStudents();
    bool AddCoursesToStudent(shared_ptr<Student> student, vector<int> CourseIDs);
    bool AddCoursesToStudent(string StudentName, vector<int> CourseIDs);
    bool AddCoursesToStudent(int StudentID, vector<int> CourseIDs);
    bool AddCoursesToStudent(shared_ptr<Student> student, vector<string> CourseNames);
    bool AddCoursesToStudent(string StudentName, vector<string> CourseNames);
    bool AddCoursesToStudent(int StudentID, vector<string> CourseNames);


    bool AddCourse(int ID, string Name);
    shared_ptr<Course> GetCourse(int ID);
    shared_ptr<Course> GetCourse(string Name);
    bool RemoveCourse(int ID);
    bool RemoveCourse(string Name);
    void DisplayCourses();
    bool AddSubjectsToCourse(shared_ptr<Course> course, vector<int> SubjectIDs);
    bool AddSubjectsToCourse(int CourseID, vector<int> SubjectIDs);
    bool AddSubjectsToCourse(string CourseName, vector<int> SubjectIDs);
    bool AddSubjectsToCourse(shared_ptr<Course> course, vector<string> SubjectNames);
    bool AddSubjectsToCourse(int CourseID, vector<string> SubjectNames);
    bool AddSubjectsToCourse(string CourseName, vector<string> SubjectNames);


    void DisplayAll()
    {
        DisplaySubjects();
        DisplayCourses();
        DisplayStudents();
    }

    template<typename T1, typename T2, typename T3>
    void Serialize(T1& SubjectStream, T2& CourseStream, T3& StudentStream)
    {
        for (auto subject : m_pSubjectList)
            subject->Serialize(SubjectStream);
        for (auto course : m_pCourseList)
            course->Serialize(CourseStream);
        for (auto student : m_pStudentList)
            student->Serialize(StudentStream);
    }
    template<typename T1, typename T2, typename T3>
    static School Deserialize(T1& SubjectStream, T2& CourseStream, T3& StudentStream)
    {
        School S;
        while(!SubjectStream.eof())
            S.m_pSubjectList.push_back(Subject::Deserialize(SubjectStream));
        while (!CourseStream.eof())
            S.m_pCourseList.push_back(Course::Deserialize(CourseStream));
        while (!StudentStream.eof())
            S.m_pStudentList.push_back(Student::Deserialize(StudentStream));
        
        for (auto course : S.m_pCourseList)
        {
            for (auto subjectID : course->m_SubjectIDs)
                course->AddSubject(S.GetSubject(subjectID));
        }
        for (auto student : S.m_pStudentList)
        {
            for(auto courseID:student->m_CourseIDs)
                student->AddCourse(S.GetCourse(courseID));
        }
        return S;
    }
};