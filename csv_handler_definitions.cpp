//
// Created by ChrisD on 13/10/23.
//
#include "csv_handler.h"
// Read more

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

    for (const auto &content : csvLines)
    {
        std::cout << "Content: " << content << std::endl;
    }

    settingsCSV.close();
}

// Start adding data to the CSV file
void CSV_Handler::WriteSavedSearchOptions(std::string keyword, std::string& url) {
    std::ofstream csvfile;
    csvfile.open("../settings/settings.csv",
                 std::ios::app);

    if (!csvfile.is_open())
    {
        std::cout << "Failed to open settings file." << std::endl;
        std::cout << std::strerror(errno) << std::endl;
        exit(errno);
    }

    std::string csvData = url;
    csvData.append(",");
    csvData.append(keyword);
    csvfile << csvData << std::endl;

    csvfile.close();
}

// Clearing a file quite easy, just open it without setting
// an input or output mode and clsoe it
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