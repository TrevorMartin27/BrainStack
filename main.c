#include "types.h"

/* Enter in the BrainF*ck program below (currently Hello World!) */
static const char* program = "++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.";

/* The necessary libc functions, for the '.' and ',' commands */
void output(Cell cell);
Cell input(void);

/* The unnecessary functions for error handling */
void error_unclosed_bracket(void);
void error_access_before_tape_start(void);

/* index may be a negative number (so long as we have that many
cells previously allocated, but index shouldn't be greater than
or equal to SECTION_SIZE */
Cell* lookup(struct Section* section, int index) {
	while(index < 0) {
		index += SECTION_SIZE;
		section = section->previous;
		if(section == (void*)0) { /* Haha! No libc! */
			error_access_before_tape_start();
		}
	}

	return section->cells + index;
}

/* Using an integer; pointer comparison is undefined behavior >:( */
static unsigned int instruction_pointer;

void execute(struct Section* previous) {
	struct Section section = {
		.previous = previous,
		.cells = {0}
	};

	/* Relative to start of current section */
	int data_pointer = 0;

	for(;;) {
		const char command = program[instruction_pointer++];

		switch(command) {
			/* Movement Commands */
			case '<':
				data_pointer--;
				break;
			case '>':
				data_pointer++;
				if(data_pointer >= SECTION_SIZE) {
					execute(&section);
					return; /* Once execute returns, we're done */
				}
				break;

			/* Arithmetic Commands */
			case '+':
				*lookup(&section, data_pointer) += 1;
				break;
			case '-':
				*lookup(&section, data_pointer) -= 1;
				break;

			/* Input/Output Commands (see io.c) */
			case '.':
				output(*lookup(&section, data_pointer));
				break;
			case ',':
				*lookup(&section, data_pointer) = input();
				break;

			/* Conditional Branching Commands */
			case '[':
				if(*lookup(&section, data_pointer) == 0) {
					/* Look for the corresponding ']' */
					int counter = 1;
					while(counter > 0) {
						const char seek = program[++instruction_pointer];
						if(seek == '\0') { /* Reached End of String */
							error_unclosed_bracket();
						} else if(seek == '[') {
							counter++;
						} else if(seek == ']') {
							counter--;
						}
					}
				}
				break;
			case ']':
				if(*lookup(&section, data_pointer) != 0) {
					/* Look for corresponding '[' */
					int counter = 1;

					/* The fetch of command increments, need to undo */
					instruction_pointer--;
					while(counter > 0) {
						if(instruction_pointer == 0) {
							error_unclosed_bracket();
						}

						const char seek = program[--instruction_pointer];
						if(seek == '[') {
							counter--;
						} else if(seek == ']') {
							counter++;
						}
					}
				}
				break;

			/* Reached End of Program */
			case '\0':
				return;
		}
	}
}

int main(void) {
	instruction_pointer = 0;

	execute((void*)0);
	
	return 0;
}
