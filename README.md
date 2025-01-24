# aQC studies

## Content

### Files
1. **runList.txt** contains the path of the QC objects that need to be analyzed. The path to put is the one found in the mail [QA] Daily report from MonALISA

### Macros
1. **QC_readFiles.C** reads the QC file downloaded by the `download-QC-from-grid.py` script and saves a simple root file with the histograms of local board occupancy and 44/all tracks efficiency for a selected run. It saves also the png of these histograms in the images folder. It needs in input the period, the apass, and the run number, passed as strings, e.g. `QC_readfiles.C("LHC23zd", "apass4", "538932")`. It can be executed by the script `runReady.py`.

2. **QC_Comparison.C** reads the root files produced by QC_readFile.C macro and obtain the ratios of histograms from apass4 and apass1. It saves the png of these ratios in the folder ratio-images. It needs in input the period and the run number, passes as strings, e.g. `QC_comparison.C("LHC23zd", "538932")`. It can be executed by the script `runComparison.py`.

### Scripts
1. **download-QC-from-grid.py** downloads the QC_fullrun.root files from the grid whose path is listed in *runList.txt* file. Run it using *`python3 download-QC-from-grid.py`.
2. **runRead.py** is a script that runs `QC_readFiles.C` macro for all the runs listed in `runList.txt` file. Run it using `python3 runRead.py`.
3. **runComparison.py** is a script that runs `QC_comparions.C` for all the runs listed in `runList.txt` file. Run it using `python3 runComparison.py`.
4. **writeAnalyzedRuns.py** read a txt file with the runs that have been analyzed: to create this file copy the runs fro the *QA - Daily report mail*. Periods need to be separated by a blank line. The file needs to start with a black line. The output file `out.txt` contains the list of analyzed runs in each period.
5. **download-data-mc.py** is a script that reads a txt file containing a list of runs (eg `runs.txt`). All the runs needs to be processed by the same pass for data, and to belong to the same production for MC. The name of the file, the pass and the MC production needs to be passed as input, as explained by running `python3 download-data-mc.py --help`.

### Workflow
The scripts to make the comparison between passes **need** to be run in the following order because each of them takes as input the output of the previos:
1. download-QC-from-grid.py
2. runRead.py
3. runComparison.py

### Workflow for data/MC comparison
1. download the data using the script `download-data-mc.py`.
