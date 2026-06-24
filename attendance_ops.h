#ifndef ATTENDANCE_OPS_H
#define ATTENDANCE_OPS_H

#include <string>
#include <vector>
#include "student_ops.h"
#include "course_ops.h"

struct AttendanceRecord {
    std::string rollNumber;
    std::string courseCode;
    std::string date;
    std::string status;
};

void markAttendance(std::vector<AttendanceRecord>& attendance, const std::vector<Student>& students, const std::vector<Course>& courses);
void viewAttendanceReport(const std::vector<AttendanceRecord>& attendance);
bool courseExists(const std::vector<Course>& courses, const std::string& courseCode);
bool isValidDate(const std::string& date);

#endif
