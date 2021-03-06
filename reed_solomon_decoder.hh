/*
Reed Solomon Decoder

Copyright 2018 Ahmet Inan <inan@aicodix.de>
*/

#ifndef REED_SOLOMON_DECODER_HH
#define REED_SOLOMON_DECODER_HH

#include "reed_solomon_error_correction.hh"

namespace CODE {

template <int ROOTS, int FCR, typename GF>
class ReedSolomonDecoder
{
public:
	typedef typename GF::value_type value_type;
	typedef typename GF::ValueType ValueType;
	typedef typename GF::IndexType IndexType;
	static const int NR = ROOTS;
	static const int N = GF::N, K = N - NR, NP = NR;
private:
	ReedSolomonErrorCorrection<NR, FCR, GF> algorithm;
	void update_syndromes(ValueType *poly, ValueType *syndromes, int begin, int end)
	{
		for (int j = begin; j < end; ++j) {
			ValueType coeff(poly[j]);
			IndexType root(FCR), pe(1);
			for (int i = 0; i < NR; ++i) {
				syndromes[i] = fma(root, syndromes[i], coeff);
				root *= pe;
			}
		}
	}
public:
	int compute_syndromes(ValueType *data, ValueType *parity, ValueType *syndromes, int data_len = K)
	{
		assert(0 < data_len && data_len <= K);
		// $syndromes_i = code(pe^{FCR+i})$
		ValueType coeff(data[0]);
		for (int i = 0; i < NR; ++i)
			syndromes[i] = coeff;
		update_syndromes(data, syndromes, 1, data_len);
		update_syndromes(parity, syndromes, 0, NP);
		int nonzero = 0;
		for (int i = 0; i < NR; ++i)
			nonzero += !!syndromes[i];
		return nonzero;
	}
	int operator()(ValueType *data, ValueType *parity, IndexType *erasures = 0, int erasures_count = 0, int data_len = K)
	{
		assert(0 <= erasures_count && erasures_count <= NR);
		assert(0 < data_len && data_len <= K);
		if (0) {
			for (int i = 0; i < erasures_count; ++i) {
				int idx = (int)erasures[i];
				if (idx < data_len)
					data[idx] = ValueType(0);
				else
					parity[idx-data_len] = ValueType(0);
			}
		}
		if (erasures_count && data_len < K) {
			for (int i = 0; i < erasures_count; ++i)
				erasures[i] = IndexType((int)erasures[i] + K - data_len);
		}
		ValueType syndromes[NR];
		if (!compute_syndromes(data, parity, syndromes, data_len))
			return 0;
		IndexType locations[NR];
		ValueType magnitudes[NR];
		int count = algorithm(syndromes, locations, magnitudes, erasures, erasures_count);
		if (count <= 0)
			return count;
		for (int i = 0; i < count; ++i)
			if ((int)locations[i] < K - data_len)
				return -1;
		for (int i = 0; i < count; ++i) {
			int idx = (int)locations[i] + data_len - K;
			if (idx < data_len)
				data[idx] += magnitudes[i];
			else
				parity[idx-data_len] += magnitudes[i];
		}
		int corrections_count = 0;
		for (int i = 0; i < count; ++i)
			corrections_count += !!magnitudes[i];
		return corrections_count;
	}
	int operator()(value_type *data, value_type *parity, value_type *erasures = 0, int erasures_count = 0, int data_len = K)
	{
		return (*this)(reinterpret_cast<ValueType *>(data), reinterpret_cast<ValueType *>(parity), reinterpret_cast<IndexType *>(erasures), erasures_count, data_len);
	}
	int compute_syndromes(value_type *data, value_type *parity, value_type *syndromes, int data_len = K)
	{
		return compute_syndromes(reinterpret_cast<ValueType *>(data), reinterpret_cast<ValueType *>(parity), reinterpret_cast<ValueType *>(syndromes), data_len);
	}
};

}

#endif
