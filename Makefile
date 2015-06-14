MAKE := $(MAKE) --no-print-directory

all:
	@$(MAKE) -C res/
	@$(MAKE) -C src/
clean:
	@$(MAKE) -C src/ clean
