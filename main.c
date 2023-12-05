#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
// start the program at memory address 0
#define DEFAULTPC 0

typedef enum {
	NOOP = 0,
	HALT = 1,
	SET = 2,
	ADD = 3,
	CPY = 4,
	MEMSET = 5,
	MEMLD = 6,
	OUT = 7,
	PSH = 8,
	POP = 9,
	SUB = 10,

	CMP = 11,

	JZ = 12,
	JNZ = 13,
	JE = 14,
	JL = 15,
	JG = 16,
	IN = 17,
	CMPV = 18,
	/// @brief 
	INST_COUNT
} INST;
typedef enum {
	A,
	B,
	C,
	D,
	E,
	F,
	SP,	// stack
	PC,	// program
	SR,	// soruce register (for ram addressing)
	REGISTER_COUNT
} _REGISTERS_;

short int REGISTERS[REGISTER_COUNT];

// memory
union _MEMORY_ {
	short int RAM[512];
	short int ROM[512];
	short int STACK[256];
	short int MEM[1024 + 256];
};	// end of memory

union _MEMORY_ MEMORY;

int RomLoadI = DEFAULTPC + 1;
void loadROM(char path[]) {
	puts("loading ROM...\n");
	FILE *fileptr;
	fileptr = fopen(path, "r");
	char TMP[25];
	
	while(fgets(TMP, 25, fileptr)) {
		//printf("ROMLOADER: line %d contents: %d\n", RomLoadI, atoi(TMP));
		MEMORY.ROM[RomLoadI] = atoi(TMP);
		printf("LOADING: %d\n", MEMORY.ROM[RomLoadI]);
		RomLoadI++;
	}
	free(fileptr);
	printf("\nFINISHED LOADING!\n\n");
}

// gets keyboard input
char readKey(void) {
	return getch();
}

void init(void) {
	// set all registers to 0
	REGISTERS[A] = 0;
	REGISTERS[B] = 0;
	REGISTERS[C] = 0;
	REGISTERS[D] = 0;
	REGISTERS[E] = 0;
	REGISTERS[F] = 0;
	
	// stack pointer
	REGISTERS[SP] = 0;
	// ram pointer
	REGISTERS[SR] = 0;
	// goto default program counter position
	REGISTERS[PC] = DEFAULTPC;
}

// fetch the command
int fetch(void) {
	REGISTERS[PC]++;
	return MEMORY.MEM[REGISTERS[PC]];
}	// end of fetch

int inst;
int reg;
int execute() {
	inst = fetch();
	switch (inst) {
		case NOOP: {
			break;
		}
		
		case HALT: {
			break;
		}
		
		case SET: {
			reg = fetch();
			REGISTERS[reg] = fetch();
			//printf("set register to: %d\n", REGISTERS[reg]);
			break;
		}
		
		case ADD: {
			// add a to b
			reg = fetch();
			REGISTERS[reg] = REGISTERS[reg] + REGISTERS[fetch()];
			break;
		}

		case CPY: {
			reg = fetch();
			REGISTERS[reg] = REGISTERS[fetch()];
			break;
		}

		case MEMSET: {
			MEMORY.MEM[REGISTERS[SR]] = REGISTERS[fetch()];
			break;
		}

		case MEMLD: {
			REGISTERS[fetch()] = MEMORY.MEM[REGISTERS[SR]];
			break;
		}

		case OUT: {
			reg = fetch();
			if (REGISTERS[reg] == 10) {
				printf("\n");
			}
			if (REGISTERS[reg] == 7) {
				printf("\a");
			} else {
				printf("%c", REGISTERS[reg]);
			}
			break;
		}

		case PSH: {
			MEMORY.STACK[++REGISTERS[SP]] = REGISTERS[fetch()];
			break;
		}

		case POP:{
			REGISTERS[fetch()] = MEMORY.STACK[--REGISTERS[SP]];
			break;
		}

		case SUB: {
			reg = fetch();
			REGISTERS[reg] = REGISTERS[reg] - REGISTERS[fetch()];
			break;
		}

		case CMP: {
			reg = fetch();
			int tmp = fetch();
			if (REGISTERS[reg] == REGISTERS[tmp]) {REGISTERS[F] = 1;}
			if (REGISTERS[reg] < REGISTERS[tmp]) {REGISTERS[F] = 2;}
			if (REGISTERS[reg] > REGISTERS[tmp]) {REGISTERS[F] = 3;}
			break;
		}

		case JZ: {
			if (REGISTERS[F] == 0)
				REGISTERS[PC] = fetch();
			break;
		}

		case JNZ: {
			if (REGISTERS[F] != 0)
				REGISTERS[PC] = fetch();
			break;
		}

		case JE: {
			if (REGISTERS[F] == 1)
				REGISTERS[PC] = fetch();
			break;
		}

		case JL: {
			if (REGISTERS[F] == 2)
				REGISTERS[PC] = fetch();
			break;
		}

		case JG: {
			if (REGISTERS[F] == 3)
				REGISTERS[PC] = fetch();
			break;
		}

		case IN: {
			reg = fetch();
			REGISTERS[reg] = readKey();
			break;
		}

		case CMPV: {
			reg = fetch();
			int tmp = MEMORY.MEM[REGISTERS[SR]];
			if (REGISTERS[reg] == REGISTERS[tmp]) {REGISTERS[F] = 1;}
			if (REGISTERS[reg] < REGISTERS[tmp]) {REGISTERS[F] = 2;}
			if (REGISTERS[reg] > REGISTERS[tmp]) {REGISTERS[F] = 3;}
			break;
		}
	}
	return inst;
}

int exec = 0;
// main function
int main(int argc, char *argv[]) {
	if (argc > 1) {
		loadROM(argv[1]);
		init();
		printf("memory size: %d\n\n", sizeof(MEMORY));
		while ((int)exec != HALT && REGISTERS[PC] < sizeof(MEMORY)) {
			exec = execute();
		}
		puts("\nhalted!\n");
		printf("register dump:\nA: %d\nB: %d\nC: %d\nD: %d\nE: %d\nF: %d\nSP: %d\nPC: %d\nSR: %d", REGISTERS[A], REGISTERS[B], REGISTERS[C], REGISTERS[D], REGISTERS[E], REGISTERS[F], REGISTERS[SP], REGISTERS[PC], REGISTERS[SR]);
		return 0;
	} else {puts("no file to load!");}
} // end of main
