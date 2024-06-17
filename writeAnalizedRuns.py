import os

inputFile = open('list.txt', 'r')
path = inputFile.readlines()

#Run: 528336, job id: 3090187862, path: /alice/data/2022/LHC22o/528336/apass7_skimmed/2230/QC

spl_comma = ','
spl_sc = ':'
spl_slash = '/'

fileOut = "out.txt"
f = open(fileOut,"w")

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
                f.write("Analyzed runs:")

            runW = str(run) + ","
            f.write(runW)
    

