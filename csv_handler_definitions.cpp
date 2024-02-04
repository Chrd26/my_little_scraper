//
// Created by ChrisD on 13/10/23.
//
#include "csv_handler.h"
// Read more

void CSV_Handler::ReadFile()
{
    std::ifstream file;
    size_t counter = 0;
    file.open("/Users/chrisd/Desktop/my_little_scraper/example-csv/example.csv");
    std::regex reg("^www.*|https?.*");

    if(!file.is_open())
    {
        std::cout << std::strerror(errno) << std::endl;
        exit(1);
    }

    std::vector<std::string> fileURLS;
    std::string column;
    std::string val;
    getline(file, column, ',');

    while(getline(file,column, ','))
    {
        std::cout << std::regex_match(column, reg) << std::endl;

        if (std::regex_match(column,reg))
        {
            links.push_back(column);
        }else{
            keywords.push_back(column);
        }
    }

    file.close();
    std::cout << "CSV file reading is complete" << std::endl;
    std::cin.get();
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
    csvData.append(", ");
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

void CSV_Handler::WriteNumberOfUrls(int num)
{
    std::ofstream urlAmount;
    urlAmount.open("../settings/url-amount.txt");

    if (!urlAmount.is_open())
    {
        std::cout << "Failed to save settings" << std::endl;
        std::cout << std::strerror(errno) << std::endl;
        exit(errno);
    }

    urlAmount << num << std::endl;
    urlAmount.close();
}