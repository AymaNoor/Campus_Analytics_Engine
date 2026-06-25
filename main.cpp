#include <iostream>
#include <iomanip>
#include "filehandler.h"
#include "student_ops.h"
#include "course_ops.h"
#include "attendance_ops.h"
#include "grades_ops.h"
#include "fee_ops.h"
#include "reports.h"

using namespace std;

// -----------------------------------------------------------------------
// TODAY'S DATE — set manually once per semester (no <ctime> allowed)
// -----------------------------------------------------------------------
const string TODAY_DATE = "25-06-2026";  // DD-MM-YYYY

// -----------------------------------------------------------------------
// Global data arrays and counts
// -----------------------------------------------------------------------
Student          students[MAX_STUDENT_RECORDS];
int              studentCount = 0;

Course           courses[MAX_COURSE_RECORDS];
int              courseCount = 0;

AttendanceRecord attendance[MAX_ATTENDANCE_RECORDS];
int              attendanceCount = 0;

GradeRecord      grades[MAX_GRADE_RECORDS];
int              gradeCount = 0;

FeeRecord        fees[MAX_FEE_RECORDS];
int              feeCount = 0;

// -----------------------------------------------------------------------
// Startup / Shutdown
// -----------------------------------------------------------------------
void loadAllData() {
    cout << "\n--- Loading data from files ---\n";
    loadStudents(students, studentCount);
    loadCourses(courses, courseCount);
    loadAttendance(attendance, attendanceCount);
    loadGrades(grades, gradeCount);
    loadFees(fees, feeCount);
    cout << "--- Data loading complete ---\n";
}

void saveAllData() {
    cout << "\n--- Saving data to files ---\n";
    saveStudents(students, studentCount);
    saveCourses(courses, courseCount);
    saveAttendance(attendance, attendanceCount);
    saveGrades(grades, gradeCount);
    saveFees(fees, feeCount);
    cout << "--- All data saved. ---\n";
}

// -----------------------------------------------------------------------
// Menu display functions
// -----------------------------------------------------------------------
void displayMainMenu() {
    cout << "\n========================================" << endl;
    cout << "    Campus Analytics Engine - Main Menu  " << endl;
    cout << "========================================" << endl;
    cout << "1. Student Management" << endl;
    cout << "2. Course Management" << endl;
    cout << "3. Attendance Management" << endl;
    cout << "4. Grades Management" << endl;
    cout << "5. Fee Tracking Management" << endl;
    cout << "6. Reports & Analytics" << endl;
    cout << "7. Enrollment Management" << endl;
    cout << "8. Exit Application" << endl;
    cout << "----------------------------------------" << endl;
    cout << "Select an option (1-8): ";
}

void displayStudentMenu() {
    cout << "\n====== Student Management ======" << endl;
    cout << "1. Add New Student" << endl;
    cout << "2. Search by Roll Number" << endl;
    cout << "3. Search by Name" << endl;
    cout << "4. Update Student" << endl;
    cout << "5. Soft Delete Student" << endl;
    cout << "6. List Active Students" << endl;
    cout << "7. Back to Main Menu" << endl;
    cout << "--------------------------------" << endl;
    cout << "Select an option (1-7): ";
}

void displayCourseMenu() {
    cout << "\n====== Course Management ======" << endl;
    cout << "1. Add New Course" << endl;
    cout << "2. View All Courses" << endl;
    cout << "3. Update Course" << endl;
    cout << "4. Back to Main Menu" << endl;
    cout << "-------------------------------" << endl;
    cout << "Select an option (1-4): ";
}

void displayAttendanceMenu() {
    cout << "\n====== Attendance Management ======" << endl;
    cout << "1. Mark Attendance" << endl;
    cout << "2. View Attendance Report" << endl;
    cout << "3. Get Attendance Percentage" << endl;
    cout << "4. Get Shortage List (<75%)" << endl;
    cout << "5. Undo Last Session" << endl;
    cout << "6. Print Daily Sheet" << endl;
    cout << "7. Back to Main Menu" << endl;
    cout << "-----------------------------------" << endl;
    cout << "Select an option (1-7): ";
}

