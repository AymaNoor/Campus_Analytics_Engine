#include "course_ops.h"
#include <iostream>

using namespace std;

bool isValidCourseCode(const string& code) {
    if (code.length() < 6 || code.length() > 7) return false;
    
    int hyphenPos = -1;
    
    if (code.length() == 6 && code[2] == '-') {
        hyphenPos = 2;
    } else if (code.length() == 7 && code[3] == '-') {
        hyphenPos = 3;
    } else {
        return false;
    }
    
    for (int i = 0; i < hyphenPos; i++) {
        if (!(code[i] >= 'A' && code[i] <= 'Z')) {
            return false;
        }
    }
    
    for (int i = hyphenPos + 1; i < code.length(); i++) {
        if (!(code[i] >= '0' && code[i] <= '9')) {
            return false;
        }
    }
    
    return true;
}

bool isValidCreditHours(int hours) {
    return hours >= 1 && hours <= 4;
}

bool courseCodeExists(Course courses[], int& courseCount, const string& code) {
    for (int i = 0; i < courseCount; i++) {
        if (courses[i].courseCode == code) {
            return true;
        }
    }
    return false;
}

void addCourse(Course courses[], int& courseCount) {
    Course newCourse;
    
    cout << "\n=== Add New Course ===" << endl;

    if (courseCount >= MAX_COURSE_RECORDS) {
        cout << "Course storage is full. Cannot add more records." << endl;
        return;
    }
    
    cout << "Enter Course Code (Format: XX-XXX or XXX-XXX, e.g., CS-101): ";
    getline(cin, newCourse.courseCode);
    
    if (!isValidCourseCode(newCourse.courseCode)) {
        cout << "Invalid course code format. Expected: 2-3 uppercase letters, hyphen, 3 digits." << endl;
        return;
    }
    
    if (courseCodeExists(courses, courseCount, newCourse.courseCode)) {
        cout << "Course code already exists!" << endl;
        return;
    }
    
    cout << "Enter Course Title: ";
    getline(cin, newCourse.courseTitle);
    
    cout << "Enter Credit Hours (1-4): ";
    cin >> newCourse.creditHours;
    cin.ignore();
    
    if (!isValidCreditHours(newCourse.creditHours)) {
        cout << "Invalid credit hours. Must be between 1 and 4." << endl;
        return;
    }
    
    cout << "Enter Maximum Capacity: ";
    cin >> newCourse.maxCapacity;
    cin.ignore();
    
    if (newCourse.maxCapacity <= 0) {
        cout << "Invalid capacity. Must be greater than 0." << endl;
        return;
    }
    
    newCourse.currentEnrollment = 0;
    courses[courseCount] = newCourse;
    courseCount++;
    
    cout << "Course added successfully!" << endl;
}

void listCourses(Course courses[], int& courseCount) {
    if (courseCount == 0) {
        cout << "\n=== Registered Courses ===" << endl;
        cout << "No courses registered yet." << endl;
        return;
    }
    
    cout << "\n=== Registered Courses ===" << endl;
    cout << "----------------------------------------------------------------------" << endl;
    cout << "Code      | Title               | Credits | Capacity | Enrolled | Available" << endl;
    cout << "----------------------------------------------------------------------" << endl;
    
    for (int i = 0; i < courseCount; i++) {
        int availableSeats = courses[i].maxCapacity - courses[i].currentEnrollment;
        
        cout.width(9);
        cout << left << courses[i].courseCode << "| ";
        cout.width(19);
        cout << left << courses[i].courseTitle << "| ";
        cout.width(7);
        cout << left << courses[i].creditHours << "| ";
        cout.width(8);
        cout << left << courses[i].maxCapacity << "| ";
        cout.width(8);
        cout << left << courses[i].currentEnrollment << "| ";
        cout << availableSeats << endl;
    }
    
    cout << "----------------------------------------------------------------------" << endl;
}

void updateCourse(Course courses[], int& courseCount) {
    string code;
    
    cout << "\n=== Update Course ===" << endl;
    cout << "Enter Course Code to Update: ";
    getline(cin, code);
    
    int index = -1;
    for (int i = 0; i < courseCount; i++) {
        if (courses[i].courseCode == code) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        cout << "Course not found." << endl;
        return;
    }
    
    cout << "\nCurrent Course Details:" << endl;
    cout << "Title: " << courses[index].courseTitle << endl;
    cout << "Credit Hours: " << courses[index].creditHours << endl;
    cout << "Max Capacity: " << courses[index].maxCapacity << endl;
    cout << "Current Enrollment: " << courses[index].currentEnrollment << endl;
    
    cout << "\nEnter new Title (or press Enter to skip): ";
    string newTitle;
    getline(cin, newTitle);
    if (newTitle.length() > 0) {
        courses[index].courseTitle = newTitle;
    }
    
    cout << "Enter new Credit Hours (or enter -1 to skip, range 1-4): ";
    int newCredits;
    cin >> newCredits;
    cin.ignore();
    if (newCredits != -1) {
        if (isValidCreditHours(newCredits)) {
            courses[index].creditHours = newCredits;
        } else {
            cout << "Invalid credit hours. Update skipped for this field." << endl;
        }
    }
    
    cout << "Enter new Max Capacity (or enter -1 to skip, must be >= current enrollment): ";
    int newCapacity;
    cin >> newCapacity;
    cin.ignore();
    if (newCapacity != -1) {
        if (newCapacity >= courses[index].currentEnrollment && newCapacity > 0) {
            courses[index].maxCapacity = newCapacity;
        } else {
            cout << "Invalid capacity. Must be >= " << courses[index].currentEnrollment << " and > 0." << endl;
        }
    }
    
    cout << "Course updated successfully!" << endl;
}
