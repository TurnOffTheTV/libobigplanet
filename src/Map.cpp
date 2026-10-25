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

#include <cstring>
#include <iostream>
#include <lbp/Map.hpp>
#include <endianness.hpp>
#include <hex.hpp>
#include <lbp_error.hpp>

using namespace LBP;

MapEntry Map::constructEntry(std::filesystem::path path,uint64_t timestamp,uint32_t size,std::string hash,uint32_t guid){
	MapEntry newEntry;
	newEntry.path=path;
	newEntry.timestamp=timestamp;
	newEntry.size=size;
	newEntry.hash=hash;
	newEntry.guid=guid;
	return newEntry;
}

Map::Map(){
	_loaded=false;
	_revision=256;//LBP 1/2 default
}

Map::Map(std::iostream& stream):Map(){
	load(stream);
}

void Map::load(std::iostream& stream){

	//Check if already loaded
	if(_loaded) throw lbp_error("MAP already loaded");

	//Read revision
	stream.read(reinterpret_cast<char*>(&_revision),sizeof(_revision));
	_revision=bigToSystem(_revision);
	bool isLBP3 = (_revision>>16)>327;

	//Read number of entries
	uint32_t numEntries;
	stream.read(reinterpret_cast<char*>(&numEntries),sizeof(numEntries));
	numEntries=bigToSystem(numEntries);

	for(size_t i=0;i<numEntries;i++){

		MapEntry newEntry;

		//Read path
		uint32_t pathLength = 0;
		if(isLBP3){
			stream.read(reinterpret_cast<char*>(&pathLength),2);
			pathLength<<=16;
		}else{
			stream.read(reinterpret_cast<char*>(&pathLength),4);
		}
		pathLength=bigToSystem(pathLength);

		char* pathBuffer = new char[pathLength];
		stream.read(pathBuffer,pathLength);
		newEntry.path=std::string(pathBuffer,pathLength);
		delete[] pathBuffer;

		//Read timestamp
		newEntry.timestamp=0;
		if(isLBP3){
			stream.read(reinterpret_cast<char*>(&newEntry.timestamp),4);
			newEntry.timestamp<<=32;
		}else{
			stream.read(reinterpret_cast<char*>(&newEntry.timestamp),8);
		}
		newEntry.timestamp=bigToSystem(newEntry.timestamp);

		//Read size
		stream.read(reinterpret_cast<char*>(&newEntry.size),sizeof(newEntry.size));
		newEntry.size=bigToSystem(newEntry.size);

		//Read hash
		uint8_t* hashBuffer = new uint8_t[20];
		stream.read(reinterpret_cast<char*>(hashBuffer),20);
		newEntry.hash=bufferToHexString(hashBuffer,20);
		delete[] hashBuffer;

		//Read GUID
		stream.read(reinterpret_cast<char*>(&newEntry.guid),sizeof(newEntry.guid));
		newEntry.guid=bigToSystem(newEntry.guid);

		//Set in all the places
		_guidIndices[newEntry.guid]=_entries.size();
		_hashIndices[newEntry.hash]=_entries.size();
		_entries.push_back(newEntry);

	}

	_loaded=true;

}

void Map::write(std::iostream& stream){

	bool isLBP3 = (_revision>>16)>327;

	//Write revision
	uint32_t revision = bigToSystem(_revision);
	stream.write(reinterpret_cast<char*>(&revision),sizeof(_revision));

	//Write entry count
	uint32_t numEntries = _entries.size();
	numEntries = bigToSystem(numEntries);
	stream.write(reinterpret_cast<char*>(&numEntries),sizeof(numEntries));

	for(unsigned int i=0;i<_entries.size();i++){

		//Write path
		uint32_t pathLength = _entries[i].path.string().length();
		pathLength = bigToSystem(pathLength);
		if(isLBP3){
			pathLength>>=16;
			stream.write(reinterpret_cast<char*>(&pathLength),2);
		}else{
			stream.write(reinterpret_cast<char*>(&pathLength),4);
		}
		stream.write(_entries[i].path.string().c_str(),_entries[i].path.string().length());

		//Write timestamp
		uint64_t timestamp = bigToSystem(_entries[i].timestamp);
		if(isLBP3){
			timestamp>>=32;
			stream.write(reinterpret_cast<char*>(&timestamp),4);
		}else{
			stream.write(reinterpret_cast<char*>(&timestamp),8);
		}

		//Write size
		uint32_t size = bigToSystem(_entries[i].size);
		stream.write(reinterpret_cast<char*>(&size),sizeof(size));

		//Write hash
		size_t hashLength;
		char* hashBuffer = hexStringToBuffer(_entries[i].hash,hashLength);
		stream.write(hashBuffer,20);
		delete[] hashBuffer;

		//Write GUID
		uint32_t guid = bigToSystem(_entries[i].guid);
		stream.write(reinterpret_cast<char*>(&guid),sizeof(guid));

	}

}

MapEntry Map::getEntry(uint32_t index){

	if(index>=_entries.size()) throw lbp_error("Map entry index out of range");

	return _entries[index];

}

void Map::addEntry(MapEntry& entry){
	MapEntry newEntry = MapEntry(entry);
	_entries.push_back(newEntry);
	_loaded=true;
}

void Map::removeEntry(uint32_t index){

	if(index>=_entries.size()) throw lbp_error("Map entry index out of range");

	_entries.erase(_entries.begin()+index);

}

uint32_t Map::getEntryIndexByGuid(uint32_t guid){

	if(!_guidIndices.contains(guid)) throw lbp_error("GUID not found");
	return _guidIndices[guid];

}

uint32_t Map::getEntryIndexByHash(std::string hash){

	if(!_hashIndices.contains(hash)) throw lbp_error("Hash not found");
	return _hashIndices[hash];

}