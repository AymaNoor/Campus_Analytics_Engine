#include "student_ops.h"
#include <iostream>
#include <fstream>

using namespace std;

bool isValidRollFormat(const string& roll) {
    if (roll.length() != 11) return false;
    if (roll[0] != 'B' || roll[1] != 'S' || roll[2] != 'A' || roll[3] != 'I' || roll[4] != '-') return false;
    
    for (int i = 5; i < 7; i++) {
        if (roll[i] < '0' || roll[i] > '9') return false;
    }
    
    if (roll[7] != '-') return false;
    
    for (int i = 8; i < 11; i++) {
        if (roll[i] < '0' || roll[i] > '9') return false;
    }
    
    return true;
}

bool isValidName(const string& name) {
    for (int i = 0; i < name.length(); i++) {
        if (name[i] >= '0' && name[i] <= '9') return false;
    }
    return name.length() > 0;
}

bool isValidCGPA(double cgpa) {
    return cgpa >= 0.0 && cgpa <= 4.0;
}

bool rollExists(Student students[], int& studentCount, const string& roll) {
    for (int i = 0; i < studentCount; i++) {
        if (students[i].roll == roll && students[i].status == "active") {
            return true;
        }
    }
    return false;
}

void addStudent(Student students[], int& studentCount) {
    Student newStudent;
    
    cout << "\n=== Add New Student ===" << endl;

    if (studentCount >= MAX_STUDENT_RECORDS) {
        cout << "Student storage is full. Cannot add more records." << endl;
        return;
    }
    
    cout << "Enter Roll Number (Format: BSAI-YY-XXX): ";
    getline(cin, newStudent.roll);
    
    if (!isValidRollFormat(newStudent.roll)) {
        cout << "Invalid roll format. Expected format: BSAI-YY-XXX" << endl;
        return;
    }
    
    if (rollExists(students, studentCount, newStudent.roll)) {
        cout << "Roll number already exists!" << endl;
        return;
    }
    
    cout << "Enter Student Name: ";
    getline(cin, newStudent.name);
    
    if (!isValidName(newStudent.name)) {
        cout << "Invalid name. Name cannot contain digits." << endl;
        return;
    }
    
    cout << "Enter Department: ";
    getline(cin, newStudent.dept);
    
    cout << "Enter CGPA (0.0 - 4.0): ";
    cin >> newStudent.cgpa;
    cin.ignore();
    
    if (!isValidCGPA(newStudent.cgpa)) {
        cout << "Invalid CGPA. Must be between 0.0 and 4.0." << endl;
        return;
    }
    
    newStudent.status = "active";
    students[studentCount] = newStudent;
    studentCount++;
    
    cout << "Student added successfully!" << endl;
}

void searchByRoll(Student students[], int& studentCount) {
    string roll;
    
    cout << "\n=== Search by Roll Number ===" << endl;
    cout << "Enter Roll Number: ";
    getline(cin, roll);
    
    bool found = false;
    for (int i = 0; i < studentCount; i++) {
        if (students[i].roll == roll && students[i].status == "active") {
            cout << "\nStudent Found:" << endl;
            cout << "Roll: " << students[i].roll << endl;
            cout << "Name: " << students[i].name << endl;
            cout << "Department: " << students[i].dept << endl;
            cout << "CGPA: " << students[i].cgpa << endl;
            cout << "Status: " << students[i].status << endl;
            found = true;
            break;
        }
    }
    
    if (!found) {
        cout << "Student not found." << endl;
    }
}

void searchByName(Student students[], int& studentCount) {
    string searchName;
    
    cout << "\n=== Search by Name ===" << endl;
    cout << "Enter Name (or part of name): ";
    getline(cin, searchName);

    int resultCount = 0;
    for (int i = 0; i < studentCount; i++) {
        if (students[i].status == "active" && students[i].name.find(searchName) != string::npos) {
            resultCount++;
        }
    }

    if (resultCount == 0) {
        cout << "No students found matching the name." << endl;
        return;
    }
    
    cout << "\nStudents Found:" << endl;
    int serial = 1;
    for (int i = 0; i < studentCount; i++) {
        if (students[i].status == "active" && students[i].name.find(searchName) != string::npos) {
            cout << "\n" << serial << ". Roll: " << students[i].roll << " | Name: " << students[i].name
                 << " | Dept: " << students[i].dept << " | CGPA: " << students[i].cgpa << endl;
            serial++;
        }
    }
}

void updateStudent(Student students[], int& studentCount) {
    string roll;
    
    cout << "\n=== Update Student ===" << endl;
    cout << "Enter Roll Number of Student to Update: ";
    getline(cin, roll);
    
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
    cout << "Name: " << students[index].name << endl;
    cout << "Department: " << students[index].dept << endl;
    cout << "CGPA: " << students[index].cgpa << endl;
    
    cout << "\nEnter new Name (or press Enter to skip): ";
    string newName;
    getline(cin, newName);
    if (newName.length() > 0) {
        if (isValidName(newName)) {
            students[index].name = newName;
        } else {
            cout << "Invalid name. Update skipped for name field." << endl;
        }
    }
    
    cout << "Enter new Department (or press Enter to skip): ";
    string newDept;
    getline(cin, newDept);
    if (newDept.length() > 0) {
        students[index].dept = newDept;
    }
    
    cout << "Enter new CGPA (or enter -1 to skip): ";
    double newCGPA;
    cin >> newCGPA;
    cin.ignore();
    if (newCGPA != -1) {
        if (isValidCGPA(newCGPA)) {
            students[index].cgpa = newCGPA;
        } else {
            cout << "Invalid CGPA. Update skipped for CGPA field." << endl;
        }
    }
    
    cout << "Student updated successfully!" << endl;
}

void softDelete(Student students[], int& studentCount) {
    string roll;
    
    cout << "\n=== Delete Student (Soft Delete) ===" << endl;
    cout << "Enter Roll Number of Student to Delete: ";
    getline(cin, roll);
    
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
    cout << "Student marked as inactive (soft deleted)." << endl;
}

void selectionSort(Student students[], int count) {
    for (int i = 0; i < count - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < count; j++) {
            if (students[j].roll < students[minIdx].roll) {
                minIdx = j;
            }
        }
        
        Student temp = students[i];
        students[i] = students[minIdx];
        students[minIdx] = temp;
    }
}

void listActiveStudents(Student students[], int& studentCount) {
    Student activeStudents[MAX_STUDENT_RECORDS];
    int activeCount = 0;
    
    for (int i = 0; i < studentCount; i++) {
        if (students[i].status == "active") {
            activeStudents[activeCount] = students[i];
            activeCount++;
        }
    }
    
    if (activeCount == 0) {
        cout << "\n=== Active Students List ===" << endl;
        cout << "No active students found." << endl;
        return;
    }
    
    selectionSort(activeStudents, activeCount);
    
    cout << "\n=== Active Students List (Sorted by Roll Number) ===" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "Roll          | Name            | Dept       | CGPA    " << endl;
    cout << "-----------------------------------------------------------" << endl;
    
    for (int i = 0; i < activeCount; i++) {
        cout.width(13);
        cout << left << activeStudents[i].roll << "| ";
        cout.width(15);
        cout << left << activeStudents[i].name << "| ";
        cout.width(10);
        cout << left << activeStudents[i].dept << "| ";
        cout.width(7);
        cout << left << activeStudents[i].cgpa << endl;
    }
    
    cout << "-----------------------------------------------------------" << endl;
}
