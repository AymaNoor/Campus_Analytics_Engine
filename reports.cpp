#include "reports.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <ctime>

using namespace std;

void printMeritList(const Student students[], int studentCount) {
    Student active[MAX_RECORDS];
    int activeCount = 0;

    for (int i = 0; i < studentCount; i++) {
        if (students[i].status == "active") {
            active[activeCount] = students[i];
            activeCount++;
        }
    }

    if (activeCount == 0) {
        cout << "\nNo active students to display." << endl;
        return;
    }

    for (int pass = 0; pass < activeCount - 1; pass++) {
        for (int j = 0; j < activeCount - pass - 1; j++) {
            if (active[j].cgpa < active[j + 1].cgpa) {
                Student temp = active[j];
                active[j] = active[j + 1];
                active[j + 1] = temp;
            }
        }
    }

    cout << setfill('=') << setw(68) << "=" << endl;
    cout << setfill(' ') << setw(46) << "MERIT LIST (Ranked by CGPA)" << endl;
    cout << setfill('=') << setw(68) << "=" << endl;
    cout << left << setw(8) << "Rank"
         << setw(16) << "Roll Number"
         << setw(24) << "Name"
         << setw(14) << "Department"
         << "CGPA" << endl;
    cout << setfill('-') << setw(68) << "-" << endl;
    cout << setfill(' ');

    cout << fixed << setprecision(2);
    for (int i = 0; i < activeCount; i++) {
        cout << left << setw(8) << (i + 1)
             << setw(16) << active[i].roll
             << setw(24) << active[i].name
             << setw(14) << active[i].dept
             << active[i].cgpa << endl;
    }

    cout << setfill('=') << setw(68) << "=" << endl;
    cout << setfill(' ');
}

void printAttendanceDefaulters(const Student students[], int studentCount, const AttendanceRecord attendance[], int attendanceCount) {
    string defRolls[MAX_RECORDS];
    string defNames[MAX_RECORDS];
    string defCourses[MAX_RECORDS];
    double defPcts[MAX_RECORDS];
    int defCount = 0;

    for (int s = 0; s < studentCount; s++) {
        if (students[s].status != "active") continue;

        string seenCourses[MAX_RECORDS];
        int seenCount = 0;

        for (int a = 0; a < attendanceCount; a++) {
            if (attendance[a].rollNumber != students[s].roll) continue;

            bool already = false;
            for (int sc = 0; sc < seenCount; sc++) {
                if (seenCourses[sc] == attendance[a].courseCode) {
                    already = true;
                    break;
                }
            }
            if (!already && seenCount < MAX_RECORDS) {
                seenCourses[seenCount] = attendance[a].courseCode;
                seenCount++;
            }
        }

        for (int c = 0; c < seenCount; c++) {
            int total = 0;
            int present = 0;

            for (int a = 0; a < attendanceCount; a++) {
                if (attendance[a].rollNumber == students[s].roll &&
                    attendance[a].courseCode == seenCourses[c]) {
                    total++;
                    if (attendance[a].status == "Present") {
                        present++;
                    }
                }
            }

            if (total == 0) continue;

            double pct = (double)present / total * 100.0;
            if (pct < 75.0 && defCount < MAX_RECORDS) {
                defRolls[defCount] = students[s].roll;
                defNames[defCount] = students[s].name;
                defCourses[defCount] = seenCourses[c];
                defPcts[defCount] = pct;
                defCount++;
            }
        }
    }

    if (defCount == 0) {
        cout << "\nNo attendance defaulters found." << endl;
        return;
    }

    cout << setfill('=') << setw(72) << "=" << endl;
    cout << setfill(' ') << setw(48) << "ATTENDANCE DEFAULTERS (< 75%)" << endl;
    cout << setfill('=') << setw(72) << "=" << endl;
    cout << left << setw(16) << "Roll Number"
         << setw(24) << "Name"
         << setw(14) << "Course Code"
         << "Attendance %" << endl;
    cout << setfill('-') << setw(72) << "-" << endl;
    cout << setfill(' ');

    cout << fixed << setprecision(1);
    for (int i = 0; i < defCount; i++) {
        cout << left << setw(16) << defRolls[i]
             << setw(24) << defNames[i]
             << setw(14) << defCourses[i]
             << defPcts[i] << "%" << endl;
    }

    cout << setfill('=') << setw(72) << "=" << endl;
    cout << setfill(' ');
}

