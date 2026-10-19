#include <cstring>
#include <iostream>
#include <endianness.hpp>
#include <lbp_error.hpp>
#include <TranslationTable.hpp>

using namespace LBP;

TranslationTable::TranslationTable(){
	_loaded=false;
}

TranslationTable::TranslationTable(char* buffer):TranslationTable(){
	load(buffer);
}

TranslationTable::TranslationTable(std::fstream* stream):TranslationTable(){
	load(stream);
}

void TranslationTable::load(char* buffer){

	if(_loaded) throw lbp_error("Translation Table already loaded");

	//Read entry count
	unsigned int numEntries;
	memcpy(&numEntries,buffer,4);
	numEntries=bigToSystem(numEntries);

	//Read entries
	unsigned int dataOffset = numEntries*8+6;
	for(unsigned int i=0;i<numEntries;i++){

		//Read entry id
		unsigned int entryKey;
		memcpy(&entryKey,buffer+4+i*8,4);
		entryKey=bigToSystem(entryKey);

		//Read entry offset
		unsigned int entryOffset;
		memcpy(&entryOffset,buffer+8+i*8,4);
		entryOffset=bigToSystem(entryOffset);

		//Read string data
		std::u16string entryString;
		unsigned short entryChar;
		while(true){
			memcpy(&entryChar,buffer+dataOffset+entryOffset,2);
			entryChar=bigToSystem(entryChar);
			if(entryChar==0xFEFF || entryChar==0) break;
			entryString.push_back(entryChar);
			entryOffset+=2;
		}
		//NOTE: last string in file is not terminated with 0xFEFF, so we assume the buffer will be null terminated
		//TODO: find a better way to do this (know buffer length ahead of time)

		//Set entry
		_keyIndices.push_back(entryKey);
		_entries[entryKey]=entryString;

	}

	_loaded=true;

}

void TranslationTable::load(std::fstream* stream){

	if(_loaded) throw lbp_error("Translation Table already loaded");

	//Read entry count
	unsigned int numEntries;
	stream->read((char*)&numEntries,4);
	numEntries=bigToSystem(numEntries);

	//Read entries
	unsigned int dataOffset = numEntries*8+6;
	for(unsigned int i=0;i<numEntries;i++){

		//Read entry key
		unsigned int entryKey;
		stream->read((char*)&entryKey,4);
		entryKey=bigToSystem(entryKey);

		//Read entry offset
		unsigned int entryOffset;
		stream->read((char*)&entryOffset,4);
		entryOffset=bigToSystem(entryOffset);

		//Read string data
		unsigned int pos = stream->tellg();
		stream->seekg(dataOffset+entryOffset,std::ios::beg);
		std::u16string entryString;
		unsigned short entryChar;
		while(stream->read((char*)&entryChar,2)){
			entryChar=bigToSystem(entryChar);
			if(entryChar==0xFEFF || entryChar==0) break;
			entryString.push_back(entryChar);
		}
		stream->seekg(pos,std::ios::beg);

		//Set entry
		_keyIndices.push_back(entryKey);
		_entries[entryKey]=entryString;

	}

	_loaded=true;

}

char* TranslationTable::write(unsigned int& length){

	if(!_loaded) throw lbp_error("Translation Table not yet loaded");

	//Calculate buffer length
	length=4+_entries.size()*8;
	for(unsigned int i=0;i<_keyIndices.size();i++) length+=_entries[_keyIndices[i]].length()*2+2;

	char* buffer = new char[length];

	//Write entry count
	unsigned int numEntries = _keyIndices.size();
	numEntries=bigToSystem(numEntries);
	memcpy(buffer,&numEntries,4);

	//Write entry table
	unsigned int offset = 4;
	unsigned int dataOffset = 0;
	for(unsigned int i=0;i<_keyIndices.size();i++){

		//Get entry values
		unsigned int key = _keyIndices[i];
		unsigned int entryOffset = dataOffset;
		dataOffset+=_entries[key].length()*2+2;
		key=bigToSystem(key);
		entryOffset=bigToSystem(entryOffset);

		//Write entry values
		memcpy(buffer+offset,&key,4);
		memcpy(buffer+offset+4,&entryOffset,4);

		offset+=8;
	}

	//Write entry strings
	for(unsigned int i=0;i<_keyIndices.size();i++){

		memcpy(buffer+offset,"\xFE\xFF",2);

		for(unsigned int j=0;j<_entries[_keyIndices[i]].length();j++){

			//Convert characters to big-endian
			unsigned short entryChar;
			memcpy(&entryChar,_entries[_keyIndices[i]].c_str()+j,2);
			entryChar=bigToSystem(entryChar);
			memcpy(buffer+offset+j*2+2,&entryChar,2);

		}

		offset+=_entries[_keyIndices[i]].length()*2+2;

	}

	return buffer;

}

void TranslationTable::write(std::fstream* stream){

	if(!_loaded) throw lbp_error("Translation Table not yet loaded");

	//Write entry count
	unsigned int numEntries = _keyIndices.size();
	numEntries=bigToSystem(numEntries);
	stream->write((char*)&numEntries,4);

	//Write entry table
	unsigned int dataOffset = 0;
	for(unsigned int i=0;i<_keyIndices.size();i++){

		//Get entry values
		unsigned int key = _keyIndices[i];
		unsigned int entryOffset = dataOffset;
		dataOffset+=_entries[key].length()*2+2;
		key=bigToSystem(key);
		entryOffset=bigToSystem(entryOffset);

		//Write entry values
		stream->write((char*)&key,4);
		stream->write((char*)&entryOffset,4);

	}

	//Write entry strings
	for(unsigned int i=0;i<_keyIndices.size();i++){

		stream->write("\xFE\xFF",2);

		for(unsigned int j=0;j<_entries[_keyIndices[i]].length();j++){

			//Convert characters to big-endian
			unsigned short entryChar;
			memcpy(&entryChar,_entries[_keyIndices[i]].c_str()+j,2);
			entryChar=bigToSystem(entryChar);
			stream->write((char*)&entryChar,2);

		}

	}

}

std::u16string TranslationTable::getEntry(unsigned int entryKey){

	if(!_entries.contains(entryKey)) throw lbp_error("Translation entry not found");
	return _entries[entryKey];

}

void TranslationTable::setEntry(unsigned int entryKey,std::u16string entry){

	_loaded=true;
	if(!_entries.contains(entryKey)) _keyIndices.push_back(entryKey);
	_entries[entryKey]=entry;

}

void TranslationTable::removeEntry(unsigned int entryKey){

	if(!_entries.contains(entryKey)) throw lbp_error("Translation entry not found");
	_entries.erase(entryKey);
	for(unsigned int i=0;i<_keyIndices.size();i++){
		if(_keyIndices[i]==entryKey){
			_keyIndices.erase(_keyIndices.begin()+i);
			break;
		}
	}

}

unsigned int TranslationTable::_transTagToKey(std::string tag){

	//No idea how this works, copied it straight from cwlib

	unsigned int v0 = 0, v1 = 0xC8509800;
	for (int i = 32; i > 0; --i)
	{
		unsigned int c = 0x20;
		if ((i - 1) < tag.length())
			c = (unsigned char)tag[i - 1];
		v0 = v0 * 0x1b + c;
	}

	if (tag.length() > 32)
	{
		v1 = 0;
		for (int i = 64; i > 32; --i)
		{
			unsigned int c = 0x20;
			if ((i - 1) < tag.length())
				c = (unsigned char)tag[i-1];
			v1 = v1 * 0x1b + c;
		}
	}

	return v0 + v1 * 0xDEADBEEF;

}