#ifndef COURSE_OPS_H
#define COURSE_OPS_H

#include <string>
#include <vector>

const int MAX_COURSE_RECORDS = 100;

struct Course {
    std::string courseCode;
    std::string courseTitle;
    int         creditHours;
    std::string instructor;
    int         maxCapacity;
    int         currentEnrollment;
    std::string prereq;          // "NONE" or another course code
};

// Enrollment result enum (no enum class — C++98 compat)
enum EnrollResult {
    ENROLL_OK,
    ENROLL_STUDENT_NOT_ACTIVE,
    ENROLL_COURSE_NOT_FOUND,
    ENROLL_SEATS_FULL,
    ENROLL_ALREADY_ENROLLED,
    ENROLL_PREREQ_NOT_MET,
    ENROLL_CREDIT_LIMIT
};

// --- File I/O ---
void loadCourses(Course courses[], int& courseCount);
void saveCourses(const Course courses[], int courseCount);

// --- CRUD ---
void addCourse(Course courses[], int& courseCount);
void listCourses(const Course courses[], int courseCount);
void updateCourse(Course courses[], int& courseCount);

// --- Enrollment (M3 spec) ---
// enrollments stored in enrollments.txt; no extra in-memory array needed
EnrollResult enrollStudent(const std::string& roll,
                           const std::string& courseCode,
                           const std::string& semester,
                           Course courses[], int courseCount);

bool dropCourse(const std::string& roll,
                const std::string& courseCode,
                const std::string& semester);

int getCreditLoad(const std::string& roll,
                  const std::string& semester,
                  const Course courses[], int courseCount);

bool checkPrerequisite(const std::string& courseCode,
                       const std::string& roll,
                       const Course courses[], int courseCount);

void listEnrolledStudents(const std::string& courseCode,
                          const std::string& semester);

// --- Validation helpers ---
bool isValidCourseCode(const std::string& code);
bool isValidCreditHours(int hours);
bool courseCodeExists(const Course courses[], int courseCount, const std::string& code);

#endif
