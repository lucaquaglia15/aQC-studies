# aQC studies

## Content

### Files
1. **runList.txt** contains the path of the QC objects that need to be analyzed. The path to put is the one found in the mail [QA] Daily report from MonALISA

### Macros
1. **QC_readFiles.C** reads the QC file downloaded by the `download-QC-from-grid.py` script and saves a simple root file with the histograms of local board occupancy and 44/all tracks efficiency for a selected run. It saves also the png of these histograms in the images folder. It needs in input the period, the apass, and the run number, passed as strings, e.g. `QC_readfiles.C("LHC23zd", "apass4", "538932")`. It can be executed by the script `runReady.py`.

2. **QC_Comparison.C** reads the root files produced by QC_readFile.C macro and obtain the ratios of histograms from apass4 and apass1. It saves the png of these ratios in the folder ratio-images. It needs in input the period and the run number, passes as strings, e.g. `QC_comparison.C("LHC23zd", "538932")`. It can be executed by the script `runComparison.py`.

3. **Data_MC_comparison_run.C** reads the ROOT files produced by 'download-from-hyperloop-data' and 'download-from-hyperloop-MC', and generates a PDF (saved in the run folder) containing the distributions of #eta, #phi, and pT for both Data and MC. This includes distributions with basic event selections and with basic event selections + pT selections. Additionally, ratios between MC and Data are also produced. It needs in input the MC_production name anchored to dataset, the period and the pass of data, and the run number, passes as strings (except the run number that is an integer), e.g. `Data_MC_comparison_run.C("LHC24g3", "PbPb2023", "apass4", 544122)`. It can be executed by the script `script_macro_data_MC_comparison_runlist.py` for a set of runs.

4. **Data_MC_comparison_fullperiod.C** reads the ROOT files for the entire period under investigation (that has to be downloaded manually from HY) and generates a PDF containing the distributions of #eta, #phi, and pT for both Data and MC. This includes distributions with basic event selections and with basic event selections + pT selections. Additionally, ratios between MC and Data are also produced. It needs in input the MC_production name anchored to dataset, the period and the pass of data, passes as strings, e.g. `Data_MC_comparison_fullperiod.C("LHC24g3", "PbPb2023", "apass4")`. 

### Scripts
1. **download-QC-from-grid.py** downloads the QC_fullrun.root files from the grid whose path is listed in *runList.txt* file. Run it using *`python3 download-QC-from-grid.py`.
2. **runRead.py** is a script that runs `QC_readFiles.C` macro for all the runs listed in `runList.txt` file. Run it using `python3 runRead.py`.
3. **runComparison.py** is a script that runs `QC_comparions.C` for all the runs listed in `runList.txt` file. Run it using `python3 runComparison.py`.
4. **writeAnalyzedRuns.py** read a txt file with the runs that have been analyzed: to create this file copy the runs fro the *QA - Daily report mail*. Periods need to be separated by a blank line. The file needs to start with a black line. The output file `out.txt` contains the list of analyzed runs in each period.
5. **download-data-mc.py** is a script that reads a txt file containing a list of runs (eg `runs.txt`). All the runs needs to be processed by the same pass for data, and to belong to the same production for MC. The name of the file, the pass and the MC production needs to be passed as input, as explained by running `python3 download-data-mc.py --help`.
5. **download-from-hyperloop.py** is a script provided by Lush Miscelletti to download the data run-by-run after a hyperloop hyperTrain has run over the data. The file also need to have the `config.yml` file where you insert the list of runs that you want to download, together with the path you fetch from hyperloop. Moreover, you need to modify the `output_dir_name` to the path where you want to save the data and that's it. To launch the script (inside ALICE-O2 environment) execute: `python download-from-hyperloop.py config.yml --download`
6. **download-from-hyperloop-data.py** and **download-from-hyperloop-MC.py** are similar to **download-from-hyperloop.py** with the exception that they run over Data and MC separately (for this reason there are a 'config_data.yml' and a 'confid_MC.yml' file) in order to save ROOT files with a different name and to produce two different runs_list (respectively 'run_list_data.txt' and 'run_list_MC.txt'). Remember to modify the `output_dir_name` to the path where you want to save the Data (and MC) outputs, together with the run_list and the path you fetch from hyperloop in the 'config_data.yml' ('config_MC.yml'). To launch the script (inside ALICE-O2 environment) execute: `python download-from-hyperloop-data.py config_data.yml --download` and/or `python download-from-hyperloop-MC.py config_MC.yml --download`.
7. **common_run_list.py** is a script that reads the 'run_list_data.txt' and 'run_list_MC.txt' and create a 'common_run_list.txt'. This is useful to perform the comparison between Data and MC only for common runs.
8. **script_macro_data_MC_comparison_runlist.py** is a script to run the macro 'Data_MC_comparison_run.C' for all the runs in 'common_run_list.txt'. Remember to modify the 'macro_dir' line to point to the folder that contains the 'Data_MC_comparison_run.C' macro.

### Workflow
The scripts to make the comparison between passes **need** to be run in the following order because each of them takes as input the output of the previous:
1. download-QC-from-grid.py
2. runRead.py
3. runComparison.py

### Workflow for data/MC comparison
1. download the data using the script `download-data-mc.py`.

### Workflow for data/MC comparison at the A02D level
1. modify the 'config_data.yml' and 'config_MC.yml' with the desired `output_dir_name`, 'run_list' and 'alien_input_path'.
2. run the 'download-from-hyperloop-data.py' and 'download-from-hyperloop-MC.py' scripts with `python download-from-hyperloop-data.py config_data.yml --download` and/or `python download-from-hyperloop-MC.py config_MC.yml --download'.
3. create a common_run list running the 'common_run_list.py' script with 'python common_run_list.py'
4. run the 'script_macro_data_MC_comparison_runlist.py' script to compare the distributions of #eta, #phi, and pT between Data and MC 'python script_macro_data_MC_comparison_runlist.py'
5. if you would like to compare Data and MC over the full period, download the merged outputs from HY and run the 'Data_MC_comparison_fullperiod.C' macro.
