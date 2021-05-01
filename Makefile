SUBDIRS = one-one many-one

all: $(SUBDIRS) 

$(SUBDIRS):
	$(MAKE) -C $@

clean:
	cd one-one && $(MAKE) clean
	cd many-one && $(MAKE) clean

.PHONY: $(SUBDIRS) clean