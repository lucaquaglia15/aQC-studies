#!/usr/bin/env python3

#for executing shell commands
import os
#for command line input
import getopt, sys

import glob
import shutil
import subprocess

# Remove 1st argument from the
# list of command line arguments
argumentList = sys.argv[1:]

#input options
options = "hp:m:f:"
#input long options
long_options = ["help","data_pass=","mc=","file="]

try:
  #default values
  data_pass = 'cpass0'
  mc = 'LHC24g3'
  file = 'runs.txt'
  help_requested = False

  # Parsing argument
  arguments, values = getopt.getopt(argumentList, options, long_options)
  # checking each argument
  for currentArgument, currentValue in arguments:
    if currentArgument in ("-h","--help"):
      print("python3 download-data-mc.py --data_pass <data_pass> --mc <mc_production> --file <input_file>")
      print("\nOptions:")
      print("[-h, --help]:    display this message")
      print("[-p, --pass]:    data pass - default: " + data_pass)
      print("[-m, --mc]:      MC production - default: " + mc)
      print("[-f, --file]:    input file with the list of runs - default: " + file)
      print('')
      help_requested = True

    elif currentArgument in ("-p","--data_pass"):
      data_pass = str(currentValue)

    elif currentArgument in ("-m","--mc"):
      mc = str(currentValue)

    elif currentArgument in ("-f","--file"):
      file = str(currentValue)

  if not help_requested:
    inputFile = open(file, 'r')

    runs = inputFile.readlines()
    for run_number in runs:
      run_number = run_number.rstrip()

      # download the data
      try:
        data_path_w = 'alien:///alice/data/2023/LHC23zzl/' + run_number + '/' + data_pass + '/*/QC/QC_fullrun.root'
        data_path = subprocess.check_output('alien_find ' + data_path_w, shell=True, text=True).replace('\n','')
        comm_data = 'alien_cp ' + 'alien://' + data_path + ' file:/home/ariffero/Desktop/aQC-studies'
        os.system(comm_data)
      except subprocess.CalledProcessError as e:
        print('Error: change the data path')

      # download the MC
      # there are 2 possible configurations for the MC schema, try both
      try:
        mc_path_w = 'alien:///alice/sim/2024/' + mc + '/' + run_number + '/QC/MIDTaskQC.root'
        subprocess.check_output('alien_ls ' + mc_path_w, shell=True, text=True)
        comm_mc = 'alien_cp ' + 'alien://' + mc_path_w + ' file:/home/ariffero/Desktop/aQC-studies'
        os.system(comm_mc)
      except subprocess.CalledProcessError as e:
        print("trying 2nd option for MC")
        try:
          mc_path_w = 'alien:///alice/sim/2024/' + mc + '/0/' + run_number + '/QC/MIDTaskQC.root'
          subprocess.check_output('alien_ls ' + mc_path_w, shell=True, text=True)
          comm_mc = 'alien_cp ' + 'alien://' + mc_path_w + ' file:/home/ariffero/Desktop/aQC-studies'
          os.system(comm_mc)
        except subprocess.CalledProcessError as e:
          print('Error: change the MC path!')
        
      os.rename('QC_fullrun.root','QC_data_'+run_number+'_'+data_pass+'.root')
      os.rename('MIDTaskQC.root' ,'QC_mc_'  +run_number+'_'+mc+'.root')
except getopt.error as err:
  print("error: " + str(err))
  print("use [-h, --help] to see available options")
