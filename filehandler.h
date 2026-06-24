#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <string>
#include <vector>

std::vector<std::string> readTXT(const std::string& filename);

void writeTXT(const std::string& filename, const std::string& header,
              const std::string rows[][10], int rowCount, int colCount);

void appendTXT(const std::string& filename,
               const std::vector<std::string>& fields);

std::vector<std::string> findRow(const std::string& filename,
                                 const std::string& targetValue,
                                 int colIndex);

bool rowExists(const std::string& filename,
               const std::string& targetValue,
               int colIndex);

#endif
