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

#ifndef FART_HPP
#define FART_HPP

#include <array>
#include <cstdint>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace LBP {

	enum FartType: uint8_t {

		FAR2='2',
		FAR3='3',
		FAR4='4',
		FAR5='5',
		FARC='C'

	};

	struct FartEntry {

			uint32_t offset;
			uint32_t length;

			std::iostream* stream;

	};

	struct FartSaveInfo {

			uint8_t revision[8];

			uint32_t userId;
			int32_t copied;
			uint32_t saveType;
			std::string fileHash;

	};

	class Fart {

		public:

			Fart();
			Fart(std::iostream& stream);

			bool isLoaded(){return _loaded;}

			void load(std::iostream& stream);

			void write(std::iostream& stream);

			FartType getType(){return _fartType;}
			void setType(FartType type){_fartType=type;}

			bool isLittleEndian(){return _littleEndian;}
			void setLittleEndian(bool littleEndian){_littleEndian=littleEndian;}

			uint32_t getNumEntries(){return _numEntries;}
			std::string getHash(uint32_t index){return _hashIndices[index];}
			bool hasHash(std::string hash){return _entries.contains(hash);}
			uint32_t getEntryLength(std::string hash);
			uint32_t getEntryLength(uint32_t index){return getEntryLength(getHash(index));}
			char* getEntryData(std::string hash);
			char* getEntryData(uint32_t index){return getEntryData(getHash(index));}
			std::string addEntry(std::iostream& stream,uint32_t length);
			void removeEntry(std::string hash);

			uint32_t getSaveUserId();
			void setSaveUserId(uint32_t userId){_saveInfo.userId=userId;}
			int32_t getSaveCopied();
			void setSaveCopied(int32_t copied){_saveInfo.copied=copied;}
			uint32_t getSaveType();
			void setSaveType(uint32_t saveType){_saveInfo.saveType=saveType;}
			std::string getSaveFileHash();
			void setSaveFileHash(std::string hash);

		private:

			bool _loaded;
			FartType _fartType;

			bool _littleEndian;
			uint32_t _numFragments;
			int32_t* _fragments;
			uint8_t _hash[20];
			FartSaveInfo _saveInfo;

			std::iostream* _stream;

			uint32_t _numEntries;
			std::vector<std::string> _hashIndices;
			std::unordered_map<std::string,FartEntry> _entries;

			size_t _getHeaderSize();
			size_t _getFatSize();
			size_t _getSaveKeySize();

	};

};

#endif