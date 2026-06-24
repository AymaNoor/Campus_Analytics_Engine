#ifndef COURSE_OPS_H
#define COURSE_OPS_H

#include <string>
#include <vector>

struct Course {
    std::string courseCode;
    std::string courseTitle;
    int creditHours;
    int maxCapacity;
    int currentEnrollment;
};

void addCourse(std::vector<Course>& courses);
void listCourses(const std::vector<Course>& courses);
void updateCourse(std::vector<Course>& courses);
bool isValidCourseCode(const std::string& code);
bool isValidCreditHours(int hours);
bool courseCodeExists(const std::vector<Course>& courses, const std::string& code);

#endif
