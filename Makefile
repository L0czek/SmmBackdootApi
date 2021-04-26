SOURCE_DIR        = $(shell pwd)
RING0_DIR    	  = $(SOURCE_DIR)/Ring0

ARCH               ?= x86_64
CROSS_PREFIX       ?= 

GREEN_COLOR = \\033[0;32m
RED_COLOR   = \\033[0;31m
NC          = \\033[0;m

compile-ring0-api: $(RING0_DIR)
	@echo "$(GREEN_COLOR)Compiling Ring0 Api.$(NC)"
	$(MAKE) -C $(RING0_DIR) build
	@echo "$(GREEN_COLOR)Finished compiling Ring0 Api.$(NC)"

clean-ring0-api: $(RING0_DIR)
	@echo "$(GREEN_COLOR)Cleaning Ring0 Api.$(NC)"
	$(MAKE) -C $(RING0_DIR) clean
	@echo "$(GREEN_COLOR)Finished cleaning Ring0 Api.$(NC)"

compile-all: compile-ring0-api

.PHONY: clean-all
clean-all: clean-ring0-api

compile-commands: $(RING0_DIR)
	@echo "$(GREEN_COLOR)Creating Ring0 Api compile_commands.json.$(NC)"
	$(MAKE) -C $(RING0_DIR) compile-commands
	@echo "$(GREEN_COLOR)Finished creating Ring0 Api compile_commands.json.$(NC)"

