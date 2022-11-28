#include <stdio.h>
#include <stdlib.h>

#include "types.h"

/* I'm putting these functions in this file just to seperate them */
void output(Cell cell) {
	putchar(cell);
}

Cell input(void) {
	return (Cell)getchar();
}

/* This function isn't used, but it can be helpful for debugging */
/* Keep in mind that it prints out the tape end -> beginning */
/*
void dump_tape(struct Section* section) {
	while(section != NULL) {
		for(int i = SECTION_SIZE - 1; i >= 0; i--) {
			printf("%u ", +section->cells[i]);
		}
		section = section->previous;
	}
	putchar('\n');
}
*/

void error_unclosed_bracket(void) {
	fputs("[ERROR] Unclosed Brackets.\n", stderr);
	exit(1);
}

void error_access_before_tape_start(void) {
	fputs("[ERROR] Access Before Start of Tape.\n", stderr);
	exit(1);
}
