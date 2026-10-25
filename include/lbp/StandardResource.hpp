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

			bool isLoaded(){return _loaded;}

			void load(std::iostream& stream);

			void write(std::iostream& stream);

			uint32_t getRevision(){return _revision;}
			void setRevision(uint32_t revision){_revision=revision;}
			uint32_t getDependencyCount(){return _dependencies.size();}

		protected:
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