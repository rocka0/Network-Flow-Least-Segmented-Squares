################################################################################
# Define variables for directories and files
################################################################################

# Directory containing source code
SRC_DIR := src
# Directory containing scripts
SCRIPTS_DIR := scripts
# Directory to output binary files
OUTPUT_DIR := bin

# Main source code file
MAIN_SRC_FILE := $(SRC_DIR)/app.cpp

# Final binary output file
FINAL_BINARY := $(OUTPUT_DIR)/app

# Input and output files
INPUT_FILE := input.txt
OUTPUT_FILE := output.txt

################################################################################
# Define functions to remove files and directories
################################################################################

# Removes a file if it exists
define remove_if_file_exists
	@if [ -e $(1) ]; then \
		rm $(1); \
	fi
endef

# Removes a directory and its contents if it exists
define remove_if_dir_exists
	@if [ -d $(1) ]; then \
		rm -rf $(1); \
	fi
endef

################################################################################
# Define the default target to:
# 1. Clear old build files
# 2. Build the final binary
################################################################################

# Default target
all: $(FINAL_BINARY)

################################################################################
# Define target and command to link object files into the final binary
################################################################################

# Compiles source code and links object files into the final binary
$(FINAL_BINARY): $(MAIN_SRC_FILE)
	@echo "Compiling $(MAIN_SRC_FILE) into $(FINAL_BINARY)"
	@mkdir -p $(OUTPUT_DIR)
	g++ -std=c++17 -O2 $(MAIN_SRC_FILE) -o $(FINAL_BINARY)

################################################################################
# Define clean target to remove all generated files
################################################################################

# Removes all generated files
clean:
	@echo "Removing unnecessary files"
	$(call remove_if_dir_exists, $(OUTPUT_DIR))
	$(call remove_if_file_exists, output.txt)
	$(call remove_if_file_exists, plot.png)

# Runs the program
run:
	@echo "Running the program"
	@./$(FINAL_BINARY) < $(INPUT_FILE) > $(OUTPUT_FILE)
	@echo "Plotting the results"
	@python3 $(SCRIPTS_DIR)/vis.py
