#include <cstring>
#include <openssl/sha.h>
#include <lbp/Fart.hpp>
#include <endianness.hpp>
#include <hex.hpp>
#include <lbp_error.hpp>

using namespace LBP;

Fart::Fart(){
	_loaded=false;
	_littleEndian=false;
	_numFragments=0;
	_numEntries=0;
}

Fart::Fart(std::iostream& stream):Fart(){
	load(stream);
}

void Fart::load(std::iostream& stream){

	//Check if already loaded
	if(_loaded) throw lbp_error("FART already loaded");

	_stream=&stream;

	char* header = new char[12];
	stream.seekg(-12,std::ios::end);
	stream.read(header,12);

	//Read magic
	if(strncmp(header+8,"FAR",3)) throw lbp_error("Magic mismatch");
	_fartType=(FartType)header[11];

	//FAR5 is only for LBP Vita, always little-endian and has fragments
	if(_fartType==FAR5){
		_littleEndian=true;
		memcpy(&_numFragments,header,4);
		_numFragments=littleToSystem(_numFragments);
	}

	//Read number of entries
	memcpy(&_numEntries,header+4,4);
	_numEntries=bigToSystem(_numEntries);

	//Detect PS4 endianness by checking if first byte has content (indicates flipped byte order)
	if(!_littleEndian && _numEntries>0xFFFFFF) _littleEndian=true;
	if(_littleEndian) _numEntries=bigToSystem(_numEntries);

	//Check FART types
	//I've personally only seen types C and 4, but cwlib supports types 2,3, and 5
	if(_fartType!=FARC && _fartType!=FAR2 && _fartType!=FAR3 && _fartType!=FAR4 && _fartType!=FAR5) throw lbp_error("Invalid FART type");

	delete[] header;

	stream.clear();
	stream.seekg(-(int)(_getHeaderSize()+_getFatSize()),std::ios::end);
	for(size_t i=0;i<_numEntries;i++){

		//Set up entry
		FartEntry newEntry;
		newEntry.stream=_stream;

		//Copy hash
		uint8_t* hashBuffer = new uint8_t[20];
		stream.read(reinterpret_cast<char*>(hashBuffer),20);
		std::string hash = bufferToHexString(hashBuffer,20);
		delete[] hashBuffer;

		//Copy data offset
		stream.read(reinterpret_cast<char*>(&newEntry.offset),sizeof(newEntry.offset));
		if(_littleEndian) newEntry.offset=littleToSystem(newEntry.offset); else newEntry.offset=bigToSystem(newEntry.offset);

		//Copy data length
		stream.read(reinterpret_cast<char*>(&newEntry.length),sizeof(newEntry.length));
		if(_littleEndian) newEntry.length=littleToSystem(newEntry.length); else newEntry.length=bigToSystem(newEntry.length);

		//Set in hash indices and hash map
		_hashIndices.push_back(hash);
		_entries.insert({hash,newEntry});

	}

	//Read file hash if it exists (FAR3+)
	if(_fartType==FAR3 || _fartType==FAR4 || _fartType==FAR5) stream.read(reinterpret_cast<char*>(_hash),20);


	//Read save key if not FARC
	if(_fartType!=FARC){

		stream.seekg(-(int)(_getHeaderSize()+_getFatSize()+_getSaveKeySize()),std::ios::end);

		if(_fartType==FAR4 || _fartType==FAR5){

			//Read revision
			stream.read(reinterpret_cast<char*>(_saveInfo.revision),8);

			//Read fragments
			if(_fartType==FAR5){
				_fragments = new int32_t[_numFragments];
				for(size_t i=0;i<_numFragments;i++){
					stream.read(reinterpret_cast<char*>(&_fragments[i]),sizeof(_fragments[i]));
				}
			}

		}else{

			//Read revision
			stream.read(reinterpret_cast<char*>(_saveInfo.revision),4);

		}

		//Read user id
		stream.read(reinterpret_cast<char*>(&_saveInfo.userId),sizeof(_saveInfo.userId));
		if(_littleEndian) _saveInfo.userId=littleToSystem(_saveInfo.userId); else _saveInfo.userId=bigToSystem(_saveInfo.userId); 

		stream.seekg(40,std::ios::cur);

		//Read "copied"
		//No idea what this does (cwlib named it) and LBP3 seems to set something else here
		stream.read(reinterpret_cast<char*>(&_saveInfo.copied),sizeof(_saveInfo.copied));

		//Read save type
		stream.read(reinterpret_cast<char*>(&_saveInfo.saveType),sizeof(_saveInfo.saveType));

		stream.seekg(12,std::ios::cur);

		//Read save file hash
		uint8_t* hashBuffer = new uint8_t[20];
		stream.read(reinterpret_cast<char*>(hashBuffer),20);
		_saveInfo.fileHash = bufferToHexString(hashBuffer,20);
		delete[] hashBuffer;

	}

	//Finish
	_loaded=true;

}

