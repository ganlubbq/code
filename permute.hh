/*
Reversible permutations

Copyright 2023 Ahmet Inan <inan@aicodix.de>
*/

#pragma once

#include "xorshift.hh"

namespace CODE {

template <int SIZE>
class ReversibleFisherYatesShuffle
{
	int seq[SIZE-1];
public:
	ReversibleFisherYatesShuffle()
	{
		CODE::Xorshift32 prng;
		for (int i = 0; i < SIZE-1; ++i)
			seq[i] = i + prng() % (SIZE - i);
	}
	template <typename TYPE>
	void forward(TYPE *array)
	{
		for (int i = 0; i < SIZE-1; ++i)
			std::swap(array[i], array[seq[i]]);
	}
	template <typename TYPE>
	void reverse(TYPE *array)
	{
		for (int i = SIZE-2; i >= 0; --i)
			std::swap(array[i], array[seq[i]]);
	}
};

}

