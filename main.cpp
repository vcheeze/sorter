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
// #include <string.h>

using namespace std;


int main(int argc, char *argv[]) {
    pid_t pid = getpid();

    pid = fork();

    if (pid == 0) {
        ;
    } else {
//     cout << argc << endl;
//     cout << argv[1] << endl;
        string input_file, executable, output_file, type;
        int k, a;
        bool ascending;

        // Getting all the arguments from the command line
        for (int i = 1; i < argc; i += 2) {
            if (strcmp(argv[i], "-f") == 0) { // file to be sorted
                input_file = argv[i + 1];
            } else if (strcmp(argv[i], "-k") == 0) { // number of sorter nodes to be created
                k = stoi(argv[i + 1]);
            } else if (strcmp(argv[i], "-e") == 0) { // name of sort executable
                executable = argv[i + 1];
            } else if (strcmp(argv[i], "-a") == 0) { // attribute number
                a = stoi(argv[i + 1]);
            } else if (strcmp(argv[i], "-t") == 0) { // type of field to be sorted
                type = argv[i + 1];
            } else if (strcmp(argv[i], "-o") == 0) { // order: a for ascending and d for descending
                if (strcmp(argv[i + 1], "a") == 0) {
                    ascending = true;
                } else if (strcmp(argv[i + 1], "d") == 0) {
                    ascending = false;
                } else {
                    perror("Please enter a valid argument.\n");
                }
            } else if (strcmp(argv[i], "-s") == 0) { // output file
                output_file = argv[i + 1];
            }
        }

        cout << executable << endl;
        cout << a << k << endl;
    }

    return 0;
}
