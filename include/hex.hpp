#ifndef UTIL_HPP
#define UTIL_HPP

#include <cstdint>
#include <string>

namespace LBP {

	std::string bufferToHexString(uint8_t* buffer,size_t length);

	char* hexStringToBuffer(std::string hex,size_t& length);

};

#endif