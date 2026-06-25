#include "reports.h"
#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

// NOTE: No #include <ctime> — dates are passed in as strings (DD-MM-YYYY)

// ===================================================================
// printMeritList — active students sorted by CGPA descending
// ===================================================================
void printMeritList(const Student students[], int studentCount) {
    Student active[MAX_STUDENT_RECORDS];
    int activeCount = 0;

    for (int i = 0; i < studentCount; i++)
        if (students[i].status == "active")
            active[activeCount++] = students[i];

    if (activeCount == 0) {
        cout << "\nNo active students to display." << endl;
        return;
    }

    // Bubble sort descending by CGPA
    for (int pass = 0; pass < activeCount - 1; pass++)
        for (int j = 0; j < activeCount - pass - 1; j++)
            if (active[j].cgpa < active[j+1].cgpa) {
                Student tmp = active[j];
                active[j] = active[j+1];
                active[j+1] = tmp;
            }

    cout << setfill('=') << setw(70) << "=" << endl;
    cout << setfill(' ') << setw(46) << "MERIT LIST (Ranked by CGPA)" << endl;
    cout << setfill('=') << setw(70) << "=" << endl;
    cout << left << setw(6)  << "Rank"
         << setw(14) << "Roll"
         << setw(24) << "Name"
         << setw(16) << "Department"
         << "CGPA" << endl;
    cout << setfill('-') << setw(70) << "-" << endl;
    cout << setfill(' ');

    cout << fixed << setprecision(2);
    for (int i = 0; i < activeCount; i++) {
        cout << left << setw(6)  << (i+1)
             << setw(14) << active[i].roll
             << setw(24) << active[i].name
             << setw(16) << active[i].dept
             << active[i].cgpa << endl;
    }
    cout << setfill('=') << setw(70) << "=" << endl;
    cout << setfill(' ');
}

