#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <string>
#include <vector>

// Read entire CSV file (skip header), return all fields flattened row-major.
std::vector<std::string> readTXT(const std::string& filename);

// Overwrite file with header + rows. Each row is a vector<string> of fields.
void writeTXT(const std::string& filename,
              const std::string& header,
              const std::vector<std::vector<std::string>>& rows);

// Append a single CSV row to the end of the file without loading all data.
void appendTXT(const std::string& filename,
               const std::vector<std::string>& fields);

// Linear search; returns fields of first matching row or empty vector.
std::vector<std::string> findRow(const std::string& filename,
                                 const std::string& targetValue,
                                 int colIndex);

// Returns true if any row has targetValue at colIndex.
bool rowExists(const std::string& filename,
               const std::string& targetValue,
               int colIndex);

// -----------------------------------------------------------------------
// Safe input helpers — handle EOF (Ctrl+Z on Windows) and bad input
// gracefully so the program NEVER crashes on unexpected input.
// -----------------------------------------------------------------------
int         safeReadInt(int fallback = -1);
double      safeReadDouble(double fallback = -999.0);
std::string safeReadLine();

#endif
