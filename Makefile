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
install.debug: build.debug
	@mkdir -p dist/debug/
	@echo "  INSTALL  kickle-clone.exe"
	@cp ./__build__/debug/kickle-clone.exe dist/debug/
	@echo "  INSTALL  data.zip"
	@cp ./__build__/debug/data.zip dist/debug/

.PHONY: install.release
install.release: build.release
	@mkdir -p dist/release/
	@echo "  INSTALL  kickle-clone.exe"
	@cp ./__build__/release/kickle-clone.exe dist/release/
	@echo "  INSTALL  data.zip"
	@cp ./__build__/release/data.zip dist/release/
