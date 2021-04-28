SOURCE_DIR     = $(shell pwd)
BUILD_DIR      = $(SOURCE_DIR)/build

.PHONY: build
build:
	@if ! [ -d $(BUILD_DIR) ]; then \
		mkdir $(BUILD_DIR); \
		(cd $(BUILD_DIR) && cmake ..) \
	fi;
	$(MAKE) -C $(BUILD_DIR)

.PHONY: clean
clean:
	@if [ -d $(BUILD_DIR) ]; then \
		$(MAKE) -C $(BUILD_DIR) clean; \
	fi;
