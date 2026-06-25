#include "student_ops.h"
#include "filehandler.h"
#include <iostream>
#include <sstream>

using namespace std;

// ===================================================================
// Helpers: double <-> string without <sstream> heavy use
// ===================================================================
static string doubleToStr(double v) {
    ostringstream oss;
    oss << v;
    return oss.str();
}

static double strToDouble(const string& s) {
    double v = 0.0;
    bool decimal = false;
    double dec = 0.1;
    size_t start = 0;
    bool neg = false;
    if (!s.empty() && s[0] == '-') { neg = true; start = 1; }
    for (size_t i = start; i < s.size(); ++i) {
        if (s[i] == '.') { decimal = true; }
        else if (s[i] >= '0' && s[i] <= '9') {
            if (!decimal) { v = v * 10 + (s[i] - '0'); }
            else { v += (s[i] - '0') * dec; dec *= 0.1; }
        }
    }
    return neg ? -v : v;
}

// ===================================================================
// Validation helpers
// ===================================================================
bool isValidRollFormat(const string& roll) {
    // Format: BSAI-YY-XXX  (length = 11)
    if (roll.length() != 11) return false;
    if (roll[0]!='B'||roll[1]!='S'||roll[2]!='A'||roll[3]!='I'||roll[4]!='-') return false;
    for (int i = 5; i < 7; i++)
        if (roll[i] < '0' || roll[i] > '9') return false;
    if (roll[7] != '-') return false;
    for (int i = 8; i < 11; i++)
        if (roll[i] < '0' || roll[i] > '9') return false;
    return true;
}

bool isValidName(const string& name) {
    if (name.empty()) return false;
    for (size_t i = 0; i < name.length(); i++)
        if (name[i] >= '0' && name[i] <= '9') return false;
    return true;
}

bool isValidCGPA(double cgpa) {
    return cgpa >= 0.0 && cgpa <= 4.0;
}

bool rollExists(Student students[], int studentCount, const string& roll) {
    for (int i = 0; i < studentCount; i++)
        if (students[i].roll == roll && students[i].status == "active")
            return true;
    return false;
}

// ===================================================================
// loadStudents — Read students.txt into array
// Columns: roll_no,name,department,semester,cgpa,status  (indices 0-5)
// ===================================================================
void loadStudents(Student students[], int& studentCount) {
    studentCount = 0;
    vector<string> flat = readTXT("students.txt");
    // 6 columns per row
    const int COLS = 6;
    int rowCount = (int)flat.size() / COLS;

    for (int r = 0; r < rowCount && studentCount < MAX_STUDENT_RECORDS; r++) {
        Student s;
        s.roll     = flat[r * COLS + 0];
        s.name     = flat[r * COLS + 1];
        s.dept     = flat[r * COLS + 2];
        s.semester = flat[r * COLS + 3];
        s.cgpa     = strToDouble(flat[r * COLS + 4]);
        s.status   = flat[r * COLS + 5];
        students[studentCount] = s;
        studentCount++;
    }
    cout << "Loaded " << studentCount << " student records from students.txt\n";
}

// ===================================================================
// saveStudents — Write all students back to students.txt
// ===================================================================
void saveStudents(const Student students[], int studentCount) {
    vector<vector<string>> rows;
    for (int i = 0; i < studentCount; i++) {
        vector<string> row;
        row.push_back(students[i].roll);
        row.push_back(students[i].name);
        row.push_back(students[i].dept);
        row.push_back(students[i].semester);
        row.push_back(doubleToStr(students[i].cgpa));
        row.push_back(students[i].status);
        rows.push_back(row);
    }
    writeTXT("students.txt", "roll_no,name,department,semester,cgpa,status", rows);
}

// ===================================================================
// selectionSort — sort active students by roll number
// ===================================================================
static void selectionSort(Student students[], int count) {
    for (int i = 0; i < count - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < count; j++)
            if (students[j].roll < students[minIdx].roll)
                minIdx = j;
        Student temp = students[i];
        students[i] = students[minIdx];
        students[minIdx] = temp;
    }
}

