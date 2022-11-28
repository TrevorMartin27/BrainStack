#pragma once

#define SECTION_SIZE 5

typedef char Cell;

struct Section {
	struct Section* previous;

	Cell cells[SECTION_SIZE];
};