// ===================================================================
// printAttendanceDefaulters — students with any course < 75% attendance
// ===================================================================
void printAttendanceDefaulters(const Student students[], int studentCount,
                               const AttendanceRecord attendance[], int attendanceCount) {
    const int MAX_DEF = 500;
    string defRolls[MAX_DEF];
    string defNames[MAX_DEF];
    string defCourses[MAX_DEF];
    double defPcts[MAX_DEF];
    int    defCount = 0;

    for (int s = 0; s < studentCount; s++) {
        if (students[s].status != "active") continue;

        // Collect unique course codes for this student
        string seenCourses[MAX_DEF];
        int seenCount = 0;

        for (int a = 0; a < attendanceCount; a++) {
            if (attendance[a].rollNumber != students[s].roll) continue;
            bool already = false;
            for (int sc = 0; sc < seenCount; sc++) {
                if (seenCourses[sc] == attendance[a].courseCode) { already = true; break; }
            }
            if (!already && seenCount < MAX_DEF)
                seenCourses[seenCount++] = attendance[a].courseCode;
        }

        for (int c = 0; c < seenCount; c++) {
            int total = 0, present = 0;
            for (int a = 0; a < attendanceCount; a++) {
                if (attendance[a].rollNumber == students[s].roll &&
                    attendance[a].courseCode == seenCourses[c]) {
                    total++;
                    if (attendance[a].status == "Present") present++;
                    else if (attendance[a].status == "Late") present++; // Late counts partially
                }
            }
            if (total == 0) continue;
            double pct = (double)present / total * 100.0;
            if (pct < 75.0 && defCount < MAX_DEF) {
                defRolls[defCount]   = students[s].roll;
                defNames[defCount]   = students[s].name;
                defCourses[defCount] = seenCourses[c];
                defPcts[defCount]    = pct;
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
         << "Attendance%" << endl;
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

// ===================================================================
// printFeeDefaulters — todayDate: DD-MM-YYYY (no ctime used here)
// ===================================================================
void printFeeDefaulters(const FeeRecord fees[], int feeCount,
                        const Student students[], int studentCount,
                        const string& todayDate) {
    const int MAX_DEF = 200;
    string rolls[MAX_DEF];
    string names[MAX_DEF];
    double outstanding[MAX_DEF];
    int    weeksOverdue[MAX_DEF];
    int    defCount = 0;

    for (int i = 0; i < feeCount; i++) {
        double remain = fees[i].amountDue - fees[i].amountPaid;
        if (remain <= 0.0) continue;

        int overdueDays = daysBetween(fees[i].dueDate, todayDate);
        if (overdueDays <= 0) continue;

        string sname = "Unknown";
        for (int s = 0; s < studentCount; s++) {
            if (students[s].roll == fees[i].rollNumber) {
                sname = students[s].name; break;
            }
        }

        if (defCount < MAX_DEF) {
            rolls[defCount]        = fees[i].rollNumber;
            names[defCount]        = sname;
            outstanding[defCount]  = remain;
            weeksOverdue[defCount] = overdueDays / 7;
            defCount++;
        }
    }

    if (defCount == 0) {
        cout << "\nNo fee defaulters found (as of " << todayDate << ")." << endl;
        return;
    }

    cout << setfill('=') << setw(72) << "=" << endl;
    cout << setfill(' ') << setw(44) << "FEE DEFAULTERS REPORT" << endl;
    cout << setfill('=') << setw(72) << "=" << endl;
    cout << left  << setw(16) << "Roll Number"
         << setw(24) << "Name"
         << right << setw(18) << "Outstanding (PKR)"
         << setw(14) << "Wks Overdue" << endl;
    cout << setfill('-') << setw(72) << "-" << endl;
    cout << setfill(' ');

    cout << fixed << setprecision(2);
    for (int i = 0; i < defCount; i++) {
        cout << left  << setw(16) << rolls[i]
             << setw(24) << names[i]
             << right << setw(14) << outstanding[i]
             << setw(14) << weeksOverdue[i] << endl;
    }
    cout << setfill('=') << setw(72) << "=" << endl;
    cout << setfill(' ');
}

// ===================================================================
// printSemesterResult — full result sheet with GPA + attendance status
// ===================================================================
void printSemesterResult(const Student students[], int studentCount,
                         const GradeRecord grades[], int gradeCount,
                         const AttendanceRecord attendance[], int attendanceCount,
                         const Course courses[], int courseCount) {
    cout << setfill('=') << setw(92) << "=" << endl;
    cout << setfill(' ') << setw(52) << "SEMESTER RESULT SHEET" << endl;
    cout << setfill('=') << setw(92) << "=" << endl;
    cout << left << setw(6)  << "S.No"
         << setw(14) << "Roll"
         << setw(22) << "Name"
         << setw(12) << "Course"
         << setw(8)  << "Grade"
         << setw(8)  << "GPA"
         << "Attendance" << endl;
    cout << setfill('-') << setw(92) << "-" << endl;
    cout << setfill(' ');

    int displayCount = 0;

    for (int s = 0; s < studentCount; s++) {
        if (students[s].status != "active") continue;

        for (int g = 0; g < gradeCount; g++) {
            if (grades[g].rollNumber != students[s].roll) continue;

            displayCount++;

            double sgpa = computeGPA(grades, gradeCount, students[s].roll, courses, courseCount);

            int attTot = 0, attPres = 0;
            for (int ar = 0; ar < attendanceCount; ar++) {
                if (attendance[ar].rollNumber == students[s].roll &&
                    attendance[ar].courseCode == grades[g].courseCode) {
                    attTot++;
                    if (attendance[ar].status == "Present") attPres++;
                }
            }

            string attStatus = (attTot == 0) ? "N/A"
                             : ((double)attPres / attTot * 100.0 >= 75.0 ? "Compliant" : "Defaulter");

            cout << left << setw(6)  << displayCount
                 << setw(14) << students[s].roll
                 << setw(22) << students[s].name
                 << setw(12) << grades[g].courseCode
                 << setw(8)  << grades[g].letterGrade
                 << setw(8)  << fixed << setprecision(2) << sgpa
                 << attStatus << endl;
        }
    }

    if (displayCount == 0)
        cout << setw(6) << "-" << setw(14) << "-" << setw(22) << "No records found" << endl;

    cout << setfill('=') << setw(92) << "=" << endl;
    cout << setfill(' ');
}

// ===================================================================
// printDepartmentSummary
// ===================================================================
void printDepartmentSummary(const Student students[], int studentCount) {
    const int MAX_DEPT = 50;
    string depts[MAX_DEPT];
    int    deptCounts[MAX_DEPT]   = {0};
    double deptCgpaSum[MAX_DEPT]  = {0.0};
    int    deptPassCount[MAX_DEPT]= {0};
    int    uniqueCount = 0;

    for (int i = 0; i < studentCount; i++) {
        if (students[i].status != "active") continue;
        int idx = -1;
        for (int d = 0; d < uniqueCount; d++) {
            if (depts[d] == students[i].dept) { idx = d; break; }
        }
        if (idx == -1 && uniqueCount < MAX_DEPT) {
            depts[uniqueCount] = students[i].dept;
            idx = uniqueCount++;
        }
        if (idx != -1) {
            deptCounts[idx]++;
            deptCgpaSum[idx] += students[i].cgpa;
            if (students[i].cgpa >= 2.0) deptPassCount[idx]++;
        }
    }

    if (uniqueCount == 0) { cout << "\nNo active students found." << endl; return; }

    cout << setfill('=') << setw(78) << "=" << endl;
    cout << setfill(' ') << setw(48) << "DEPARTMENT WISE SUMMARY" << endl;
    cout << setfill('=') << setw(78) << "=" << endl;
    cout << left << setw(6) << "S.No"
         << setw(24) << "Department"
         << setw(16) << "Total Students"
         << setw(12) << "Avg CGPA"
         << "Pass Rate" << endl;
    cout << setfill('-') << setw(78) << "-" << endl;
    cout << setfill(' ');

    cout << fixed << setprecision(2);
    for (int d = 0; d < uniqueCount; d++) {
        double avgCgpa  = deptCgpaSum[d] / deptCounts[d];
        double passRate = (double)deptPassCount[d] / deptCounts[d] * 100.0;
        cout << left << setw(6)  << (d+1)
             << setw(24) << depts[d]
             << setw(16) << deptCounts[d]
             << setw(12) << avgCgpa
             << passRate << "%" << endl;
    }
    cout << setfill('=') << setw(78) << "=" << endl;
    cout << setfill(' ');
}

// ===================================================================
// exportReportToFile — redirect cout to a file, then restore
// ===================================================================
void exportReportToFile(int reportChoice,
                        const Student students[], int studentCount,
                        const FeeRecord fees[], int feeCount,
                        const GradeRecord grades[], int gradeCount,
                        const AttendanceRecord attendance[], int attendanceCount,
                        const Course courses[], int courseCount,
                        const string& todayDate) {
    string filename;
    switch (reportChoice) {
        case 1: filename = "merit_list.txt";            break;
        case 2: filename = "attendance_defaulters.txt"; break;
        case 3: filename = "fee_defaulters.txt";        break;
        case 4: filename = "semester_result.txt";       break;
        case 5: filename = "department_summary.txt";    break;
        default:
            cout << "Invalid report choice for export." << endl;
            return;
    }

    ofstream outFile(filename.c_str());
    if (!outFile.is_open()) {
        cout << "Error: Could not create file " << filename << endl;
        return;
    }

    streambuf* originalBuf = cout.rdbuf();
    cout.rdbuf(outFile.rdbuf());

    switch (reportChoice) {
        case 1: printMeritList(students, studentCount); break;
        case 2: printAttendanceDefaulters(students, studentCount, attendance, attendanceCount); break;
        case 3: printFeeDefaulters(fees, feeCount, students, studentCount, todayDate); break;
        case 4: printSemesterResult(students, studentCount, grades, gradeCount, attendance, attendanceCount, courses, courseCount); break;
        case 5: printDepartmentSummary(students, studentCount); break;
    }

    cout.rdbuf(originalBuf);
    outFile.close();

    cout << "\nReport exported successfully to " << filename << endl;
}
