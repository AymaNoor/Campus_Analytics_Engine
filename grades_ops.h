#ifndef GRADES_OPS_H
#define GRADES_OPS_H

#include <string>
#include "student_ops.h"
#include "course_ops.h"

const int MAX_RECORDS = 100;

struct GradeRecord {
    std::string rollNumber;
    std::string courseCode;
    double quizzes[5];
    int quizCount;
    double assignments[3];
    double midTerm;
    double finalExam;
    double totalWeightedMark;
    std::string letterGrade;
};

struct ClassStats {
    double highest;
    double lowest;
    double mean;
};

double bestThreeOfFive(const double quizzes[], int quizCount);
double computeWeightedTotal(double quizAvg, const double assignments[], double mid, double finalExam);
std::string getLetterGrade(double totalMark);
void enterMarks(GradeRecord grades[], int& gradeCount, const Student students[], int studentCount, const Course courses[], int courseCount);
void computeClassStats(const GradeRecord grades[], int gradeCount, const std::string& courseCode);
double computeGPA(const GradeRecord grades[], int gradeCount, const std::string& rollNumber);
void computeClassState(const GradeRecord grades[], int gradeCount, const std::string& courseCode);
void applyAttendancePenalty(GradeRecord& grade, double attendancePct);

#endif