void displayGradesMenu() {
    cout << "\n====== Grades Management ======" << endl;
    cout << "1. Enter Student Marks" << endl;
    cout << "2. Compute Class Stats" << endl;
    cout << "3. Compute Class State (with Median)" << endl;
    cout << "4. Back to Main Menu" << endl;
    cout << "-------------------------------" << endl;
    cout << "Select an option (1-4): ";
}

void displayFeeMenu() {
    cout << "\n====== Fee Tracking ======" << endl;
    cout << "1. Add Fee Record" << endl;
    cout << "2. Record Payment" << endl;
    cout << "3. Generate Receipt" << endl;
    cout << "4. View Defaulters" << endl;
    cout << "5. Back to Main Menu" << endl;
    cout << "--------------------------" << endl;
    cout << "Select an option (1-5): ";
}

void displayReportsMenu() {
    cout << "\n====== Reports & Analytics ======" << endl;
    cout << "1. Merit List (by CGPA)" << endl;
    cout << "2. Attendance Defaulters" << endl;
    cout << "3. Fee Defaulters" << endl;
    cout << "4. Semester Result Sheet" << endl;
    cout << "5. Department Summary" << endl;
    cout << "6. Export Report to File" << endl;
    cout << "7. Back to Main Menu" << endl;
    cout << "----------------------------------" << endl;
    cout << "Select an option (1-7): ";
}

void displayEnrollmentMenu() {
    cout << "\n====== Enrollment Management ======" << endl;
    cout << "1. Enroll Student in Course" << endl;
    cout << "2. Drop Course" << endl;
    cout << "3. Get Credit Load" << endl;
    cout << "4. List Enrolled Students" << endl;
    cout << "5. Back to Main Menu" << endl;
    cout << "-----------------------------------" << endl;
    cout << "Select an option (1-5): ";
}

// -----------------------------------------------------------------------
// Handlers — every loop checks cin.eof() to handle Ctrl+Z gracefully
// -----------------------------------------------------------------------

void handleStudentManagement() {
    while (true) {
        displayStudentMenu();
        int choice = safeReadInt();

        // EOF (Ctrl+Z) — go back to main menu safely
        if (cin.eof()) {
            cout << "\n[EOF detected] Returning to Main Menu...\n";
            return;
        }

        switch (choice) {
            case 1: addStudent(students, studentCount);          break;
            case 2: searchByRoll(students, studentCount);        break;
            case 3: searchByName(students, studentCount);        break;
            case 4: updateStudent(students, studentCount);       break;
            case 5: softDelete(students, studentCount);          break;
            case 6: listActiveStudents(students, studentCount);  break;
            case 7: cout << "Returning to Main Menu...\n"; return;
            default: cout << "Invalid option. Please enter 1-7.\n";
        }
    }
}

void handleCourseManagement() {
    while (true) {
        displayCourseMenu();
        int choice = safeReadInt();

        if (cin.eof()) {
            cout << "\n[EOF detected] Returning to Main Menu...\n";
            return;
        }

        switch (choice) {
            case 1: addCourse(courses, courseCount);       break;
            case 2: listCourses(courses, courseCount);     break;
            case 3: updateCourse(courses, courseCount);    break;
            case 4: cout << "Returning to Main Menu...\n"; return;
            default: cout << "Invalid option. Please enter 1-4.\n";
        }
    }
}

