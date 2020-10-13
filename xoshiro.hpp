#ifndef XOSHIRO_HPP__
#define XOSHIRO_HPP__

#include <cstdint>
#include <random>

using std::uint64_t;
static inline uint64_t rotl(const uint64_t x, int k) {
	return (x << k) | (x >> (64 - k));
}
struct alignas(32) xoshiro_256{
	using result_type = uint64_t;
	//xoshiro_256(const xoshiro_256&) = default;
private:
	uint64_t s[4];

	inline uint64_t next() {
		const uint64_t result_starstar = rotl(s[1] * 5, 7) * 9;

		const uint64_t t = s[1] << 17;

		s[2] ^= s[0];
		s[3] ^= s[1];
		s[1] ^= s[2];
		s[0] ^= s[3];

		s[2] ^= t;

		s[3] = rotl(s[3], 45);

		return result_starstar;
	}


	/* This is the jump function for the generator. It is equivalent
	   to 2^128 calls to next(); it can be used to generate 2^128
	   non-overlapping subsequences for parallel computations. */
	public:
	inline void jump() {
		static const uint64_t JUMP[] = { 0x180ec6d33cfd0aba, 0xd5a61266f0c9392c, 0xa9582618e03fc9aa, 0x39abdc4529b1661c };

		uint64_t s0 = 0;
		uint64_t s1 = 0;
		uint64_t s2 = 0;
		uint64_t s3 = 0;
		for(unsigned i = 0; i < sizeof JUMP / sizeof *JUMP; i++)
			for(unsigned b = 0; b < 64; b++) {
				if (JUMP[i] & UINT64_C(1) << b) {
					s0 ^= s[0];
					s1 ^= s[1];
					s2 ^= s[2];
					s3 ^= s[3];
				}
				next();
			}

		s[0] = s0;
		s[1] = s1;
		s[2] = s2;
		s[3] = s3;
	}



	/* This is the long-jump function for the generator. It is equivalent to
	   2^192 calls to next(); it can be used to generate 2^64 starting points,
	   from each of which jump() will generate 2^64 non-overlapping
	   subsequences for parallel distributed computations. */

	inline void long_jump() {
		static const uint64_t LONG_JUMP[] = { 0x76e15d3efefdcbbf, 0xc5004e441c522fb3, 0x77710069854ee241, 0x39109bb02acbe635 };

		uint64_t s0 = 0;
		uint64_t s1 = 0;
		uint64_t s2 = 0;
		uint64_t s3 = 0;
		for(unsigned i = 0; i < sizeof LONG_JUMP / sizeof *LONG_JUMP; i++)
			for(unsigned b = 0; b < 64; b++) {
				if (LONG_JUMP[i] & UINT64_C(1) << b) {
					s0 ^= s[0];
					s1 ^= s[1];
					s2 ^= s[2];
					s3 ^= s[3];
				}
				next();
			}

		s[0] = s0;
		s[1] = s1;
		s[2] = s2;
		s[3] = s3;
	}
public:
	inline uint64_t operator()(){
		return next();
	}
	inline xoshiro_256(){
		std::random_device dev;
		s[0] = dev();
		s[1] = dev();
		s[2] = dev();
		s[3] = dev();
	}
	inline xoshiro_256(xoshiro_256& xo) {
		s[0] = xo.s[0];
		s[1] = xo.s[1];
		s[2] = xo.s[2];
		s[3] = xo.s[3];
	}
	inline xoshiro_256(const xoshiro_256& xo) {
		s[0] = xo.s[0];
		s[1] = xo.s[1];
		s[2] = xo.s[2];
		s[3] = xo.s[3];
	}
	inline xoshiro_256(uint64_t s0) : xoshiro_256(s0, s0 ^ 0x00f00f00f, s0 ^ 0x0f00f00f0, s0 ^ 0xf00f00f00){}
	inline xoshiro_256(uint64_t s0, uint64_t s1, uint64_t s2, uint64_t s3){
		s[0] = s0;
		s[1] = s1;
		s[2] = s2;
		s[3] = s3;
		jump();
	}
	template<typename Sseq>
	inline xoshiro_256(Sseq& seed_seq){
		seed_seq.generate(s, s + 4);
		jump();
	}
	inline void seed(uint64_t s0){
		s[0] = s0;
		s[1] = s[0] ^ 0xf0ff00dd00f0f00c;
		s[2] = rotl(s[1], 45) ^ 0xffff00f0dd0f00f7;
		s[3] = rotl(s[2] + s[1], 17) ^ 0x0000fffff00f00f07;
	}
	template<typename Sseq>
	inline void seed(Sseq& seed_seq){
		seed_seq.generate(s, s + 4);
	}
	result_type min(){
		return 0;
	}
	result_type max(){
		return ~(0ULL);
	}
};


