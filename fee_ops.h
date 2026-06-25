#ifndef FEE_OPS_H
#define FEE_OPS_H

#include <string>
#include <vector>
#include "student_ops.h"

const int MAX_FEE_RECORDS = 200;

struct FeeRecord {
    std::string feeId;        // e.g. "F001"
    std::string rollNumber;
    std::string semester;
    double      amountDue;
    double      amountPaid;
    std::string dueDate;      // DD-MM-YYYY
    std::string paidDate;     // DD-MM-YYYY or "NOT_PAID"
    std::string paymentMethod;
    std::string status;       // "paid", "partial", "unpaid", "paid_late"
};

// --- File I/O ---
void loadFees(FeeRecord fees[], int& feeCount);
void saveFees(const FeeRecord fees[], int feeCount);

// --- M6 Spec functions ---
int    daysBetween(const std::string& date1, const std::string& date2);
double computeLateFine(const FeeRecord& record);
void   recordPayment(FeeRecord fees[], int& feeCount,
                     const std::string& rollNumber,
                     const std::string& semester,
                     double paymentAmount,
                     const std::string& paymentDate);
void   generateReceipt(const FeeRecord& record);
void   getDefaulters(const FeeRecord fees[], int feeCount,
                     const Student students[], int studentCount,
                     const std::string& todayDate);

#endif
