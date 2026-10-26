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
 * @file Fart.hpp
 * @brief Defines Fart class
 * @author Peyton Anderson (peyton@turnoffthetv.xyz)
 */

#ifndef FART_HPP
#define FART_HPP

#include <array>
#include <cstdint>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <lbp/ResourceType.hpp>

/**
 * @brief libobigplanet
 */
namespace LBP {

	/**
	 * @brief FART type identifier
	 */
	enum class FartType: uint8_t {

		/// @brief FAR2 type
		FAR2='2',
		/// @brief FAR3 type
		FAR3='3',
		/// @brief FAR4 type
		FAR4='4',
		/// @brief FAR5 type
		FAR5='5',
		/// @brief FARC type
		FARC='C'

	};

	/**
	 * @brief File entry in FART
	 */
	struct FartEntry {

		/// @brief Offset of entry in FART data
		uint32_t offset;
		/// @brief Length of entry
		uint32_t length;

		/// @brief Stream to read entry from
		std::iostream* stream;

	};

	/**
	 * @brief Information about save file
	 */
	struct FartSaveInfo {

		/// @brief Game revision that saved this FART
		uint8_t revision[8];

		/// @brief ID of user that this file is from. On PS3/Vita this is a number, on PS4 it is a hash
		uint32_t userId;
		/// @brief 
		int32_t copied;
		/// @brief Save file resource type
		ResourceType saveType;
		/// @brief Hash of FART entry containing profile data
		std::string fileHash;

	};

	/**
	 * @brief FART file handler
	 * @details This class provides methods for reading/writing binary FART data and changing the data the file contains.
	 */
	class Fart {

		public:

			/**
			 * @brief Construct a Fart object
			 */
			Fart();

			/**
			 * @brief Construct a Fart object from stream
			 * @param stream Stream to load data from
			 */
			Fart(std::iostream& stream);


			/**
			 * @brief Get FART load state
			 * @returns Wether the FART is loaded
			 */
			bool isLoaded(){return _loaded;}

			/**
			 * @brief Load FART data from stream
			 * @param stream Stream to load data from
			 */
			void load(std::iostream& stream);

			/**
			 * @brief Write FART data to stream
			 * @param stream Stream to write data to
			 */
			void write(std::iostream& stream);


			/**
			 * @brief Get FART type
			 * @returns FartType FART type identifier
			 */
			FartType getType(){return _fartType;}
			/**
			 * @brief Set FART type
			 * @param type FART type identifier
			 */
			void setType(FartType type){_fartType=type;}


			/**
			 * @brief Get FART endianness
			 * @returns Wether FART is little-endian
			 */
			bool isLittleEndian(){return _littleEndian;}

			/**
			 * @brief Set FART endianness
			 * @param littleEndian Set wether FART is little-endian
			 */
			void setLittleEndian(bool littleEndian){_littleEndian=littleEndian;}


			/**
			 * @brief Get number of entries in FART
			 * @returns Number of entries
			 */
			uint32_t getNumEntries(){return _numEntries;}

			/**
			 * @brief Check if FART contains a hash
			 * @param hash Hash string to check
			 * @returns Wether an entry with that hash exists
			 */
			bool hasHash(std::string hash){return _entries.contains(hash);}

			/**
			 * @brief Get the hash for an entry by index
			 * @param index Index of entry
			 * @returns Hash string of entry
			 */
			std::string getHash(uint32_t index){return _hashIndices[index];}

			/**
			 * @brief Get the length of an entry
			 * @param hash Hash string of entry to look up
			 * @returns Entry length
			 */
			uint32_t getEntryLength(std::string hash);

			/**
			 * @brief Get the length of an entry
			 * @param index Index of entry to look up
			 * @returns Entry length
			 */
			uint32_t getEntryLength(uint32_t index){return getEntryLength(getHash(index));}

			/**
			 * @brief Get pointer to buffer with data from an entry
			 * @param hash Hash string of entry to look up
			 * @returns Entry data
			 */
			uint8_t* getEntryData(std::string hash);

			/**
			 * @brief Get pointer to buffer with data from an entry
			 * @param index Index of entry to look up
			 * @returns Entry data
			 */
			uint8_t* getEntryData(uint32_t index){return getEntryData(getHash(index));}

			/**
			 * @brief Add an entry
			 * @param stream Stream to read entry data from
			 * @param length Length of entry data to read
			 * @returns Hash string of created entry
			 */
			std::string addEntry(std::iostream& stream,uint32_t length);

			/**
			 * @brief Remove an entry
			 * @param hash Hash string of entry to remove
			 */
			void removeEntry(std::string hash);


			/**
			 * @brief Get save file user ID
			 * @returns User ID
			 */
			uint32_t getSaveUserId();

			/**
			 * @brief Set save file user ID
			 * @param userId User ID
			 */
			void setSaveUserId(uint32_t userId){_saveInfo.userId=userId;}

			/**
			 * @brief Get save file copied
			 * @returns copied
			 */
			int32_t getSaveCopied();

			/**
			 * @brief Set save file copied
			 * @param copied copied
			 */
			void setSaveCopied(int32_t copied){_saveInfo.copied=copied;}

			/**
			 * @brief Get save file resource type
			 * @returns Save file resource type
			 */
			ResourceType getSaveType();

			/**
			 * @brief Set save file resource type
			 * @param saveType Save file resource type
			 */
			void setSaveType(ResourceType saveType){_saveInfo.saveType=saveType;}

			/**
			 * @brief Get hash of profile data entry
			 * @returns Hash string of profile entry
			 */
			std::string getSaveFileHash();

			/**
			 * @brief Set hash of profile data entry
			 * @param hash Hash string of profile entry
			 */
			void setSaveFileHash(std::string hash);

		private:

			/// @brief Wether the FART is loaded
			bool _loaded;
			/// @brief FART type identifier
			FartType _fartType;

			/// @brief Wether FART is little-endian
			bool _littleEndian;
			/// @brief Number of fragments
			uint32_t _numFragments;
			/// @brief Fragment buffer
			int32_t* _fragments;
			/// @brief FART hash
			uint8_t _hash[20];
			/// @brief Information about save file
			FartSaveInfo _saveInfo;

			/// @brief Stream to load FART data from
			std::iostream* _stream;

			/// @brief Number of entries
			uint32_t _numEntries;
			/// @brief List of hashes contained in FART
			std::vector<std::string> _hashIndices;
			/// @brief Map of hashes to their entries
			std::unordered_map<std::string,FartEntry> _entries;


			/**
			 * @brief Get size of FART header section
			 * @returns Header size
			 */
			size_t _getHeaderSize();

			/**
			 * @brief Get size of FART file archive table section
			 * @returns File archive table size
			 */
			size_t _getFatSize();

			/**
			 * @brief Get size of FART save key section
			 * @returns Save key size
			 */
			size_t _getSaveKeySize();

	};

};

#endif