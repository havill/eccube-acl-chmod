GCC = gcc -Wall
binaries = eccube-acl-chmod

.PHONY: all
all: $(binaries)

thebase2eccube-pre: eccube-acl-chmod.c
	@$(GCC) $^ -o $@

.PHONY: clean
clean:
	@rm -f $(binaries)