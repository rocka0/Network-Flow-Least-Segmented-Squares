from os import system

# Compile the CPP file
system("make")

for n in range(1, 2001):
    # Generate the input file
    system(f"python3 scripts/gen.py {n} > input.txt")

    # Run the program
    system("./bin/app < input.txt > output.txt")

    # Read output from output.txt
    with open("output.txt") as f:
        output = f.read().strip()
    
    # Write the output to plot.csv
    with open("plot.csv", "a") as f:
        f.write(f"{n},{output}\n")
