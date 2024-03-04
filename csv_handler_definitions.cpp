//
// Created by ChrisD on 13/10/23.
//
#include "csv_handler.h"
// Read more

bool CSV_Handler::hasStarted = false;

void CSV_Handler::ReadSettings()
{
    std::ifstream settingsCSV;
    settingsCSV.open("../settings/settings.csv");

    if(!settingsCSV.is_open())
    {
        std::cout << std::strerror(errno) << std::endl;
        exit(errno);
    }

    std::string column;

    while(getline(settingsCSV ,column, '\n'))
    {
        csvLines.push_back(column);
    }

    settingsCSV.close();
}

// Start adding data to the CSV file
void CSV_Handler::WriteSavedSearchOptions(std::string &keyword, std::string &url)
{
    std::ofstream csvfile;

    csvfile.open("../settings/settings.csv", std::ios::app);
//    std::setlocale(LC_ALL, "el_GR.UTF-8");

//    To lower case
//    Source: https://stackoverflow.com/questions/313970/how-to-convert-an-instance-of-stdstring-to-lower-case
//    std::transform(keyword.begin(), keyword.end(), keyword.begin(),
//                   [](char c){return std::tolower(c, std::locale("el_GR"));});
//  Iterating through each character ends up with ? symbols instead of
//  printing the correct character. This is why the iterator in std::transform
//  doesn't work
    std::string keywordLowercase = boost::locale::to_lower(keyword);

    if (!csvfile.is_open())
    {
        std::cout << "Failed to open settings file." << std::endl;
        std::cout << std::strerror(errno) << std::endl;
        exit(errno);
    }

    std::string csvData = url;
    std::string separator = ",";

    csvData.append(separator);
    csvData.append(keywordLowercase);
    csvfile << csvData << std::endl;
    csvfile.close();
}

// Clearing a file quite easy, just open it without setting
// an input or output mode and close it
// Source: https://stackoverflow.com/questions/25201131/writing-csv-files-from-c
void CSV_Handler::ClearPreviousOptions()
{
    std::ofstream csvFileToClear;
    csvFileToClear.open("../settings/settings.csv");

    if (!csvFileToClear.is_open())
    {
        std::cout << "Failed to save settings." << std::endl;
        std::cout << std::strerror(errno) << std::endl;
        exit(errno);
    }

    csvFileToClear.close();
}