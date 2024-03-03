#include <iostream>
#include <vector>
#include <fstream>
#include <regex>
#include <sstream>
#include <boost/locale.hpp>
#include <boost/algorithm/string/case_conv.hpp>
// Use ofstream is used to output to files and ifstream is used to read from files.
// source: https://www.tutorialspoint.com/cplusplus/cpp_files_streams.htm
struct CSV_Handler{
    std::vector<std::string> csvLines;
    void ReadSettings();
    void WriteSavedSearchOptions(std::string &keyword, std::string &url);
    static void ClearPreviousOptions();
};