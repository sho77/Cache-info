// Cache info.cpp : Defines the entry point for the console application.
//

#include <cstdint>
#include <algorithm>
#include <cassert>
#include <stdio.h>
#include <malloc.h>

extern "C" unsigned __int64 __rdtsc();
#pragma intrinsic(__rdtsc)
inline uint64_t clockCycleCount()
{
	return __rdtsc();
}

const size_t SIZE = 1;

struct chunk
{
	chunk* next_;
	size_t data_[SIZE];
};

void fill_chunk_with_random_value(size_t *start, size_t* end, size_t chunk_size, size_t max_chaunk_size)
{
	for (; chunk_size < max_chaunk_size; chunk_size = chunk_size << 1)
	{
		printf("Randomize chunk %i\n", chunk_size);
		std::random_shuffle(start, end);
		start = end;
		end = end + chunk_size;
	}
}

void fill_chunk_with_sequantual_value(chunk *start, chunk* end, size_t sequance_root)
{
	chunk* iterator = start;
	std::for_each(start, end, [&](chunk& chk)
	{
		chk.next_ = ++iterator;
		*(chk.data_) = sequance_root++;
	}
	);

	iterator->next_ = nullptr;
	*iterator->data_ = sequance_root;
}

const size_t KB = 1024;
const size_t MB = KB * KB;
const size_t GB = KB * MB;
const size_t TOTAL_SIZE = 16 * GB;
const size_t CAPACITY = TOTAL_SIZE / sizeof(chunk);
const size_t MAX_CHUNK_SIZE = GB + 1;

int main(int argc, char* argv[])
{
	size_t sequance = 0;
	size_t chunk_size = KB;

	uint64_t before = 0;
	uint64_t after = 0;

	size_t i = 0;
	chunk* chk = 0;
	size_t elements_cout = 0;

	char buffer[255] = { 0 };

	chunk *data = (chunk *)_aligned_malloc(TOTAL_SIZE, 4*KB);

	fill_chunk_with_sequantual_value(data, data + CAPACITY, sequance);

	FILE * file = fopen("myfile.csv", "w+b");
	
	printf("%p\n", file);

	for (; chunk_size < MAX_CHUNK_SIZE; chunk_size = chunk_size << 1)
	{
		before = clockCycleCount();
		elements_cout = chunk_size / sizeof(chunk);
		for (i = 0; i < elements_cout; ++i)
		{
			chk = (data + i);
		}

		after = clockCycleCount();

		printf("%i, %f\n", chunk_size / KB, double(after - before) / chunk_size);

		sprintf(buffer, "%f,%i\n", double(after - before) / chunk_size, chunk_size / KB);

		fwrite(buffer, sizeof(char), strlen(buffer), file);
	}

	_aligned_free(data);

	fclose(file);

	printf("Test is done, check results file myfile.bin.");

	scanf("%c");

	return 0;
}