string currentDateStr() {
    time_t t = time(0);
    struct tm* now = localtime(&t);

    string dd = (now->tm_mday < 10 ? "0" : "") + to_string(now->tm_mday);
    string mm = ((now->tm_mon + 1) < 10 ? "0" : "") + to_string(now->tm_mon + 1);
    string yyyy = to_string(now->tm_year + 1900);

    return dd + "-" + mm + "-" + yyyy;
}

void printFeeDefaulters(const FeeRecord fees[], int feeCount, const Student students[], int studentCount) {
    string rolls[MAX_RECORDS];
    string names[MAX_RECORDS];
    double outstanding[MAX_RECORDS];
    int weeksOverdue[MAX_RECORDS];
    int defCount = 0;

    string today = currentDateStr();

    for (int i = 0; i < feeCount; i++) {
        double remain = fees[i].amountDue - fees[i].amountPaid;
        if (remain <= 0.0) continue;

        string sname = "Unknown";
        for (int s = 0; s < studentCount; s++) {
            if (students[s].roll == fees[i].rollNumber) {
                sname = students[s].name;
                break;
            }
        }

        int overdueDays = daysBetween(fees[i].dueDate, today);
        if (overdueDays <= 0) continue;

        if (defCount < MAX_RECORDS) {
            rolls[defCount] = fees[i].rollNumber;
            names[defCount] = sname;
            outstanding[defCount] = remain;
            weeksOverdue[defCount] = overdueDays / 7;
            defCount++;
        }
    }

    if (defCount == 0) {
        cout << "\nNo fee defaulters found." << endl;
        return;
    }

    cout << setfill('=') << setw(72) << "=" << endl;
    cout << setfill(' ') << setw(44) << "FEE DEFAULTERS REPORT" << endl;
    cout << setfill('=') << setw(72) << "=" << endl;
    cout << left << setw(16) << "Roll Number"
         << setw(24) << "Name"
         << setw(18) << "Outstanding ($)"
         << "Weeks Overdue" << endl;
    cout << setfill('-') << setw(72) << "-" << endl;
    cout << setfill(' ');

    cout << fixed << setprecision(2);
    for (int i = 0; i < defCount; i++) {
        cout << left << setw(16) << rolls[i]
             << setw(24) << names[i]
             << right << setw(14) << outstanding[i]
             << setw(18) << weeksOverdue[i] << endl;
    }

    cout << setfill('=') << setw(72) << "=" << endl;
    cout << setfill(' ');
}

void printSemesterResult(const Student students[], int studentCount, const GradeRecord grades[], int gradeCount, const AttendanceRecord attendance[], int attendanceCount) {
    cout << setfill('=') << setw(92) << "=" << endl;
    cout << setfill(' ') << setw(52) << "SEMESTER RESULT SHEET" << endl;
    cout << setfill('=') << setw(92) << "=" << endl;

    cout << left << setw(6) << "S.No"
         << setw(16) << "Roll Number"
         << setw(22) << "Name"
         << setw(14) << "Course Code"
         << setw(10) << "Grade"
         << setw(10) << "GPA"
         << setw(14) << "Attendance" << endl;

    cout << setfill('-') << setw(92) << "-" << endl;
    cout << setfill(' ');

    int displayCount = 0;

    for (int s = 0; s < studentCount; s++) {
        if (students[s].status != "active") continue;

        for (int a = 0; a < gradeCount; a++) {
            if (grades[a].rollNumber != students[s].roll) continue;

            displayCount++;

            double sgpa = computeGPA(grades, gradeCount, students[s].roll);

            int attTot = 0;
            int attPres = 0;
            for (int ar = 0; ar < attendanceCount; ar++) {
                if (attendance[ar].rollNumber == students[s].roll &&
                    attendance[ar].courseCode == grades[a].courseCode) {
                    attTot++;
                    if (attendance[ar].status == "Present") {
                        attPres++;
                    }
                }
            }

            string attStatus;
            if (attTot == 0) {
                attStatus = "N/A";
            } else {
                double attPct = (double)attPres / attTot * 100.0;
                if (attPct >= 75.0) {
                    attStatus = "Compliant";
                } else {
                    attStatus = "Defaulter";
                }
            }

            cout << left << setw(6) << displayCount
                 << setw(16) << students[s].roll
                 << setw(22) << students[s].name
                 << setw(14) << grades[a].courseCode
                 << setw(10) << grades[a].letterGrade
                 << setw(10) << fixed << setprecision(2) << sgpa
                 << attStatus << endl;
        }
    }

    if (displayCount == 0) {
        cout << left << setw(6) << "-"
             << setw(16) << "-"
             << setw(22) << "No records found"
             << setw(14) << "-"
             << setw(10) << "-"
             << setw(10) << "-"
             << "-" << endl;
    }

    cout << setfill('=') << setw(92) << "=" << endl;
    cout << setfill(' ');
}

