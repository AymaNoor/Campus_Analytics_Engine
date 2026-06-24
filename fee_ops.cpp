#include "fee_ops.h"
#include <iostream>
#include <iomanip>

using namespace std;

int toDayCount(const string& date) {
    int day = (date[0] - '0') * 10 + (date[1] - '0');
    int month = (date[3] - '0') * 10 + (date[4] - '0');
    int year = (date[6] - '0') * 1000 + (date[7] - '0') * 100 + (date[8] - '0') * 10 + (date[9] - '0');

    int months[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    int totalDays = 0;

    for (int y = 0; y < year; y++) {
        totalDays += 365;
        if ((y % 400 == 0) || (y % 4 == 0 && y % 100 != 0)) {
            totalDays += 1;
        }
    }

    for (int m = 1; m < month; m++) {
        totalDays += months[m - 1];
        if (m == 2 && ((year % 400 == 0) || (year % 4 == 0 && year % 100 != 0))) {
            totalDays += 1;
        }
    }

    totalDays += day;
    return totalDays;
}

int daysBetween(const string& date1, const string& date2) {
    if (date1.length() != 10 || date2.length() != 10) {
        return 0;
    }
    if (date1[2] != '-' || date1[5] != '-' || date2[2] != '-' || date2[5] != '-') {
        return 0;
    }

    int first = toDayCount(date1);
    int second = toDayCount(date2);

    return second - first;
}

double computeLateFine(const FeeRecord& record) {
    if (record.paidDate == "NOT_PAID") {
        return 0.0;
    }

    int overdueDays = daysBetween(record.dueDate, record.paidDate);
    if (overdueDays <= 0) {
        return 0.0;
    }

    int overdueWeeks = overdueDays / 7;
    double finePerWeek = record.amountDue * 0.02;
    return overdueWeeks * finePerWeek;
}

void recordPayment(FeeRecord fees[], int feeCount, const string& rollNumber, const string& semester, double paymentAmount, const string& paymentDate) {
    if (paymentDate.length() != 10 || paymentDate[2] != '-' || paymentDate[5] != '-') {
        cout << "Invalid payment date format. Expected DD-MM-YYYY." << endl;
        return;
    }

    if (paymentAmount <= 0.0) {
        cout << "Payment amount must be greater than 0." << endl;
        return;
    }

    int index = -1;
    for (int i = 0; i < feeCount; i++) {
        if (fees[i].rollNumber == rollNumber && fees[i].semester == semester) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        cout << "Fee record not found." << endl;
        return;
    }

    fees[index].amountPaid += paymentAmount;
    if (fees[index].amountPaid > fees[index].amountDue + computeLateFine(fees[index])) {
        fees[index].amountPaid = fees[index].amountDue + computeLateFine(fees[index]);
    }
    fees[index].paidDate = paymentDate;

    cout << "Payment recorded successfully." << endl;
}

void generateReceipt(const FeeRecord& record) {
    double fine = computeLateFine(record);
    double totalOutstanding = record.amountDue + fine;
    double remaining = totalOutstanding - record.amountPaid;
    if (remaining < 0.0) {
        remaining = 0.0;
    }

    cout << setfill('=') << setw(58) << "=" << endl;
    cout << setfill(' ') << setw(36) << "Campus Analytics Engine" << endl;
    cout << setw(33) << "Fee Payment Receipt" << endl;
    cout << setfill('=') << setw(58) << "=" << endl;

    cout << setfill(' ') << left << setw(20) << "Roll Number" << ": " << record.rollNumber << endl;
    cout << left << setw(20) << "Semester" << ": " << record.semester << endl;
    cout << left << setw(20) << "Due Date" << ": " << record.dueDate << endl;
    cout << left << setw(20) << "Paid Date" << ": " << record.paidDate << endl;

    cout << setfill('-') << setw(58) << "-" << endl;
    cout << setfill(' ');

    cout << fixed << setprecision(2);
    cout << left << setw(30) << "Tuition Due" << right << setw(12) << record.amountDue << endl;
    cout << left << setw(30) << "Late Fine" << right << setw(12) << fine << endl;
    cout << left << setw(30) << "Total Outstanding" << right << setw(12) << totalOutstanding << endl;
    cout << left << setw(30) << "Amount Paid" << right << setw(12) << record.amountPaid << endl;
    cout << left << setw(30) << "Remaining Balance" << right << setw(12) << remaining << endl;

    cout << setfill('=') << setw(58) << "=" << endl;
    cout << setfill(' ');
}

void getDefaulters(const FeeRecord fees[], int feeCount, const Student students[], int studentCount) {
    string rolls[MAX_RECORDS];
    string names[MAX_RECORDS];
    double outstanding[MAX_RECORDS];
    int weeksOverdue[MAX_RECORDS];
    int defaulterCount = 0;

    for (int i = 0; i < feeCount; i++) {
        double fine = computeLateFine(fees[i]);
        double dueWithFine = fees[i].amountDue + fine;
        double remain = dueWithFine - fees[i].amountPaid;

        if (remain <= 0.0) {
            continue;
        }

        int overdueDays = 0;
        if (fees[i].paidDate != "NOT_PAID") {
            overdueDays = daysBetween(fees[i].dueDate, fees[i].paidDate);
        } else {
            overdueDays = 1;
        }

        if (overdueDays <= 0) {
            continue;
        }

        string studentName = "Unknown";
        for (int s = 0; s < studentCount; s++) {
            if (students[s].roll == fees[i].rollNumber) {
                studentName = students[s].name;
                break;
            }
        }

        if (defaulterCount < MAX_RECORDS) {
            rolls[defaulterCount] = fees[i].rollNumber;
            names[defaulterCount] = studentName;
            outstanding[defaulterCount] = remain;
            weeksOverdue[defaulterCount] = overdueDays / 7;
            defaulterCount++;
        }
    }

    if (defaulterCount == 0) {
        cout << "\nNo defaulters found." << endl;
        return;
    }

    for (int pass = 0; pass < defaulterCount - 1; pass++) {
        for (int j = 0; j < defaulterCount - pass - 1; j++) {
            if (outstanding[j] < outstanding[j + 1]) {
                double tempOutstanding = outstanding[j];
                outstanding[j] = outstanding[j + 1];
                outstanding[j + 1] = tempOutstanding;

                int tempWeeks = weeksOverdue[j];
                weeksOverdue[j] = weeksOverdue[j + 1];
                weeksOverdue[j + 1] = tempWeeks;

                string tempRoll = rolls[j];
                rolls[j] = rolls[j + 1];
                rolls[j + 1] = tempRoll;

                string tempName = names[j];
                names[j] = names[j + 1];
                names[j + 1] = tempName;
            }
        }
    }

    cout << "\n================ Defaulters List ================" << endl;
    cout << left << setw(14) << "Roll Number"
         << setw(24) << "Name"
         << right << setw(16) << "Outstanding"
         << setw(16) << "Weeks Overdue" << endl;
    cout << "---------------------------------------------------------------" << endl;

    cout << fixed << setprecision(2);
    for (int i = 0; i < defaulterCount; i++) {
        cout << left << setw(14) << rolls[i]
             << setw(24) << names[i]
             << right << setw(16) << outstanding[i]
             << setw(16) << weeksOverdue[i] << endl;
    }
}
