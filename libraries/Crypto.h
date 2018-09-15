# pragma once

namespace Crypto {
	namespace RC4 {
		void _ksa(char *key, unsigned char *S);
		void _prga(unsigned char *S, char *plaintext, unsigned char *ciphertext);
		void _cipher(char *password, char *plaintext, unsigned char *ciphertext);
		void _attack(char *filename, int len);
	};
};