void printDepartmentSummary(const Student students[], int studentCount) {
    string depts[MAX_RECORDS];
    int deptCounts[MAX_RECORDS];
    double deptCgpaSum[MAX_RECORDS];
    int deptPassCount[MAX_RECORDS];
    int uniqueCount = 0;

    for (int i = 0; i < MAX_RECORDS; i++) {
        deptCounts[i] = 0;
        deptCgpaSum[i] = 0.0;
        deptPassCount[i] = 0;
    }

    for (int i = 0; i < studentCount; i++) {
        if (students[i].status != "active") continue;

        int idx = -1;
        for (int d = 0; d < uniqueCount; d++) {
            if (depts[d] == students[i].dept) {
                idx = d;
                break;
            }
        }

        if (idx == -1) {
            if (uniqueCount < MAX_RECORDS) {
                depts[uniqueCount] = students[i].dept;
                idx = uniqueCount;
                uniqueCount++;
            }
        }

        if (idx != -1) {
            deptCounts[idx]++;
            deptCgpaSum[idx] += students[i].cgpa;
            if (students[i].cgpa >= 2.0) {
                deptPassCount[idx]++;
            }
        }
    }

    if (uniqueCount == 0) {
        cout << "\nNo active students found." << endl;
        return;
    }

    cout << setfill('=') << setw(78) << "=" << endl;
    cout << setfill(' ') << setw(48) << "DEPARTMENT WISE SUMMARY" << endl;
    cout << setfill('=') << setw(78) << "=" << endl;
    cout << left << setw(6) << "S.No"
         << setw(22) << "Department"
         << setw(16) << "Total Students"
         << setw(16) << "Avg CGPA"
         << "Pass Rate" << endl;
    cout << setfill('-') << setw(78) << "-" << endl;
    cout << setfill(' ');

    cout << fixed << setprecision(2);
    for (int d = 0; d < uniqueCount; d++) {
        double avgCgpa = deptCgpaSum[d] / deptCounts[d];
        double passRate = (double)deptPassCount[d] / deptCounts[d] * 100.0;

        cout << left << setw(6) << (d + 1)
             << setw(22) << depts[d]
             << setw(16) << deptCounts[d]
             << setw(16) << avgCgpa
             << passRate << "%" << endl;
    }

    cout << setfill('=') << setw(78) << "=" << endl;
    cout << setfill(' ');
}

void exportReportToFile(int reportChoice, const Student students[], int studentCount, const FeeRecord fees[], int feeCount, const GradeRecord grades[], int gradeCount, const AttendanceRecord attendance[], int attendanceCount) {
    string filename;
    switch (reportChoice) {
        case 1: filename = "merit_list.txt"; break;
        case 2: filename = "attendance_defaulters.txt"; break;
        case 3: filename = "fee_defaulters.txt"; break;
        case 4: filename = "semester_result.txt"; break;
        case 5: filename = "department_summary.txt"; break;
        default:
            cout << "Invalid report choice for export." << endl;
            return;
    }

    ofstream outFile(filename);
    if (!outFile.is_open()) {
        cout << "Error: Could not create file " << filename << endl;
        return;
    }

    streambuf* originalBuf = cout.rdbuf();
    cout.rdbuf(outFile.rdbuf());

    switch (reportChoice) {
        case 1:
            printMeritList(students, studentCount);
            break;
        case 2:
            printAttendanceDefaulters(students, studentCount, attendance, attendanceCount);
            break;
        case 3:
            printFeeDefaulters(fees, feeCount, students, studentCount);
            break;
        case 4:
            printSemesterResult(students, studentCount, grades, gradeCount, attendance, attendanceCount);
            break;
        case 5:
            printDepartmentSummary(students, studentCount);
            break;
    }

    cout.rdbuf(originalBuf);
    outFile.close();

    cout << "\nReport exported successfully to " << filename << endl;
}
