SOURCE_DIR        = $(shell pwd)
MODULE_DIR    	  = $(SOURCE_DIR)/module
API_DIR  		  = $(SOURCE_DIR)/api
MODULE_INC		  = $(MODULE_DIR)/src

ARCH               ?= x86_64
CROSS_PREFIX       ?= 

GREEN_COLOR = \\033[0;32m
RED_COLOR   = \\033[0;31m
NC          = \\033[0;m

.PHONY:build
build: compile-kernel-module compile-api

compile-kernel-module:
	@echo "$(GREEN_COLOR)Compiling kernel module.$(NC)"
	$(MAKE) -C $(MODULE_DIR) build
	@echo "$(GREEN_COLOR)Finished compiling kernel module.$(NC)"

compile-api:
	@echo "$(GREEN_COLOR)Compiling Api.$(NC)"
	$(MAKE) -C $(API_DIR) MODULE_INC=$(MODULE_INC) build
	@echo "$(GREEN_COLOR)Finished compiling Api.$(NC)"

clean-kernel-module:
	@echo "$(GREEN_COLOR)Cleaning kernel module.$(NC)"
	$(MAKE) -C $(MODULE_DIR) clean
	@echo "$(GREEN_COLOR)Finished cleaning kernel module.$(NC)"

clean-api:
	@echo "$(GREEN_COLOR)Cleaning Api.$(NC)"
	$(MAKE) -C $(API_DIR) clean
	@echo "$(GREEN_COLOR)Finished cleaning Api.$(NC)"

.PHONY:clean
clean: clean-api clean-kernel-module


.PHONY: compile-commands
compile-commands:
	$(MAKE) -C $(API_DIR) MODULE_INC=$(MODULE_INC) compile-commands
	$(MAKE) -C $(MODULE_DIR) compile-commands


