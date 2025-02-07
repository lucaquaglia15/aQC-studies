#------------ Script to produce a list of common runs between data and MC (script to be run in the period folder)
# Function to read the files and remove any extra spaces or characters
def leggi_file(nome_file):
    with open(nome_file, 'r') as file:
        return set(line.strip().replace(',', '') for line in file)

# Loading the contents of the files into two sets.
list1 = leggi_file('run_list_data.txt')
list2 = leggi_file('run_list_MC.txt')

# Find the interesection
common_elements = list1 & list2

# Write the common elements in a new file
with open('common_run_list.txt', 'w') as output_file:
    for item in common_elements:
        output_file.write(item + '\n')

print("File with common elements 'common_run_list.txt' created!")


