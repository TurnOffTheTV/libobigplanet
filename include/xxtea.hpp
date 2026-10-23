#ifndef XXTEA_HPP
#define XXTEA_HPP

#include <lbp_error.hpp>

namespace LBP {

	inline constexpr uint32_t DELTA = 0x9e3779b9;
	inline constexpr uint32_t TEA_KEY[] = {0x01B70CBD,0x149607D6,0x07F94DD5,0x10DB8CA0};

	void btea(uint8_t* data,size_t length,bool shouldDecrypt,const uint32_t key[4]) {

		uint8_t padding = 4-(length%4);
		length+=padding;

		uint32_t* v = new uint32_t[length/4];
		for(size_t i=0;i<length;i+=4){
			uint32_t j;
			memcpy(&j,data+i,4);
			j=bigToSystem(j);
			v[i/4]=j;
		}
		

		uint32_t n = length/4 - 1;
		if (n < 1)
			return;

		uint32_t p, q = 6 + 52 / (n + 1);

		if (shouldDecrypt)
		{
			uint32_t z, y = v[0], sum = q * DELTA, e;
			while (sum != 0)
			{
				e = sum >> 2 & 3;
				for (p = n; p > 0; p--)
				{
					z = v[p - 1];
					y = v[p] -=
						((z >> 5 ^ y << 2) + (y >> 3 ^ z << 4) ^ (sum ^ y) + (key[p & 3 ^ e] ^ z));
				}
				z = v[n];
				y =
				v[0] -= ((z >> 5 ^ y << 2) + (y >> 3 ^ z << 4) ^ (sum ^ y) + (key[p & 3 ^ e] ^ z));
				sum = sum - DELTA;
			}
		}
		else
		{
			uint32_t z = v[n], y, sum = 0, e;
			while (q-- > 0)
			{
				sum = sum + DELTA;
				e = sum >> 2 & 3;
				for (p = 0; p < n; p++)
				{
					y = v[p + 1];
					z =
					v[p] += ((z >> 5 ^ y << 2) + (y >> 3 ^ z << 4) ^ (sum ^ y) + (key[p & 3 ^ e] ^ z));
				}
				y = v[0];
				z =
				v[n] += ((z >> 5 ^ y << 2) + (y >> 3 ^ z << 4) ^ (sum ^ y) + (key[p & 3 ^ e] ^ z));
			}
		}

		for(size_t i=0;i<length;i+=4){
			uint32_t j;
			memcpy(&j,v+i/4,4);
			j=bigToSystem(j);
			memcpy(data+i-padding,&j,4);
		}

		delete[] v;

	}

}

#endif