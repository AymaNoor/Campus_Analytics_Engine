#include "grades_ops.h"
#include "filehandler.h"
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

// -------------------------------------------------------------------
// Helpers
// -------------------------------------------------------------------
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

static int strToInt(const string& s) {
    int v = 0;
    for (size_t i = 0; i < s.size(); i++)
        if (s[i] >= '0' && s[i] <= '9') v = v * 10 + (s[i] - '0');
    return v;
}

// ===================================================================
// loadGrades — Read grades.txt
// Columns (per row): roll,course_code,q1,q2,q3,q4,q5,quizCount,
//                    a1,a2,a3,midTerm,finalExam,total,grade
//                    idx: 0   1        2  3  4  5  6   7
//                         8   9  10   11  12       13    14
// ===================================================================
void loadGrades(GradeRecord grades[], int& gradeCount) {
    gradeCount = 0;

    // Check if file exists by attempting readTXT
    vector<string> flat = readTXT("grades.txt");
    if (flat.empty()) {
        cout << "grades.txt not found or empty — starting fresh.\n";
        return;
    }

    const int COLS = 15;
    int rowCount = (int)flat.size() / COLS;

    for (int r = 0; r < rowCount && gradeCount < MAX_GRADE_RECORDS; r++) {
        GradeRecord g;
        g.rollNumber       = flat[r * COLS + 0];
        g.courseCode       = flat[r * COLS + 1];
        g.quizzes[0]       = strToDouble(flat[r * COLS + 2]);
        g.quizzes[1]       = strToDouble(flat[r * COLS + 3]);
        g.quizzes[2]       = strToDouble(flat[r * COLS + 4]);
        g.quizzes[3]       = strToDouble(flat[r * COLS + 5]);
        g.quizzes[4]       = strToDouble(flat[r * COLS + 6]);
        g.quizCount        = strToInt(flat[r * COLS + 7]);
        g.assignments[0]   = strToDouble(flat[r * COLS + 8]);
        g.assignments[1]   = strToDouble(flat[r * COLS + 9]);
        g.assignments[2]   = strToDouble(flat[r * COLS + 10]);
        g.midTerm          = strToDouble(flat[r * COLS + 11]);
        g.finalExam        = strToDouble(flat[r * COLS + 12]);
        g.totalWeightedMark = strToDouble(flat[r * COLS + 13]);
        g.letterGrade      = flat[r * COLS + 14];
        grades[gradeCount] = g;
        gradeCount++;
    }
    cout << "Loaded " << gradeCount << " grade records from grades.txt\n";
}

// ===================================================================
// saveGrades — Write all grade records to grades.txt
// ===================================================================
void saveGrades(const GradeRecord grades[], int gradeCount) {
    vector<vector<string>> rows;
    for (int i = 0; i < gradeCount; i++) {
        vector<string> row;
        row.push_back(grades[i].rollNumber);
        row.push_back(grades[i].courseCode);
        for (int q = 0; q < 5; q++)
            row.push_back(doubleToStr(grades[i].quizzes[q]));
        row.push_back(doubleToStr(grades[i].quizCount));
        for (int a = 0; a < 3; a++)
            row.push_back(doubleToStr(grades[i].assignments[a]));
        row.push_back(doubleToStr(grades[i].midTerm));
        row.push_back(doubleToStr(grades[i].finalExam));
        row.push_back(doubleToStr(grades[i].totalWeightedMark));
        row.push_back(grades[i].letterGrade);
        rows.push_back(row);
    }
    writeTXT("grades.txt",
             "roll,course_code,q1,q2,q3,q4,q5,quizCount,a1,a2,a3,midTerm,finalExam,total,grade",
             rows);
}

// ===================================================================
// bestThreeOfFive — finds best 3 (or fewer) quiz scores using a loop
// ===================================================================
double bestThreeOfFive(const double quizzes[], int quizCount) {
    if (quizCount <= 0) return 0.0;

    int takeCount = (quizCount > 3) ? 3 : quizCount;
    bool used[5] = {false, false, false, false, false};
    double sum = 0.0;

    for (int picked = 0; picked < takeCount; picked++) {
        int bestIndex = -1;
        for (int i = 0; i < quizCount; i++) {
            if (!used[i]) {
                if (bestIndex == -1 || quizzes[i] > quizzes[bestIndex])
                    bestIndex = i;
            }
        }
        if (bestIndex != -1) {
            used[bestIndex] = true;
            sum += quizzes[bestIndex];
        }
    }
    return sum / takeCount;
}

