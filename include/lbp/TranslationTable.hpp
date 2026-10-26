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
 * @file TranslationTable.hpp
 * @brief Defines TranslationTable class
 * @author Peyton Anderson (peyton@turnoffthetv.xyz)
 */

#ifndef TRANSLATIONTABLE_HPP
#define TRANSLATIONTABLE_HPP

#include <cstdint>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

/**
 * @brief libobigplanet
 */
namespace LBP {

	/**
	 * @brief Translation Table file handler
	 * @details This class provides methods for reading/writing binary translation data and changing the data the file contains.
	 */
	class TranslationTable {

		public:

			/**
			 * @brief Construct a TranslationTable object
			 */
			TranslationTable();

			/**
			 * @brief Construct a TranslationTable object
			 * @param stream Stream to load data from
			 */
			TranslationTable(std::iostream& stream);

			/**
			 * @brief Construct a TranslationTable object
			 * @param json Toolkit-style JSON to load data from
			 */
			TranslationTable(std::string json);


			/**
			 * @brief Get table load state
			 * @returns Wether the table is loaded
			 */
			bool isLoaded(){return _loaded;}

			/**
			 * @brief Load translation data from stream
			 * @param stream Stream to load data from
			 */
			void load(std::iostream& stream);

			/**
			 * @brief Load translation data from Toolkit-style JSON
			 * @param json JSON to load data from
			 */
			void load(std::string json);

			/**
			 * @brief Write translation data to stream
			 * @param stream Stream to write data to
			 */
			void write(std::iostream& stream);

			/**
			 * @brief Create Toolkit-style JSON from translation data
			 * @returns JSON translation table
			 */
			std::string write();


			/**
			 * @brief Get number of translation entries
			 * @returns Number of entries
			 */
			uint32_t getNumEntries(){return _keyIndices.size();}

			/**
			 * @brief Get an entry key at a specific index
			 * @param index Index to look up
			 * @returns Entry key at index
			 */
			uint32_t getEntryKey(uint32_t index){return _keyIndices[index];}

			/**
			 * @brief Get entry by entry key
			 * @param entryKey Entry key to look up
			 * @returns Translation string
			 */
			std::u16string getEntry(uint32_t entryKey);

			/**
			 * @brief Get entry by translation tage
			 * @param translationTag Translation tag to look up
			 * @returns Translation string
			 */
			std::u16string getEntry(std::string translationTag){return getEntry(_transTagToKey(translationTag));}

			/**
			 * @brief Set an entry by entry key
			 * @param entryKey Entry key to set
			 * @param entry Translation string to set
			 */
			void setEntry(uint32_t entryKey,std::u16string entry);
			
			/**
			 * @brief Set an entry by translation tag
			 * @param translationTag Translation tag to set
			 * @param entry Translation string to set
			 */
			void setEntry(std::string translationTag,std::u16string entry){setEntry(_transTagToKey(translationTag),entry);}
			
			/**
			 * @brief Remove an entry by entry key
			 * @param entryKey Entry key to remove
			 */
			void removeEntry(uint32_t entryKey);

			/**
			 * @brief Remove an entry by translation tag
			 * @param translationTag Translation tag to remove
			 */
			void removeEntry(std::string translationTag){removeEntry(_transTagToKey(translationTag));}

		private:

			/// @brief Wether the table is loaded
			bool _loaded;

			/// @brief List of entry keys
			std::vector<uint32_t> _keyIndices;
			/// @brief Map of entry keys to translation strings
			std::unordered_map<uint32_t,std::u16string> _entries;

			/**
			 * @brief Compute entry key from translation tag
			 * @param tag Translation tag
			 * @returns Entry key
			 */
			uint32_t _transTagToKey(std::string tag);

	};

};

#endif