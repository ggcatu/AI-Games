LEN = 2
CC = gcc
CXX = g++
OPT = -g -Wall -O3 -Wno-unused-function -Wno-unused-variable
PSVNOPT = --backwards_moves --history_len=$(LEN)
FOLDER = ../../../
psvn2c_core.c:
	cp $(FOLDER)src/psvn2c_core.c ./psvn2c_core.c

psvn2c_state_map.c:
	cp $(FOLDER)src/psvn2c_state_map.c ./psvn2c_state_map.c

psvn2c_abstraction.c:
	cp $(FOLDER)src/psvn2c_abstraction.c ./psvn2c_abstraction.c

%.c: %.psvn psvn2c_core.c psvn2c_state_map.c psvn2c_abstraction.c
	$(FOLDER)bin/psvn2c $(PSVNOPT) --name=$(*F) < $< > $@
	rm -f ./psvn2c_core.c ./psvn2c_state_map.c ./psvn2c_abstraction.c

.PRECIOUS: %.c

%.IDA_manhattan: %.c ./IDA_manhattan.c
	$(CXX) $(OPT) ./IDA_manhattan.c -include $< -o $@_$(LEN) -lm
	rm -f $*.c

.PHONY: clean
clean:
	rm -fr *.IDA_manhattan psvn2c_core.c psvn2c_state_map.c psvn2c_abstraction.c *.dSYM *.o *~
