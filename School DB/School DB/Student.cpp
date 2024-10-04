#include "Student.h"


//Remove a course from student
bool Student::RemoveCourse(int CourseID)
{
	size_t i;
	for (i = 0; i < m_pCourseList.size(); i++)
	{
		if (m_pCourseList[i]->m_CourseID == CourseID)
		{
			break;
		}
	}
	if (i < m_pCourseList.size())
	{
		shared_ptr<Course> temp = m_pCourseList[i];
		m_pCourseList.erase(m_pCourseList.begin() + i);
		temp->RemoveStudent(m_StudentID);
		return true;
	}
	return false;
}

bool Student::AddCourse(shared_ptr<Course> course)
{
	if (nullptr == course)return false;
	for (auto sub : m_pCourseList)
	{
		if (sub->m_CourseID == course->m_CourseID)
			return false;
	}
	m_pCourseList.push_back(shared_ptr<Course>(course));
	course->AddStudent(shared_from_this());
	return true;
}

void Student::DisplayStudentInfo(bool first)
{
	struct tm* Date = new struct tm();
	char buf[64];
	localtime_s(Date, &m_DateOfJoining);
	strftime(buf, 64, "%F", Date);
	cout << "ID : " << m_StudentID << "\nName : " << m_StudentName << "\nDate of Joining : " << buf << endl;
	if (first)
	{
		cout << "Course List : \n";
		for (auto course : m_pCourseList)
			course->DisplayCourseDetails(false);
		cout << "\n\n";
	}
}