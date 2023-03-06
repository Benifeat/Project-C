FLAGS.C = gcc -c -Wall -ansi -pedantic

FLAGS.G = gcc -g -Wall -ansi -pedantic

OBJECTS = assembler.o memory_utilities.o preassembler.o first_pass.o second_pass.o opcode.o error_case.o

HEADERS = assembler.h memory_utilities.h preassembler.h first_pass.h second_pass.h opcode.h error_case.h defined.h

assembler: clean $(OBJECTS)
	$(FLAGS.G) $(OBJECTS) -lm -o assembler

assembler.o: assembler.c $(HEADERS)
	$(FLAGS.C) assembler.c -lm -o assembler.o

memory_utilities.o: memory_utilities.c $(HEADERS)
	$(FLAGS.C) memory_utilities.c -lm -o memory_utilities.o

preassembler.o: preassembler.c $(HEADERS)
	$(FLAGS.C) preassembler.c -o preassembler.o

first_pass.o: first_pass.c $(HEADERS)
	$(FLAGS.C) first_pass.c -o first_pass.o

second_pass.o: second_pass.c $(HEADERS)
	$(FLAGS.C) second_pass.c -o second_pass.o

opcode.o: opcode.c $(HEADERS)
	$(FLAGS.C) opcode.c -lm -o opcode.o

error_case.o: error_case.c $(HEADERS)
	$(FLAGS.C) error_case.c -o error_case.o
clean:
	rm -rf *.o
