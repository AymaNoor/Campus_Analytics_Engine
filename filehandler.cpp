#include "filehandler.h"
#include <fstream>
#include <iostream>

using namespace std;

// -------------------------------------------------------------------
// Helper: Parse a single CSV line into fields using a character-by-
// character loop. Handles double-quoted fields where commas inside
// quotes are treated as literal characters.
// -------------------------------------------------------------------
static void parseCSVLine(const string& line, vector<string>& fields) {
    fields.clear();
    string current;
    bool insideQuotes = false;

    for (size_t i = 0; i < line.length(); ++i) {
        char ch = line[i];

        if (ch == '"') {
            // Toggle quote state when we see a double-quote
            insideQuotes = !insideQuotes;
        } else if (ch == ',' && !insideQuotes) {
            // Comma outside quotes ends the current field
            fields.push_back(current);
            current.clear();
        } else {
            // Regular character or comma inside quotes
            current += ch;
        }
    }

    // Don't forget the last field (after the final comma or end-of-line)
    fields.push_back(current);
}

// -------------------------------------------------------------------
// Helper: Write a single row (vector of fields) to an output stream
// in CSV format. If a field contains a comma, it is wrapped in double
// quotes.
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
        // Skip empty lines
        if (line.empty())
            continue;

        if (firstLine) {
            // Skip the header row
            firstLine = false;
            continue;
        }

        // Parse the data row into fields
        vector<string> fields;
        parseCSVLine(line, fields);

        // Append all fields to the flat result vector
        for (size_t i = 0; i < fields.size(); ++i) {
            result.push_back(fields[i]);
        }
    }

    inFile.close();
    return result;
}

// ===================================================================
// 2. writeTXT — Overwrite file with a header row and data rows.
//    Any field containing a comma is automatically quoted.
// ===================================================================
void writeTXT(const string& filename, const string& header,
              const string rows[][10], int rowCount, int colCount) {
    ofstream outFile(filename.c_str());

    if (!outFile.is_open()) {
        cerr << "Error: Could not open file \"" << filename
             << "\" for writing.\n";
        return;
    }

    // Write the header row
    outFile << header << '\n';

    // Write each data row
    for (int r = 0; r < rowCount; ++r) {
        for (int c = 0; c < colCount; ++c) {
            if (c > 0)
                outFile << ',';

            const string& field = rows[r][c];
            bool needsQuotes = false;

            // Manual check for comma in the field (no <algorithm>)
            for (size_t i = 0; i < field.length(); ++i) {
                if (field[i] == ',') {
                    needsQuotes = true;
                    break;
                }
            }

            if (needsQuotes) {
                outFile << '"' << field << '"';
            } else {
                outFile << field;
            }
        }
        outFile << '\n';
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
        if (line.empty())
            continue;

        if (firstLine) {
            firstLine = false;
            continue;
        }

        vector<string> fields;
        parseCSVLine(line, fields);

        // Check if the requested column exists and matches
        if (colIndex >= 0 && colIndex < (int)fields.size()) {
            if (fields[colIndex] == targetValue) {
                result = fields;
                inFile.close();
                return result;
            }
        }
    }

    inFile.close();
    return result;  // empty — no match
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
