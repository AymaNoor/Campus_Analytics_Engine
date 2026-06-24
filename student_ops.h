#ifndef STUDENT_OPS_H
#define STUDENT_OPS_H

#include <string>
#include <vector>

struct Student {
    std::string roll;
    std::string name;
    std::string dept;
    double cgpa;
    std::string status;
};

void addStudent(std::vector<Student>& students);
void searchByRoll(const std::vector<Student>& students);
void searchByName(const std::vector<Student>& students);
void updateStudent(std::vector<Student>& students);
void softDelete(std::vector<Student>& students);
void listActiveStudents(std::vector<Student>& students);
bool isValidRollFormat(const std::string& roll);
bool isValidName(const std::string& name);
bool isValidCGPA(double cgpa);
bool rollExists(const std::vector<Student>& students, const std::string& roll);

#endif