void handleAttendanceManagement() {
    while (true) {
        displayAttendanceMenu();
        int choice = safeReadInt();

        if (cin.eof()) {
            cout << "\n[EOF detected] Returning to Main Menu...\n";
            return;
        }

        switch (choice) {
            case 1:
                markAttendance(attendance, attendanceCount,
                               students, studentCount, courses, courseCount);
                break;
            case 2:
                viewAttendanceReport(attendance, attendanceCount);
                break;
            case 3: {
                cout << "Enter Roll: ";
                string roll = safeReadLine();
                cout << "Enter Course Code: ";
                string courseCode = safeReadLine();
                if (!roll.empty() && !courseCode.empty()) {
                    double pct = getAttendancePct(attendance, attendanceCount, roll, courseCode);
                    cout << "Attendance for " << roll << " in " << courseCode
                         << ": " << fixed << setprecision(1) << pct << "%" << endl;
                }
                break;
            }
            case 4: {
                cout << "Enter Course Code: ";
                string courseCode = safeReadLine();
                if (!courseCode.empty())
                    getShortageList(attendance, attendanceCount,
                                    students, studentCount, courseCode);
                break;
            }
            case 5: {
                cout << "Enter Course Code: ";
                string courseCode = safeReadLine();
                cout << "Enter Date (DD-MM-YYYY): ";
                string date = safeReadLine();
                if (!courseCode.empty() && !date.empty())
                    undoLastSession(attendance, attendanceCount, courseCode, date);
                break;
            }
            case 6: {
                cout << "Enter Course Code: ";
                string courseCode = safeReadLine();
                cout << "Enter Date (DD-MM-YYYY): ";
                string date = safeReadLine();
                if (!courseCode.empty() && !date.empty())
                    printDailySheet(attendance, attendanceCount,
                                    students, studentCount, courseCode, date);
                break;
            }
            case 7: cout << "Returning to Main Menu...\n"; return;
            default: cout << "Invalid option. Please enter 1-7.\n";
        }
    }
}

void handleGradesManagement() {
    while (true) {
        displayGradesMenu();
        int choice = safeReadInt();

        if (cin.eof()) {
            cout << "\n[EOF detected] Returning to Main Menu...\n";
            return;
        }

        switch (choice) {
            case 1:
                enterMarks(grades, gradeCount, students, studentCount, courses, courseCount);
                break;
            case 2: {
                cout << "Enter Course Code: ";
                string courseCode = safeReadLine();
                if (!courseCode.empty())
                    computeClassStats(grades, gradeCount, courseCode);
                break;
            }
            case 3: {
                cout << "Enter Course Code: ";
                string courseCode = safeReadLine();
                if (!courseCode.empty())
                    computeClassState(grades, gradeCount, courseCode);
                break;
            }
            case 4: cout << "Returning to Main Menu...\n"; return;
            default: cout << "Invalid option. Please enter 1-4.\n";
        }
    }
}

