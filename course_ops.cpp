#include "course_ops.h"
#include "filehandler.h"
#include <iostream>
#include <sstream>

using namespace std;

// -------------------------------------------------------------------
// Helpers
// -------------------------------------------------------------------
static string intToStr(int v) {
    ostringstream oss;
    oss << v;
    return oss.str();
}

static int strToInt(const string& s) {
    int v = 0;
    for (size_t i = 0; i < s.size(); i++)
        if (s[i] >= '0' && s[i] <= '9') v = v * 10 + (s[i] - '0');
    return v;
}

// -------------------------------------------------------------------
// Validation
// -------------------------------------------------------------------
bool isValidCourseCode(const string& code) {
    // Allow codes like CS101, CS-101, MATH-201, HU101 — 4-7 chars
    if (code.length() < 4 || code.length() > 8) return false;
    // Must contain at least one letter and one digit
    bool hasLetter = false, hasDigit = false;
    for (size_t i = 0; i < code.length(); i++) {
        if ((code[i] >= 'A' && code[i] <= 'Z') ||
            (code[i] >= 'a' && code[i] <= 'z')) hasLetter = true;
        if (code[i] >= '0' && code[i] <= '9') hasDigit = true;
    }
    return hasLetter && hasDigit;
}

bool isValidCreditHours(int hours) {
    return hours >= 1 && hours <= 4;
}

bool courseCodeExists(const Course courses[], int courseCount, const string& code) {
    for (int i = 0; i < courseCount; i++)
        if (courses[i].courseCode == code) return true;
    return false;
}

// ===================================================================
// loadCourses — Read courses.txt into array
// Columns: course_code,course_name,credit_hours,instructor,capacity,enrolled,prerequisite
//          idx:  0           1          2           3         4        5          6
// ===================================================================
void loadCourses(Course courses[], int& courseCount) {
    courseCount = 0;
    vector<string> flat = readTXT("courses.txt");
    const int COLS = 7;
    int rowCount = (int)flat.size() / COLS;

    for (int r = 0; r < rowCount && courseCount < MAX_COURSE_RECORDS; r++) {
        Course c;
        c.courseCode         = flat[r * COLS + 0];
        c.courseTitle        = flat[r * COLS + 1];
        c.creditHours        = strToInt(flat[r * COLS + 2]);
        c.instructor         = flat[r * COLS + 3];
        c.maxCapacity        = strToInt(flat[r * COLS + 4]);
        c.currentEnrollment  = strToInt(flat[r * COLS + 5]);
        c.prereq             = flat[r * COLS + 6];
        courses[courseCount] = c;
        courseCount++;
    }
    cout << "Loaded " << courseCount << " course records from courses.txt\n";
}

// ===================================================================
// saveCourses — Write all courses back to courses.txt
// ===================================================================
void saveCourses(const Course courses[], int courseCount) {
    vector<vector<string>> rows;
    for (int i = 0; i < courseCount; i++) {
        vector<string> row;
        row.push_back(courses[i].courseCode);
        row.push_back(courses[i].courseTitle);
        row.push_back(intToStr(courses[i].creditHours));
        row.push_back(courses[i].instructor);
        row.push_back(intToStr(courses[i].maxCapacity));
        row.push_back(intToStr(courses[i].currentEnrollment));
        row.push_back(courses[i].prereq);
        rows.push_back(row);
    }
    writeTXT("courses.txt",
             "course_code,course_name,credit_hours,instructor,capacity,enrolled,prerequisite",
             rows);
}

