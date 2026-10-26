/*
	Copyright (C) 2026  Peyton Anderson

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

/**
 * @file hex.hpp
 * @brief Defines hex string conversion functions
 * @author Peyton Anderson (peyton@turnoffthetv.xyz)
 */

#ifndef UTIL_HPP
#define UTIL_HPP

#include <cstdint>
#include <iomanip>
#include <ios>
#include <sstream>
#include <string>

/**
 * @brief libobigplanet
 */
namespace LBP {

	/**
	 * @brief Create a hex string from a buffer
	 * @param buffer Buffer to read
	 * @param length Length of buffer
	 * @returns Hex string of data
	 */
	inline std::string bufferToHexString(uint8_t* buffer,size_t length){
		std::ostringstream hexStream;
		hexStream << std::hex << std::setfill('0'); 
		for (unsigned int i = 0;i<length;i++){
			unsigned char cleanByte = static_cast<unsigned char>(buffer[i]);
			hexStream << std::setw(2) << static_cast<int>(cleanByte);
		}
		return hexStream.str();
	}

	/**
	 * @brief Convert data from a hex string
	 * @param hex Hex string
	 * @param length Variable to set to final buffer length
	 * @returns Pointer to data buffer
	 */
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