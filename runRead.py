import os

#read the path of the QC objects in the grid
#put in the file only the string that apper in the [QA] Daily report from MonAlisa
inputFile = open('runList.txt', 'r')
path = inputFile.readlines()

spl_word = '/'
for line in path:
    line = line.rstrip('\n')

    spl = line.split(spl_word)

    period = spl[4]
    apass = spl[6]
    run = spl[5]

    arg = '\"'+ period + '\", \"' + apass + '\", \"' + run + '\"'
    arg2 = period + ',' + apass + ',' + run
    comm = 'root -l -q \'QC_readfiles.C(' + arg + ')\''

    print(comm)

    os.system(comm)