// ===================================================================
// computeWeightedTotal — quiz*0.10 + asgn*0.10 + mid*0.30 + final*0.50
// ===================================================================
double computeWeightedTotal(double quizAvg, const double assignments[],
                            double mid, double finalExam) {
    double asgSum = 0.0;
    for (int i = 0; i < 3; i++) asgSum += assignments[i];
    double asgAvg = asgSum / 3.0;
    return (quizAvg * 0.10) + (asgAvg * 0.10) + (mid * 0.30) + (finalExam * 0.50);
}

// ===================================================================
// getLetterGrade
// ===================================================================
string getLetterGrade(double totalMark) {
    if (totalMark >= 85.0) return "A";
    if (totalMark >= 80.0) return "B+";
    if (totalMark >= 70.0) return "B";
    if (totalMark >= 65.0) return "C+";
    if (totalMark >= 60.0) return "C";
    if (totalMark >= 50.0) return "D";
    return "F";
}

// ===================================================================
// applyAttendancePenalty — if attendance < 75%, force grade to F
// ===================================================================
void applyAttendancePenalty(GradeRecord& grade, double attendancePct) {
    if (attendancePct < 75.0) {
        grade.letterGrade = "F";
    }
}

// ===================================================================
// enterMarks
// ===================================================================
void enterMarks(GradeRecord grades[], int& gradeCount,
                const Student students[], int studentCount,
                const Course courses[], int courseCount) {
    if (gradeCount >= MAX_GRADE_RECORDS) {
        cout << "Grade storage is full." << endl;
        return;
    }

    GradeRecord record;
    cout << "\n=== Enter Student Marks ===" << endl;

    cout << "Enter Course Code: ";
    record.courseCode = safeReadLine();
    if (record.courseCode.empty()) { cout << "Input cancelled.\n"; return; }

    bool courseFound = false;
    for (int i = 0; i < courseCount; i++) {
        if (courses[i].courseCode == record.courseCode) { courseFound = true; break; }
    }
    if (!courseFound) { cout << "Course not found." << endl; return; }

    cout << "Enter Student Roll Number: ";
    record.rollNumber = safeReadLine();
    if (record.rollNumber.empty()) { cout << "Input cancelled.\n"; return; }

    bool studentFound = false;
    for (int i = 0; i < studentCount; i++) {
        if (students[i].roll == record.rollNumber && students[i].status == "active") {
            studentFound = true; break;
        }
    }
    if (!studentFound) { cout << "Active student not found." << endl; return; }

    cout << "Enter number of quizzes (1-5): ";
    record.quizCount = safeReadInt();
    if (record.quizCount < 1 || record.quizCount > 5) {
        cout << "Invalid quiz count." << endl; return;
    }

    for (int i = 0; i < 5; i++) record.quizzes[i] = 0.0;

    for (int i = 0; i < record.quizCount; i++) {
        cout << "Quiz " << (i+1) << " mark (0-100): ";
        record.quizzes[i] = safeReadDouble();
        if (record.quizzes[i] == -999.0) { cout << "Input cancelled.\n"; return; }
        if (record.quizzes[i] < 0.0 || record.quizzes[i] > 100.0) {
            cout << "Invalid quiz mark." << endl; return;
        }
    }

    for (int i = 0; i < 3; i++) {
        cout << "Assignment " << (i+1) << " mark (0-100): ";
        record.assignments[i] = safeReadDouble();
        if (record.assignments[i] == -999.0) { cout << "Input cancelled.\n"; return; }
        if (record.assignments[i] < 0.0 || record.assignments[i] > 100.0) {
            cout << "Invalid assignment mark." << endl; return;
        }
    }

    cout << "Mid-term mark (0-40): ";
    record.midTerm = safeReadDouble();
    if (record.midTerm == -999.0) { cout << "Input cancelled.\n"; return; }
    if (record.midTerm < 0.0 || record.midTerm > 40.0) {
        cout << "Invalid mid-term mark." << endl; return;
    }

    cout << "Final exam mark (0-60): ";
    record.finalExam = safeReadDouble();
    if (record.finalExam == -999.0) { cout << "Input cancelled.\n"; return; }
    if (record.finalExam < 0.0 || record.finalExam > 60.0) {
        cout << "Invalid final exam mark." << endl; return;
    }

    double quizAvg = bestThreeOfFive(record.quizzes, record.quizCount);
    record.totalWeightedMark = computeWeightedTotal(quizAvg, record.assignments,
                                                     record.midTerm, record.finalExam);
    record.letterGrade = getLetterGrade(record.totalWeightedMark);

    cout << "Attendance percentage (0-100): ";
    double attendancePct = safeReadDouble();
    if (attendancePct == -999.0) { cout << "Input cancelled.\n"; return; }
    if (attendancePct < 0.0 || attendancePct > 100.0) {
        cout << "Invalid attendance percentage." << endl; return;
    }

    applyAttendancePenalty(record, attendancePct);

    grades[gradeCount] = record;
    gradeCount++;

    // Persist
    saveGrades(grades, gradeCount);

    cout << "Marks saved successfully." << endl;
    cout << "Total Weighted Mark: " << record.totalWeightedMark << endl;
    cout << "Letter Grade: " << record.letterGrade << endl;
}

