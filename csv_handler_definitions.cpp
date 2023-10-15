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
        // std::cout << "Value: " << column << std::endl;
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