void handleFeeManagement() {
    while (true) {
        displayFeeMenu();
        int choice = safeReadInt();

        if (cin.eof()) {
            cout << "\n[EOF detected] Returning to Main Menu...\n";
            return;
        }

        switch (choice) {
            case 1: {
                if (feeCount >= MAX_FEE_RECORDS) {
                    cout << "Fee storage is full.\n"; break;
                }
                FeeRecord rec;
                cout << "Enter Student Roll Number: ";
                rec.rollNumber = safeReadLine();
                if (rec.rollNumber.empty()) break;

                bool found = false;
                for (int i = 0; i < studentCount; i++) {
                    if (students[i].roll == rec.rollNumber && students[i].status == "active") {
                        found = true; break;
                    }
                }
                if (!found) { cout << "Active student not found.\n"; break; }

                cout << "Enter Semester (e.g., 2): ";
                rec.semester = safeReadLine();
                if (rec.semester.empty()) break;

                bool dup = false;
                for (int i = 0; i < feeCount; i++) {
                    if (fees[i].rollNumber == rec.rollNumber && fees[i].semester == rec.semester) {
                        dup = true; break;
                    }
                }
                if (dup) { cout << "Fee record already exists.\n"; break; }

                cout << "Enter Amount Due: ";
                rec.amountDue = safeReadDouble();
                if (rec.amountDue <= 0.0 || rec.amountDue == -999.0) {
                    cout << "Invalid amount.\n"; break;
                }

                rec.amountPaid = 0.0;

                cout << "Enter Due Date (DD-MM-YYYY): ";
                rec.dueDate = safeReadLine();
                if (rec.dueDate.length() != 10 || rec.dueDate[2] != '-' || rec.dueDate[5] != '-') {
                    cout << "Invalid due date format.\n"; break;
                }

                // Auto-generate fee ID
                rec.feeId = "F";
                int num = feeCount + 1;
                if      (num < 10)  rec.feeId += "00";
                else if (num < 100) rec.feeId += "0";
                rec.feeId        += to_string(num);
                rec.paidDate      = "NOT_PAID";
                rec.paymentMethod = "NONE";
                rec.status        = "unpaid";

                fees[feeCount] = rec;
                feeCount++;
                saveFees(fees, feeCount);
                cout << "Fee record added successfully.\n";
                break;
            }
            case 2: {
                cout << "Enter Roll Number: ";
                string roll = safeReadLine();
                cout << "Enter Semester: ";
                string semester = safeReadLine();
                cout << "Enter Payment Amount: ";
                double amount = safeReadDouble();
                cout << "Enter Payment Date (DD-MM-YYYY): ";
                string payDate = safeReadLine();
                if (!roll.empty() && !semester.empty() && amount > 0.0 && amount != -999.0)
                    recordPayment(fees, feeCount, roll, semester, amount, payDate);
                else
                    cout << "Invalid input. Payment not recorded.\n";
                break;
            }
            case 3: {
                cout << "Enter Roll Number: ";
                string roll = safeReadLine();
                cout << "Enter Semester: ";
                string semester = safeReadLine();
                int idx = -1;
                for (int i = 0; i < feeCount; i++) {
                    if (fees[i].rollNumber == roll && fees[i].semester == semester) {
                        idx = i; break;
                    }
                }
                if (idx == -1) { cout << "Fee record not found.\n"; break; }
                generateReceipt(fees[idx]);
                break;
            }
            case 4:
                getDefaulters(fees, feeCount, students, studentCount, TODAY_DATE);
                break;
            case 5: cout << "Returning to Main Menu...\n"; return;
            default: cout << "Invalid option. Please enter 1-5.\n";
        }
    }
}

void handleReportsMenu() {
    while (true) {
        displayReportsMenu();
        int choice = safeReadInt();

        if (cin.eof()) {
            cout << "\n[EOF detected] Returning to Main Menu...\n";
            return;
        }

        switch (choice) {
            case 1: printMeritList(students, studentCount); break;
            case 2:
                printAttendanceDefaulters(students, studentCount,
                                          attendance, attendanceCount);
                break;
            case 3:
                printFeeDefaulters(fees, feeCount, students, studentCount, TODAY_DATE);
                break;
            case 4:
                printSemesterResult(students, studentCount, grades, gradeCount,
                                    attendance, attendanceCount, courses, courseCount);
                break;
            case 5: printDepartmentSummary(students, studentCount); break;
            case 6: {
                cout << "\nSelect report to export:\n";
                cout << "1.Merit List  2.Attendance Defaulters  3.Fee Defaulters\n";
                cout << "4.Semester Result  5.Department Summary\n";
                cout << "Enter choice (1-5): ";
                int exportChoice = safeReadInt();
                if (exportChoice >= 1 && exportChoice <= 5)
                    exportReportToFile(exportChoice,
                                       students, studentCount,
                                       fees, feeCount,
                                       grades, gradeCount,
                                       attendance, attendanceCount,
                                       courses, courseCount,
                                       TODAY_DATE);
                else
                    cout << "Invalid export choice.\n";
                break;
            }
            case 7: cout << "Returning to Main Menu...\n"; return;
            default: cout << "Invalid option. Please enter 1-7.\n";
        }
    }
}

