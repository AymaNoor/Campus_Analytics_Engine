#include "attendance_ops.h"
#include "filehandler.h"
#include <iostream>
#include <iomanip>

using namespace std;

// ===================================================================
// Validation helpers
// ===================================================================
bool isValidDate(const string& date) {
    if (date.length() != 10) return false;
    if (date[2] != '-' || date[5] != '-') return false;
    for (int i = 0; i < 2; i++)
        if (!(date[i] >= '0' && date[i] <= '9')) return false;
    for (int i = 3; i < 5; i++)
        if (!(date[i] >= '0' && date[i] <= '9')) return false;
    for (int i = 6; i < 10; i++)
        if (!(date[i] >= '0' && date[i] <= '9')) return false;
    int day   = (date[0]-'0')*10 + (date[1]-'0');
    int month = (date[3]-'0')*10 + (date[4]-'0');
    return (day >= 1 && day <= 31 && month >= 1 && month <= 12);
}

bool courseExistsInList(const Course courses[], int courseCount, const string& courseCode) {
    for (int i = 0; i < courseCount; i++)
        if (courses[i].courseCode == courseCode) return true;
    return false;
}

// ===================================================================
// loadAttendance — Read attendance_log.txt
// Columns: log_id,roll_no,course_code,session_date,status  (indices 0-4)
// Status values: P=Present, A=Absent, L=Late
// ===================================================================
void loadAttendance(AttendanceRecord attendance[], int& attendanceCount) {
    attendanceCount = 0;
    vector<string> flat = readTXT("attendance_log.txt");
    const int COLS = 5;
    int rowCount = (int)flat.size() / COLS;

    for (int r = 0; r < rowCount && attendanceCount < MAX_ATTENDANCE_RECORDS; r++) {
        AttendanceRecord rec;
        // col0=log_id (skip), col1=roll_no, col2=course_code, col3=date, col4=status
        rec.rollNumber = flat[r * COLS + 1];
        rec.courseCode = flat[r * COLS + 2];
        rec.date       = flat[r * COLS + 3];
        // Normalize short codes to full words
        string rawStatus = flat[r * COLS + 4];
        if      (rawStatus == "P") rec.status = "Present";
        else if (rawStatus == "L") rec.status = "Late";
        else                        rec.status = "Absent";
        attendance[attendanceCount] = rec;
        attendanceCount++;
    }
    cout << "Loaded " << attendanceCount << " attendance records from attendance_log.txt\n";
}

// ===================================================================
// saveAttendance — Write all records back to attendance_log.txt
// Writes 5-column format: log_id,roll_no,course_code,session_date,status
// Status stored as P/A/L (short codes)
// ===================================================================
void saveAttendance(const AttendanceRecord attendance[], int attendanceCount) {
    vector<vector<string>> rows;
    for (int i = 0; i < attendanceCount; i++) {
        vector<string> row;
        // Build log ID: L00001 format
        string logId = "L";
        int num = i + 1;
        if (num < 10)    logId += "0000";
        else if (num < 100)  logId += "000";
        else if (num < 1000) logId += "00";
        else if (num < 10000) logId += "0";
        // Append number manually
        string numStr;
        int tmp = num;
        if (tmp == 0) numStr = "0";
        while (tmp > 0) { numStr = char('0' + tmp % 10) + numStr; tmp /= 10; }
        logId += numStr;

        row.push_back(logId);
        row.push_back(attendance[i].rollNumber);
        row.push_back(attendance[i].courseCode);
        row.push_back(attendance[i].date);
        // Convert full word back to short code
        string sc = "A";
        if (attendance[i].status == "Present") sc = "P";
        else if (attendance[i].status == "Late") sc = "L";
        row.push_back(sc);
        rows.push_back(row);
    }
    writeTXT("attendance_log.txt", "log_id,roll_no,course_code,session_date,status", rows);
}

