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
 * @file memstream.hpp
 * @brief Defines memstream class
 * @author Peyton Anderson (peyton@turnoffthetv.xyz)
 */

#ifndef MEMSTREAM_HPP
#define MEMSTREAM_HPP

#include <cstdint>
#include <streambuf>

/**
 * @brief libobigplanet
 */
namespace LBP {

	/**
	 * @brief Memory stream
	 * @details This class creates a stream for reading/writing from a buffer in memory. Use it to pass data in memory to this library's file classes.
	 */
	class memstream : public std::streambuf {

		public:

			/**
			 * @brief Construct a memstream object
			 * @param buffer Buffer to read/write
			 * @param size Size of buffer
			 */
			memstream(uint8_t* buffer,size_t size){
				char* beg = reinterpret_cast<char*>(buffer);
				char* end = beg+size;
				setg(beg,beg,end);
				setp(beg,end);
			}

	};

};

#endif