CC = gcc
CXX = g++
OPT = -g -Wall -O3 -Wno-unused-function -Wno-unused-variable
PSVNOPT = --state_map --backward_moves --fwd_history_len=0 --bwd_history_len=0
PSVNOPT1 = --backwards_moves --history_len=2
FOLDER = ../../../

psvn2c_core.c:
	cp $(FOLDER)src/psvn2c_core.c ./psvn2c_core.c

psvn2c_state_map.c:
	cp $(FOLDER)src/psvn2c_state_map.c ./psvn2c_state_map.c

psvn2c_abstraction.c:
	cp $(FOLDER)src/psvn2c_abstraction.c ./psvn2c_abstraction.c

pancakes28.c: pancakes28.psvn psvn2c_core.c psvn2c_state_map.c psvn2c_abstraction.c
	$(FOLDER)bin/psvn2c $(PSVNOPT1) --name=$(*F) < $< > $@
	rm -f ./psvn2c_core.c ./psvn2c_state_map.c ./psvn2c_abstraction.c

15Puzzle.c: 15Puzzle.psvn psvn2c_core.c psvn2c_state_map.c psvn2c_abstraction.c
	$(FOLDER)bin/psvn2c $(PSVNOPT1) --name=$(*F) < $< > $@
	rm -f ./psvn2c_core.c ./psvn2c_state_map.c ./psvn2c_abstraction.c

%.c: %.psvn psvn2c_core.c psvn2c_state_map.c psvn2c_abstraction.c
	$(FOLDER)bin/psvn2c $(PSVNOPT) --name=$(*F) < $< > $@
	rm -f ./psvn2c_core.c ./psvn2c_state_map.c ./psvn2c_abstraction.c
 
.PRECIOUS: %.c

aestrella_gap: pancakes28.c ./aestrella.c
	g++ $(OPT) ./aestrella.c -include $< -o $@ -lm
	rm -f pancakes28.c

aestrella_puzzle: 15Puzzle.c ./aestrella.c
	g++ $(OPT) ./aestrella.c -include $< -o $@ -lm
	rm -f 15Puzzle.c

%.succ: %.c ../global/succ.c
	$(CC) $(OPT) $(FOLDER)global/succ.c -include $< -o $@
	rm -f $*.c

%.dist: %.c ../global/dist.cpp
	$(CXX) $(OPT) $(FOLDER)global/dist.cpp -include $< -o $@
	rm -f $*.c

%.distSummary: %.c $(FOLDER)global/distSummary.cpp
	$(CXX) $(OPT) $(FOLDER)global/distSummary.cpp -include $< -o $@
	rm -f $*.c

abstractor:
	$(CXX) $(OPT) $(FOLDER)src/abstractor.cpp $(FOLDER)src/psvn.cpp -o $@

%.pdb: abstractor
	@rm -f `basename $*`.abst `basename $*`.pdb `basename $*`.psvn
	./abstractor `dirname $*`.psvn `basename $*` < `basename $*`.txt
	make `basename $*`.distSummary
	echo Calculating `basename $*.pdb` ...
	@./`basename $*`.distSummary `basename $*`.pdb

%.dist_pdb: %.c $(FOLDER)global/dist_pdb.cpp
	$(CXX) $(OPT) $(FOLDER)global/dist_pdb.cpp -include $< -o $@
	rm -f $*.c


pdb: 
	make 15Puzzle/15PuzzleAbs1.pdb
	make 15Puzzle/15PuzzleAbs2.pdb
	make 15Puzzle/15PuzzleAbs3.pdb

.PHONY: clean
clean:
	rm -fr *.succ *.dist *.distSummary *.dist_pdb aestrella_gap aestrella_puzzle psvn2c_core.c psvn2c_state_map.c psvn2c_abstraction.c *.dSYM *.o *~
