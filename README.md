# BrainStack
A while ago, I had a question about [Turing Completeness](https://en.wikipedia.org/wiki/Turing_completeness).
Specifically, if a language could be Turing Complete without access to the heap;
could it be complete with only a stack. So today (amid some especially boring
lectures) I decided to answer that question.

BrainStack is a BrainF*ck implementation written in C without the usage of heap
allocation or libc\*.

*\*Some functionality required the usage of libc (and implicitly access to the
heap), specifically the '.' and ',' commands (output and input), and error
handling (which isn't necessary, but helpful for my sanity).*

## How It Works
The actual implementation of the interpreter isn't special; as I didn't do
anything fancy when it came to the actual interpreter. The special part was
how I was able to dynamically allocate the tape, and the answer is recursion.

By allocating sections of the tape in each call of the function, and recursing
whenever we need more memory, we can easily get access to more memory if necessary
(at the cost of potentially running out of stack space; 1MB Windows, 10MB Linux).

For example:
```C
struct Section {
	struct Section* previous;
 	/* Data */
};

void stack_allocation(struct Section* previous, int depth) {
	struct Section section;
	section.previous = previous;
	/* Initialize section.data */

	if(depth > 0) {
		stack_allocation(&section, depth - 1);
	} else {
		/* Once we reach here, we have a stack-allocated linked list */

		/* TODO: Do cool stuff */
	}
}

/* Call Somewhere */
stack_allocation(NULL);
```

BrainStack does something similar, except it just will just allocate more
memory only if we need it.

## Downsides
There are three main problems with the current implementation.

1. Running out of stack space. Ultimately unavoidable, but no BrainF*ck program
should ever use more than 1MB.
3. Set tape start. Currently, the tape can only be extended in one direction. There
could probably be solved easily, but I should probably be doing my homework right
now.
5. The program is stored in the source code/executable. This kind of defeats the
purpose of the project, but this could be trivially solved by doing some more
stack-allocation magic.

If anyone else is bored during a couple classes, feel free to solve
number 2 or number 3 (you will need to add more io functions).