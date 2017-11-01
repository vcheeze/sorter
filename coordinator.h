//
// Created by Peeta on 10/30/17.
//

#ifndef COORDINATOR_H
#define COORDINATOR_H

#include <fstream>

using namespace std;

class Coordinator {
    ifstream inputFile;
    int number_of_sorters;

public:
    Coordinator();
    ~Coordinator();

    ifstream getInputFile();
    void setInputFile(ifstream f);
};


#endif // COORDINATOR_H
