# include "Math.h"
# include <Windows.h>
# include <math.h>

double Math::ShannonEntropy(const unsigned char *p, unsigned int size) {
	double entropy = 0.0;

	int histogram[256];
	size_t i;

	memset(histogram, 0, sizeof(int) * 256);
	for(i = 0; i < size; ++i)
		++histogram[p[i]];

	for(i = 0; i < 256; ++i) {
		if (histogram[i])
			entropy -= (double)histogram[i] / size * log2((double)histogram[i] / size);
	}
	return(entropy);
}