void Fart::write(std::iostream& stream){

	//Write files
	for(unsigned int i=0;i<_numEntries;i++){
		char* entry = getEntryData(_hashIndices[i]);
		stream.write(entry,getEntryLength(_hashIndices[i]));
		delete[] entry;
	}

	while(stream.tellp()%4) stream.write("\x00",1);

	//Write save key
	if(_fartType!=FARC){

		if(_fartType==FAR4 || _fartType==FAR5){

			//Write revision
			stream.write(reinterpret_cast<char*>(_saveInfo.revision),8);

			//Write fragments
			if(_fartType==FAR5){
				for(size_t i=0;i<_numFragments;i++){
					stream.write(reinterpret_cast<char*>(&_fragments[i]),sizeof(_fragments[i]));
				}
			}

		}else{

			//Write revision
			stream.write(reinterpret_cast<char*>(_saveInfo.revision),4);

		}

		//Write user id
		uint32_t userId = _saveInfo.userId;
		if(_littleEndian) userId=littleToSystem(userId); else userId=bigToSystem(userId);
		stream.write(reinterpret_cast<char*>(&userId),sizeof(userId));

		for(size_t i=0;i<40;i++) stream.write("\x00",1);

		//Write "copied"
		stream.write(reinterpret_cast<char*>(&_saveInfo.copied),sizeof(_saveInfo.copied));

		//Write save type
		stream.write(reinterpret_cast<char*>(&_saveInfo.saveType),sizeof(_saveInfo.saveType));

		for(size_t i=0;i<12;i++) stream.write("\x00",1);

		//Write save file hash
		size_t length;
		char* hashBytes = hexStringToBuffer(_saveInfo.fileHash,length);
		stream.write(hashBytes,length);
		delete[] hashBytes;

		for(size_t i=0;i<40;i++) stream.write("\x00",1);

	}

	//Write fat
	uint32_t fileOffset = 0;
	for(size_t i=0;i<_numEntries;i++){

		std::string hash = _hashIndices[i];

		//Write hash
		size_t length;
		char* hashBytes = hexStringToBuffer(hash,length);
		stream.write(hashBytes,length);
		delete[] hashBytes;

		//Write offset
		uint32_t writtenOffset;
		if(_littleEndian) writtenOffset = littleToSystem(fileOffset); else writtenOffset = bigToSystem(fileOffset);
		stream.write(reinterpret_cast<char*>(&writtenOffset),sizeof(writtenOffset));
		fileOffset+=_entries[hash].length;

		//Write length
		uint32_t writtenLength;
		if(_littleEndian) writtenLength = littleToSystem(_entries[hash].length); else writtenLength = bigToSystem(_entries[hash].length);
		stream.write(reinterpret_cast<char*>(&writtenLength),sizeof(writtenLength));

	}

	//Write file hash if FARC3+
	if(_fartType==FAR3 || _fartType==FAR4 || _fartType==FAR5) stream.write(reinterpret_cast<char*>(_hash),20);

	//Write fragment count
	if(_fartType==FAR5){
		uint32_t writtenNumFragments = littleToSystem(_numFragments);
		stream.write(reinterpret_cast<char*>(&writtenNumFragments),sizeof(writtenNumFragments));
	}

	//Write entry count
	uint32_t writtenNumEntries;
	if(_littleEndian) writtenNumEntries = littleToSystem(_numEntries); else writtenNumEntries = bigToSystem(_numEntries);
	stream.write(reinterpret_cast<char*>(&writtenNumEntries),sizeof(writtenNumEntries));

	//Write magic
	std::string magic = "FAR";
	magic+=_fartType;
	stream.write(magic.c_str(),4);

}

