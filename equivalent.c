#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

static const uint32_t magic = 0x9e3779b9;

static uint32_t TEA_encrypt(uint32_t *block, const uint32_t *key) {
	for (uint32_t i = magic; i != magic * 33; i += magic) {
		block[0] += (block[1] << 4) + key[0] ^ block[1] + i ^ (block[1] >> 5) + key[1];
		block[1] += (block[0] << 4) + key[2] ^ block[0] + i ^ (block[0] >> 5) + key[3];
	}
}

static uint32_t decode_little_endian(const char *encoded) {
	return (
		(uint32_t) encoded[0] |
		(uint32_t) encoded[1] << 8 |
		(uint32_t) encoded[2] << 16 |
		(uint32_t) encoded[3] << 24
	);
}

int main(int argc, char **argv) {
	uint32_t key[4] = {
		decode_little_endian(argv[1] + 8),
		decode_little_endian(argv[1] + 16),
		decode_little_endian(argv[1] + 12),
		decode_little_endian(argv[1] + 20)
	};

	uint32_t block[2] = {
		decode_little_endian(argv[1] + 4),
		decode_little_endian(argv[1])
	};

	while (true) {
		int byte = getchar();

		if (byte == EOF) {
			break;
		}

		TEA_encrypt(block, key);

		putchar((byte ^ block[1]) & 0xff);
	}

	return EXIT_SUCCESS;
}
