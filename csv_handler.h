#include <iostream>
#include <vector>
#include <fstream>
// Use cerrno to handle errors
#include <cerrno>
#include <regex>
// Handle CSV
// Read here for more: https://iq.opengenus.org/read-and-write-in-csv-in-cpp/
// About CSV
// When separated with commas, the first column of the next line will have problems
// There is a big amount of space between the next column and the value which makes regex not possible to work.
//This is how I managed to fix it:
//,url,keyword 1,keyword2,keyword3,keyword 4,keyword5,
//,https://www.in.gr/,ισραήλ,βία,χαμάς,χεζμπολάχ,νεκροί,
//,https://www.ethnos.gr/,ισραήλ,βία,χαμάς,χεζμπολάχ,νεκροί,
//,https://www.cnn.gr/,ισραήλ,βία,χαμάς,χεζμπολάχ,νεκροί,
//,https://www.tanea.gr/,ισραήλ,βία,χαμάς,χεζμπολάχ,νεκροί,
// So, an empty column before and after the content is needed to make it work properly
struct CSV_Handler{
    std::vector<std::string> keywords;
    std::vector<std::string> links;
    void ReadFile();
    void WriteSavedSearchOptions(std::string keyword, std::string& url);
    void ClearPreviousOptions();
};