#include <iostream>
#include "student_ops.h"
#include "course_ops.h"
#include "attendance_ops.h"
#include "grades_ops.h"

using namespace std;

void displayMainMenu() {
    cout << "\n========================================" << endl;
    cout << "    Campus Analytics Engine - Main Menu  " << endl;
    cout << "========================================" << endl;
    cout << "1. Student Management" << endl;
    cout << "2. Course Management" << endl;
    cout << "3. Attendance Management" << endl;
    cout << "4. Grades Management" << endl;
    cout << "5. Reports & Analytics" << endl;
    cout << "6. Enrollment Management" << endl;
    cout << "7. Exit Application" << endl;
    cout << "----------------------------------------" << endl;
    cout << "Select an option (1-7): ";
}

void displayStudentMenu() {
    cout << "\n====== Student Management Menu ======" << endl;
    cout << "1. Add New Student" << endl;
    cout << "2. Search Student" << endl;
    cout << "3. Update Student" << endl;
    cout << "4. Delete Student" << endl;
    cout << "5. List All Active Students" << endl;
    cout << "6. Back to Main Menu" << endl;
    cout << "--------------------------------------" << endl;
    cout << "Select an option (1-6): ";
}

void displaySearchMenu() {
    cout << "\n====== Search Student Menu ======" << endl;
    cout << "1. Search by Roll Number" << endl;
    cout << "2. Search by Name" << endl;
    cout << "3. Back to Student Management" << endl;
    cout << "----------------------------------" << endl;
    cout << "Select an option (1-3): ";
}

void displayCourseMenu() {
    cout << "\n====== Course Management Menu ======" << endl;
    cout << "1. Add New Course" << endl;
    cout << "2. View All Courses" << endl;
    cout << "3. Update Course" << endl;
    cout << "4. Back to Main Menu" << endl;
    cout << "--------------------------------------" << endl;
    cout << "Select an option (1-4): ";
}

void displayReportsMenu() {
    cout << "\n====== Reports & Analytics Menu ======" << endl;
    cout << "1. Student Performance Report" << endl;
    cout << "2. Enrollment Statistics" << endl;
    cout << "3. Course Popularity Report" << endl;
    cout << "4. Back to Main Menu" << endl;
    cout << "----------------------------------------" << endl;
    cout << "Select an option (1-4): ";
}

void displayEnrollmentMenu() {
    cout << "\n====== Enrollment Management Menu ======" << endl;
    cout << "1. Enroll Student in Course" << endl;
    cout << "2. View Enrollments" << endl;
    cout << "3. Remove Enrollment" << endl;
    cout << "4. Back to Main Menu" << endl;
    cout << "------------------------------------------" << endl;
    cout << "Select an option (1-4): ";
}

void displayAttendanceMenu() {
    cout << "\n====== Attendance Management Menu ======" << endl;
    cout << "1. Mark Attendance" << endl;
    cout << "2. View Attendance Report" << endl;
    cout << "3. Back to Main Menu" << endl;
    cout << "------------------------------------------" << endl;
    cout << "Select an option (1-3): ";
}

void displayGradesMenu() {
    cout << "\n====== Grades Management Menu ======" << endl;
    cout << "1. Enter Student Marks" << endl;
    cout << "2. Compute Course Class Stats" << endl;
    cout << "3. Back to Main Menu" << endl;
    cout << "------------------------------------" << endl;
    cout << "Select an option (1-3): ";
}

void handleSearchStudent(Student students[], int& studentCount) {
    int choice;
    
    while (true) {
        displaySearchMenu();
        cin >> choice;
        cin.ignore();
        
        switch (choice) {
            case 1:
                searchByRoll(students, studentCount);
                break;
            case 2:
                searchByName(students, studentCount);
                break;
            case 3:
                cout << "Returning to Student Management Menu..." << endl;
                return;
            default:
                cout << "Invalid option. Please try again." << endl;
        }
    }
}

void handleStudentManagement(Student students[], int& studentCount) {
    int choice;
    
    while (true) {
        displayStudentMenu();
        cin >> choice;
        cin.ignore();
        
        switch (choice) {
            case 1:
                addStudent(students, studentCount);
                break;
            case 2:
                handleSearchStudent(students, studentCount);
                break;
            case 3:
                updateStudent(students, studentCount);
                break;
            case 4:
                softDelete(students, studentCount);
                break;
            case 5:
                listActiveStudents(students, studentCount);
                break;
            case 6:
                cout << "Returning to Main Menu..." << endl;
                return;
            default:
                cout << "Invalid option. Please try again." << endl;
        }
    }
}

void handleCourseManagement(Course courses[], int& courseCount) {
    int choice;
    
    while (true) {
        displayCourseMenu();
        cin >> choice;
        cin.ignore();
        
        switch (choice) {
            case 1:
                addCourse(courses, courseCount);
                break;
            case 2:
                listCourses(courses, courseCount);
                break;
            case 3:
                updateCourse(courses, courseCount);
                break;
            case 4:
                cout << "Returning to Main Menu..." << endl;
                return;
            default:
                cout << "Invalid option. Please try again." << endl;
        }
    }
}

