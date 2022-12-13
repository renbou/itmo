BIN_DIR=$(PWD)/bin
ANTLR=$(BIN_DIR)/antlr.jar

.bin-dir:
	[ -d $(BIN_DIR) ] || mkdir -p $(BIN_DIR)

.deps: .bin-dir
	[ -f $(ANTLR) ] || curl "https://www.antlr.org/download/antlr-4.11.1-complete.jar" -o $(ANTLR)

.generate-grammar: .deps
	java -Xmx500M -cp "$(ANTLR):$$CLASSPATH" org.antlr.v4.Tool \
		-Dlanguage=Go \
		-no-listener -no-visitor \
		-o grammar -package grammar *.g4 && \
	rm -f **/*.interp **/*.tokens

generate: .generate-grammar