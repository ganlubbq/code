/*
Reed Solomon Error Correction

Copyright 2018 Ahmet Inan <inan@aicodix.de>
*/

#ifndef REED_SOLOMON_ERROR_CORRECTION_HH
#define REED_SOLOMON_ERROR_CORRECTION_HH

namespace CODE {
namespace RS {

template <int NR, typename GF>
struct Chien
{
	typedef typename GF::value_type value_type;
	typedef typename GF::ValueType ValueType;
	typedef typename GF::IndexType IndexType;
	static const int N = GF::N, K = N - NR;
	static int search(ValueType *locator, int locator_degree, IndexType *locations)
	{
		ValueType tmp[locator_degree+1];
		for (int i = 0; i <= locator_degree; ++i)
			tmp[i] = locator[i];
		int count = 0;
		for (int i = 0; i < N; ++i) {
			ValueType sum(tmp[0]);
			for (int j = 1; j <= locator_degree; ++j)
				sum += tmp[j] *= IndexType(j);
			if (!sum)
				locations[count++] = IndexType(i);
		}
		return count;
	}
};

template <int NR, typename GF>
struct LocationFinder
{
	typedef typename GF::value_type value_type;
	typedef typename GF::ValueType ValueType;
	typedef typename GF::IndexType IndexType;
	static const int Q = GF::Q, N = GF::N;
	ValueType Artin_Schreier_imap[Q];
	LocationFinder()
	{
		for (int i = 0; i < Q; ++i)
			Artin_Schreier_imap[i] = ValueType(0);
		for (int i = 2; i < N; i += 2) {
			ValueType x(i);
			ValueType xxx = x * x + x;
			if (xxx == ValueType(N))
				continue;
			assert(xxx.v);
			assert(!Artin_Schreier_imap[xxx.v].v);
			Artin_Schreier_imap[xxx.v] = x;
		}
	}
	ValueType imap(ValueType a) {
		assert(a.v <= a.N);
		assert(a.v);
		return Artin_Schreier_imap[a.v];
	}
	int operator()(ValueType *locator, int locator_degree, IndexType *locations)
	{
		if (locator_degree == 1) {
			locations[0] = (index(locator[0]) / index(locator[1])) / IndexType(1);
			return 1;
		}
		if (locator_degree == 2) {
			if (!locator[1] || !locator[0])
				return 0;
			ValueType a(locator[2]), b(locator[1]), c(locator[0]);
			ValueType ba(b/a), R(imap(a*c/(b*b)));
			if (!R)
				return 0;
			locations[0] = index(ba * R) / IndexType(1);
			locations[1] = index(ba * R + ba) / IndexType(1);
			return 2;
		}
		return Chien<NR, GF>::search(locator, locator_degree, locations);
	}
};

template <int NR, int FCR, typename GF>
struct Forney
{
	typedef typename GF::value_type value_type;
	typedef typename GF::ValueType ValueType;
	typedef typename GF::IndexType IndexType;
	static const int N = GF::N, K = N - NR;
	static int compute_evaluator(ValueType *syndromes, ValueType *locator, int locator_degree, ValueType *evaluator)
	{
		// $evaluator = (syndromes * locator) \bmod{x^{NR}}$
		int tmp = std::min(locator_degree, NR-1);
		int degree = -1;
		for (int i = 0; i <= tmp; ++i) {
			evaluator[i] = syndromes[i] * locator[0];
			for (int j = 1; j <= i; ++j)
				evaluator[i] += syndromes[i-j] * locator[j];
			if (evaluator[i])
				degree = i;
		}
		return degree;
	}
	static void compute_magnitudes(ValueType *locator, IndexType *locations, int count, ValueType *evaluator, int evaluator_degree, ValueType *magnitudes)
	{
		// $magnitude = root^{FCR-1} * \frac{evaluator(root)}{locator'(root)}$
		for (int i = 0; i < count; ++i) {
			IndexType root(locations[i] * IndexType(1)), tmp(root);
			ValueType eval(evaluator[0]);
			for (int j = 1; j <= evaluator_degree; ++j) {
				eval += evaluator[j] * tmp;
				tmp *= root;
			}
			if (!eval) {
				magnitudes[i] = ValueType(0);
				continue;
			}
			ValueType deriv(locator[1]);
			IndexType root2(root * root), tmp2(root2);
			for (int j = 3; j <= count; j += 2) {
				deriv += locator[j] * tmp2;
				tmp2 *= root2;
			}
			IndexType magnitude(index(eval) / index(deriv));
			if (FCR == 0)
				magnitude /= root;
			if (FCR > 1)
				for (int j = 1; j < FCR; ++j)
					magnitude *= root;
			magnitudes[i] = value(magnitude);
		}
	}
	static int algorithm(ValueType *syndromes, ValueType *locator, IndexType *locations, int count, ValueType *evaluator, ValueType *magnitudes)
	{
		int evaluator_degree = compute_evaluator(syndromes, locator, count, evaluator);
		compute_magnitudes(locator, locations, count, evaluator, evaluator_degree, magnitudes);
		return evaluator_degree;
	}
};

template <int NR, typename GF>
struct BerlekampMassey
{
	typedef typename GF::value_type value_type;
	typedef typename GF::ValueType ValueType;
	typedef typename GF::IndexType IndexType;
	static const int N = GF::N, K = N - NR;
	static int algorithm(ValueType *s, ValueType *C, int count = 0)
	{
		ValueType B[NR+1];
		for (int i = 0; i <= NR; ++i)
			B[i] = C[i];
		int L = count;
		for (int n = count, m = 1; n < NR; ++n) {
			ValueType d(s[n]);
			for (int i = 1; i <= L; ++i)
				d += C[i] * s[n-i];
			if (!d) {
				++m;
			} else {
				ValueType T[NR+1];
				for (int i = 0; i < m; ++i)
					T[i] = C[i];
				for (int i = m; i <= NR; ++i)
					T[i] = fma(d, B[i-m], C[i]);
				if (2 * L <= n + count) {
					L = n + count + 1 - L;
					for (int i = 0; i <= NR; ++i)
						B[i] = C[i] / d;
					m = 1;
				} else {
					++m;
				}
				for (int i = 0; i <= NR; ++i)
					C[i] = T[i];
			}
		}
		return L;
	}
};

}

template <int NR, int FCR, typename GF>
struct ReedSolomonErrorCorrection
{
	typedef typename GF::value_type value_type;
	typedef typename GF::ValueType ValueType;
	typedef typename GF::IndexType IndexType;
	static const int N = GF::N, K = N - NR;
	RS::LocationFinder<NR, GF> search;
	int operator()(ValueType *syndromes, IndexType *locations, ValueType *magnitudes, IndexType *erasures = 0, int erasures_count = 0)
	{
		assert(0 <= erasures_count && erasures_count <= NR);
		ValueType locator[NR+1];
		locator[0] = ValueType(1);
		for (int i = 1; i <= NR; ++i)
			locator[i] = ValueType(0);
		// $locator = \prod_{i=0}^{count}(1-x\,pe^{N-1-erasures_i})$
		if (erasures_count)
			locator[1] = value(IndexType(N-1) / erasures[0]);
		for (int i = 1; i < erasures_count; ++i) {
			IndexType tmp(IndexType(N-1) / erasures[i]);
			for (int j = i; j >= 0; --j)
				locator[j+1] += tmp * locator[j];
		}
		int locator_degree = RS::BerlekampMassey<NR, GF>::algorithm(syndromes, locator, erasures_count);
		assert(locator_degree);
		assert(locator_degree <= NR);
		assert(locator[0] == ValueType(1));
		while (!locator[locator_degree])
			if (--locator_degree < 0)
				return -1;
		int count = search(locator, locator_degree, locations);
		if (count < locator_degree)
			return -1;
		ValueType evaluator[NR];
		int evaluator_degree = RS::Forney<NR, FCR, GF>::algorithm(syndromes, locator, locations, count, evaluator, magnitudes);
		if (0) {
			static bool once;
			if (!once) {
				once = true;
				std::cout << "syndromes =";
				for (int i = 0; i < NR; ++i)
					std::cout << " " << (int)syndromes[i];
				std::cout << std::endl;
				std::cout << "locator = ";
				for (int i = NR; i > 0; --i) {
					if (!locator[i])
						continue;
					if (locator[i] != ValueType(1))
						std::cout << (int)locator[i] << "*";
					std::cout << "x";
					if (i != 1)
						std::cout << "^" << i;
					std::cout << " + ";
				}
				std::cout << (int)locator[0] << std::endl;
				std::cout << "locations =";
				for (int i = 0; i < count; ++i)
					std::cout << " " << (int)locations[i];
				std::cout << std::endl;
				std::cout << "evaluator = ";
				for (int i = evaluator_degree; i > 0; --i) {
					if (!evaluator[i])
						continue;
					if (evaluator[i] != ValueType(1))
						std::cout << (int)evaluator[i] << "*";
					std::cout << "x";
					if (i != 1)
						std::cout << "^" << i;
					if (i != 1 || evaluator[0])
						std::cout << " + ";
				}
				if (evaluator[0])
					std::cout << (int)evaluator[0];
				std::cout << std::endl;
				std::cout << "magnitudes =";
				for (int i = 0; i < count; ++i)
					std::cout << " " << (int)magnitudes[i];
				std::cout << std::endl;
			}
		}
		return count;
	}
};

}

#endif