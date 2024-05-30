.PHONY: all
all:
	@sh ./autogen.sh
	@make -C __build__/ all

.PHONY: install
install:
	@sh ./autogen.sh
	@make -C __build__/ install

.PHONY: run
run:
	@exec $(PWD)/.shipp/dist/bin/kickle-clone
