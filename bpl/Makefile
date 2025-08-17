# Root Makefile: delegate to src/Makefile so build artifacts go into src/bild
.PHONY: all clean run help

all:
	$(MAKE) -C src

run:
	$(MAKE) -C src run


compile:
	$(MAKE) -C src compile

clean:
	$(MAKE) -C src clean
	@rm -rf bild

help:
	@echo "Use 'make' (or 'make -C src') to build. Artifacts will be in src/bild/."