void handleReportsMenu(Student students[], int& studentCount) {
    int choice;
    
    while (true) {
        displayReportsMenu();
        cin >> choice;
        cin.ignore();
        
        switch (choice) {
            case 1:
                cout << "\n--- Student Performance Report ---" << endl;
                cout << "[Reports Feature - Coming Soon]" << endl;
                break;
            case 2:
                cout << "\n--- Enrollment Statistics ---" << endl;
                cout << "[Reports Feature - Coming Soon]" << endl;
                break;
            case 3:
                cout << "\n--- Course Popularity Report ---" << endl;
                cout << "[Reports Feature - Coming Soon]" << endl;
                break;
            case 4:
                cout << "Returning to Main Menu..." << endl;
                return;
            default:
                cout << "Invalid option. Please try again." << endl;
        }
    }
}

void handleEnrollmentManagement(Student students[], int& studentCount) {
    int choice;
    
    while (true) {
        displayEnrollmentMenu();
        cin >> choice;
        cin.ignore();
        
        switch (choice) {
            case 1:
                cout << "\n--- Enroll Student in Course ---" << endl;
                cout << "[Enrollment Management Feature - Coming Soon]" << endl;
                break;
            case 2:
                cout << "\n--- View Enrollments ---" << endl;
                cout << "[Enrollment Management Feature - Coming Soon]" << endl;
                break;
            case 3:
                cout << "\n--- Remove Enrollment ---" << endl;
                cout << "[Enrollment Management Feature - Coming Soon]" << endl;
                break;
            case 4:
                cout << "Returning to Main Menu..." << endl;
                return;
            default:
                cout << "Invalid option. Please try again." << endl;
        }
    }
}

void handleAttendanceManagement(AttendanceRecord attendance[], int& attendanceCount, Student students[], int& studentCount, Course courses[], int& courseCount) {
    int choice;
    
    while (true) {
        displayAttendanceMenu();
        cin >> choice;
        cin.ignore();
        
        switch (choice) {
            case 1:
                markAttendance(attendance, attendanceCount, students, studentCount, courses, courseCount);
                break;
            case 2:
                viewAttendanceReport(attendance, attendanceCount);
                break;
            case 3:
                cout << "Returning to Main Menu..." << endl;
                return;
            default:
                cout << "Invalid option. Please try again." << endl;
        }
    }
}

void handleGradesManagement(GradeRecord grades[], int& gradeCount, Student students[], int& studentCount, Course courses[], int& courseCount) {
    int choice;

    while (true) {
        displayGradesMenu();
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                enterMarks(grades, gradeCount, students, studentCount, courses, courseCount);
                break;
            case 2: {
                string courseCode;
                cout << "Enter Course Code: ";
                getline(cin, courseCode);
                computeClassStats(grades, gradeCount, courseCode);
                break;
            }
            case 3:
                cout << "Returning to Main Menu..." << endl;
                return;
            default:
                cout << "Invalid option. Please try again." << endl;
        }
    }
}

int main() {
    const int MAX_RECORDS = 100;
    Student students[MAX_RECORDS];
    int studentCount = 0;

    Course courses[MAX_RECORDS];
    int courseCount = 0;

    AttendanceRecord attendance[MAX_RECORDS];
    int attendanceCount = 0;

    GradeRecord grades[MAX_RECORDS];
    int gradeCount = 0;
    
    Course mockCourse1;
    mockCourse1.courseCode = "CS-101";
    mockCourse1.courseTitle = "Programming Fundamentals";
    mockCourse1.creditHours = 4;
    mockCourse1.maxCapacity = 50;
    mockCourse1.currentEnrollment = 0;
    if (courseCount < MAX_RECORDS) {
        courses[courseCount] = mockCourse1;
        courseCount++;
    }
    
    Course mockCourse2;
    mockCourse2.courseCode = "MATH-201";
    mockCourse2.courseTitle = "Discrete Mathematics";
    mockCourse2.creditHours = 3;
    mockCourse2.maxCapacity = 40;
    mockCourse2.currentEnrollment = 0;
    if (courseCount < MAX_RECORDS) {
        courses[courseCount] = mockCourse2;
        courseCount++;
    }
    
    int mainChoice;
    
    cout << "\n=====================================" << endl;
    cout << "  Welcome to Campus Analytics Engine" << endl;
    cout << "=====================================" << endl;
    
    while (true) {
        displayMainMenu();
        cin >> mainChoice;
        cin.ignore();
        
        switch (mainChoice) {
            case 1:
                handleStudentManagement(students, studentCount);
                break;
            case 2:
                handleCourseManagement(courses, courseCount);
                break;
            case 3:
                handleAttendanceManagement(attendance, attendanceCount, students, studentCount, courses, courseCount);
                break;
            case 4:
                handleGradesManagement(grades, gradeCount, students, studentCount, courses, courseCount);
                break;
            case 5:
                handleReportsMenu(students, studentCount);
                break;
            case 6:
                handleEnrollmentManagement(students, studentCount);
                break;
            case 7:
                cout << "\nThank you for using Campus Analytics Engine!" << endl;
                cout << "Exiting application..." << endl;
                return 0;
            default:
                cout << "Invalid option. Please select 1-7." << endl;
        }
    }
    
    return 0;
}
