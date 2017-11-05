/**
 * This is the root node, the anchor of the entire hierarchy.
 * It orchestrates the collective sorting task by creating a coordinator node.
 * It then passes 4 arguments to the coord node:
 *     the file to be sorted
 *     k: number of sorter nodes
 *     attribute on which sorting will be carried out
 *     the executable to be used for sorting
 */

#include <iostream>
#include <unistd.h>
#include <fstream>

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
        /*=============== fork k sorter nodes ===============*/
        pid_t pids[k];
        int n = k;
        for (int i = 0; i < 2; i++) {
            if ((pids[i] = fork()) < 0) {
                cerr << "Fork failed in Coordinator" << endl;
                abort();
            } else if (pids[i] == 0) { // sorter nodes
                // do sorting here
                cout << "In child " << to_string(i) << endl;
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
            else { // in the parent - coordinator node
              int status;
              pid_t cpid;
              while (n > 0) {
                  cpid = wait(&status);
                  cout << "Child with PID " << cpid << " exited with status " << status << endl;
                  n--;
                  break;
              }
            }
        }
        /*===================================================*/

        // int status;
        // pid_t cpid;
        // while (n > 0) {
        //     cpid = wait(&status);
        //     cout << "Child with PID " << cpid << " exited with status " << status << endl;
        //     n--;
        //     break;
        // }
    } else if (pid < 0) { // coordinator fork failed
        cerr << "Failed to fork" << endl;
        exit(1);
    } else { // parent - root node
        cout << executable << endl;
        cout << a << k << endl;
    }

    return 0;
}
