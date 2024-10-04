#include "School.h"


//####				FUNCTIONS FOR SUBJECT

bool School::AddSubject(int ID, string Name)
{
	for (auto subject : m_pSubjectList)
		if (ID == subject->m_SubjectID)
			return false;

	m_pSubjectList.push_back(make_shared<Subject>(ID, Name));
	return true;
}
shared_ptr<Subject> School::GetSubject(int ID)
{
	for (auto subject : m_pSubjectList)
		if (ID == subject->m_SubjectID)
			return subject;
	return nullptr;
}

shared_ptr<Subject> School::GetSubject(string Name)
{
	for (auto subject : m_pSubjectList)
		if (Name == subject->m_SubjectName)
			return subject;
	return nullptr;
}

bool School::RemoveSubject(int ID)
{
	size_t i;
	for (i = 0; i < m_pSubjectList.size(); i++)
	{
		if (m_pSubjectList[i]->m_SubjectID == ID)
			break;
	}
	if (i < m_pSubjectList.size())
	{
		shared_ptr<Subject> temp = m_pSubjectList[i];
		m_pSubjectList.erase(m_pSubjectList.begin() + i);
		for (auto course : m_pCourseList)
			course->RemoveSubject(temp->m_SubjectID);
		return true;
	}
	return false;
}
bool School::RemoveSubject(string Name)
{
	size_t i;
	for (i = 0; i < m_pSubjectList.size(); i++)
	{
		if (m_pSubjectList[i]->m_SubjectName == Name)
			break;
	}
	if (i < m_pSubjectList.size())
	{
		shared_ptr<Subject> temp = m_pSubjectList[i];
		m_pSubjectList.erase(m_pSubjectList.begin() + i);
		for (auto course : m_pCourseList)
			course->RemoveSubject(temp->m_SubjectID);
		return true;
	}
	return false;
}

void School::DisplaySubjects()
{
	for (auto subject : m_pSubjectList)
		subject->DisplaySubjectDetails();
}


//####				FUNCTIONS FOR STUDENT


bool School::AddStudent(int ID, string Name, long int Date)
{
	for (auto student : m_pStudentList)
		if (ID == student->m_StudentID)
			return false;

	m_pStudentList.push_back(make_shared<Student>(ID, Name, Date));
	return true;
}
bool School::AddStudent(int ID, string Name, struct tm* Date)
{
	for (auto student : m_pStudentList)
		if (ID == student->m_StudentID)
			return false;

	m_pStudentList.push_back(make_shared<Student>(ID, Name, Date));
	return true;
}
shared_ptr<Student> School::GetStudent(int ID)
{
	for (auto student : m_pStudentList)
		if (ID == student->m_StudentID)
			return student;
	return nullptr;
}

shared_ptr<Student> School::GetStudent(string Name)
{
	for (auto student : m_pStudentList)
		if (Name == student->m_StudentName)
			return student;
	return nullptr;
}

bool School::RemoveStudent(int ID)
{
	
	size_t i;
	for (i = 0; i < m_pStudentList.size(); i++)
	{
		if (m_pStudentList[i]->m_StudentID == ID)
			break;
	}
	if (i < m_pStudentList.size())
	{
		shared_ptr<Student> temp = m_pStudentList[i];
		m_pStudentList.erase(m_pStudentList.begin() + i);
		for (auto course : m_pCourseList)
			course->RemoveStudent(temp->m_StudentID);
		return true;
	}
	return false;
}
bool School::RemoveStudent(string Name)
{
	size_t i;
	for (i = 0; i < m_pStudentList.size(); i++)
	{
		if (m_pStudentList[i]->m_StudentName == Name)
			break;
	}
	if (i < m_pStudentList.size())
	{
		shared_ptr<Student> temp = m_pStudentList[i];
		m_pStudentList.erase(m_pStudentList.begin() + i);
		for (auto course : m_pCourseList)
			course->RemoveStudent(temp->m_StudentID);
		return true;
	}
	return false;
}

void School::DisplayStudents()
{
	for (auto student : m_pStudentList)
		student->DisplayStudentInfo();
}

bool School::AddCoursesToStudent(shared_ptr<Student> student, vector<int> CourseIDs)
{
	if (nullptr == student)return false;
	for (auto courseID : CourseIDs)
	{
		shared_ptr<Course> course = GetCourse(courseID);
		if (course)
			student->AddCourse(course);
	}
	return true;
}

