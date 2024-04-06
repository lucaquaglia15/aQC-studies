import os

#read the path of the QC objects in the grid
#put in the file only the string that apper in the [QA] Daily report from MonAlisa
inputFile = open('runList.txt', 'r')
path = inputFile.readlines()

spl_word = '/'
for line in path:
    line = line.rstrip('\n')
    #form the command to download the file 
    #files downloaded in ~/Desktop/testGridDownload/ folder
    comm = 'alien_cp alien://' + line + '/QC_fullrun.root file://~/Desktop/aQC-studies/'
    print(comm)
    #start download
    os.system(comm)

    spl = line.split(spl_word)

    period = spl[4]
    apass = spl[6]
    run = spl[5]
    
    #the dowloaded file is called QC_fullrun.root, remane using run info
    os.rename('QC_fullrun.root', 'QC_fullrun_'+period + '_' + apass + '_' + run  +'.root')
    
    print('--------')