// ===================================================================
// addStudent
// ===================================================================
void addStudent(Student students[], int& studentCount) {
    cout << "\n=== Add New Student ===" << endl;

    if (studentCount >= MAX_STUDENT_RECORDS) {
        cout << "Student storage is full. Cannot add more records." << endl;
        return;
    }

    Student newStudent;

    cout << "Enter Roll Number (Format: BSAI-YY-XXX): ";
    newStudent.roll = safeReadLine();
    if (newStudent.roll.empty()) { cout << "Input cancelled.\n"; return; }

    if (!isValidRollFormat(newStudent.roll)) {
        cout << "Invalid roll format. Expected format: BSAI-YY-XXX" << endl;
        return;
    }

    if (rollExists(students, studentCount, newStudent.roll)) {
        cout << "Roll number already exists!" << endl;
        return;
    }

    cout << "Enter Student Name: ";
    newStudent.name = safeReadLine();
    if (newStudent.name.empty()) { cout << "Input cancelled.\n"; return; }
    if (!isValidName(newStudent.name)) {
        cout << "Invalid name. Name cannot contain digits." << endl;
        return;
    }

    cout << "Enter Department: ";
    newStudent.dept = safeReadLine();
    if (newStudent.dept.empty()) { cout << "Input cancelled.\n"; return; }

    cout << "Enter Semester (e.g., 2): ";
    newStudent.semester = safeReadLine();
    if (newStudent.semester.empty()) newStudent.semester = "1";

    cout << "Enter CGPA (0.0 - 4.0): ";
    newStudent.cgpa = safeReadDouble();
    if (newStudent.cgpa == -999.0) { cout << "Input cancelled.\n"; return; }

    if (!isValidCGPA(newStudent.cgpa)) {
        cout << "Invalid CGPA. Must be between 0.0 and 4.0." << endl;
        return;
    }

    newStudent.status = "active";
    students[studentCount] = newStudent;
    studentCount++;

    // Persist: append new row to file
    vector<string> row;
    row.push_back(newStudent.roll);
    row.push_back(newStudent.name);
    row.push_back(newStudent.dept);
    row.push_back(newStudent.semester);
    row.push_back(doubleToStr(newStudent.cgpa));
    row.push_back(newStudent.status);
    appendTXT("students.txt", row);

    cout << "Student added successfully!" << endl;
}

// ===================================================================
// searchByRoll
// ===================================================================
void searchByRoll(Student students[], int studentCount) {
    cout << "\n=== Search by Roll Number ===" << endl;
    cout << "Enter Roll Number: ";
    string roll = safeReadLine();

    for (int i = 0; i < studentCount; i++) {
        if (students[i].roll == roll && students[i].status == "active") {
            cout << "\nStudent Found:" << endl;
            cout << "Roll     : " << students[i].roll     << endl;
            cout << "Name     : " << students[i].name     << endl;
            cout << "Dept     : " << students[i].dept     << endl;
            cout << "Semester : " << students[i].semester << endl;
            cout << "CGPA     : " << students[i].cgpa     << endl;
            cout << "Status   : " << students[i].status   << endl;
            return;
        }
    }
    cout << "Student not found." << endl;
}

// ===================================================================
// searchByName
// ===================================================================
void searchByName(Student students[], int studentCount) {
    cout << "\n=== Search by Name ===" << endl;
    cout << "Enter Name (or part of name): ";
    string searchName = safeReadLine();

    int serial = 1;
    bool found = false;
    for (int i = 0; i < studentCount; i++) {
        if (students[i].status == "active" &&
            students[i].name.find(searchName) != string::npos) {
            if (!found) {
                cout << "\nStudents Found:" << endl;
                found = true;
            }
            cout << serial << ". Roll: " << students[i].roll
                 << " | Name: "     << students[i].name
                 << " | Dept: "     << students[i].dept
                 << " | CGPA: "     << students[i].cgpa << endl;
            serial++;
        }
    }
    if (!found) cout << "No students found matching the name." << endl;
}