bool School::AddCoursesToStudent(shared_ptr<Student> student, vector<string> CourseNames)
{
	if (nullptr == student)return false;
	for (auto courseName : CourseNames)
	{
		shared_ptr<Course> course = GetCourse(courseName);
		if (course)
			student->AddCourse(course);
	}
	return true;
}

bool School::AddCoursesToStudent(int StudentID, vector<int> CourseIDs)
{
	shared_ptr<Student> student = GetStudent(StudentID);
	return AddCoursesToStudent(student, CourseIDs);
}

bool School::AddCoursesToStudent(int StudentID, vector<string> CourseNames)
{
	shared_ptr<Student> student = GetStudent(StudentID);
	return AddCoursesToStudent(student, CourseNames);
}

bool School::AddCoursesToStudent(string StudentName, vector<int> CourseIDs)
{
	shared_ptr<Student> student = GetStudent(StudentName);
	return AddCoursesToStudent(student, CourseIDs);
}

bool School::AddCoursesToStudent(string StudentName, vector<string> CourseNames)
{
	shared_ptr<Student> student = GetStudent(StudentName);
	return AddCoursesToStudent(student, CourseNames);
}

//####				FUNCTIONS FOR COURSE


bool School::AddCourse(int ID, string Name)
{
	for (auto course : m_pCourseList)
		if (ID == course->m_CourseID)
			return false;

	m_pCourseList.push_back(make_shared<Course>(ID, Name));
	return true;
}
shared_ptr<Course> School::GetCourse(int ID)
{
	for (auto course : m_pCourseList)
		if (ID == course->m_CourseID)
			return course;
	return nullptr;
}

shared_ptr<Course> School::GetCourse(string Name)
{
	for (auto course : m_pCourseList)
		if (Name == course->m_CourseName)
			return course;
	return nullptr;
}

bool School::RemoveCourse(int ID)
{
	size_t i;
	for (i = 0; i < m_pCourseList.size(); i++)
	{
		if (m_pCourseList[i]->m_CourseID == ID)
			break;
	}
	if (i < m_pCourseList.size())
	{
		shared_ptr<Course> temp = m_pCourseList[i];
		m_pCourseList.erase(m_pCourseList.begin() + i);
		for (auto student : m_pStudentList)
			student->RemoveCourse(temp->m_CourseID);
		return true;
	}
	return false;
}
bool School::RemoveCourse(string Name)
{
	size_t i;
	for (i = 0; i < m_pCourseList.size(); i++)
	{
		if (m_pCourseList[i]->m_CourseName == Name)
			break;
	}
	if (i < m_pCourseList.size())
	{
		shared_ptr<Course> temp = m_pCourseList[i];
		m_pCourseList.erase(m_pCourseList.begin() + i);
		for (auto student : m_pStudentList)
			student->RemoveCourse(temp->m_CourseID);
		return true;
	}
	return false;
}

void School::DisplayCourses()
{
	for (auto course : m_pCourseList)
		course->DisplayCourseDetails();
}

bool School::AddSubjectsToCourse(shared_ptr<Course> course, vector<int> SubjectIDs)
{
	if (nullptr == course)return false;
	for (auto subjectID : SubjectIDs)
	{
		shared_ptr<Subject> sub = GetSubject(subjectID);
		if(sub)
			course->AddSubject(sub);
	}
	return true;
}
bool School::AddSubjectsToCourse(int CourseID, vector<int> SubjectIDs)
{
	shared_ptr<Course> course = GetCourse(CourseID);
	return AddSubjectsToCourse(course, SubjectIDs);
}

bool School::AddSubjectsToCourse(string CourseName, vector<int> SubjectIDs)
{
	shared_ptr<Course> course = GetCourse(CourseName);
	return AddSubjectsToCourse(course, SubjectIDs);
}

bool School::AddSubjectsToCourse(shared_ptr<Course> course, vector<string> SubjectNames)
{
	if (nullptr == course)return false;
	for (auto subjectName : SubjectNames)
	{
		shared_ptr<Subject> sub = GetSubject(subjectName);
		if (sub)
			course->AddSubject(sub);
	}
	return true;
}

bool School::AddSubjectsToCourse(int CourseID, vector<string> SubjectNames)
{
	shared_ptr<Course> course = GetCourse(CourseID);
	return AddSubjectsToCourse(course, SubjectNames);
}

bool School::AddSubjectsToCourse(string CourseName, vector<string> SubjectNames)
{
	shared_ptr<Course> course = GetCourse(CourseName);
	return AddSubjectsToCourse(course, SubjectNames);
}
