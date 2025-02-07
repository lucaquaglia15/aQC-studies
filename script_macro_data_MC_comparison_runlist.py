#------------ Script for run the macro "Data_MC_comparison_run.C" for all the runs in the common_run_list.txt
import subprocess
import os

# Function that execute the ROOT macro
def execution_macro_comparison(run):
    
    macro_dir = "/home/sarapc/Desktop/MID_QC/ComparisonDataMC_A02D"  # path of the folder that contains Data_MC_comparison_run.C
    comando = ["root", "-l", "-q", f'{macro_dir}/Data_MC_comparison_run.C+("LHC24g3","PbPb2023","apass4",{run})']

    result = subprocess.run(comando, capture_output=True, text=True)

    print("STDOUT:", result.stdout)
    print("STDERR:", result.stderr)

# Read the run list from common_run_list.txt
percorso_file = os.path.join(os.getcwd(), '/home/sarapc/Desktop/MID_QC/ComparisonDataMC_A02D/PbPb2023_apass4/common_run_list.txt') #directory of the period

with open(percorso_file, 'r') as file:
    for line in file:
        run = line.strip()  # remove any extra spaces or characters
        if run: 
            print(f"Run the comparison for the run: {run}")
            execution_macro_comparison(run)

