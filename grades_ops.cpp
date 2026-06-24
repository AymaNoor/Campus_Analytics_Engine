#include "grades_ops.h"
#include <iostream>

using namespace std;

double bestThreeOfFive(const double quizzes[], int quizCount) {
    if (quizCount <= 0) {
        return 0.0;
    }

    int takeCount = quizCount;
    if (takeCount > 3) {
        takeCount = 3;
    }

    bool used[5] = {false, false, false, false, false};
    double sum = 0.0;

    for (int picked = 0; picked < takeCount; picked++) {
        int bestIndex = -1;

        for (int i = 0; i < quizCount; i++) {
            if (!used[i]) {
                if (bestIndex == -1 || quizzes[i] > quizzes[bestIndex]) {
                    bestIndex = i;
                }
            }
        }

        if (bestIndex != -1) {
            used[bestIndex] = true;
            sum += quizzes[bestIndex];
        }
    }

    return sum / takeCount;
}

double computeWeightedTotal(double quizAvg, const double assignments[], double mid, double finalExam) {
    double assignmentSum = 0.0;
    for (int i = 0; i < 3; i++) {
        assignmentSum += assignments[i];
    }

    double assignmentAvg = assignmentSum / 3.0;
    return (quizAvg * 0.10) + (assignmentAvg * 0.10) + (mid * 0.30) + (finalExam * 0.50);
}

string getLetterGrade(double totalMark) {
    if (totalMark >= 85.0) return "A";
    if (totalMark >= 80.0) return "B+";
    if (totalMark >= 70.0) return "B";
    if (totalMark >= 65.0) return "C+";
    if (totalMark >= 60.0) return "C";
    if (totalMark >= 50.0) return "D";
    return "F";
}

void enterMarks(GradeRecord grades[], int& gradeCount, const Student students[], int studentCount, const Course courses[], int courseCount) {
    if (gradeCount >= MAX_RECORDS) {
        cout << "Grade storage is full. Cannot add more records." << endl;
        return;
    }

    GradeRecord record;

    cout << "\n=== Enter Student Marks ===" << endl;

    cout << "Enter Course Code: ";
    getline(cin, record.courseCode);

    bool courseFound = false;
    for (int i = 0; i < courseCount; i++) {
        if (courses[i].courseCode == record.courseCode) {
            courseFound = true;
            break;
        }
    }

    if (!courseFound) {
        cout << "Course not found." << endl;
        return;
    }

    cout << "Enter Student Roll Number: ";
    getline(cin, record.rollNumber);

    bool studentFound = false;
    for (int i = 0; i < studentCount; i++) {
        if (students[i].roll == record.rollNumber && students[i].status == "active") {
            studentFound = true;
            break;
        }
    }

    if (!studentFound) {
        cout << "Active student not found." << endl;
        return;
    }

    cout << "Enter number of quizzes (1-5): ";
    cin >> record.quizCount;
    cin.ignore();

    if (record.quizCount < 1 || record.quizCount > 5) {
        cout << "Invalid quiz count." << endl;
        return;
    }

    for (int i = 0; i < 5; i++) {
        record.quizzes[i] = 0.0;
    }

    for (int i = 0; i < record.quizCount; i++) {
        cout << "Quiz " << (i + 1) << " mark (0-100): ";
        cin >> record.quizzes[i];
        cin.ignore();

        if (record.quizzes[i] < 0.0 || record.quizzes[i] > 100.0) {
            cout << "Invalid quiz mark." << endl;
            return;
        }
    }

    for (int i = 0; i < 3; i++) {
        cout << "Assignment " << (i + 1) << " mark (0-100): ";
        cin >> record.assignments[i];
        cin.ignore();

        if (record.assignments[i] < 0.0 || record.assignments[i] > 100.0) {
            cout << "Invalid assignment mark." << endl;
            return;
        }
    }

    cout << "Mid-term mark (0-40): ";
    cin >> record.midTerm;
    cin.ignore();
    if (record.midTerm < 0.0 || record.midTerm > 40.0) {
        cout << "Invalid mid-term mark." << endl;
        return;
    }

    cout << "Final exam mark (0-60): ";
    cin >> record.finalExam;
    cin.ignore();
    if (record.finalExam < 0.0 || record.finalExam > 60.0) {
        cout << "Invalid final exam mark." << endl;
        return;
    }

    double quizAvg = bestThreeOfFive(record.quizzes, record.quizCount);
    record.totalWeightedMark = computeWeightedTotal(quizAvg, record.assignments, record.midTerm, record.finalExam);
    record.letterGrade = getLetterGrade(record.totalWeightedMark);

    double attendancePct;
    cout << "Attendance percentage (0-100): ";
    cin >> attendancePct;
    cin.ignore();
    if (attendancePct < 0.0 || attendancePct > 100.0) {
        cout << "Invalid attendance percentage." << endl;
        return;
    }

    applyAttendancePenalty(record, attendancePct);

    grades[gradeCount] = record;
    gradeCount++;

    cout << "Marks saved successfully." << endl;
    cout << "Total Weighted Mark: " << record.totalWeightedMark << endl;
    cout << "Letter Grade: " << record.letterGrade << endl;
}

