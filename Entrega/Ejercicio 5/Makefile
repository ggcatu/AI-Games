OPT = -g -Wall -O3 -Wno-unused-function -Wno-unused-variable
PSVNOPT1 = --no_state_map --no_abstraction --no_backwards_moves --history_len=2
PSVNOPT2 = --no_state_map --no_abstraction --history_len=2
PSVNOPT3 = --history_len=2
FOLDER = ../../

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

WIDA_gap: pancakes28.c ./WIDA_gap.cpp
	g++ $(OPT) ./WIDA_gap.cpp -include $< -o $@ -lm
	rm -f pancakes28.c

WIDA_manhattan: 15Puzzle.c ./WIDA_manhattan.cpp
	g++ $(OPT) ./WIDA_manhattan.cpp -include $< -o $@ -lm
	rm -f 15Puzzle.c

WIDA_PDB: 15Puzzle_2.c ./WIDA_PDB.cpp
	g++ $(OPT) ./WIDA_PDB.cpp -include $< -o $@ -lm
	rm -f 15Puzzle_2.c

.PHONY: clean
clean:
	rm -fr WIDA_gap WIDA_manhattan WIDA_PDB psvn2c_core.c psvn2c_state_map.c psvn2c_abstraction.c *.dSYM *.o *~
