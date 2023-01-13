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
		-o . -package grammar grammar/LL1Grammar.g4 && \
	rm -f **/*.interp **/*.tokens

generate: .generate-grammar

install: generate
	go install ll1gen

run-examples:
	echo "Example of FIRST/FIRST conflict" &&\
	go run ./main.go -i ./examples/conflicts/first-first-conflict.ll1 ||\
	echo "\n\nExample of FIRST/FOLLOW conflict" &&\
	go run ./main.go -i ./examples/conflicts/first-follow-conflict.ll1
