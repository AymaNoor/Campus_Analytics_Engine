#ifndef ATTENDANCE_OPS_H
#define ATTENDANCE_OPS_H

#include <string>
#include <vector>
#include "student_ops.h"
#include "course_ops.h"

const int MAX_ATTENDANCE_RECORDS = 5000;

struct AttendanceRecord {
    std::string rollNumber;
    std::string courseCode;
    std::string date;
    std::string status;   // "Present", "Absent", "Late"
};

// --- File I/O ---
void loadAttendance(AttendanceRecord attendance[], int& attendanceCount);
void saveAttendance(const AttendanceRecord attendance[], int attendanceCount);

// --- M4 Spec functions ---
void markAttendance(AttendanceRecord attendance[], int& attendanceCount,
                    const Student students[], int studentCount,
                    const Course courses[], int courseCount);

double getAttendancePct(const AttendanceRecord attendance[], int attendanceCount,
                        const std::string& roll,
                        const std::string& courseCode);

void getShortageList(const AttendanceRecord attendance[], int attendanceCount,
                     const Student students[], int studentCount,
                     const std::string& courseCode);

bool undoLastSession(AttendanceRecord attendance[], int& attendanceCount,
                     const std::string& courseCode,
                     const std::string& date);

void printDailySheet(const AttendanceRecord attendance[], int attendanceCount,
                     const Student students[], int studentCount,
                     const std::string& courseCode,
                     const std::string& date);

// --- View report ---
void viewAttendanceReport(const AttendanceRecord attendance[], int attendanceCount);

// --- Helpers ---
bool courseExistsInList(const Course courses[], int courseCount, const std::string& courseCode);
bool isValidDate(const std::string& date);

#endif
