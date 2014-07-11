//
//  mpsReader.cpp
//  cppipm
//
//  Created by Yiming Yan on 11/07/2014.
//  Copyright (c) 2014 Yiming. All rights reserved.
//

#include "mpsReader.h"
#include <fstream>

mpsReader::mpsReader(std::string fileName)
{
    std::ifstream readFile(fileName);

    if (readFile.is_open())
    {
        // get rid of comments or blank line
        _findPos2Start(readFile);
        
        while (!readFile.eof())
        {
            // check field name
            int field = _checkFieldName(readFile);
            if (field == 1)
                readFile >> Name;
            else if (field == 2) {}
            else if (field == 3) {}
            else if (field == 4) {}
            else if (field == 5) {}
            else if (field == 6) {}
            else if (field == 7) {}
            else if (field == 8) {}
            else if (field == 9) {}
            else if (field == 10)
            {
                std::cout<< "End of file." <<std::endl;
                readFile.close();
                break;
            }
            else
            {
                std::string line;
                getline(readFile, line);
                //std::cout<<line<<std::endl;
            }
            
        }
    }
    else
    {
        std::cout<<"Error: MPSREADER - File not found"<<std::endl;
    }
    
    readFile.close();
}

int mpsReader::_checkFieldName(std::ifstream &readFile)
{
    std::streampos pos = readFile.tellg();     // current position
    std::string checkWord;  // first word
    
    readFile >> checkWord;
    
    if (checkWord.compare("NAME") == 0)
        return 1;
    else if (checkWord.compare("ROWS") == 0)
        return 2;
    else if (checkWord.compare("COLUMNS") == 0)
        return 3;
    else if (checkWord.compare("RHS") == 0)
        return 4;
    else if (checkWord.compare("BOUNDS") == 0)
        return 5;
    else if (checkWord.compare("QUADOBJ") == 0)
        return 6;
    else if (checkWord.compare("OBJSENSE") == 0)
        return 7;
    else if (checkWord.compare("SOS") == 0)
        return 8;
    else if (checkWord.compare("RANGES") == 0)
        return 9;
    else if (checkWord.compare("ENDATA") == 0)
        return 10;
    else
        readFile.seekg(pos, std::ios::beg);   // go back one line
        return -1;
}

void mpsReader::_findPos2Start(std::ifstream &readFile)
{
    std::streampos pos = readFile.tellg();     // current position
    std::string line;
    std::string firdWord;
    while (true)
    {
        getline(readFile, line);
        if ( line.empty() )
            continue;
        else if ( line.find("*") == 0)
            continue;
        else
        {
            readFile.seekg(pos, std::ios::beg);   // go back one line
            break;
        }
    }
}