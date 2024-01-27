.PHONY: all
all:
	@./autogen.sh
	@make -C __build__/ all

.PHONY: install
install:
	@./autogen.sh
	@make -C __build__/ install

.PHONY: run
run:
	@exec $(PWD)/.shipp/dist/bin/kickle-clone
