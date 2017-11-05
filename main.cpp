/**
 * This is the root node, the anchor of the entire hierarchy.
 * It orchestrates the collective sorting task by creating a coordinator node.
 * It then passes 4 arguments to the coord node:
 *     the file to be sorted
 *     k: number of sorter nodes
 *     attribute on which sorting will be carried out
 *     the executable to be used for sorting
 *
 * pipe --> fork --> dup --> exec
 * close the read for children (a[0])
 * close the write for parent once you spawn merger, because merger has to read
 * gets() with file stream
 *
 * or use named pipes
 */

#include <iostream>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;


int main(int argc, char *argv[]) {
//     cout << argc << endl;
//     cout << argv[1] << endl;

    string input_file, executable, output_file, type, order;
    int l, k, a, range;

    // Getting all the arguments from the command line
    for (int i = 1; i < argc; i += 2) {
        if (strcmp(argv[i], "-f") == 0) {        // file to be sorted
            input_file = argv[i + 1];
        } else if (strcmp(argv[i], "-l") == 0) { // number of lines in file to be sorted
            l = stoi(argv[i+1]);
        } else if (strcmp(argv[i], "-k") == 0) { // number of sorter nodes to be created
            k = stoi(argv[i + 1]);
        } else if (strcmp(argv[i], "-e") == 0) { // name of sort executable
            executable = argv[i + 1];
        } else if (strcmp(argv[i], "-a") == 0) { // attribute number
            a = stoi(argv[i + 1]);
        } else if (strcmp(argv[i], "-t") == 0) { // type of field to be sorted
            type = argv[i + 1];
        } else if (strcmp(argv[i], "-o") == 0) { // order: a for ascending and d for descending
            order = argv[i + 1];
        } else if (strcmp(argv[i], "-s") == 0) { // output file
            output_file = argv[i + 1];
        }
    }

    // calculate the range for each sorter node
    // since I take the ceiling of this division, it means I have to check for eof in the sorters
    range = l/k + (l % k != 0); // length of the file / number of sorters to be created

    // create the coordinator node
    pid_t pid = fork();

    if (pid == 0) { // child - coordinator node
        /*==================== fork k Sorter Nodes ====================*/
        pid_t pids[k];
        int n = k;
        for (int i = 0; i < n; i++) {
            if ((pids[i] = fork()) < 0) {
                cerr << "Failed to fork Sorter Node in Coordinator" << endl;
                abort();
            } else if (pids[i] == 0) { // sorter nodes
                // cout << "In child " << to_string(i) << endl;
                string out_file = "output/sorted" + to_string(i) + ".txt";
                ofstream outFile(out_file);
                string line;

                // open the input file
                ifstream inputFile;
                inputFile.open(input_file);
                if (inputFile.is_open()) {
                    cout << "Opened input file" << endl;
                } else {
                    cout << "Failed to open input file" << endl;
                }

                int count = 0;
                while (getline(inputFile, line)) {
                    if (count >= i*range && count < (i+1)*range) {
                        // cout << count << ": " << line << endl;
                        outFile << line << endl;
                    }
                    count++;
                }
                // close files
                outFile.close();
                inputFile.close();

                // exec call to sort
                char *args[] = {const_cast<char *>("./sorterexec"),
                                const_cast<char *>("-f"), const_cast<char *>(out_file.c_str()),
                                const_cast<char *>("-k"), const_cast<char *>(to_string(range).c_str()),
                                const_cast<char *>("-a"), const_cast<char *>(to_string(a).c_str()),
                                const_cast<char *>("-o"), const_cast<char *>(order.c_str()),
                                NULL};
                execvp(args[0], args);

                cout << "This shouldn't get printed if exec call was successful" << endl;
                _exit(1);
            }
        }
        /*=============================================================*/

        int status;
        pid_t cpid;
        while (n > 0) {
            cpid = wait(&status);
            cout << "Child with PID " << cpid << " exited with status " << status << endl;
            n--;
        }

        /*======================== Merger Node ========================*/
        pid_t merger_pid;
        if ((merger_pid = fork()) < 0) {
            cerr << "Failed to fork Merger Node in Coordinator" << endl;
            abort();
        } else if (merger_pid == 0) { // child - merger node
            int i;
            n = k;
            ifstream inFiles[k];
            int index[10] = {0}; // the indices pointing to each file

            // open the files to be read
            for (i = 0; i < n; i++) {
                string file_path = "output/sorted" + to_string(i) + ".txt";
                inFiles[i].open(file_path);
                if (inFiles[i].is_open()) {
                    cout << "Opened " << file_path << " in Merger Node" << endl;
                } else {
                    cout << "Failed to open file in Merger Node" << endl;
                }
            }

            // open final output file
            ofstream outputFile;
            outputFile.open(output_file);
            if (outputFile.is_open()) {
                cout << "Opened final output file" << endl;
            } else {
                cout << "Failed to open final output file" << endl;
            }

            vector< vector<string> > records;
            vector<string> values;
            // get first value from every file
            for (i = 0; i < n; i++) {
                string line;
                getline(inFiles[i], line);
                istringstream iss(line);
                vector<string> record;
                copy(istream_iterator<string>(iss),
                     istream_iterator<string>(),
                     back_inserter(record));
                records.push_back(record);
                values.push_back(record[a]);
            }

            while (true) {
                int done = 0;
                if (done == n) {
                    break;
                }

                // get the minimum element
                string min = *min_element(values.begin(), values.end());
                // get the index of the minimum element
                int min_index = static_cast<int>(min_element(values.begin(), values.end()) - values.begin());

                // write to the final output file
                outputFile << records[min_index][0] << " "
                           << records[min_index][1] << " "
                           << records[min_index][2] << " "
                           << records[min_index][3] << endl;

                index[min_index]++; // increment the index to point to the next smallest element
                if (index[min_index] == range) {
                    done++; // increment variable 'done' by 1 when a file reaches end
                }

                // get the next line from the file
                string line;
                getline(inFiles[min_index], line);
                istringstream iss(line);
                vector<string> record;
                copy(istream_iterator<string>(iss),
                     istream_iterator<string>(),
                     back_inserter(record));
                records[min_index] = record;
                values[min_index] = record[a];
//                records.insert(records.begin() + min_index, record);
//                values.insert(values.begin() + min_index, record[a]);

                // break;
            }

            // close all files
            for (i = 0; i < n; i++) {
                inFiles[i].close();
            }
            outputFile.close();
            /*=============================================================*/
        }
    } else if (pid < 0) { // coordinator fork failed
        cerr << "Failed to fork" << endl;
        exit(1);
    } else { // parent - root node
        // cout << executable << endl;
        // cout << a << k << endl;
    }

    return 0;
}
