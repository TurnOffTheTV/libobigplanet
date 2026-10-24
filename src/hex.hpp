#ifndef UTIL_HPP
#define UTIL_HPP

#include <cstdint>
#include <iomanip>
#include <ios>
#include <sstream>
#include <string>

namespace LBP {

	inline std::string bufferToHexString(uint8_t* buffer,size_t length){
		std::ostringstream hexStream;
		hexStream << std::hex << std::setfill('0'); 
		for (unsigned int i = 0;i<length;i++){
			unsigned char cleanByte = static_cast<unsigned char>(buffer[i]);
			hexStream << std::setw(2) << static_cast<int>(cleanByte);
		}
		return hexStream.str();
	}

	inline char* hexStringToBuffer(std::string hex,size_t& length){

		length=hex.length()/2;
		char* buffer = new char[length];

		for(unsigned int i=0;i<length;i++){

			unsigned char lowNybble;
			if (hex[i*2] >= '0' && hex[i*2] <= '9') lowNybble=hex[i*2]-'0'; else if(hex[i*2]>='a' && hex[i*2]<='f') lowNybble=hex[i*2]-'a'+10;
			unsigned char highNybble;
			if (hex[i*2+1] >= '0' && hex[i*2+1] <= '9') highNybble=hex[i*2+1]-'0'; else if(hex[i*2+1]>='a' && hex[i*2+1]<='f') highNybble=hex[i*2+1]-'a'+10;

			buffer[i] = (lowNybble << 4)|highNybble;

		}

		return buffer;
	}

};

#endif