// ===================================================================
// addCourse
// ===================================================================
void addCourse(Course courses[], int& courseCount) {
    cout << "\n=== Add New Course ===" << endl;

    if (courseCount >= MAX_COURSE_RECORDS) {
        cout << "Course storage is full." << endl;
        return;
    }

    Course newCourse;

    cout << "Enter Course Code (e.g., CS201): ";
    newCourse.courseCode = safeReadLine();
    if (newCourse.courseCode.empty()) { cout << "Input cancelled.\n"; return; }

    if (!isValidCourseCode(newCourse.courseCode)) {
        cout << "Invalid course code format." << endl;
        return;
    }

    if (courseCodeExists(courses, courseCount, newCourse.courseCode)) {
        cout << "Course code already exists!" << endl;
        return;
    }

    cout << "Enter Course Title: ";
    newCourse.courseTitle = safeReadLine();
    if (newCourse.courseTitle.empty()) { cout << "Input cancelled.\n"; return; }

    cout << "Enter Credit Hours (1-4): ";
    newCourse.creditHours = safeReadInt();
    if (!isValidCreditHours(newCourse.creditHours)) {
        cout << "Invalid credit hours. Must be 1-4." << endl;
        return;
    }

    cout << "Enter Instructor Name: ";
    newCourse.instructor = safeReadLine();
    if (newCourse.instructor.empty()) newCourse.instructor = "TBA";

    cout << "Enter Maximum Capacity: ";
    newCourse.maxCapacity = safeReadInt();
    if (newCourse.maxCapacity <= 0) {
        cout << "Invalid capacity." << endl;
        return;
    }

    cout << "Enter Prerequisite Course Code (or NONE): ";
    newCourse.prereq = safeReadLine();
    if (newCourse.prereq.empty()) newCourse.prereq = "NONE";

    newCourse.currentEnrollment = 0;
    courses[courseCount] = newCourse;
    courseCount++;

    saveCourses(courses, courseCount);
    cout << "Course added successfully!" << endl;
}

// ===================================================================
// listCourses
// ===================================================================
void listCourses(const Course courses[], int courseCount) {
    cout << "\n=== Registered Courses ===" << endl;
    if (courseCount == 0) { cout << "No courses registered.\n"; return; }

    cout << "---------------------------------------------------------------------------------------------" << endl;
    cout.width(9);  cout << left << "Code";
    cout.width(30); cout << left << "Title";
    cout.width(5);  cout << left << "Cr";
    cout.width(22); cout << left << "Instructor";
    cout.width(5);  cout << left << "Cap";
    cout.width(5);  cout << left << "Enr";
    cout << "Prereq" << endl;
    cout << "---------------------------------------------------------------------------------------------" << endl;

    for (int i = 0; i < courseCount; i++) {
        cout.width(9);  cout << left << courses[i].courseCode;
        cout.width(30); cout << left << courses[i].courseTitle;
        cout.width(5);  cout << left << courses[i].creditHours;
        cout.width(22); cout << left << courses[i].instructor;
        cout.width(5);  cout << left << courses[i].maxCapacity;
        cout.width(5);  cout << left << courses[i].currentEnrollment;
        cout << courses[i].prereq << endl;
    }
    cout << "---------------------------------------------------------------------------------------------" << endl;
}

// ===================================================================
// updateCourse
// ===================================================================
void updateCourse(Course courses[], int& courseCount) {
    cout << "\n=== Update Course ===" << endl;
    cout << "Enter Course Code to Update: ";
    string code = safeReadLine();
    if (code.empty()) { cout << "Input cancelled.\n"; return; }

    int index = -1;
    for (int i = 0; i < courseCount; i++) {
        if (courses[i].courseCode == code) { index = i; break; }
    }

    if (index == -1) { cout << "Course not found." << endl; return; }

    cout << "\nCurrent: " << courses[index].courseTitle
         << " | Credits: " << courses[index].creditHours
         << " | Capacity: " << courses[index].maxCapacity
         << " | Prereq: " << courses[index].prereq << endl;

    cout << "Enter new Title (Enter to skip): ";
    string newTitle = safeReadLine();
    if (!newTitle.empty()) courses[index].courseTitle = newTitle;

    cout << "Enter new Credit Hours (-1 to skip, 1-4): ";
    int newCr = safeReadInt(-1);
    if (newCr != -1) {
        if (isValidCreditHours(newCr)) courses[index].creditHours = newCr;
        else cout << "Invalid credit hours, skipped." << endl;
    }

    cout << "Enter new Capacity (-1 to skip): ";
    int newCap = safeReadInt(-1);
    if (newCap != -1) {
        if (newCap >= courses[index].currentEnrollment && newCap > 0)
            courses[index].maxCapacity = newCap;
        else cout << "Invalid capacity, skipped." << endl;
    }

    cout << "Enter new Instructor (Enter to skip): ";
    string newInst = safeReadLine();
    if (!newInst.empty()) courses[index].instructor = newInst;

    cout << "Enter new Prerequisite (Enter to skip): ";
    string newPrereq = safeReadLine();
    if (!newPrereq.empty()) courses[index].prereq = newPrereq;

    saveCourses(courses, courseCount);
    cout << "Course updated successfully!" << endl;
}

