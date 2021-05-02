SUBDIRS = one-one many-one

all: $(SUBDIRS) 

$(SUBDIRS):
	$(MAKE) -C $@

clean:
	cd one-one && $(MAKE) clean
	cd many-one && $(MAKE) clean

run:
	cd one-one && $(MAKE) run
	cd many-one && $(MAKE) run

.PHONY: $(SUBDIRS) clean run