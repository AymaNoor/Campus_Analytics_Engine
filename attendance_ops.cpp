#include "attendance_ops.h"
#include <iostream>

using namespace std;

bool isValidDate(const string& date) {
    if (date.length() != 10) return false;
    
    if (date[2] != '-' || date[5] != '-') return false;
    
    for (int i = 0; i < 2; i++) {
        if (!(date[i] >= '0' && date[i] <= '9')) return false;
    }
    for (int i = 3; i < 5; i++) {
        if (!(date[i] >= '0' && date[i] <= '9')) return false;
    }
    for (int i = 6; i < 10; i++) {
        if (!(date[i] >= '0' && date[i] <= '9')) return false;
    }
    
    int day = (date[0] - '0') * 10 + (date[1] - '0');
    int month = (date[3] - '0') * 10 + (date[4] - '0');
    
    if (day < 1 || day > 31 || month < 1 || month > 12) return false;
    
    return true;
}

bool courseExists(const vector<Course>& courses, const string& courseCode) {
    for (int i = 0; i < courses.size(); i++) {
        if (courses[i].courseCode == courseCode) {
            return true;
        }
    }
    return false;
}

void markAttendance(vector<AttendanceRecord>& attendance, const vector<Student>& students, const vector<Course>& courses) {
    string courseCode, date;
    
    cout << "\n=== Mark Attendance ===" << endl;
    
    cout << "Enter Course Code: ";
    getline(cin, courseCode);
    
    if (!courseExists(courses, courseCode)) {
        cout << "Course not found!" << endl;
        return;
    }
    
    cout << "Enter Date (DD-MM-YYYY): ";
    getline(cin, date);
    
    if (!isValidDate(date)) {
        cout << "Invalid date format. Expected DD-MM-YYYY." << endl;
        return;
    }
    
    cout << "\n--- Marking Attendance for Course " << courseCode << " on " << date << " ---" << endl;
    cout << endl;
    
    for (int i = 0; i < students.size(); i++) {
        if (students[i].status == "active") {
            cout << "Roll: " << students[i].roll << " | Name: " << students[i].name << endl;
            cout << "Status (P=Present, A=Absent, L=Leave): ";
            
            string statusInput;
            getline(cin, statusInput);
            
            string attendanceStatus;
            if (statusInput == "P" || statusInput == "p") {
                attendanceStatus = "Present";
            } else if (statusInput == "A" || statusInput == "a") {
                attendanceStatus = "Absent";
            } else if (statusInput == "L" || statusInput == "l") {
                attendanceStatus = "Leave";
            } else {
                cout << "Invalid input. Marking as Absent." << endl;
                attendanceStatus = "Absent";
            }
            
            AttendanceRecord record;
            record.rollNumber = students[i].roll;
            record.courseCode = courseCode;
            record.date = date;
            record.status = attendanceStatus;
            
            attendance.push_back(record);
            cout << endl;
        }
    }
    
    cout << "Attendance marked successfully for " << courseCode << "!" << endl;
}

void viewAttendanceReport(const vector<AttendanceRecord>& attendance) {
    string courseCode;
    
    cout << "\n=== View Attendance Report ===" << endl;
    cout << "Enter Course Code: ";
    getline(cin, courseCode);
    
    vector<AttendanceRecord> courseAttendance;
    
    for (int i = 0; i < attendance.size(); i++) {
        if (attendance[i].courseCode == courseCode) {
            courseAttendance.push_back(attendance[i]);
        }
    }
    
    if (courseAttendance.size() == 0) {
        cout << "\nNo attendance records found for course " << courseCode << "." << endl;
        return;
    }
    
    cout << "\n--- Attendance Report for Course " << courseCode << " ---" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "Roll Number   | Date       | Status" << endl;
    cout << "-----------------------------------------------------------" << endl;
    
    for (int i = 0; i < courseAttendance.size(); i++) {
        cout.width(13);
        cout << left << courseAttendance[i].rollNumber << "| ";
        cout.width(10);
        cout << left << courseAttendance[i].date << "| ";
        cout << courseAttendance[i].status << endl;
    }
    
    cout << "-----------------------------------------------------------" << endl;
}