// ===================================================================
// updateStudent
// ===================================================================
void updateStudent(Student students[], int& studentCount) {
    cout << "\n=== Update Student ===" << endl;
    cout << "Enter Roll Number of Student to Update: ";
    string roll = safeReadLine();
    if (roll.empty()) { cout << "Input cancelled.\n"; return; }

    int index = -1;
    for (int i = 0; i < studentCount; i++) {
        if (students[i].roll == roll && students[i].status == "active") {
            index = i;
            break;
        }
    }

    if (index == -1) {
        cout << "Student not found." << endl;
        return;
    }

    cout << "\nCurrent Details:" << endl;
    cout << "Name     : " << students[index].name     << endl;
    cout << "Dept     : " << students[index].dept     << endl;
    cout << "Semester : " << students[index].semester << endl;
    cout << "CGPA     : " << students[index].cgpa     << endl;

    cout << "\nEnter new Name (or press Enter to skip): ";
    string newName = safeReadLine();
    if (newName.length() > 0) {
        if (isValidName(newName)) students[index].name = newName;
        else cout << "Invalid name. Update skipped." << endl;
    }

    cout << "Enter new Department (or press Enter to skip): ";
    string newDept = safeReadLine();
    if (newDept.length() > 0) students[index].dept = newDept;

    cout << "Enter new Semester (or press Enter to skip): ";
    string newSem = safeReadLine();
    if (newSem.length() > 0) students[index].semester = newSem;

    cout << "Enter new CGPA (or enter -1 to skip): ";
    double newCGPA = safeReadDouble(-1.0);
    if (newCGPA != -1.0 && newCGPA != -999.0) {
        if (isValidCGPA(newCGPA)) students[index].cgpa = newCGPA;
        else cout << "Invalid CGPA. Update skipped." << endl;
    }

    // Persist entire file
    saveStudents(students, studentCount);
    cout << "Student updated successfully!" << endl;
}

// ===================================================================
// softDelete
// ===================================================================
void softDelete(Student students[], int& studentCount) {
    cout << "\n=== Delete Student (Soft Delete) ===" << endl;
    cout << "Enter Roll Number of Student to Delete: ";
    string roll = safeReadLine();
    if (roll.empty()) { cout << "Input cancelled.\n"; return; }

    int index = -1;
    for (int i = 0; i < studentCount; i++) {
        if (students[i].roll == roll && students[i].status == "active") {
            index = i;
            break;
        }
    }

    if (index == -1) {
        cout << "Student not found." << endl;
        return;
    }

    students[index].status = "inactive";

    // Persist entire file
    saveStudents(students, studentCount);
    cout << "Student marked as inactive (soft deleted)." << endl;
}

// ===================================================================
// listActiveStudents — selection sort then display table
// ===================================================================
void listActiveStudents(Student students[], int studentCount) {
    Student active[MAX_STUDENT_RECORDS];
    int activeCount = 0;

    for (int i = 0; i < studentCount; i++)
        if (students[i].status == "active")
            active[activeCount++] = students[i];

    cout << "\n=== Active Students List (Sorted by Roll Number) ===" << endl;
    if (activeCount == 0) {
        cout << "No active students found." << endl;
        return;
    }

    selectionSort(active, activeCount);

    cout << "---------------------------------------------------------------" << endl;
    cout.width(13); cout << left << "Roll"
         << "| ";
    cout.width(22); cout << left << "Name"    << "| ";
    cout.width(18); cout << left << "Dept"    << "| ";
    cout.width(4);  cout << left << "Sem"     << "| CGPA" << endl;
    cout << "---------------------------------------------------------------" << endl;

    for (int i = 0; i < activeCount; i++) {
        cout.width(13); cout << left << active[i].roll     << "| ";
        cout.width(22); cout << left << active[i].name     << "| ";
        cout.width(18); cout << left << active[i].dept     << "| ";
        cout.width(4);  cout << left << active[i].semester << "| ";
        cout << active[i].cgpa << endl;
    }
    cout << "---------------------------------------------------------------" << endl;
}
