//
// Created by ChrisD on 13/10/23.
//
#include "csv_handler.h"

std::vector<std::string> CSV_Handler::ReadFile()
{
    // File pointer
    std::fstream fin;

    // Open file
    fin.open("example-csv/example.csv", std::ios::in);

    // Read row
    std::vector<std::string> row;
    std::string line, word, temp;

    while (fin >> temp)
    {
        // Clear before using
        row.clear();

        // Read an entire row and store it in the line
        // string variable
        std::getline(fin, line);

        // Seperate words
        std::stringstream s(line);

        // Read every column data of a row and store it
        // in a string variable
        while(std::getline(s, word, ", "))
        {
            // add all the column data of a row to the row vector
            row.push_back(word);
        }


    }

}