// ===================================================================
// markAttendance — Iterates enrolled students, prompts P/A/L for each.
// Saves a backup vector before writing. Appends rows to attendance_log.txt
// ===================================================================
void markAttendance(AttendanceRecord attendance[], int& attendanceCount,
                    const Student students[], int studentCount,
                    const Course courses[], int courseCount) {
    cout << "\n=== Mark Attendance ===" << endl;

    cout << "Enter Course Code: ";
    string courseCode = safeReadLine();
    if (courseCode.empty()) { cout << "Input cancelled.\n"; return; }

    if (!courseExistsInList(courses, courseCount, courseCode)) {
        cout << "Course not found!" << endl;
        return;
    }

    cout << "Enter Date (DD-MM-YYYY): ";
    string date = safeReadLine();
    if (date.empty()) { cout << "Input cancelled.\n"; return; }

    if (!isValidDate(date)) {
        cout << "Invalid date format. Expected DD-MM-YYYY." << endl;
        return;
    }

    // --- Pre-session watermark is implicit: undoLastSession uses course+date to remove ---

    cout << "\n--- Marking Attendance for Course " << courseCode
         << " on " << date << " ---\n" << endl;

    // Iterate through enrolled students (read enrollments.txt)
    vector<string> enrollFlat = readTXT("enrollments.txt");
    const int E_COLS = 6;
    int eRows = (int)enrollFlat.size() / E_COLS;

    int marked = 0;
    for (int e = 0; e < eRows; e++) {
        string eRoll   = enrollFlat[e * E_COLS + 1];
        string eCourse = enrollFlat[e * E_COLS + 2];
        string eStatus = enrollFlat[e * E_COLS + 5];

        if (eCourse != courseCode || eStatus != "active") continue;

        if (attendanceCount >= MAX_ATTENDANCE_RECORDS) {
            cout << "Attendance storage full. Remaining students not recorded." << endl;
            break;
        }

        // Find student name
        string studentName = eRoll;
        for (int s = 0; s < studentCount; s++) {
            if (students[s].roll == eRoll) { studentName = students[s].name; break; }
        }

        cout << "Roll: " << eRoll << " | Name: " << studentName << endl;
        cout << "Status (P=Present, A=Absent, L=Late): ";
        string statusInput = safeReadLine();
        if (cin.eof()) { cout << "[EOF] Attendance marking cancelled.\n"; break; }

        string attendanceStatus = "Absent";
        if (statusInput == "P" || statusInput == "p") attendanceStatus = "Present";
        else if (statusInput == "L" || statusInput == "l") attendanceStatus = "Late";

        AttendanceRecord rec;
        rec.rollNumber = eRoll;
        rec.courseCode = courseCode;
        rec.date       = date;
        rec.status     = attendanceStatus;

        attendance[attendanceCount] = rec;
        attendanceCount++;
        marked++;

        // Append immediately to file (5-column format)
        vector<string> row;
        // Generate log ID based on current count
        string logId = "L";
        int newNum = attendanceCount; // already incremented
        if (newNum < 10)    logId += "0000";
        else if (newNum < 100)  logId += "000";
        else if (newNum < 1000) logId += "00";
        else if (newNum < 10000) logId += "0";
        string numStr2;
        int tmp2 = newNum;
        while (tmp2 > 0) { numStr2 = char('0' + tmp2 % 10) + numStr2; tmp2 /= 10; }
        logId += numStr2;

        row.push_back(logId);
        row.push_back(rec.rollNumber);
        row.push_back(rec.courseCode);
        row.push_back(rec.date);
        // Convert to short code
        string sc2 = "A";
        if (rec.status == "Present") sc2 = "P";
        else if (rec.status == "Late") sc2 = "L";
        row.push_back(sc2);
        appendTXT("attendance_log.txt", row);

        cout << endl;
    }

    if (marked == 0) {
        cout << "No enrolled students found for this course." << endl;
    } else {
        cout << "Attendance marked for " << marked << " student(s). Course: " << courseCode << endl;
    }
}

// ===================================================================
// getAttendancePct — (present + 0.5 * late) / totalSessions * 100
// ===================================================================
double getAttendancePct(const AttendanceRecord attendance[], int attendanceCount,
                        const string& roll, const string& courseCode) {
    int total   = 0;
    double score = 0.0;

    for (int i = 0; i < attendanceCount; i++) {
        if (attendance[i].rollNumber == roll &&
            attendance[i].courseCode == courseCode) {
            total++;
            if (attendance[i].status == "Present") score += 1.0;
            else if (attendance[i].status == "Late") score += 0.5;
        }
    }
    if (total == 0) return 0.0;
    return (score / total) * 100.0;
}