// ===================================================================
// checkPrerequisite — check grades file for non-F grade in prereq
// ===================================================================
bool checkPrerequisite(const string& courseCode,
                       const string& roll,
                       const Course courses[], int courseCount) {
    // Find prereq for this course
    string prereq = "NONE";
    for (int i = 0; i < courseCount; i++) {
        if (courses[i].courseCode == courseCode) {
            prereq = courses[i].prereq;
            break;
        }
    }
    if (prereq == "NONE" || prereq.empty()) return true;

    // Search grades.txt for roll + prereq course with non-F grade
    // grades.txt columns: roll,course_code,... (check if file exists)
    // We use a simple linear search via readTXT
    vector<string> flat = readTXT("grades.txt");
    if (flat.empty()) return true;

    // grades.txt: roll, course_code, grade (col 0,1,14 from 15-col format)
    // checkPrerequisite uses the simplified 15-col grades.txt — look at col0=roll, col1=course, col14=grade
    const int G_COLS = 15;
    for (int r = 0; r * G_COLS < (int)flat.size(); r++) {
        if (flat[r * G_COLS + 0] == roll && flat[r * G_COLS + 1] == prereq) {
            string grade = flat[r * G_COLS + 14]; // col14 = letterGrade
            return (grade != "F" && !grade.empty());
        }
    }
    return false; // prereq course record not found → not passed
}

// ===================================================================
// getCreditLoad — sum credit hours of all active enrollments
// ===================================================================
int getCreditLoad(const string& roll,
                  const string& semester,
                  const Course courses[], int courseCount) {
    // enrollments.txt: enrollment_id,roll_no,course_code,semester,enrollment_date,status
    //                  idx: 0          1        2           3         4              5
    vector<string> flat = readTXT("enrollments.txt");
    const int COLS = 6;
    int rowCount = (int)flat.size() / COLS;
    int totalCredits = 0;

    for (int r = 0; r < rowCount; r++) {
        string eRoll     = flat[r * COLS + 1];
        string eCourse   = flat[r * COLS + 2];
        string eSemester = flat[r * COLS + 3];
        string eStatus   = flat[r * COLS + 5];

        if (eRoll == roll && eSemester == semester && eStatus == "active") {
            // Find credit hours for this course
            for (int c = 0; c < courseCount; c++) {
                if (courses[c].courseCode == eCourse) {
                    totalCredits += courses[c].creditHours;
                    break;
                }
            }
        }
    }
    return totalCredits;
}

