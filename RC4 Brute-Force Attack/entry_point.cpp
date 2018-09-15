# include <Windows.h>
# include <stdio.h>


# include "../libraries/Crypto.h"
# include "../libraries/FileSystem.h"
# include "../libraries/Math.h"

void program_header(void) {
	printf("\n");
	printf("*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n");
	printf("  -= NES Special Cypherpunk Reverser =-  \n");
	printf(" Created: by @mekhalleh (for Pirates.RE) \n");
	printf("-----------------------------------------\n");
	printf("     ARCFOUR Brute-Force Attack Tool     \n");
	printf("               version 1.0               \n");
	printf("*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n");
}

int main(int argc, char *argv[]) {
	program_header();

	if (argc != 2) {
		printf(" Usage: %s <filename>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	Crypto::RC4::_attack(argv[1], 10);

	return(EXIT_SUCCESS);
}