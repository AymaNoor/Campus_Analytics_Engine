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

bool courseExists(Course courses[], int& courseCount, const string& courseCode) {
    for (int i = 0; i < courseCount; i++) {
        if (courses[i].courseCode == courseCode) {
            return true;
        }
    }
    return false;
}

void markAttendance(AttendanceRecord attendance[], int& attendanceCount, Student students[], int& studentCount, Course courses[], int& courseCount) {
    string courseCode, date;
    
    cout << "\n=== Mark Attendance ===" << endl;
    
    cout << "Enter Course Code: ";
    getline(cin, courseCode);
    
    if (!courseExists(courses, courseCount, courseCode)) {
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
    
    for (int i = 0; i < studentCount; i++) {
        if (students[i].status == "active") {
            if (attendanceCount >= MAX_ATTENDANCE_RECORDS) {
                cout << "Attendance storage is full. Remaining students were not recorded." << endl;
                break;
            }

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
            
            attendance[attendanceCount] = record;
            attendanceCount++;
            cout << endl;
        }
    }
    
    cout << "Attendance marked successfully for " << courseCode << "!" << endl;
}

void viewAttendanceReport(AttendanceRecord attendance[], int& attendanceCount) {
    string courseCode;
    
    cout << "\n=== View Attendance Report ===" << endl;
    cout << "Enter Course Code: ";
    getline(cin, courseCode);

    int courseAttendanceCount = 0;
    for (int i = 0; i < attendanceCount; i++) {
        if (attendance[i].courseCode == courseCode) {
            courseAttendanceCount++;
        }
    }

    if (courseAttendanceCount == 0) {
        cout << "\nNo attendance records found for course " << courseCode << "." << endl;
        return;
    }
    
    cout << "\n--- Attendance Report for Course " << courseCode << " ---" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "Roll Number   | Date       | Status" << endl;
    cout << "-----------------------------------------------------------" << endl;

    for (int i = 0; i < attendanceCount; i++) {
        if (attendance[i].courseCode == courseCode) {
            cout.width(13);
            cout << left << attendance[i].rollNumber << "| ";
            cout.width(10);
            cout << left << attendance[i].date << "| ";
            cout << attendance[i].status << endl;
        }
    }
    
    cout << "-----------------------------------------------------------" << endl;
}
