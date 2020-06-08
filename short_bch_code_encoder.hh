/*
Encoder for short BCH codes

Copyright 2020 Ahmet Inan <inan@aicodix.de>
*/

#pragma once

namespace CODE {

template <int N, int K>
class ShortBCHCodeEncoder
{
	static const int P = N - K;
	static const int W = 1 << K;
	short par[W];
	static_assert(N < 8 * sizeof(int), "codeword type not wide enough");
	static_assert(P < 8 * sizeof(par[0]), "parity type not wide enough");
	static int modgen(int inp, int gen)
	{
		for (int i = K-1; i >= 0; --i) {
			int tmp = inp >> (i+P);
			inp ^= (tmp & 1) * (gen << i);
		}
		return inp;
	}
public:
	ShortBCHCodeEncoder(int generator)
	{
		for (int i = 0; i < W; ++i)
			par[i] = modgen(i << P, generator);
	}
	int operator()(int msg)
	{
		return (msg << P) | par[msg];
	}
	void operator()(int8_t *code, int msg)
	{
		int cw = (*this)(msg);
		for (int i = 0; i < N; ++i)
			code[i] = 1 - 2 * ((cw >> i) & 1);
	}
};

}

