#ifndef FEE_OPS_H
#define FEE_OPS_H

#include <string>
#include "student_ops.h"

#ifndef CAMPUS_MAX_RECORDS_DEFINED
#define CAMPUS_MAX_RECORDS_DEFINED
const int MAX_RECORDS = 100;
#endif

struct FeeRecord {
    std::string rollNumber;
    std::string semester;
    double amountDue;
    double amountPaid;
    std::string dueDate;
    std::string paidDate;
};

int daysBetween(const std::string& date1, const std::string& date2);
double computeLateFine(const FeeRecord& record);
void recordPayment(FeeRecord fees[], int feeCount, const std::string& rollNumber, const std::string& semester, double paymentAmount, const std::string& paymentDate);
void generateReceipt(const FeeRecord& record);
void getDefaulters(const FeeRecord fees[], int feeCount, const Student students[], int studentCount);

#endif
