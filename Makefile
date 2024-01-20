.PHONY: all
all:
	@echo "Targets:"
	@echo "  - build.debug"
	@echo "  - build.release"
	@echo "  - run.debug"
	@echo "  - run.release"
	@echo "  - install.debug"
	@echo "  - install.release"

.PHONY: build.debug
build.debug:
	@./autogen.sh
	@make -C __build__/debug/

.PHONY: build.release
build.release:
	@./autogen.sh -r
	@make -C __build__/release/

.PHONY: run.debug
run.debug: build.debug
	@./__build__/debug/kickle-clone.exe

.PHONY: run.release
run.release: build.release
	@./__build__/release/kickle-clone.exe

.PHONY: install.debug
install.debug:
	@./autogen.sh
	@make -C __build__/debug/ install

.PHONY: install.release
install.release:
	@./autogen.sh -r
	@make -C __build__/release/ install
