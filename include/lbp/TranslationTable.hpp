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

#ifndef TRANSLATIONTABLE_HPP
#define TRANSLATIONTABLE_HPP

#include <cstdint>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace LBP {

	class TranslationTable {

		public:
			TranslationTable();
			TranslationTable(std::iostream& stream);
			TranslationTable(std::string json);

			bool isLoaded(){return _loaded;}

			void load(std::iostream& stream);
			void load(std::string json);

			void write(std::iostream& stream);
			std::string write();

			uint32_t getNumEntries(){return _keyIndices.size();}
			uint32_t getEntryKey(uint32_t index){return _keyIndices[index];}
			std::u16string getEntry(uint32_t entryKey);
			std::u16string getEntry(std::string translationTag){return getEntry(_transTagToKey(translationTag));}
			void setEntry(uint32_t entryKey,std::u16string entry);
			void setEntry(std::string translationTag,std::u16string entry){setEntry(_transTagToKey(translationTag),entry);}
			void removeEntry(uint32_t entryKey);
			void removeEntry(std::string translationTag){removeEntry(_transTagToKey(translationTag));}

		private:

			bool _loaded;
			std::vector<uint32_t> _keyIndices;
			std::unordered_map<uint32_t,std::u16string> _entries;

			uint32_t _transTagToKey(std::string tag);

	};

};

#endif