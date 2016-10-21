OPT = -g -Wall -O3 -Wno-unused-function -Wno-unused-variable
PSVNOPT1 = --no_state_map --no_abstraction --no_backwards_moves --history_len=2
PSVNOPT2 = --no_state_map --no_abstraction --history_len=2
PSVNOPT3 = --history_len=2
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
	$(FOLDER)bin/psvn2c $(PSVNOPT2) --name=$(*F) < $< > $@
	rm -f ./psvn2c_core.c ./psvn2c_state_map.c ./psvn2c_abstraction.c

15Puzzle_2.c: 15Puzzle.psvn psvn2c_core.c psvn2c_state_map.c psvn2c_abstraction.c
	$(FOLDER)bin/psvn2c $(PSVNOPT3) --name=$(*F) < $< > $@
	rm -f ./psvn2c_core.c ./psvn2c_state_map.c ./psvn2c_abstraction.c

15PuzzleAbs1.pdb: 15PuzzleAbs1.txt
	$(FOLDER)Lesson05a/make  
 
.PRECIOUS: %.c

IDA_gap: pancakes28.c ./IDA_gap.cpp
	g++ $(OPT) ./IDA_gap.cpp -include $< -o $@ -lm
	rm -f pancakes28.c

IDA_manhattan: 15Puzzle.c ./IDA_manhattan.cpp
	g++ $(OPT) ./IDA_manhattan.cpp -include $< -o $@ -lm
	rm -f 15Puzzle.c

IDA_PDB: 15Puzzle_2.c ./IDA_PDB.cpp
	g++ $(OPT) ./IDA_PDB.cpp -include $< -o $@ -lm
	rm -f 15Puzzle_2.c

.PHONY: clean
clean:
	rm -fr IDA_gap IDA_manhattan IDA_PDB psvn2c_core.c psvn2c_state_map.c psvn2c_abstraction.c *.dSYM *.o *~
