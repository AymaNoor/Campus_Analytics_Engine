#include "fee_ops.h"
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

// ===================================================================
// loadFees — Read fees.txt into array
// Columns: fee_id,roll_no,semester,total_fee,amount_paid,due_date,payment_date,payment_method,status
//          idx:   0       1        2          3            4         5          6               7               8
// ===================================================================
void loadFees(FeeRecord fees[], int& feeCount) {
    feeCount = 0;
    vector<string> flat = readTXT("fees.txt");
    const int COLS = 9;
    int rowCount = (int)flat.size() / COLS;

    for (int r = 0; r < rowCount && feeCount < MAX_FEE_RECORDS; r++) {
        FeeRecord f;
        f.feeId         = flat[r * COLS + 0];
        f.rollNumber    = flat[r * COLS + 1];
        f.semester      = flat[r * COLS + 2];
        f.amountDue     = strToDouble(flat[r * COLS + 3]);
        f.amountPaid    = strToDouble(flat[r * COLS + 4]);
        f.dueDate       = flat[r * COLS + 5];
        // Normalize sentinel: "00-00-0000" -> "NOT_PAID"
        string pd       = flat[r * COLS + 6];
        f.paidDate      = (pd == "00-00-0000" || pd.empty()) ? "NOT_PAID" : pd;
        f.paymentMethod = flat[r * COLS + 7];
        f.status        = flat[r * COLS + 8];
        fees[feeCount]  = f;
        feeCount++;
    }
    cout << "Loaded " << feeCount << " fee records from fees.txt\n";
}

// ===================================================================
// saveFees — Write all fee records back to fees.txt
// ===================================================================
void saveFees(const FeeRecord fees[], int feeCount) {
    vector<vector<string>> rows;
    for (int i = 0; i < feeCount; i++) {
        vector<string> row;
        row.push_back(fees[i].feeId);
        row.push_back(fees[i].rollNumber);
        row.push_back(fees[i].semester);
        row.push_back(doubleToStr(fees[i].amountDue));
        row.push_back(doubleToStr(fees[i].amountPaid));
        row.push_back(fees[i].dueDate);
        // Write "NOT_PAID" directly — no more 00-00-0000
        row.push_back(fees[i].paidDate);
        row.push_back(fees[i].paymentMethod);
        row.push_back(fees[i].status);
        rows.push_back(row);
    }
    writeTXT("fees.txt",
             "fee_id,roll_no,semester,total_fee,amount_paid,due_date,payment_date,payment_method,status",
             rows);
}

// ===================================================================
// toDayCount — convert DD-MM-YYYY to total day count (no ctime)
// ===================================================================
static int toDayCount(const string& date) {
    if (date.length() != 10 || date[2] != '-' || date[5] != '-') return 0;

    int day   = (date[0]-'0')*10 + (date[1]-'0');
    int month = (date[3]-'0')*10 + (date[4]-'0');
    int year  = (date[6]-'0')*1000 + (date[7]-'0')*100
              + (date[8]-'0')*10   + (date[9]-'0');

    // Manual month lengths (no ctime)
    int months[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

    int totalDays = 0;
    for (int y = 0; y < year; y++) {
        totalDays += 365;
        if ((y % 400 == 0) || (y % 4 == 0 && y % 100 != 0))
            totalDays += 1;
    }
    for (int m = 1; m < month; m++) {
        totalDays += months[m - 1];
        if (m == 2 && ((year % 400 == 0) || (year % 4 == 0 && year % 100 != 0)))
            totalDays += 1;
    }
    totalDays += day;
    return totalDays;
}

// ===================================================================
// daysBetween — returns (date2 - date1) in days; positive = date2 later
// ===================================================================
int daysBetween(const string& date1, const string& date2) {
    if (date1.length() != 10 || date2.length() != 10) return 0;
    if (date1 == "NOT_PAID" || date2 == "NOT_PAID") return 0;
    return toDayCount(date2) - toDayCount(date1);
}

// ===================================================================
// computeLateFine — 2% per complete week overdue
// ===================================================================
double computeLateFine(const FeeRecord& record) {
    if (record.paidDate == "NOT_PAID") return 0.0;

    int overdueDays = daysBetween(record.dueDate, record.paidDate);
    if (overdueDays <= 0) return 0.0;

    int overdueWeeks = overdueDays / 7;
    double finePerWeek = record.amountDue * 0.02;
    return overdueWeeks * finePerWeek;
}

// ===================================================================
// recordPayment — validate date, find record, update paid amount + date
// ===================================================================
void recordPayment(FeeRecord fees[], int& feeCount,
                   const string& rollNumber,
                   const string& semester,
                   double paymentAmount,
                   const string& paymentDate) {
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
            index = i; break;
        }
    }
    if (index == -1) { cout << "Fee record not found." << endl; return; }

    fees[index].amountPaid += paymentAmount;

    double totalDue = fees[index].amountDue + computeLateFine(fees[index]);
    if (fees[index].amountPaid >= totalDue) {
        fees[index].amountPaid = totalDue;
        fees[index].status = "paid";
    } else {
        fees[index].status = "partial";
    }

    fees[index].paidDate      = paymentDate;
    fees[index].paymentMethod = "Online"; // default (caller could prompt)

    // Persist
    saveFees(fees, feeCount);
    cout << "Payment recorded successfully." << endl;
}

