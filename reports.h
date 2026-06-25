#ifndef REPORTS_H
#define REPORTS_H

#include <string>
#include "student_ops.h"
#include "course_ops.h"
#include "fee_ops.h"
#include "grades_ops.h"
#include "attendance_ops.h"

// M7 — All report functions

void printMeritList(const Student students[], int studentCount);

void printAttendanceDefaulters(const Student students[], int studentCount,
                               const AttendanceRecord attendance[], int attendanceCount);

// todayDate passed in (DD-MM-YYYY) so we don't need <ctime>
void printFeeDefaulters(const FeeRecord fees[], int feeCount,
                        const Student students[], int studentCount,
                        const std::string& todayDate);

void printSemesterResult(const Student students[], int studentCount,
                         const GradeRecord grades[], int gradeCount,
                         const AttendanceRecord attendance[], int attendanceCount,
                         const Course courses[], int courseCount);

void printDepartmentSummary(const Student students[], int studentCount);

void exportReportToFile(int reportChoice,
                        const Student students[], int studentCount,
                        const FeeRecord fees[], int feeCount,
                        const GradeRecord grades[], int gradeCount,
                        const AttendanceRecord attendance[], int attendanceCount,
                        const Course courses[], int courseCount,
                        const std::string& todayDate);

#endif
