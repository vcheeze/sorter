# sorter
Operating Systems Project 2

## How to run
Type `./mysortapp -k NumOfWorkers -e NameOfSortExecutable -a AttributeNumber -t
Type -o Order -s OutFile`, where `mysortapp` is the name of your executable
program, `NumOfWorkers` is the number of *sorters* to be spawned,
`NameOfSortExecutable` is the name of the program to be used by the *sorter*
nodes, `AttributeNumber` is a valid numeric ID that designates the field on
which sorting is to be carried out, `Type` is the type of the field (numeric or
alphanumeric), `Order` is either ascending (`a`) or descending (`d`), and
`OutFile` is the file in which the outcome should be saved. There is no
predetermined order with which the above flags can be entered in the command
line.
