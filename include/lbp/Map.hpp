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
 * @file Map.hpp
 * @brief Defines Map class
 * @author Peyton Anderson (peyton@turnoffthetv.xyz)
 */

#ifndef MAP_HPP
#define MAP_HPP

#include <filesystem>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <lbp/GUID.hpp>

/**
 * @brief libobigplanet
 */
namespace LBP {

	/**
	 * @brief File entry in MAP
	 */
	struct MapEntry {

		/// @brief Path to file
		std::filesystem::path path;
		/// @brief File last modified date, unix timestamp in seconds
		uint64_t timestamp;
		/// @brief File size
		uint32_t size;
		/// @brief File string hash
		std::string hash;
		/// @brief File GUID
		GUID guid;

	};

	/**
	 * @brief MAP file handler
	 * @details This class provides methods for reading/writing binary MAP data and changing the data the file contains.
	 */
	class Map {

		public:

			/**
			 * @brief Build a MapEntry to add to a MAP file
			 * @param path File path to store
			 * @param timestamp Last modified date to store
			 * @param size File size to store
			 * @param GUID GUID to store
			 * @returns MapEntry with all the data
			 */
			static MapEntry constructEntry(std::filesystem::path path,uint64_t timestamp,uint32_t size,std::string hash,GUID guid);

			/**
			 * @brief Construct a Map object
			 */
			Map();
			/**
			 * @brief Construct a Map object
			 * @param stream Stream to load data from
			 */
			Map(std::iostream& stream);


			/**
			 * @brief Get MAP load state
			 * @returns Wether the MAP is loaded
			 */
			bool isLoaded(){return _loaded;}

			/**
			 * @brief Load MAP data from stream
			 * @param stream Stream to load data from
			 */
			void load(std::iostream& stream);

			/**
			 * @brief Write MAP data to stream
			 * @param stream Stream to write data to
			 */
			void write(std::iostream& stream);


			/**
			 * @brief Get number of entries in MAP
			 * @returns Number of entries
			 */
			uint32_t getNumEntries(){return _entries.size();}

			/**
			 * @brief Get an entry by its index
			 * @param index Index of entry
			 * @returns Entry at index
			 */
			MapEntry getEntry(uint32_t index);

			/**
			 * @brief Add a new entry to the MAP
			 * @param entry MapEntry to add
			 */
			void addEntry(MapEntry& entry);

			/**
			 * @brief Remove an entry from the MAP
			 * @param index Index of entry to remove
			 */
			void removeEntry(uint32_t index);


			/**
			 * @brief Get the index of an entry using its GUID
			 * @param guid GUID to look up
			 * @returns Index of entry
			 */
			uint32_t getEntryIndexByGuid(GUID guid);

			/**
			 * @brief Get the index of an entry using its hash
			 * @param hash String hash to look up
			 * @returns Index of entry
			 */
			uint32_t getEntryIndexByHash(std::string hash);

		private:

			/// @brief Wether the MAP is loaded
			bool _loaded;
			/// @brief MAP revision
			uint32_t _revision;

			/// @brief List of entries in MAP
			std::vector<MapEntry> _entries;
			/// @brief Map of GUIDs to MAP indices
			std::unordered_map<GUID,uint32_t> _guidIndices;
			/// @brief Map of string hashes to MAP indices
			std::unordered_map<std::string,uint32_t> _hashIndices;

	};

}

#endif