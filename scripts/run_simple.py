import pandas as pd
import matplotlib.pyplot as plt

from os import system
system("make")

# Vary last line of input.txt i.e. segment cost

for i in range(1, 101):
    # Generate the new line for the input file
    new_line = str(i) + '\n'

    # Read the contents of the input file, except the last line
    with open('input.txt', 'r') as f:
        contents = f.readlines()[:-1]

    # Add the new line to the end of the contents
    contents.append(new_line)

    # Write the new contents to the input file
    with open('input.txt', 'w') as f:
        f.writelines(contents)

    # Run the program
    system("./bin/app < input.txt > output.txt")

    # Read the first line from output from output.txt
    with open("output.txt") as f:
        output = f.readline()

    # Write the output to plot_simple.csv
    with open("plot_simple.csv", "a") as f:
        f.write(f"{i},{output}\n")