void handleEnrollmentManagement() {
    while (true) {
        displayEnrollmentMenu();
        int choice = safeReadInt();

        if (cin.eof()) {
            cout << "\n[EOF detected] Returning to Main Menu...\n";
            return;
        }

        switch (choice) {
            case 1: {
                cout << "Enter Student Roll: ";
                string roll = safeReadLine();
                cout << "Enter Course Code: ";
                string courseCode = safeReadLine();
                cout << "Enter Semester: ";
                string semester = safeReadLine();
                if (roll.empty() || courseCode.empty() || semester.empty()) {
                    cout << "Input cancelled.\n"; break;
                }

                EnrollResult res = enrollStudent(roll, courseCode, semester, courses, courseCount);
                switch (res) {
                    case ENROLL_OK:              cout << "Student enrolled successfully!\n"; break;
                    case ENROLL_STUDENT_NOT_ACTIVE: cout << "Student not found or inactive.\n"; break;
                    case ENROLL_COURSE_NOT_FOUND: cout << "Course not found.\n"; break;
                    case ENROLL_SEATS_FULL:      cout << "Course is full (no seats available).\n"; break;
                    case ENROLL_ALREADY_ENROLLED: cout << "Student already enrolled in this course.\n"; break;
                    case ENROLL_PREREQ_NOT_MET:  cout << "Prerequisite not met.\n"; break;
                    case ENROLL_CREDIT_LIMIT:    cout << "Credit limit exceeded (max 21 hours).\n"; break;
                }
                break;
            }
            case 2: {
                cout << "Enter Student Roll: ";
                string roll = safeReadLine();
                cout << "Enter Course Code: ";
                string courseCode = safeReadLine();
                cout << "Enter Semester: ";
                string semester = safeReadLine();
                if (!roll.empty() && !courseCode.empty() && !semester.empty())
                    if (dropCourse(roll, courseCode, semester))
                        cout << "Course dropped successfully.\n";
                break;
            }
            case 3: {
                cout << "Enter Student Roll: ";
                string roll = safeReadLine();
                cout << "Enter Semester: ";
                string semester = safeReadLine();
                if (!roll.empty() && !semester.empty()) {
                    int load = getCreditLoad(roll, semester, courses, courseCount);
                    cout << "Credit load for " << roll << " in semester "
                         << semester << ": " << load << " hours\n";
                }
                break;
            }
            case 4: {
                cout << "Enter Course Code: ";
                string courseCode = safeReadLine();
                cout << "Enter Semester: ";
                string semester = safeReadLine();
                if (!courseCode.empty() && !semester.empty())
                    listEnrolledStudents(courseCode, semester);
                break;
            }
            case 5: cout << "Returning to Main Menu...\n"; return;
            default: cout << "Invalid option. Please enter 1-5.\n";
        }
    }
}

// -----------------------------------------------------------------------
// main
// -----------------------------------------------------------------------
int main() {
    cout << "\n=============================================" << endl;
    cout << "    Welcome to Campus Analytics Engine" << endl;
    cout << "    Date: " << TODAY_DATE << endl;
    cout << "=============================================" << endl;

    loadAllData();

    while (true) {
        displayMainMenu();
        int mainChoice = safeReadInt();

        // Ctrl+Z at main menu → save and exit cleanly
        if (cin.eof()) {
            cout << "\n[EOF detected] Saving data and exiting...\n";
            saveAllData();
            cout << "Thank you for using Campus Analytics Engine!\n";
            return 0;
        }

        switch (mainChoice) {
            case 1: handleStudentManagement();    break;
            case 2: handleCourseManagement();     break;
            case 3: handleAttendanceManagement(); break;
            case 4: handleGradesManagement();     break;
            case 5: handleFeeManagement();        break;
            case 6: handleReportsMenu();          break;
            case 7: handleEnrollmentManagement(); break;
            case 8:
                saveAllData();
                cout << "\nThank you for using Campus Analytics Engine!\n";
                cout << "Exiting...\n";
                return 0;
            default:
                cout << "Invalid option. Please select 1-8.\n";
        }
    }

    return 0;
}
