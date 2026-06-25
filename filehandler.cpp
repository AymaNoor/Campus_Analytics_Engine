#include "filehandler.h"
#include <fstream>
#include <iostream>
#include <limits>

using namespace std;

// ===================================================================
// Safe input helpers — EOF / bad-input protection
// ===================================================================

// Read an integer from stdin safely.
// Returns fallback if EOF (Ctrl+Z) or non-numeric input is given.
int safeReadInt(int fallback) {
    if (cin.eof()) return fallback;
    int val;
    if (!(cin >> val)) {
        cin.clear();
        if (cin.eof()) return fallback;
        // Discard the bad token up to newline
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return fallback;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return val;
}

// Read a double from stdin safely.
// Returns fallback (-999.0 signals "user skipped / bad input").
double safeReadDouble(double fallback) {
    if (cin.eof()) return fallback;
    double val;
    if (!(cin >> val)) {
        cin.clear();
        if (cin.eof()) return fallback;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return fallback;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return val;
}

// Read a full line from stdin safely.
// Returns empty string on EOF or error.
string safeReadLine() {
    if (cin.eof()) return "";
    string line;
    if (!getline(cin, line)) {
        if (!cin.eof()) cin.clear();
        return "";
    }
    return line;
}


// -------------------------------------------------------------------
// Helper: Parse a single CSV line into fields using a character-by-
// character loop. Handles double-quoted fields (commas inside quotes
// are treated as literal characters).
// -------------------------------------------------------------------
static void parseCSVLine(const string& line, vector<string>& fields) {
    fields.clear();
    string current;
    bool insideQuotes = false;

    for (size_t i = 0; i < line.length(); ++i) {
        char ch = line[i];

        if (ch == '"') {
            insideQuotes = !insideQuotes;
        } else if (ch == ',' && !insideQuotes) {
            fields.push_back(current);
            current.clear();
        } else {
            current += ch;
        }
    }
    // Last field
    fields.push_back(current);
}

// -------------------------------------------------------------------
// Helper: Write a single row (vector of fields) to an output stream
// in CSV format. Fields containing commas are wrapped in double quotes.
// -------------------------------------------------------------------
static void writeCSVRow(ofstream& out, const vector<string>& fields) {
    for (size_t i = 0; i < fields.size(); ++i) {
        if (i > 0)
            out << ',';

        bool needsQuotes = false;
        for (size_t j = 0; j < fields[i].length(); ++j) {
            if (fields[i][j] == ',') {
                needsQuotes = true;
                break;
            }
        }

        if (needsQuotes) {
            out << '"' << fields[i] << '"';
        } else {
            out << fields[i];
        }
    }
    out << '\n';
}

// ===================================================================
// 1. readTXT — Read entire CSV file (skip header), return all fields
//    flattened into a single vector (row-major order).
// ===================================================================
vector<string> readTXT(const string& filename) {
    vector<string> result;
    ifstream inFile(filename.c_str());

    if (!inFile.is_open()) {
        cerr << "Error: Could not open file \"" << filename
             << "\" for reading.\n";
        return result;
    }

    string line;
    bool firstLine = true;

    while (getline(inFile, line)) {
        // Strip trailing \r for Windows line endings
        if (!line.empty() && line[line.size() - 1] == '\r') {
            line.erase(line.size() - 1);
        }
        if (line.empty())
            continue;

        if (firstLine) {
            firstLine = false;
            continue; // skip header
        }

        vector<string> fields;
        parseCSVLine(line, fields);

        for (size_t i = 0; i < fields.size(); ++i) {
            result.push_back(fields[i]);
        }
    }

    inFile.close();
    return result;
}

// ===================================================================
// 2. writeTXT — Overwrite file with a header row and data rows.
//    Accepts vector<vector<string>> — one inner vector per row.
//    Any field containing a comma is automatically quoted.
// ===================================================================
void writeTXT(const string& filename,
              const string& header,
              const vector<vector<string>>& rows) {
    ofstream outFile(filename.c_str());

    if (!outFile.is_open()) {
        cerr << "Error: Could not open file \"" << filename
             << "\" for writing.\n";
        return;
    }

    outFile << header << '\n';

    for (size_t r = 0; r < rows.size(); ++r) {
        writeCSVRow(outFile, rows[r]);
    }

    outFile.close();
}

// ===================================================================
// 3. appendTXT — Append a single CSV row to the end of the file
//    without reading the rest into memory.
// ===================================================================
void appendTXT(const string& filename,
               const vector<string>& fields) {
    ofstream outFile(filename.c_str(), ios::app);

    if (!outFile.is_open()) {
        cerr << "Error: Could not open file \"" << filename
             << "\" for appending.\n";
        return;
    }

    writeCSVRow(outFile, fields);
    outFile.close();
}

// ===================================================================
// 4. findRow — Linear scan of the file; returns the fields of the
//    first row where the field at colIndex matches targetValue.
//    Returns an empty vector if no match is found.
// ===================================================================
vector<string> findRow(const string& filename,
                       const string& targetValue,
                       int colIndex) {
    vector<string> result;
    ifstream inFile(filename.c_str());

    if (!inFile.is_open()) {
        cerr << "Error: Could not open file \"" << filename
             << "\" for reading.\n";
        return result;
    }

    string line;
    bool firstLine = true;

    while (getline(inFile, line)) {
        if (!line.empty() && line[line.size() - 1] == '\r') {
            line.erase(line.size() - 1);
        }
        if (line.empty())
            continue;

        if (firstLine) {
            firstLine = false;
            continue;
        }

        vector<string> fields;
        parseCSVLine(line, fields);

        if (colIndex >= 0 && colIndex < (int)fields.size()) {
            if (fields[colIndex] == targetValue) {
                result = fields;
                inFile.close();
                return result;
            }
        }
    }

    inFile.close();
    return result;
}

// ===================================================================
// 5. rowExists — Same as findRow, but returns true/false only.
//    Exits early as soon as a match is found.
// ===================================================================
bool rowExists(const string& filename,
               const string& targetValue,
               int colIndex) {
    ifstream inFile(filename.c_str());

    if (!inFile.is_open()) {
        cerr << "Error: Could not open file \"" << filename
             << "\" for reading.\n";
        return false;
    }

    string line;
    bool firstLine = true;

    while (getline(inFile, line)) {
        if (!line.empty() && line[line.size() - 1] == '\r') {
            line.erase(line.size() - 1);
        }
        if (line.empty())
            continue;

        if (firstLine) {
            firstLine = false;
            continue;
        }

        vector<string> fields;
        parseCSVLine(line, fields);

        if (colIndex >= 0 && colIndex < (int)fields.size()) {
            if (fields[colIndex] == targetValue) {
                inFile.close();
                return true;
            }
        }
    }

    inFile.close();
    return false;
}