// ===================================================================
// enrollStudent — check all conditions, write to enrollments.txt
// ===================================================================
EnrollResult enrollStudent(const string& roll,
                           const string& courseCode,
                           const string& semester,
                           Course courses[], int courseCount) {
    // 1. Find course
    int courseIdx = -1;
    for (int i = 0; i < courseCount; i++) {
        if (courses[i].courseCode == courseCode) { courseIdx = i; break; }
    }
    if (courseIdx == -1) return ENROLL_COURSE_NOT_FOUND;

    // 2. Seats available?
    if (courses[courseIdx].currentEnrollment >= courses[courseIdx].maxCapacity)
        return ENROLL_SEATS_FULL;

    // 3. Already enrolled?
    vector<string> flat = readTXT("enrollments.txt");
    const int COLS = 6;
    int rowCount = (int)flat.size() / COLS;
    int maxId = 0;

    for (int r = 0; r < rowCount; r++) {
        string eId       = flat[r * COLS + 0];
        string eRoll     = flat[r * COLS + 1];
        string eCourse   = flat[r * COLS + 2];
        string eSemester = flat[r * COLS + 3];
        string eStatus   = flat[r * COLS + 5];

        // Extract numeric part of ID for max
        int idNum = 0;
        for (size_t k = 1; k < eId.size(); k++)
            if (eId[k] >= '0' && eId[k] <= '9')
                idNum = idNum * 10 + (eId[k] - '0');
        if (idNum > maxId) maxId = idNum;

        if (eRoll == roll && eCourse == courseCode &&
            eSemester == semester && eStatus == "active")
            return ENROLL_ALREADY_ENROLLED;
    }

    // 4. Credit load <= 21
    int load = getCreditLoad(roll, semester, courses, courseCount);
    if (load + courses[courseIdx].creditHours > 21) return ENROLL_CREDIT_LIMIT;

    // 5. Prerequisite check
    if (!checkPrerequisite(courseCode, roll, courses, courseCount))
        return ENROLL_PREREQ_NOT_MET;

    // All checks passed — add enrollment
    maxId++;
    ostringstream idStr;
    idStr << "E";
    if (maxId < 10)   idStr << "000";
    else if (maxId < 100) idStr << "00";
    else if (maxId < 1000) idStr << "0";
    idStr << maxId;

    vector<string> row;
    row.push_back(idStr.str());
    row.push_back(roll);
    row.push_back(courseCode);
    row.push_back(semester);
    row.push_back("15-01-2024"); // default enrollment date
    row.push_back("active");
    appendTXT("enrollments.txt", row);

    // Update in-memory enrollment count
    courses[courseIdx].currentEnrollment++;
    saveCourses(courses, courseCount);

    return ENROLL_OK;
}

// ===================================================================
// dropCourse — only if no attendance rows exist for student+course+semester
// ===================================================================
bool dropCourse(const string& roll,
                const string& courseCode,
                const string& semester) {
    // Check attendance_log.txt for any record (5 columns: log_id,roll,course,date,status)
    vector<string> attFlat = readTXT("attendance_log.txt");
    const int A_COLS = 5;
    int aRows = (int)attFlat.size() / A_COLS;
    for (int r = 0; r < aRows; r++) {
        // col1=roll_no, col2=course_code
        if (attFlat[r * A_COLS + 1] == roll &&
            attFlat[r * A_COLS + 2] == courseCode) {
            cout << "Cannot drop: attendance records exist for this course.\n";
            return false;
        }
    }

    // Read enrollments, update status to dropped
    vector<string> flat = readTXT("enrollments.txt");
    const int COLS = 6;
    int rowCount = (int)flat.size() / COLS;

    vector<vector<string>> newRows;
    bool found = false;

    for (int r = 0; r < rowCount; r++) {
        vector<string> row;
        for (int c = 0; c < COLS; c++)
            row.push_back(flat[r * COLS + c]);

        if (row[1] == roll && row[2] == courseCode &&
            row[3] == semester && row[5] == "active") {
            row[5] = "dropped";
            found = true;
        }
        newRows.push_back(row);
    }

    if (!found) { cout << "Enrollment not found.\n"; return false; }

    writeTXT("enrollments.txt",
             "enrollment_id,roll_no,course_code,semester,enrollment_date,status",
             newRows);
    return true;
}

// ===================================================================
// listEnrolledStudents — list all active enrolled students in a course
// ===================================================================
void listEnrolledStudents(const string& courseCode,
                          const string& semester) {
    vector<string> flat = readTXT("enrollments.txt");
    const int COLS = 6;
    int rowCount = (int)flat.size() / COLS;

    cout << "\n=== Enrolled Students: " << courseCode
         << " (Semester " << semester << ") ===" << endl;
    cout << "--------------------------------------------------" << endl;

    int count = 0;
    for (int r = 0; r < rowCount; r++) {
        if (flat[r * COLS + 2] == courseCode &&
            flat[r * COLS + 3] == semester  &&
            flat[r * COLS + 5] == "active") {
            count++;
            cout << count << ". " << flat[r * COLS + 1] << endl;
        }
    }
    if (count == 0) cout << "No students enrolled." << endl;
    cout << "--------------------------------------------------" << endl;
}
