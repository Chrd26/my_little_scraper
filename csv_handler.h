#include <iostream>
#include <vector>
#include <fstream>
#include <regex>

// Use ofstream is used to output to files and ifstream is used to read from files.
// source: https://www.tutorialspoint.com/cplusplus/cpp_files_streams.htm
struct CSV_Handler{
    std::vector<std::string> keywords;
    std::vector<std::string> links;
    void ReadSettings();
    void WriteSavedSearchOptions(std::string keyword, std::string& url);
    void ClearPreviousOptions();
};