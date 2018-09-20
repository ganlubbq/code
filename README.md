This is a work in progress and a long overdue attempt to bring all our not-DSP-related code together and make it reusable for our future projects.

Before using any of this you should enter the tests directory and execute "make".
This will check if your compiler is able to create binaries that are able to produce correct results when executed.

What we have included so far:

### [crc.hh](crc.hh)

A [Cyclic redundancy check](https://en.wikipedia.org/wiki/Cyclic_redundancy_check) helps ensuring data integrity.

For example, if we need to integrate CRC32 checking for a few bytes, like in the following:
```
# echo -n 'Hello World!' | rhash -C -
(stdin) 1C291CA3
```
We can add it to our project as simple as that:
```
DSP::CRC<uint32_t> crc(0xEDB88320, 0xFFFFFFFF);
for (uint8_t c: std::string("Hello World!")) crc(c);
assert(~crc() == 0x1C291CA3);
```

### [xorshift.hh](xorshift.hh)

Sometimes we need a sequence of ["random enough"](https://en.wikipedia.org/wiki/Diehard_tests) numbers but don't want to store them in an array to get a repeatable sequence.
Here a [Pseudorandom number generator](https://en.wikipedia.org/wiki/Pseudorandom_number_generator) can help by prodiving a deterministic and thus repeatable sequence of numbers.
[George Marsaglia](https://en.wikipedia.org/wiki/George_Marsaglia) discovered a class of simple and fast pseudorandom number generators, which he called [Xorshift](https://en.wikipedia.org/wiki/Xorshift).

### [bitstream.hh](bitstream.hh)

When dealing with unaligned and arbitrary-bit-sized elements in a data stream, the bitwise stream container might help avoiding some headaches.

### [galois_field.hh](galois_field.hh)

We have to thank [Évariste Galois](https://en.wikipedia.org/wiki/%C3%89variste_Galois) for his contribution of the [Finite field](https://en.wikipedia.org/wiki/Finite_field) to mathematics, which laid the cornerstone for a variety of applications that we take for granted today.
One of them is [Reed–Solomon error correction](https://en.wikipedia.org/wiki/Reed%E2%80%93Solomon_error_correction):

### [reed_solomon_error_correction.hh](reed_solomon_error_correction.hh)

Implemented are the following Encoders and Decoders:
* [reed_solomon_encoder.hh](reed_solomon_encoder.hh)
* [reed_solomon_decoder.hh](reed_solomon_decoder.hh)
* [bose_chaudhuri_hocquenghem_encoder.hh](bose_chaudhuri_hocquenghem_encoder.hh)
* [bose_chaudhuri_hocquenghem_decoder.hh](bose_chaudhuri_hocquenghem_decoder.hh)