unsigned int Fart::getEntryLength(std::string hash){
	if(!_entries.contains(hash)) throw lbp_error("Hash not found in FART");
	return _entries[hash].length;
}

char* Fart::getEntryData(std::string hash){

	if(!_entries.contains(hash)) throw lbp_error("Hash not found in FART");

	//Allocate entry data buffer
	char* buffer = new char[_entries[hash].length];

	//Get data from FART source
	_entries[hash].stream->clear();
	_entries[hash].stream->seekg(_entries[hash].offset,std::ios::beg);
	_entries[hash].stream->read(buffer,_entries[hash].length);

	return buffer;

}

std::string Fart::addEntry(std::iostream& stream,uint32_t length){

	//Create Fart entry
	FartEntry newEntry;
	newEntry.stream=&stream;
	newEntry.offset=stream.tellg();
	newEntry.length=length;

	//Create buffer
	char* buffer = new char[length];
	stream.read(buffer,length);

	//Get hash of data
	uint8_t* hashBuffer = new uint8_t[20];
	SHA1(reinterpret_cast<const unsigned char*>(buffer),length,hashBuffer);
	std::string hash = bufferToHexString(hashBuffer,20);
	delete[] hashBuffer;

	//Set in hash indices and hash map
	_numEntries++;
	_hashIndices.push_back(hash);
	_entries[hash]=newEntry;

	_loaded=true;
	delete[] buffer;
	return hash;
	
}

void Fart::removeEntry(std::string hash){

	if(!_entries.contains(hash)) throw lbp_error("Hash not found in FART");

	_numEntries--;

	//Remove hash from indices
	for(unsigned int i=0;i<_hashIndices.size();i++){
		if(_hashIndices[i]==hash){
			_hashIndices.erase(_hashIndices.begin()+i);
			break;
		}
	}

	//Remove from hash map
	_entries.erase(hash);

}

uint32_t Fart::getSaveUserId(){

	if(_fartType==FARC) throw lbp_error("FARC does not have save info");
	return _saveInfo.userId;

}

int32_t Fart::getSaveCopied(){

	if(_fartType==FARC) throw lbp_error("FARC does not have save info");
	return _saveInfo.copied;

}

uint32_t Fart::getSaveType(){

	if(_fartType==FARC) throw lbp_error("FARC does not have save info");
	return _saveInfo.saveType;

}

std::string Fart::getSaveFileHash(){

	if(_fartType==FARC) throw lbp_error("FARC does not have save info");
	return _saveInfo.fileHash;

}

void Fart::setSaveFileHash(std::string hash){

	if(!_entries.contains(hash)) throw lbp_error("Hash not found in FART");
	_saveInfo.fileHash=hash;

}

size_t Fart::_getHeaderSize(){

	if(_fartType==FAR5) return 12;
	return 8;

}

size_t Fart::_getFatSize(){

	unsigned int fatSize = _numEntries*28;
	if(_fartType==FAR3 || _fartType==FAR4 || _fartType==FAR5) fatSize+=20;//Including file hash in FAT even though it's not really

	return fatSize;
}

size_t Fart::_getSaveKeySize(){

	//FARC is not a save, has no save key
	if(_fartType==FARC) return 0;

	unsigned int saveKeySize = 128;

	if(_fartType==FAR4) saveKeySize+=4;
	if(_fartType==FAR5) saveKeySize+=8+_numFragments*4;

	return saveKeySize;

}