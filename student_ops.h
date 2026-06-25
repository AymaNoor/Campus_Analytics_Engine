#ifndef STUDENT_OPS_H
#define STUDENT_OPS_H

#include <string>

const int MAX_STUDENT_RECORDS = 200;

struct Student {
    std::string roll;
    std::string name;
    std::string dept;
    std::string semester;   // e.g. "2"
    double cgpa;
    std::string status;     // "active" or "inactive"
};

// --- File I/O ---
void loadStudents(Student students[], int& studentCount);
void saveStudents(const Student students[], int studentCount);

// --- Core operations ---
void addStudent(Student students[], int& studentCount);
void searchByRoll(Student students[], int studentCount);
void searchByName(Student students[], int studentCount);
void updateStudent(Student students[], int& studentCount);
void softDelete(Student students[], int& studentCount);
void listActiveStudents(Student students[], int studentCount);

// --- Validation helpers ---
bool isValidRollFormat(const std::string& roll);
bool isValidName(const std::string& name);
bool isValidCGPA(double cgpa);
bool rollExists(Student students[], int studentCount, const std::string& roll);

#endif