// ===================================================================
// generateReceipt — formatted receipt using setw/setfill (iomanip)
// ===================================================================
void generateReceipt(const FeeRecord& record) {
    double fine           = computeLateFine(record);
    double totalOutstanding = record.amountDue + fine;
    double remaining      = totalOutstanding - record.amountPaid;
    if (remaining < 0.0) remaining = 0.0;

    cout << setfill('=') << setw(58) << "=" << endl;
    cout << setfill(' ') << setw(38) << "Campus Analytics Engine" << endl;
    cout << setw(35) << "Fee Payment Receipt" << endl;
    cout << setfill('=') << setw(58) << "=" << endl;

    cout << setfill(' ') << left << setw(20) << "Fee ID"       << ": " << record.feeId       << endl;
    cout << left << setw(20) << "Roll Number"  << ": " << record.rollNumber  << endl;
    cout << left << setw(20) << "Semester"     << ": " << record.semester    << endl;
    cout << left << setw(20) << "Due Date"     << ": " << record.dueDate     << endl;
    cout << left << setw(20) << "Paid Date"    << ": " << record.paidDate    << endl;
    cout << left << setw(20) << "Status"       << ": " << record.status      << endl;

    cout << setfill('-') << setw(58) << "-" << endl;
    cout << setfill(' ');

    cout << fixed << setprecision(2);
    cout << left << setw(32) << "Tuition Due"       << right << setw(12) << record.amountDue    << endl;
    cout << left << setw(32) << "Late Fine"         << right << setw(12) << fine                << endl;
    cout << left << setw(32) << "Total Outstanding" << right << setw(12) << totalOutstanding    << endl;
    cout << left << setw(32) << "Amount Paid"       << right << setw(12) << record.amountPaid   << endl;
    cout << left << setw(32) << "Remaining Balance" << right << setw(12) << remaining           << endl;

    cout << setfill('=') << setw(58) << "=" << endl;
    cout << setfill(' ');
}

// ===================================================================
// getDefaulters — students with balance > 0, past due date.
//                 todayDate passed in (DD-MM-YYYY) — no ctime.
//                 Sorted by outstanding amount (bubble sort).
// ===================================================================
void getDefaulters(const FeeRecord fees[], int feeCount,
                   const Student students[], int studentCount,
                   const string& todayDate) {
    // Temporary parallel arrays (no STL containers, no <map>)
    string rolls[MAX_FEE_RECORDS];
    string names[MAX_FEE_RECORDS];
    double outstanding[MAX_FEE_RECORDS];
    int    weeksOverdue[MAX_FEE_RECORDS];
    int    defaulterCount = 0;

    for (int i = 0; i < feeCount; i++) {
        double remain = fees[i].amountDue - fees[i].amountPaid;
        if (remain <= 0.0) continue;

        int overdueDays = daysBetween(fees[i].dueDate, todayDate);
        if (overdueDays <= 0) continue;

        string studentName = "Unknown";
        for (int s = 0; s < studentCount; s++) {
            if (students[s].roll == fees[i].rollNumber) {
                studentName = students[s].name; break;
            }
        }

        if (defaulterCount < MAX_FEE_RECORDS) {
            rolls[defaulterCount]        = fees[i].rollNumber;
            names[defaulterCount]        = studentName;
            outstanding[defaulterCount]  = remain;
            weeksOverdue[defaulterCount] = overdueDays / 7;
            defaulterCount++;
        }
    }

    if (defaulterCount == 0) {
        cout << "\nNo fee defaulters found." << endl;
        return;
    }

    // Bubble sort descending by outstanding amount
    for (int pass = 0; pass < defaulterCount - 1; pass++) {
        for (int j = 0; j < defaulterCount - pass - 1; j++) {
            if (outstanding[j] < outstanding[j + 1]) {
                double tmpO = outstanding[j];
                outstanding[j] = outstanding[j+1];
                outstanding[j+1] = tmpO;

                int tmpW = weeksOverdue[j];
                weeksOverdue[j] = weeksOverdue[j+1];
                weeksOverdue[j+1] = tmpW;

                string tmpR = rolls[j];
                rolls[j] = rolls[j+1];
                rolls[j+1] = tmpR;

                string tmpN = names[j];
                names[j] = names[j+1];
                names[j+1] = tmpN;
            }
        }
    }

    cout << "\n================ Defaulters List ================" << endl;
    cout << left  << setw(14) << "Roll Number"
         << setw(24) << "Name"
         << right << setw(16) << "Outstanding"
         << setw(16) << "Weeks Overdue" << endl;
    cout << "----------------------------------------------------------" << endl;

    cout << fixed << setprecision(2);
    for (int i = 0; i < defaulterCount; i++) {
        cout << left  << setw(14) << rolls[i]
             << setw(24) << names[i]
             << right << setw(16) << outstanding[i]
             << setw(16) << weeksOverdue[i] << endl;
    }
    cout << "==================================================" << endl;
}
