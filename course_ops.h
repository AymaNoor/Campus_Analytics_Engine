#ifndef COURSE_OPS_H
#define COURSE_OPS_H

#include <string>

const int MAX_COURSE_RECORDS = 100;

struct Course {
    std::string courseCode;
    std::string courseTitle;
    int creditHours;
    int maxCapacity;
    int currentEnrollment;
};

void addCourse(Course courses[], int& courseCount);
void listCourses(Course courses[], int& courseCount);
void updateCourse(Course courses[], int& courseCount);
bool isValidCourseCode(const std::string& code);
bool isValidCreditHours(int hours);
bool courseCodeExists(Course courses[], int& courseCount, const std::string& code);

#endif
