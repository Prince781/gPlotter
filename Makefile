all:
	$(MAKE) -C res/
	/bin/cp res/*.c src/resources.c
	$(MAKE) -C src/
clean:
	$(MAKE) -C res/ clean
	$(MAKE) -C src/ clean
