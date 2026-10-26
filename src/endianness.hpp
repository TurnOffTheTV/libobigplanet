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
 * @file endianness.hpp
 * @brief Defines byte-swapping functions
 * @author Peyton Anderson (peyton@turnoffthetv.xyz)
 */

#ifndef ENDIANNESS_HPP
#define ENDIANNESS_HPP

#include <bit>
#include <concepts>

/**
 * @brief libobigplanet
 */
namespace LBP {

	/**
	 * @brief Swap from big endian to system endianness, AKA swap endianness if on little-endian system
	 */
	template <std::integral T> constexpr T inline bigToSystem(T num){
		if constexpr(std::endian::native==std::endian::little){
			return std::byteswap(num);
		}else{
			return num;
		}
	}

	/**
	 * @brief Swap from little endian to system endianness, AKA swap endianness if on big-endian system
	 */
	template <std::integral T> constexpr T inline littleToSystem(T num){
		if constexpr(std::endian::native==std::endian::big){
			return std::byteswap(num);
		}else{
			return num;
		}
	}

};

#endif