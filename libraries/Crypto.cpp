# include "Crypto.h"
# include "FileSystem.h"
# include "Math.h"

# include <Windows.h>
# include <stdio.h>

# define N 256				// 2^8
# define BYTES_READ 4096

static const char ALPHABET[] =
	"abcdefghijklmnopqrstuvwxyz"
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"0123456789";

static const int ALPHABET_SIZE = sizeof(ALPHABET) - 1;

static void	bf_arcfour_loop(char *pass, int index, int actlen, char *encrypted) {
	char *data = encrypted;

	int	i      = 0;
	while (i < ALPHABET_SIZE) {
		pass[index] = ALPHABET[i];

		if (index == (actlen - 1)) {
			unsigned char *ciphertext = (unsigned char*)malloc(sizeof(int) * BYTES_READ + 1);
			Crypto::RC4::_cipher(pass, encrypted, ciphertext);

			double entropy            = Math::ShannonEntropy((unsigned char *)ciphertext, BYTES_READ);
			if(entropy < 7.9) {
				printf("[*] Password found: %s (with decoded text entropy %f).\n", pass, entropy);

				//printf("file:\n");
				//for (size_t i = 0, len = strlen((char *)ciphertext); i < len; i++) {
				//for (size_t i = 0, len = 15; i < len; i++) {
				//	printf("%02hhX", ciphertext[i]);
				//}
				exit(EXIT_SUCCESS);
			}
			free(ciphertext);
		} else { bf_arcfour_loop(pass, (index + 1), actlen, encrypted); }
		++i;
	}
}

static void swap_byte(unsigned char *a, unsigned char *b) {
	int t = *a; *a = *b; *b = t;
}

//

void Crypto::RC4::_cipher(char *password, char *plaintext, unsigned char *ciphertext) {
	unsigned char S[N];
	char *key;

	if(strlen(password) < 255) {
		key = (char *)malloc((256 + 1) * sizeof(char));
		ZeroMemory(key, 256 + 1);
		int x = 0;
		for (int n = 0; n <= 255; n++) {
			if (x == strlen(password)) { x = 0; }
			key[n] = password[x];
			x++;
		}
	}

	_ksa(key, S);
	_prga(S, plaintext, ciphertext);
	free(key);
}

void Crypto::RC4::_ksa(char *key, unsigned char *S) {
	int i, j;
	for (i = 0; i < N; i++) {
		S[i] = 255 - i;
	}

	j = 0;
	for (i = 0; i < 256; i++) {
		j = (j + S[i] + key[i]) % N;
		swap_byte(&S[i], &S[j]);
	}
}

void Crypto::RC4::_prga(unsigned char *S, char *plaintext, unsigned char *ciphertext) {
	int i = 0;
	int j = 0;
	for (size_t n = 0, len = BYTES_READ; n < len; n++) {
		i             = (i + 1) % N;
		j             = (j + S[i]) % N;
		swap_byte(&S[i], &S[j]);
		int xor_key   = S[(S[i] + S[j]) % N];
		ciphertext[n] = xor_key ^ plaintext[n];
	}
}

void Crypto::RC4::_attack(char *filename, int len) {
	DWORD BytesRead = BYTES_READ;

	CHAR pData[BYTES_READ + 1];
	ZeroMemory(&pData, BytesRead + 1);

	printf("[*] Open encrypted file: %s\n", filename);
	HANDLE hFile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile != INVALID_HANDLE_VALUE) {
		if (FileSystem::_ReadFile(hFile, pData, BytesRead, &BytesRead, NULL)) {
			printf("[*] Read: %d bytes.\n", BYTES_READ);
		} else { 
			printf("[FileSystem::_ReadFile] Error: %s\n", filename);
			CloseHandle(hFile);

			printf("Exiting...");
			exit(EXIT_FAILURE);
		}
		CloseHandle(hFile);
	} else { 
		printf("[CreateFileA] Error: Crypto::RC4::_attack (%s)\n", filename);
		printf("Exiting...");
		exit(EXIT_FAILURE);
	}
	
	printf("[*] Attempt brute-force attack: %d char(s).\n", len);
	int	i = 1;
	while(i <= len) {
		printf("  - Start with: %d char(s).\n", i);
		
		char *candidate = (char *)malloc(len + 1);
		ZeroMemory(candidate, len + 1);
		bf_arcfour_loop(candidate, 0, i, pData);
		free(candidate);

		++i;
	}
}