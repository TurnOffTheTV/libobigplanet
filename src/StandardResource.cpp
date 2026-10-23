#include <cstring>
#include <zlib.h>
#include <endianness.hpp>
#include <hex.hpp>
#include <lbp_error.hpp>
#include <StandardResource.hpp>
#include <xxtea.hpp>

using namespace LBP;

StandardResource::StandardResource(){
	_loaded=false;
	_branchId=0;
	_branchRevision=0;
	_compressionFlags=0;
	_compressed=false;
	_dataLength=0;
	_data=nullptr;
}

StandardResource::StandardResource(std::iostream& stream):StandardResource(){
	load(stream);
}

StandardResource::~StandardResource(){
	if(_dataLength) delete[] _data;
}

void StandardResource::load(std::iostream& stream){

	uint32_t dependencyTableOffset = 0;

	//Read header
	stream.read(reinterpret_cast<char*>(_type),4);

	switch(_type[3]){
		case 'b':
		case 'e':{

			//Read revision
			stream.read(reinterpret_cast<char*>(&_revision),sizeof(_revision));
			_revision=bigToSystem(_revision);

			//Read dependency table offset
			stream.read(reinterpret_cast<char*>(&dependencyTableOffset),sizeof(dependencyTableOffset));
			dependencyTableOffset=bigToSystem(dependencyTableOffset);

			if(_revision>=0x189){

				//Read branch info
				if(_revision>=0x271){

					stream.read(reinterpret_cast<char*>(&_branchId),sizeof(_branchId));
					stream.read(reinterpret_cast<char*>(&_branchRevision),sizeof(_branchRevision));
					_branchId=bigToSystem(_branchId);
					_branchRevision=bigToSystem(_branchRevision);

				}

				//Read compression flags
				if(_revision>=0x297 || _revision==0x272 && _branchId=='LD' && _branchRevision>2) stream.read(reinterpret_cast<char*>(&_compressionFlags),sizeof(_compressionFlags));

				//Read wether compressed
				stream.read(reinterpret_cast<char*>(&_compressed),sizeof(_compressed));

			}

			unsigned int thing = dependencyTableOffset-stream.tellg();
			uint8_t* fileDataBuffer = new uint8_t[dependencyTableOffset-stream.tellg()];
			stream.read(reinterpret_cast<char*>(fileDataBuffer),dependencyTableOffset-stream.tellg());
			size_t offset = 0;

			//Decrypt
			if(_type[3]=='e'){

				uint32_t size;
				memcpy(&size,fileDataBuffer,sizeof(size));
				size=bigToSystem(size);
				offset+=sizeof(size);
				btea(fileDataBuffer+offset,size,true,TEA_KEY);

			}

			if(_compressed){

				//Check unknown value
				uint16_t unk1;
				memcpy(&unk1,fileDataBuffer+offset,sizeof(unk1));
				unk1=bigToSystem(unk1);
				if(unk1!=1) throw lbp_error("unk1 is not 0x0001");
				offset+=sizeof(unk1);

				//Chunk reading variables
				std::vector<uint16_t> chunkCompressedSizes;
				std::vector<uint16_t> chunkDecompressedSizes;

				//Read chunk count
				uint16_t numChunks;
				memcpy(&numChunks,fileDataBuffer+offset,sizeof(numChunks));
				numChunks=bigToSystem(numChunks);
				offset+=sizeof(numChunks);

				for(uint16_t i=0;i<numChunks;i++){

					//Read compressed chunk size
					uint16_t cmpSize;
					memcpy(&cmpSize,fileDataBuffer+offset,sizeof(cmpSize));
					cmpSize=bigToSystem(cmpSize);
					chunkCompressedSizes.push_back(cmpSize);
					offset+=sizeof(cmpSize);

					//Read decompressed chunk size
					uint16_t dcmpSize;
					memcpy(&dcmpSize,fileDataBuffer+offset,sizeof(dcmpSize));
					dcmpSize=bigToSystem(dcmpSize);
					chunkDecompressedSizes.push_back(dcmpSize);
					_dataLength+=dcmpSize;
					offset+=sizeof(dcmpSize);

				}

				//Decompress data
				size_t decOffset = 0;
				char* decompressedBuffer = new char[_dataLength];
				for(uint16_t i=0;i<numChunks;i++){

					if(chunkCompressedSizes[i]==chunkDecompressedSizes[i]){

						//Directly copy chunk
						memcpy(decompressedBuffer+decOffset,fileDataBuffer+offset,chunkCompressedSizes[i]);
						decOffset+=chunkCompressedSizes[i];

					}else{

						//Decompress chunk
						uint8_t* cmpChunk = new uint8_t[chunkCompressedSizes[i]];
						uint8_t* dcmpChunk = new uint8_t[chunkDecompressedSizes[i]];
						memcpy(cmpChunk,fileDataBuffer+offset,chunkCompressedSizes[i]);
						uLongf s = chunkDecompressedSizes[i];
						int result = uncompress(dcmpChunk,&s,cmpChunk,chunkCompressedSizes[i]);
						if(result!=Z_OK) throw lbp_error("Decompression error: "+std::to_string(result));
						memcpy(decompressedBuffer+decOffset,dcmpChunk,chunkDecompressedSizes[i]);
						decOffset+=chunkDecompressedSizes[i];
						delete[] cmpChunk;
						delete[] dcmpChunk;

					}
					
					offset+=chunkCompressedSizes[i];

				}

				//Copy to data buffer
				_data = new uint8_t[_dataLength];
				memcpy(_data,decompressedBuffer,_dataLength);

				delete[] decompressedBuffer;

			}else{

				_dataLength=thing-offset;
				_data = new uint8_t[_dataLength];
				memcpy(_data,fileDataBuffer+offset,_dataLength);

			}

			delete[] fileDataBuffer;

			//Read dependencies
			stream.seekg(dependencyTableOffset,std::ios::beg);
			uint32_t numDependencies;
			stream.read(reinterpret_cast<char*>(&numDependencies),sizeof(numDependencies));
			numDependencies=bigToSystem(numDependencies);

			for(uint32_t i=0;i<numDependencies;i++){

				//Create reference
				ResourceReference ref;
				stream.read(reinterpret_cast<char*>(&ref.flags),sizeof(ref.flags));

				//Read references
				if(ref.flags & 2) stream.read(reinterpret_cast<char*>(&ref.guid),sizeof(ref.guid));
				if(ref.flags & 1){
					uint8_t* hashBuffer = new uint8_t[20];
					stream.read(reinterpret_cast<char*>(hashBuffer),20);
					ref.hash=bufferToHexString(hashBuffer,20);
					delete[] hashBuffer;
				}

				//Read resource type
				stream.read(reinterpret_cast<char*>(&ref.resourceType),sizeof(ref.resourceType));

				//Add to dependency list
				_dependencies.push_back(ref);

			}
			
		}break;
		case 't':

			stream.seekg(0,std::ios::end);
			_dataLength=stream.tellg();
			stream.clear();
			stream.seekg(0,std::ios::beg);
			stream.read(reinterpret_cast<char*>(_data),_dataLength);

		break;
		default:
			throw lbp_error("Unknown serialization type");
		break;
	}

	_loaded=true;

}

void StandardResource::write(std::iostream& stream){

	if(!_loaded) throw lbp_error("Resource not yet loaded");

	//stream.write(reinterpret_cast<char*>(_type),4);
	stream.write(reinterpret_cast<char*>(_data),_dataLength);
}