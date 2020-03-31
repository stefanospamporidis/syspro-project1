#ifndef MainUtils_hpp
#define MainUtils_hpp

#define INFTY std::numeric_limits<int>::max()

using namespace  std ;

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <iostream>
#include "Database.hpp"
#include "Structs.hpp"
#include <fstream>
#include <sstream>
#include <limits>
#include <utility>

struct Parameters
{
	string patientRecordsFile;
	int diseaseHashtableNumOfEntries;
	int countryHashtableNumOfEntries;
	int bucketSize;

};


class MainUtils {
private:
    Parameters params ;
    CasesList *caselist;
	Hashtable *countryHashtable, *diseaseHashtable ;
public:
    MainUtils(int argc,char **argv);
    int isNumber(char* number);
    inline bool file_exists (string& name);
    void getArgs(int argc,char **argv);
    void printArgs();
    void getCases();
    void readCommands();
    void parseCommands(char *buffer);
    ~MainUtils(){
    	delete caselist;
    	delete countryHashtable;
    	delete diseaseHashtable;
    }
protected:

};


#endif /* MainUtils_hpp */