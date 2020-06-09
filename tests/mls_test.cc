/*
Test for the Maximum length sequence

Copyright 2020 Ahmet Inan <inan@aicodix.de>
*/

#include <cassert>
#include <iostream>
#include "mls.hh"

static const int poly[] = {
	0b111,
	0b1011,
	0b1101,
	0b10011,
	0b11001,
	0b100101,
	0b101001,
	0b101111,
	0b110111,
	0b111011,
	0b111101,
	0b1000011,
	0b1011011,
	0b1100001,
	0b1100111,
	0b1101101,
	0b1110011,
	0b10000011,
	0b10001001,
	0b10001111,
	0b10010001,
	0b10011101,
	0b10100111,
	0b10101011,
	0b10111001,
	0b10111111,
	0b11000001,
	0b11001011,
	0b11010011,
	0b11010101,
	0b11100101,
	0b11101111,
	0b11110001,
	0b11110111,
	0b11111101,
	0b100011101,
	0b100101011,
	0b100101101,
	0b101001101,
	0b101011111,
	0b101100011,
	0b101100101,
	0b101101001,
	0b101110001,
	0b110000111,
	0b110001101,
	0b110101001,
	0b111000011,
	0b111001111,
	0b111100111,
	0b111110101,
	0b1000010001,
	0b1000011011,
	0b1000100001,
	0b1000101101,
	0b1000110011,
	0b1001011001,
	0b1001011111,
	0b1001101001,
	0b1001101111,
	0b1001110111,
	0b1001111101,
	0b1010000111,
	0b1010010101,
	0b1010100011,
	0b1010100101,
	0b1010101111,
	0b1010110111,
	0b1010111101,
	0b1011001111,
	0b1011010001,
	0b1011011011,
	0b1011110101,
	0b1011111001,
	0b1100010011,
	0b1100010101,
	0b1100011111,
	0b1100100011,
	0b1100110001,
	0b1100111011,
	0b1101001111,
	0b1101011011,
	0b1101100001,
	0b1101101011,
	0b1101101101,
	0b1101110011,
	0b1101111111,
	0b1110000101,
	0b1110001111,
	0b1110110101,
	0b1110111001,
	0b1111000111,
	0b1111001011,
	0b1111001101,
	0b1111010101,
	0b1111011001,
	0b1111100011,
	0b1111101001,
	0b1111111011,
	0b10000001001,
	0b10000011011,
	0b10000100111,
	0b10000101101,
	0b10001100101,
	0b10001101111,
	0b10010000001,
	0b10010001011,
	0b10011000101,
	0b10011010111,
	0b10011100111,
	0b10011110011,
	0b10011111111,
	0b10100001101,
	0b10100011001,
	0b10100100011,
	0b10100110001,
	0b10100111101,
	0b10101000011,
	0b10101010111,
	0b10101101011,
	0b10110000101,
	0b10110001111,
	0b10110010111,
	0b10110100001,
	0b10111000111,
	0b10111100101,
	0b10111110111,
	0b10111111011,
	0b11000010011,
	0b11000010101,
	0b11000100101,
	0b11000110111,
	0b11001000011,
	0b11001001111,
	0b11001011011,
	0b11001111001,
	0b11001111111,
	0b11010001001,
	0b11010110101,
	0b11011000001,
	0b11011010011,
	0b11011011111,
	0b11011111101,
	0b11100010111,
	0b11100011101,
	0b11100100001,
	0b11100111001,
	0b11101000111,
	0b11101001101,
	0b11101010101,
	0b11101011001,
	0b11101100011,
	0b11101111101,
	0b11110001101,
	0b11110010011,
	0b11110110001,
	0b11111011011,
	0b11111110011,
	0b11111111001,
	0b100000000101,
	0b100000010111,
	0b100000101011,
	0b100000101101,
	0b100001000111,
	0b100001100011,
	0b100001100101,
	0b100001110001,
	0b100001111011,
	0b100010001101,
	0b100010010101,
	0b100010011111,
	0b100010101001,
	0b100010110001,
	0b100011001111,
	0b100011010001,
	0b100011100001,
	0b100011100111,
	0b100011101011,
	0b100011110101,
	0b100100001101,
	0b100100010011,
	0b100100100101,
	0b100100101001,
	0b100100111011,
	0b100100111101,
	0b100101000101,
	0b100101001001,
	0b100101010001,
	0b100101011011,
	0b100101110011,
	0b100101110101,
	0b100101111111,
	0b100110000011,
	0b100110001111,
	0b100110101011,
	0b100110101101,
	0b100110111001,
	0b100111000111,
	0b100111011001,
	0b100111100101,
	0b100111110111,
	0b101000000001,
	0b101000000111,
	0b101000010011,
	0b101000010101,
	0b101000101001,
	0b101001001001,
	0b101001100001,
	0b101001101101,
	0b101001111001,
	0b101001111111,
	0b101010000101,
	0b101010010001,
	0b101010011101,
	0b101010100111,
	0b101010101011,
	0b101010110011,
	0b101010110101,
	0b101011010101,
	0b101011011111,
	0b101011101001,
	0b101011101111,
	0b101011110001,
	0b101011111011,
	0b101100000011,
	0b101100001001,
	0b101100010001,
	0b101100110011,
	0b101100111111,
	0b101101000001,
	0b101101001011,
	0b101101011001,
	0b101101011111,
	0b101101100101,
	0b101101101111,
	0b101101111101,
	0b101110000111,
	0b101110001011,
	0b101110010011,
	0b101110010101,
	0b101110101111,
	0b101110110111,
	0b101110111101,
	0b101111001001,
	0b101111011011,
	0b101111011101,
	0b101111100111,
	0b101111101101,
	0b110000001011,
	0b110000001101,
	0b110000011001,
	0b110000011111,
	0b110001010111,
	0b110001100001,
	0b110001101011,
	0b110001110011,
	0b110010000101,
	0b110010001001,
	0b110010010111,
	0b110010011011,
	0b110010011101,
	0b110010110011,
	0b110010111111,
	0b110011000111,
	0b110011001101,
	0b110011010011,
	0b110011010101,
	0b110011100011,
	0b110011101001,
	0b110011110111,
	0b110100000011,
	0b110100001111,
	0b110100011101,
	0b110100100111,
	0b110100101101,
	0b110101000001,
	0b110101000111,
	0b110101010101,
	0b110101011001,
	0b110101100011,
	0b110101101111,
	0b110101110001,
	0b110110010011,
	0b110110011111,
	0b110110101001,
	0b110110111011,
	0b110110111101,
	0b110111001001,
	0b110111010111,
	0b110111011011,
	0b110111100001,
	0b110111100111,
	0b110111110101,
	0b111000000101,
	0b111000011101,
	0b111000100001,
	0b111000100111,
	0b111000101011,
	0b111000110011,
	0b111000111001,
	0b111001000111,
	0b111001001011,
	0b111001010101,
	0b111001011111,
	0b111001110001,
	0b111001111011,
	0b111001111101,
	0b111010000001,
	0b111010010011,
	0b111010011111,
	0b111010100011,
	0b111010111011,
	0b111011001111,
	0b111011011101,
	0b111011110011,
	0b111011111001,
	0b111100001011,
	0b111100011001,
	0b111100110001,
	0b111100110111,
	0b111101011101,
	0b111101101011,
	0b111101101101,
	0b111101110101,
	0b111110000011,
	0b111110010001,
	0b111110010111,
	0b111110011011,
	0b111110100111,
	0b111110101101,
	0b111110110101,
	0b111111001101,
	0b111111010011,
	0b111111100101,
	0b111111101001,
	0b1000001010011,
	0b1000001101001,
	0b1000001111011,
	0b1000001111101,
	0b1000010011001,
	0b1000011010001,
	0b1000011101011,
	0b1000100000111,
	0b1000100011111,
	0b1000100100011,
	0b1000100111011,
	0b1000101001111,
	0b1000101010111,
	0b1000101100001,
	0b1000101101011,
	0b1000110000101,
	0b1000110110011,
	0b1000111011001,
	0b1000111011111,
	0b1001000001101,
	0b1001000110111,
	0b1001000111101,
	0b1001001100111,
	0b1001001110011,
	0b1001001111111,
	0b1001010111001,
	0b1001011000001,
	0b1001011001011,
	0b1001100001111,
	0b1001100011101,
	0b1001100100001,
	0b1001100111001,
	0b1001100111111,
	0b1001101001101,
	0b1001101110001,
	0b1001110011001,
	0b1001110100011,
	0b1001110101001,
	0b1010000000111,
	0b1010000110001,
	0b1010000110111,
	0b1010001001111,
	0b1010001011101,
	0b1010001100111,
	0b1010001110101,
	0b1010010100111,
	0b1010010101101,
	0b1010011010011,
	0b1010100001111,
	0b1010100011101,
	0b1010101001101,
	0b1010110010011,
	0b1010111000101,
	0b1010111010111,
	0b1010111011101,
	0b1010111101011,
	0b1011000001001,
	0b1011001000111,
	0b1011001010101,
	0b1011001011001,
	0b1011010100101,
	0b1011010111101,
	0b1011100010101,
	0b1011100011001,
	0b1011101000011,
	0b1011101000101,
	0b1011101110101,
	0b1011110001001,
	0b1011110101101,
	0b1011110110011,
	0b1011110111111,
	0b1011111000001,
	0b1100001010111,
	0b1100001011101,
	0b1100010010001,
	0b1100010010111,
	0b1100010111001,
	0b1100011101111,
	0b1100100011011,
	0b1100100110101,
	0b1100101000001,
	0b1100101100101,
	0b1100101111011,
	0b1100110001011,
	0b1100110110001,
	0b1100110111101,
	0b1100111001001,
	0b1100111001111,
	0b1100111100111,
	0b1101000011011,
	0b1101000101011,
	0b1101000110011,
	0b1101001101001,
	0b1101010001011,
	0b1101011010001,
	0b1101011100001,
	0b1101011110101,
	0b1101100001011,
	0b1101100010011,
	0b1101100011111,
	0b1101101010111,
	0b1101110010001,
	0b1101110100111,
	0b1101110111111,
	0b1101111000001,
	0b1101111010011,
	0b1110000000101,
	0b1110000010001,
	0b1110000010111,
	0b1110000100111,
	0b1110001001101,
	0b1110010000111,
	0b1110010011111,
	0b1110010100101,
	0b1110010111011,
	0b1110011000101,
	0b1110011001001,
	0b1110011001111,
	0b1110011110011,
	0b1110100000111,
	0b1110100100011,
	0b1110101000011,
	0b1110101010001,
	0b1110101011011,
	0b1110101110101,
	0b1110110000101,
	0b1110110001001,
	0b1111000010101,
	0b1111000011001,
	0b1111000101111,
	0b1111001000101,
	0b1111001010001,
	0b1111001100111,
	0b1111001110011,
	0b1111010001111,
	0b1111011100011,
	0b1111100010001,
	0b1111100011011,
	0b1111100100111,
	0b1111101110001,
	0b1111110011001,
	0b1111110111011,
	0b1111110111101,
	0b1111111001001,
};

int main()
{
	const int num = sizeof(poly) / sizeof(*poly);
	int pos = 0;
	for (int i = 0b101; i <= 0b1111111111111; ++i) {
		CODE::MLS seq(i);
		if (seq.bad())
			continue;
		assert(pos < num);
		assert(poly[pos] == i);
		++pos;
	}
	assert(pos == num);
	std::cerr << "Maximum length sequence test passed!" << std::endl;
	return 0;
}
