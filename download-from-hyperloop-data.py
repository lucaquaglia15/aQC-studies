from telnetlib import Telnet
import matplotlib.pyplot as plt
import array as arr
import numpy as np
import os
import sys
import argparse
import yaml
import ROOT
from os import path

def download(inputCfg):    
    if os.path.isfile(("{}/run_list_data.txt").format(inputCfg["input"]["output_dir_name"])):
        os.system(("rm {}/run_list_data.txt").format(inputCfg["input"]["output_dir_name"]))

    fOut = open(("{}/run_list_data.txt").format(inputCfg["input"]["output_dir_name"]), 'x')

    output_dir = inputCfg["input"]["output_dir_name"]
    if not os.path.isdir("%s" % (output_dir)):
        os.system("mkdir -p %s" % (output_dir))
    
    #Sorting of the run list
    #sorted_runs = sorted(inputCfg["input"]["run_list_data"])
    #print("Sorted runs:", sorted_runs)
    
    print("----- Download and save files in %s -----" % (inputCfg["input"]["output_dir_name"]))
    for iRun in range(0, len(inputCfg["input"]["run_list_data"])):

        file_type = inputCfg["input"]["file_type"]
        run = inputCfg["input"]["run_list_data"][iRun]
        alien_path = inputCfg["input"]["alien_input_path"][iRun]

        os.system("mkdir -p %s/%s" % (output_dir, run))
        
        os.system("alien_cp alien://%s/AnalysisResults.root file:%s/%s/AnalysisResultsData_run_%s.root" % (alien_path, output_dir, run, run))
       
        fOut.write("{}\n".format(run))
    
    fOut.close()

def merge(inputCfg):
    fInPath = inputCfg["input"]["file_path"]
    file_type = inputCfg["input"]["file_type"]
    os.system("mkdir -p {}/merged_files".format(fInPath))
    runs = inputCfg["input"]["run_list_data"]
    for run in runs:
        #print("mkdir -p {}/merged_files/{}".format(fInPath, run))
        os.system(f'mkdir -p {fInPath}/merged_files/{run}')
        #print("hadd {}/merged_files/{}/AnalysisResults.root {}/{}/*/AnalysisResults.root".format(fInPath, run, fInPath, run))
        os.system(f'hadd {fInPath}/merged_files/{run}/{file_type} {fInPath}/{run}/*/{file_type}')

### ### ###
def main():
    parser = argparse.ArgumentParser(description='Arguments to pass')
    parser.add_argument('cfgFileName', metavar='text', default='config_data.yml', help='config file name')
    parser.add_argument("--download", help="Download single files", action="store_true")
    parser.add_argument("--merge", help="Do the merging of the downloaded files", action="store_true")
    args = parser.parse_args()

    print('Loading task configuration: ...', end='\r')
    with open(args.cfgFileName, 'r') as ymlCfgFile:
        inputCfg = yaml.load(ymlCfgFile, yaml.FullLoader)
    print('Loading task configuration: Done!')

    if args.download:
        download(inputCfg)
    if args.merge:
        merge(inputCfg)

main()
