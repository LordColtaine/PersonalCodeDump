#include "Courses.h"

bool Course::RemoveSubject(int SubjectID)
{
	size_t i;
	for (i = 0; i<m_pSubjectList.size();i++)
	{
		if (m_pSubjectList[i]->m_SubjectID == SubjectID)
		{
			break;
		}
	}
	if (i < m_pSubjectList.size())
	{
		m_pSubjectList.erase(m_pSubjectList.begin()+i);
		return true;
	}
	return false;
}

bool Course::AddSubject(shared_ptr<Subject> subject)
{
	if (nullptr == subject)return false;
	for (auto sub : m_pSubjectList)
	{
		if (sub->m_SubjectID == subject->m_SubjectID)
			return false;
	}
	m_pSubjectList.push_back((subject));
	return true;
}

bool Course::RemoveStudent(int StudentID)
{
	size_t i;
	for (i = 0; i < m_pStudentList.size(); i++)
	{
		if (m_pStudentList[i]->m_StudentID == StudentID)
		{
			break;
		}
	}
	if (i < m_pStudentList.size())
	{
		shared_ptr<Student> temp = m_pStudentList[i];
		m_pStudentList.erase(m_pStudentList.begin() + i);
		temp->RemoveCourse(m_CourseID);
		return true;
	}
	return false;
}

bool Course::AddStudent(shared_ptr<Student> student)
{
	if (nullptr == student)return false;
	for (auto sub : m_pStudentList)
	{
		if (sub->m_StudentID == student->m_StudentID)
			return false;
	}
	m_pStudentList.push_back(student);
	student->AddCourse(shared_from_this());
	return true;
}

void Course::DisplayCourseDetails(bool first)
{
	cout << "CourseID : " << m_CourseID <<"\nCourse Name : "<<m_CourseName<< endl;
	if (first)
	{
		cout << "Subject List :\n";
		for (auto subject : m_pSubjectList)
			subject->DisplaySubjectDetails();
		cout << "Student List : \n";
		for (auto student : m_pStudentList)
			student->DisplayStudentInfo(false);
		cout << "\n\n";
	}
}