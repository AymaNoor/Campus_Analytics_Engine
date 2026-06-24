#include <iostream>
#include <vector>
#include "student_ops.h"
#include "course_ops.h"

using namespace std;

void displayMainMenu() {
    cout << "\n========================================" << endl;
    cout << "    Campus Analytics Engine - Main Menu  " << endl;
    cout << "========================================" << endl;
    cout << "1. Student Management" << endl;
    cout << "2. Course Management" << endl;
    cout << "3. Reports & Analytics" << endl;
    cout << "4. Enrollment Management" << endl;
    cout << "5. Exit Application" << endl;
    cout << "----------------------------------------" << endl;
    cout << "Select an option (1-5): ";
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

void handleSearchStudent(vector<Student>& students) {
    int choice;
    
    while (true) {
        displaySearchMenu();
        cin >> choice;
        cin.ignore();
        
        switch (choice) {
            case 1:
                searchByRoll(students);
                break;
            case 2:
                searchByName(students);
                break;
            case 3:
                cout << "Returning to Student Management Menu..." << endl;
                return;
            default:
                cout << "Invalid option. Please try again." << endl;
        }
    }
}

void handleStudentManagement(vector<Student>& students) {
    int choice;
    
    while (true) {
        displayStudentMenu();
        cin >> choice;
        cin.ignore();
        
        switch (choice) {
            case 1:
                addStudent(students);
                break;
            case 2:
                handleSearchStudent(students);
                break;
            case 3:
                updateStudent(students);
                break;
            case 4:
                softDelete(students);
                break;
            case 5:
                listActiveStudents(students);
                break;
            case 6:
                cout << "Returning to Main Menu..." << endl;
                return;
            default:
                cout << "Invalid option. Please try again." << endl;
        }
    }
}

void handleCourseManagement(vector<Course>& courses) {
    int choice;
    
    while (true) {
        displayCourseMenu();
        cin >> choice;
        cin.ignore();
        
        switch (choice) {
            case 1:
                addCourse(courses);
                break;
            case 2:
                listCourses(courses);
                break;
            case 3:
                updateCourse(courses);
                break;
            case 4:
                cout << "Returning to Main Menu..." << endl;
                return;
            default:
                cout << "Invalid option. Please try again." << endl;
        }
    }
}

void handleReportsMenu(vector<Student>& students) {
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

void handleEnrollmentManagement(vector<Student>& students) {
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

int main() {
    vector<Student> students;
    vector<Course> courses;
    
    Course mockCourse1;
    mockCourse1.courseCode = "CS-101";
    mockCourse1.courseTitle = "Programming Fundamentals";
    mockCourse1.creditHours = 4;
    mockCourse1.maxCapacity = 50;
    mockCourse1.currentEnrollment = 0;
    courses.push_back(mockCourse1);
    
    Course mockCourse2;
    mockCourse2.courseCode = "MATH-201";
    mockCourse2.courseTitle = "Discrete Mathematics";
    mockCourse2.creditHours = 3;
    mockCourse2.maxCapacity = 40;
    mockCourse2.currentEnrollment = 0;
    courses.push_back(mockCourse2);
    
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
                handleStudentManagement(students);
                break;
            case 2:
                handleCourseManagement(courses);
                break;
            case 3:
                handleReportsMenu(students);
                break;
            case 4:
                handleEnrollmentManagement(students);
                break;
            case 5:
                cout << "\nThank you for using Campus Analytics Engine!" << endl;
                cout << "Exiting application..." << endl;
                return 0;
            default:
                cout << "Invalid option. Please select 1-5." << endl;
        }
    }
    
    return 0;
}
