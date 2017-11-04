//
// Created by Peeta on 11/4/17.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

/*typedef struct recordType {
    long ssNumber;
    string lastName;
    string firstName;
    long income;
} recordType;*/

/*========================= Comparator Functions =========================*/
// all of the functions are in ascending order
bool ssnumber_comparator(vector<string> record1, vector<string> record2) {
    long ssNumber1 = stol(record1[0]);
    long ssNumber2 = stol(record2[0]);
    return ssNumber1 < ssNumber2;
}

bool lastname_comparator(vector<string> record1, vector<string> record2) {
    string lastname1 = record1[1];
    string lastname2 = record2[1];
    return lastname1.compare(lastname2) < 0;
}

bool firstname_comparator(vector<string> record1, vector<string> record2) {
    string firstname1 = record1[2];
    string firstname2 = record2[2];
    return firstname1.compare(firstname2) < 0;
}

bool income_comparator(vector<string> record1, vector<string> record2) {
    long income1 = stol(record1[3]);
    long income2 = stol(record2[3]);
    return income1 < income2;
}
/*========================================================================*/

int main(int argc, char *argv[]) {
    cout << argc << endl;
    for (int i = 1; i < argc; i++) {
        cout << argv[i] << endl;
    }

    string input_file, line;
    int k, a = 0, count = 0;
    bool ascending;
    vector<string> record;
    vector<vector<string>> records;

    for (int i = 1; i < argc; i += 2) {
        if (strcmp(argv[i], "-f") == 0) {        // file to be sorted
            input_file = argv[i + 1];
        } else if (strcmp(argv[i], "-k") == 0) { // range of sorting
            k = stoi(argv[i + 1]);
        } else if (strcmp(argv[i], "-a") == 0) { // attribute number
            a = stoi(argv[i+1]);
        } else if (strcmp(argv[i], "-o") == 0) { // sorting order
            if (strcmp(argv[i + 1], "a") == 0) {
                ascending = true;
            } else if (strcmp(argv[i + 1], "d") == 0) {
                ascending = false;
            } else {
                cerr << "Please enter a valid argument.\n" << endl;
            }
        }
    }

    // open the file
    ifstream inputFile;
    inputFile.open(input_file);
    if (inputFile.is_open()) {
        cout << "Sorter executable: opened input file" << endl;
    } else {
        cout << "Sorter executable: failed to open input file" << endl;
    }
    // read the file line by line
    while (getline(inputFile, line) && count < k) {
        istringstream iss(line);
        copy(istream_iterator<string>(iss),
             istream_iterator<string>(),
             back_inserter(record));
        records.push_back(record);
        count++;
    }
    // close the file
    inputFile.close();

    // sorting
    if (a == 0) {
        sort(records.begin(), records.end(), ssnumber_comparator);
    } else if (a == 1) {
        sort(records.begin(), records.end(), lastname_comparator);
    } else if (a == 2) {
        sort(records.begin(), records.end(), firstname_comparator);
    } else if (a == 3) {
        sort(records.begin(), records.end(), income_comparator);
    }

    // call reverse later depending on sorting order
    if (!ascending) {
        reverse(records.begin(), records.end());
    }

    ofstream outputFile;
    outputFile.open("output/")


    return 0;
}