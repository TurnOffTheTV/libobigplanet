#include <cstring>
#include <iostream>
#include <endianness.hpp>
#include <lbp_error.hpp>
#include <TranslationTable.hpp>

using namespace LBP;

TranslationTable::TranslationTable(){
	_loaded=false;
}

TranslationTable::TranslationTable(std::iostream& stream):TranslationTable(){
	load(stream);
}

void TranslationTable::load(std::iostream& stream){

	if(_loaded) throw lbp_error("Translation Table already loaded");

	//Read entry count
	uint32_t numEntries;
	stream.read(reinterpret_cast<char*>(&numEntries),sizeof(numEntries));
	numEntries=bigToSystem(numEntries);

	//Read entries
	uint32_t dataOffset = numEntries*8+6;
	for(unsigned int i=0;i<numEntries;i++){

		//Read entry key
		uint32_t entryKey;
		stream.read(reinterpret_cast<char*>(&entryKey),sizeof(entryKey));
		entryKey=bigToSystem(entryKey);

		//Read entry offset
		uint32_t entryOffset;
		stream.read(reinterpret_cast<char*>(&entryOffset),sizeof(entryOffset));
		entryOffset=bigToSystem(entryOffset);

		//Read string data
		size_t pos = stream.tellg();
		stream.seekg(dataOffset+entryOffset,std::ios::beg);
		std::u16string entryString;
		char16_t entryChar;
		while(stream.read(reinterpret_cast<char*>(&entryChar),sizeof(entryChar))){
			entryChar=bigToSystem(entryChar);
			if(entryChar==0xFEFF || entryChar==0) break;
			entryString.push_back(entryChar);
		}
		stream.seekg(pos,std::ios::beg);

		//Set entry
		_keyIndices.push_back(entryKey);
		_entries[entryKey]=entryString;

	}

	_loaded=true;

}

void TranslationTable::write(std::iostream& stream){

	if(!_loaded) throw lbp_error("Translation Table not yet loaded");

	//Write entry count
	uint32_t numEntries = _keyIndices.size();
	numEntries=bigToSystem(numEntries);
	stream.write(reinterpret_cast<char*>(&numEntries),sizeof(numEntries));

	//Write entry table
	uint32_t dataOffset = 0;
	for(uint32_t i=0;i<_keyIndices.size();i++){

		//Get entry values
		uint32_t key = _keyIndices[i];
		uint32_t entryOffset = dataOffset;
		dataOffset+=_entries[key].length()*2+2;
		key=bigToSystem(key);
		entryOffset=bigToSystem(entryOffset);

		//Write entry values
		stream.write(reinterpret_cast<char*>(&key),sizeof(key));
		stream.write(reinterpret_cast<char*>(&entryOffset),sizeof(entryOffset));

	}

	//Write entry strings
	for(uint32_t i=0;i<_keyIndices.size();i++){

		stream.write("\xFE\xFF",2);

		for(size_t j=0;j<_entries[_keyIndices[i]].length();j++){

			//Convert characters to big-endian
			char16_t entryChar;
			memcpy(&entryChar,_entries[_keyIndices[i]].c_str()+j,sizeof(entryChar));
			entryChar=bigToSystem(entryChar);
			stream.write(reinterpret_cast<char*>(&entryChar),sizeof(entryChar));

		}

	}

}

std::u16string TranslationTable::getEntry(uint32_t entryKey){

	if(!_entries.contains(entryKey)) throw lbp_error("Translation entry not found");
	return _entries[entryKey];

}

void TranslationTable::setEntry(uint32_t entryKey,std::u16string entry){

	_loaded=true;
	if(!_entries.contains(entryKey)) _keyIndices.push_back(entryKey);
	_entries[entryKey]=entry;

}

void TranslationTable::removeEntry(uint32_t entryKey){

	if(!_entries.contains(entryKey)) throw lbp_error("Translation entry not found");
	_entries.erase(entryKey);
	for(uint32_t i=0;i<_keyIndices.size();i++){
		if(_keyIndices[i]==entryKey){
			_keyIndices.erase(_keyIndices.begin()+i);
			break;
		}
	}

}

uint32_t TranslationTable::_transTagToKey(std::string tag){

	//No idea how this works, copied it straight from cwlib

	uint32_t v0 = 0, v1 = 0xC8509800;
	for (size_t i = 32; i > 0; --i)
	{
		uint32_t c = 0x20;
		if ((i - 1) < tag.length())
			c = (uint8_t)tag[i - 1];
		v0 = v0 * 0x1b + c;
	}

	if (tag.length() > 32)
	{
		v1 = 0;
		for (size_t i = 64; i > 32; --i)
		{
			uint32_t c = 0x20;
			if ((i - 1) < tag.length())
				c = (uint8_t)tag[i-1];
			v1 = v1 * 0x1b + c;
		}
	}

	return v0 + v1 * 0xDEADBEEF;

}