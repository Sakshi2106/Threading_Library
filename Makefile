SUBDIRS = one-one many-one

all: $(SUBDIRS) 

$(SUBDIRS):
	$(MAKE) --no-print-directory -C $@

clean:
	cd one-one && $(MAKE) --no-print-directory clean
	cd many-one && $(MAKE) --no-print-directory clean

run:
	cd one-one && $(MAKE) --no-print-directory run
	cd many-one && $(MAKE) --no-print-directory run

.PHONY: $(SUBDIRS) clean run