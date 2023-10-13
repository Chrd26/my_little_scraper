#include <iostream>
#include <vector>
#include <sstream>
// Include fstream to use it.
#include <fstream>

// Handle CSV
// Source for more: https://www.geeksforgeeks.org/csv-file-management-using-c/
class CSV_Handler{
public:
    std::vector<std::string> ReadFile();
};