// ===================================================================
// getShortageList — students with attendance < 75% in a given course
// ===================================================================
void getShortageList(const AttendanceRecord attendance[], int attendanceCount,
                     const Student students[], int studentCount,
                     const string& courseCode) {
    cout << "\n=== Attendance Shortage List: " << courseCode << " (<75%) ===" << endl;
    cout << "----------------------------------------------" << endl;
    cout << left << setw(14) << "Roll" << setw(22) << "Name" << "Attendance%" << endl;
    cout << "----------------------------------------------" << endl;

    int count = 0;
    for (int s = 0; s < studentCount; s++) {
        if (students[s].status != "active") continue;
        double pct = getAttendancePct(attendance, attendanceCount,
                                      students[s].roll, courseCode);
        // Only show if they have any records (pct could be 0 if no records)
        // Check if student has any record in this course
        bool hasRecord = false;
        for (int a = 0; a < attendanceCount; a++) {
            if (attendance[a].rollNumber == students[s].roll &&
                attendance[a].courseCode == courseCode) {
                hasRecord = true; break;
            }
        }
        if (!hasRecord) continue;

        if (pct < 75.0) {
            cout << left << setw(14) << students[s].roll
                 << setw(22) << students[s].name
                 << fixed << setprecision(1) << pct << "%" << endl;
            count++;
        }
    }
    if (count == 0) cout << "No shortage students found." << endl;
    cout << "----------------------------------------------" << endl;
}

// ===================================================================
// undoLastSession — remove all records for a given course+date.
// Returns false if no such session exists.
// ===================================================================
bool undoLastSession(AttendanceRecord attendance[], int& attendanceCount,
                     const string& courseCode, const string& date) {
    int newCount = 0;
    bool found = false;

    for (int i = 0; i < attendanceCount; i++) {
        if (attendance[i].courseCode == courseCode &&
            attendance[i].date == date) {
            found = true; // skip (remove) this record
        } else {
            attendance[newCount] = attendance[i];
            newCount++;
        }
    }

    if (!found) {
        cout << "No session found for course " << courseCode
             << " on " << date << endl;
        return false;
    }

    attendanceCount = newCount;
    saveAttendance(attendance, attendanceCount);
    cout << "Session for " << courseCode << " on " << date << " has been undone." << endl;
    return true;
}

// ===================================================================
// printDailySheet — formatted table of all students for a course+date
// ===================================================================
void printDailySheet(const AttendanceRecord attendance[], int attendanceCount,
                     const Student students[], int studentCount,
                     const string& courseCode, const string& date) {
    cout << "\n=== Daily Attendance Sheet ===" << endl;
    cout << "Course: " << courseCode << "  |  Date: " << date << endl;
    cout << "--------------------------------------------" << endl;
    cout << left << setw(14) << "Roll" << setw(22) << "Name" << "Status" << endl;
    cout << "--------------------------------------------" << endl;

    int count = 0;
    for (int a = 0; a < attendanceCount; a++) {
        if (attendance[a].courseCode == courseCode &&
            attendance[a].date == date) {
            // Find student name
            string name = attendance[a].rollNumber;
            for (int s = 0; s < studentCount; s++) {
                if (students[s].roll == attendance[a].rollNumber) {
                    name = students[s].name; break;
                }
            }
            cout << left << setw(14) << attendance[a].rollNumber
                 << setw(22) << name
                 << attendance[a].status << endl;
            count++;
        }
    }
    if (count == 0) cout << "No records for this date." << endl;
    cout << "--------------------------------------------" << endl;
}

// ===================================================================
// viewAttendanceReport — general report for a course (all dates)
// ===================================================================
void viewAttendanceReport(const AttendanceRecord attendance[], int attendanceCount) {
    cout << "\n=== View Attendance Report ===" << endl;
    cout << "Enter Course Code: ";
    string courseCode = safeReadLine();
    if (courseCode.empty()) { cout << "Input cancelled.\n"; return; }

    int found = 0;
    for (int i = 0; i < attendanceCount; i++)
        if (attendance[i].courseCode == courseCode) found++;

    if (found == 0) {
        cout << "No attendance records found for course " << courseCode << "." << endl;
        return;
    }

    cout << "\n--- Attendance Report: " << courseCode << " ---" << endl;
    cout << "----------------------------------------------" << endl;
    cout << left << setw(14) << "Roll"
         << setw(12) << "Date" << "Status" << endl;
    cout << "----------------------------------------------" << endl;

    for (int i = 0; i < attendanceCount; i++) {
        if (attendance[i].courseCode == courseCode) {
            cout << left << setw(14) << attendance[i].rollNumber
                 << setw(12) << attendance[i].date
                 << attendance[i].status << endl;
        }
    }
    cout << "----------------------------------------------" << endl;
}
