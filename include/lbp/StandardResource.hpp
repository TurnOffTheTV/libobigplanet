#ifndef STANDARDRESOURCE_HPP
#define STANDARDRESOURCE_HPP

#include <cstdint>
#include <iostream>
#include <vector>
#include <lbp/guid.hpp>

namespace LBP {

	struct ResourceReference {
		uint8_t flags;
		GUID guid;
		std::string hash;
		uint32_t resourceType;
	};

	class StandardResource {

		public:
			StandardResource();
			StandardResource(std::iostream& stream);
			~StandardResource();

			void load(std::iostream& stream);

			void write(std::iostream& stream);

			uint32_t getRevision(){return _revision;}
			void setRevision(uint32_t revision){_revision=revision;}
			uint32_t getDependencyCount(){return _dependencies.size();}

		private:
			bool _loaded;
			char8_t _type[4];
			uint32_t _revision;
			uint16_t _branchId;
			uint16_t _branchRevision;
			uint8_t _compressionFlags;
			bool _compressed;
			std::vector<ResourceReference> _dependencies;
			size_t _dataLength;
			uint8_t* _data;

	};

};

#endif