struct alignas(32) xoshiro_128{
	using result_type = uint64_t;
private:
	uint64_t s[2];

	inline uint64_t next() {
		const uint64_t s0 = s[0];
		uint64_t s1 = s[1];
		const uint64_t result = rotl(s0 * 5, 7) * 9;
		s1 ^= s0;
		s[0] = rotl(s0, 24) ^ s1 ^ (s1 << 16); // a, b
		s[1] = rotl(s1, 37); // c
		return result;
	}



	 /* This is the jump function for the generator. It is equivalent
   to 2^64 calls to next(); it can be used to generate 2^64
   non-overlapping subsequences for parallel computations. */
	void jump(void) {
	 	static const uint64_t JUMP[] = { 0xdf900294d8f554a5, 0x170865df4b3201fc };

	 	uint64_t s0 = 0;
	 	uint64_t s1 = 0;
	 	for(unsigned i = 0; i < sizeof JUMP / sizeof *JUMP; i++)
	 		for(unsigned b = 0; b < 64; b++) {
	 			if (JUMP[i] & UINT64_C(1) << b) {
	 				s0 ^= s[0];
	 				s1 ^= s[1];
	 			}
	 			next();
	 		}

	 	s[0] = s0;
	 	s[1] = s1;
	 }




	 /* This is the long-jump function for the generator. It is equivalent to
    2^96 calls to next(); it can be used to generate 2^32 starting points,
    from each of which jump() will generate 2^32 non-overlapping
    subsequences for parallel distributed computations. */

 void long_jump(void) {
 	static const uint64_t LONG_JUMP[] = { 0xd2a98b26625eee7b, 0xdddf9b1090aa7ac1 };

 	uint64_t s0 = 0;
 	uint64_t s1 = 0;
 	for(unsigned i = 0; i < sizeof LONG_JUMP / sizeof *LONG_JUMP; i++)
 		for(int b = 0; b < 64; b++) {
 			if (LONG_JUMP[i] & UINT64_C(1) << b) {
 				s0 ^= s[0];
 				s1 ^= s[1];
 			}
 			next();
 		}

 	s[0] = s0;
 	s[1] = s1;
 }

public:
	inline uint64_t operator()(){
		return next();
	}
	inline xoshiro_128(){
		std::random_device dev;
		s[0] = dev();
		s[1] = dev();
	}
	inline xoshiro_128(uint64_t s0) : xoshiro_128(s0, ~s0 + 117171717){}
	inline xoshiro_128(uint64_t s0, uint64_t s1){
		s[0] = s0;
		s[1] = s1;
		jump();
	}
	template<typename Sseq>
	inline xoshiro_128(Sseq& seed_seq){
		seed_seq.generate(s, s + 2);
		jump();
	}
	inline void seed(uint64_t s0){
		s[0] = s0;
		s[1] = ~rotl(s0, 17) + 117171717;
	}
	template<typename Sseq>
	inline void seed(Sseq& seed_seq){
		seed_seq.generate(s, s + 2);
	}
	result_type min(){
		return 0;
	}
	result_type max(){
		return ~(0ULL);
	}
};

#endif // !XOSHIRO_HPP__