// ===================================================================
// computeClassStats
// ===================================================================
void computeClassStats(const GradeRecord grades[], int gradeCount,
                       const string& courseCode) {
    int matched = 0;
    double total = 0.0;
    double highest = 0.0, lowest = 0.0;

    for (int i = 0; i < gradeCount; i++) {
        if (grades[i].courseCode == courseCode) {
            if (matched == 0) {
                highest = grades[i].totalWeightedMark;
                lowest  = grades[i].totalWeightedMark;
            }
            if (grades[i].totalWeightedMark > highest) highest = grades[i].totalWeightedMark;
            if (grades[i].totalWeightedMark < lowest)  lowest  = grades[i].totalWeightedMark;
            total += grades[i].totalWeightedMark;
            matched++;
        }
    }

    if (matched == 0) {
        cout << "\nNo grade records found for course " << courseCode << "." << endl;
        return;
    }

    double mean = total / matched;

    cout << "\n=== Class Statistics: " << courseCode << " ===" << endl;
    cout << "----------------------------------" << endl;
    cout << left << setw(18) << "Highest:" << highest << endl;
    cout << left << setw(18) << "Lowest:"  << lowest  << endl;
    cout << left << setw(18) << "Mean:"    << mean    << endl;
    cout << "----------------------------------" << endl;
}

// ===================================================================
// computeGPA — credit-weighted average using courses array
// ===================================================================
double computeGPA(const GradeRecord grades[], int gradeCount,
                  const string& rollNumber,
                  const Course courses[], int courseCount) {
    double totalGradePoints = 0.0;
    double totalCredits     = 0.0;

    for (int i = 0; i < gradeCount; i++) {
        if (grades[i].rollNumber != rollNumber) continue;

        double points = 0.0;
        if      (grades[i].letterGrade == "A")  points = 4.0;
        else if (grades[i].letterGrade == "B+") points = 3.5;
        else if (grades[i].letterGrade == "B")  points = 3.0;
        else if (grades[i].letterGrade == "C+") points = 2.5;
        else if (grades[i].letterGrade == "C")  points = 2.0;
        else if (grades[i].letterGrade == "D")  points = 1.0;
        else                                    points = 0.0;

        // Find credit hours for this course
        double cr = 1.0;
        for (int c = 0; c < courseCount; c++) {
            if (courses[c].courseCode == grades[i].courseCode) {
                cr = (double)courses[c].creditHours; break;
            }
        }
        totalGradePoints += points * cr;
        totalCredits     += cr;
    }

    return (totalCredits == 0.0) ? 0.0 : totalGradePoints / totalCredits;
}

// ===================================================================
// computeClassState — highest, lowest, mean, median (bubble sort)
// ===================================================================
void computeClassState(const GradeRecord grades[], int gradeCount,
                       const string& courseCode) {
    double marks[MAX_GRADE_RECORDS];
    int marksCount = 0;

    for (int i = 0; i < gradeCount; i++) {
        if (grades[i].courseCode == courseCode && marksCount < MAX_GRADE_RECORDS)
            marks[marksCount++] = grades[i].totalWeightedMark;
    }

    if (marksCount == 0) {
        cout << "\nNo grade records found for course " << courseCode << "." << endl;
        return;
    }

    double highest = marks[0], lowest = marks[0], sum = 0.0;
    for (int i = 0; i < marksCount; i++) {
        if (marks[i] > highest) highest = marks[i];
        if (marks[i] < lowest)  lowest  = marks[i];
        sum += marks[i];
    }

    // Bubble sort for median
    for (int pass = 0; pass < marksCount - 1; pass++)
        for (int j = 0; j < marksCount - 1 - pass; j++)
            if (marks[j] > marks[j+1]) {
                double tmp = marks[j]; marks[j] = marks[j+1]; marks[j+1] = tmp;
            }

    double mean   = sum / marksCount;
    double median = (marksCount % 2 == 1)
                  ? marks[marksCount / 2]
                  : (marks[(marksCount/2)-1] + marks[marksCount/2]) / 2.0;

    cout << "\n=== Class State: " << courseCode << " ===" << endl;
    cout << "----------------------------------" << endl;
    cout << left << setw(18) << "Highest:" << highest << endl;
    cout << left << setw(18) << "Lowest:"  << lowest  << endl;
    cout << left << setw(18) << "Mean:"    << mean    << endl;
    cout << left << setw(18) << "Median:"  << median  << endl;
    cout << "----------------------------------" << endl;
}
