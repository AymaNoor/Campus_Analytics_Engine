#ifndef ATTENDANCE_OPS_H
#define ATTENDANCE_OPS_H

#include <string>
#include "student_ops.h"
#include "course_ops.h"

const int MAX_ATTENDANCE_RECORDS = 100;

struct AttendanceRecord {
    std::string rollNumber;
    std::string courseCode;
    std::string date;
    std::string status;
};

void markAttendance(AttendanceRecord attendance[], int& attendanceCount, Student students[], int& studentCount, Course courses[], int& courseCount);
void viewAttendanceReport(AttendanceRecord attendance[], int& attendanceCount);
bool courseExists(Course courses[], int& courseCount, const std::string& courseCode);
bool isValidDate(const std::string& date);

#endif
