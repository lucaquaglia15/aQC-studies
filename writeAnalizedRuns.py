#for executing shell commands
import os
#for command line input
import getopt, sys

# Remove 1st argument from the
# list of command line arguments
argumentList = sys.argv[1:]

#input options
options = "h:i:o"
#input long options
long_options = ["help","in=","out="]

try:
    #default values
    inputFileName = 'list.txt'
    outputFileName = 'out.txt'
    help_requested = False
    allGood = False

    # Parsing argument
    arguments, values = getopt.getopt(argumentList, options, long_options)
    # checking each argument
    for currentArgument, currentValue in arguments:
        if currentArgument in ("-h","--help"):
            print("\nOptions:")
            print("[-h, --help]:   display this message")
            print("[-i --in]:      input file")
            print("[-o --out]:     output file")
            help_requested = True

        elif currentArgument in ("-i","--in"):
            inputFileName = str(currentValue)

        elif currentArgument in ("-o","--out"):
            outputFileName = str(currentValue)


    if help_requested == False:

        inputFile = open(inputFileName, 'r')
        path = inputFile.readlines()

        #Run: 528336, job id: 3090187862, path: /alice/data/2022/LHC22o/528336/apass7_skimmed/2230/QC

        spl_comma = ','
        spl_sc = ':'
        spl_slash = '/'

        f = open(outputFileName,"w")

        i = 0
        prev_empty = False
        for line in path:
            if line.isspace():
                prev_empty = True
                i = 0

            else:
                if (prev_empty):
                    f.write('\n')
                    line = line.rstrip('\n')
                    f.write(line)

                    prev_empty = False

                else:
                    i = i + 1

                    line = line.rstrip('\n')
                    splComma = line.split(spl_comma)
                    splSC = splComma[0].split(spl_sc)
                    run = splSC[1]
                    splPath = splComma[2].split(spl_slash)
                    apass = splPath[6]

                    if i==1:
                        f.write(' ' + str(apass) + '\n')
                        f.write("Runs:")

                    runW = str(run) + ","
                    f.write(runW)


except getopt.error as err:
    print("error: " + str(err))
    print("use [-h, --help] to see available options")