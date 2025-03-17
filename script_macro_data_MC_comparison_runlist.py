#------------ Script for run the macro "Data_MC_comparison_run.C" for all the runs in the common_run_list.txt
import subprocess
import os

# Function that execute the ROOT macro
def execution_macro_comparison(run):
    
    macro_dir = "/home/sarapc/Desktop/MID_QC/ComparisonDataMC_A02D"  # path of the folder that contains Data_MC_comparison_run.C
    comando = ["root", "-l", "-q", f'{macro_dir}/Data_MC_comparison_run.C+("LHC23k4g","pp2023","apass4",{run})']

    result = subprocess.run(comando, capture_output=True, text=True)

    print("STDOUT:", result.stdout)
    print("STDERR:", result.stderr)

# Read the run list from common_run_list.txt
percorso_file = os.path.join(os.getcwd(), '/home/sarapc/Desktop/MID_QC/ComparisonDataMC_A02D/pp136TeV2023_apass4/common_run_list.txt') #directory of the period

baseFodlder = "/home/sarapc/Desktop/MID_QC/ComparisonDataMC_A02D/pp136TeV2023_apass4"
try:
    os.remove(baseFodlder+"goodRuns.txt")
except OSError:
    pass

try:
    os.remove(baseFodlder+"goodRuns_2sigmas.txt")
except OSError:
    pass

try:
    os.remove(baseFodlder+"goodRuns_chi2.txt")
except OSError:
    pass
    
try:
    os.remove(baseFodlder+"goodRuns_thresholds_sigmas.txt")
except OSError:
    pass

try:
    os.remove(baseFodlder+"badRuns.txt")
except OSError:
    pass

try:
    os.remove(baseFodlder+"badRuns_2sigmas.txt")
except OSError:
    pass

try:
    os.remove(baseFodlder+"badRuns_chi2.txt")
except OSError:
    pass
    
try:
    os.remove(baseFodlder+"badRuns_thresholds_sigmas.txt")
except OSError:
    pass
with open(percorso_file, 'r') as file:
    for line in file:
        run = line.strip()  # remove any extra spaces or characters
        if run: 
            print(f"Run the comparison for the run: {run}")
            execution_macro_comparison(run)

