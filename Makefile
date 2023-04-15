################################################################################
# Define variables for directories and files
################################################################################
SRC_DIR := src
SCRIPTS_DIR := scripts
OUTPUT_DIR := bin

MAIN_SRC_FILE := $(SRC_DIR)/app.cpp

FINAL_BINARY := $(OUTPUT_DIR)/app

################################################################################
# Define functions to remove files and directories
################################################################################
define remove_if_file_exists
	@if [ -e $(1) ]; then \
		rm $(1); \
	fi
endef

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
all: clean $(FINAL_BINARY)

################################################################################
# Define target and command to link object files into the final binary
################################################################################
$(FINAL_BINARY): $(MAIN_SRC_FILE)
	@echo "Compiling $(MAIN_SRC_FILE) into $(FINAL_BINARY)"
	@mkdir -p $(OUTPUT_DIR)
	g++ -std=c++17 -O2 $(MAIN_SRC_FILE) -o $(FINAL_BINARY)

################################################################################
# Define clean target to remove all generated files
################################################################################
clean:
	@echo "Removing unnecessary files"
	$(call remove_if_dir_exists, $(OUTPUT_DIR))

################################################################################
# Define target to run the final binary
################################################################################
run: $(FINAL_BINARY)
	@echo "Running $(FINAL_BINARY)"
	@./$(FINAL_BINARY)
