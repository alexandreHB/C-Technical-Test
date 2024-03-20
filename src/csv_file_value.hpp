#ifndef MAIN_HPP
#define MAIN_HPP

#include <iostream>
#include <map>
#include <filesystem> 
#include <fstream>
#include <sstream>
#include <string>

#include <locale>


const auto PROJECT_PATH = std::filesystem::current_path().parent_path();    // projetc/build
const auto CSV_FILE = PROJECT_PATH/"values.csv";                            // project/values.csv


//create a map with a char key and their associated value given a csv file
const std::map<wchar_t,int> create_map_from_csv(auto csv_filename)
{
    std::wifstream csv_file(csv_filename, std::wios::binary);                           // Open the CSV file with binary mode
    csv_file.imbue(std::locale(csv_file.getloc(), new std::codecvt_utf8<wchar_t>));     // Set locale to handle UTF-8
    std::locale::global(std::locale("")); // Set global locale to default


    if(std::filesystem::is_empty(csv_filename))         
        throw std::invalid_argument("this csv file is empty !");                        //when file is empty
            
    if (!csv_file.is_open())
        throw std::invalid_argument("this csv file does not exist !");                  //when file is invalid
        

    std::map<wchar_t, int> myMap;

    // building our map by running through the csv file line by line
    std::wstring line;
    while (std::getline(csv_file, line))
    {
        std::wistringstream iss(line);
        wchar_t key;
        int value;
        wchar_t delim;

        if (iss >> key >> std::noskipws >> delim >> value)
            myMap[key] = value;
    }
    
    csv_file.close();
    return myMap;
}

//display map info  
//template in case we want to print out another type of map
template<typename key, typename value> 
void display_map_info(const std::map<key, value>& m)
{
    std::wcout << "***  MAP INFO  ***" << std::endl;
    for (const auto& pair : m) 
        std::wcout << std::showbase << pair.first << " = " << pair.second << " ";
    std::wcout << std::endl;

    std::wcout << "size of the map : " << m.size() << std::endl;
    std::wcout << "***  MAP INFO  ***" << std::endl << std::endl;
}

#endif