void computeClassStats(const GradeRecord grades[], int gradeCount, const string& courseCode) {
    ClassStats stats;
    stats.highest = 0.0;
    stats.lowest = 0.0;
    stats.mean = 0.0;

    int matched = 0;
    double total = 0.0;

    for (int i = 0; i < gradeCount; i++) {
        if (grades[i].courseCode == courseCode) {
            if (matched == 0) {
                stats.highest = grades[i].totalWeightedMark;
                stats.lowest = grades[i].totalWeightedMark;
            }

            if (grades[i].totalWeightedMark > stats.highest) {
                stats.highest = grades[i].totalWeightedMark;
            }
            if (grades[i].totalWeightedMark < stats.lowest) {
                stats.lowest = grades[i].totalWeightedMark;
            }

            total += grades[i].totalWeightedMark;
            matched++;
        }
    }

    if (matched == 0) {
        cout << "\nNo grade records found for course " << courseCode << "." << endl;
        return;
    }

    stats.mean = total / matched;

    cout << "\n=== Class Statistics for " << courseCode << " ===" << endl;
    cout << "----------------------------------" << endl;
    cout.width(18);
    cout << left << "Highest:" << stats.highest << endl;
    cout.width(18);
    cout << left << "Lowest:" << stats.lowest << endl;
    cout.width(18);
    cout << left << "Mean:" << stats.mean << endl;
    cout << "----------------------------------" << endl;
}

double computeGPA(const GradeRecord grades[], int gradeCount, const string& rollNumber) {
    double totalGradePoints = 0.0;
    double totalCredits = 0.0;

    for (int i = 0; i < gradeCount; i++) {
        if (grades[i].rollNumber == rollNumber) {
            double points = 0.0;

            if (grades[i].letterGrade == "A") points = 4.0;
            else if (grades[i].letterGrade == "B+") points = 3.5;
            else if (grades[i].letterGrade == "B") points = 3.0;
            else if (grades[i].letterGrade == "C+") points = 2.5;
            else if (grades[i].letterGrade == "C") points = 2.0;
            else if (grades[i].letterGrade == "D") points = 1.0;
            else points = 0.0;

            double creditWeight = 1.0;
            totalGradePoints += points * creditWeight;
            totalCredits += creditWeight;
        }
    }

    if (totalCredits == 0.0) {
        return 0.0;
    }

    return totalGradePoints / totalCredits;
}

void computeClassState(const GradeRecord grades[], int gradeCount, const string& courseCode) {
    double marks[MAX_RECORDS];
    int marksCount = 0;

    for (int i = 0; i < gradeCount; i++) {
        if (grades[i].courseCode == courseCode) {
            if (marksCount < MAX_RECORDS) {
                marks[marksCount] = grades[i].totalWeightedMark;
                marksCount++;
            }
        }
    }

    if (marksCount == 0) {
        cout << "\nNo grade records found for course " << courseCode << "." << endl;
        return;
    }

    double highest = marks[0];
    double lowest = marks[0];
    double sum = 0.0;

    for (int i = 0; i < marksCount; i++) {
        if (marks[i] > highest) highest = marks[i];
        if (marks[i] < lowest) lowest = marks[i];
        sum += marks[i];
    }

    for (int pass = 0; pass < marksCount - 1; pass++) {
        for (int j = 0; j < marksCount - 1 - pass; j++) {
            if (marks[j] > marks[j + 1]) {
                double temp = marks[j];
                marks[j] = marks[j + 1];
                marks[j + 1] = temp;
            }
        }
    }

    double mean = sum / marksCount;
    double median;

    if (marksCount % 2 == 1) {
        median = marks[marksCount / 2];
    } else {
        median = (marks[(marksCount / 2) - 1] + marks[marksCount / 2]) / 2.0;
    }

    cout << "\n=== Class State for " << courseCode << " ===" << endl;
    cout << "----------------------------------" << endl;
    cout.width(18);
    cout << left << "Highest:" << highest << endl;
    cout.width(18);
    cout << left << "Lowest:" << lowest << endl;
    cout.width(18);
    cout << left << "Mean:" << mean << endl;
    cout.width(18);
    cout << left << "Median:" << median << endl;
    cout << "----------------------------------" << endl;
}

void applyAttendancePenalty(GradeRecord& grade, double attendancePct) {
    if (attendancePct < 75.0) {
        grade.letterGrade = "F";
    }
}
