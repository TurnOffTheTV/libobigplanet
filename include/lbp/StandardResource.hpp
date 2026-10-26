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
 * @file StandardResource.hpp
 * @brief Defines StandardResource class
 * @author Peyton Anderson (peyton@turnoffthetv.xyz)
 */

#ifndef STANDARDRESOURCE_HPP
#define STANDARDRESOURCE_HPP

#include <cstdint>
#include <iostream>
#include <vector>
#include <lbp/GUID.hpp>
#include <lbp/ResourceType.hpp>

/**
 * @brief libobigplanet
 */
namespace LBP {

	/**
	 * @brief Flags for how to reference resources
	 */
	enum ReferenceFlag : uint8_t {

		/// @brief Reference using hash
		BY_HASH=1,
		/// @brief Reference using GUID
		BY_GUID=2
	};

	/**
	 * @brief Reference to resource
	 */
	struct ResourceReference {

		/// @brief Reference flags
		ReferenceFlag flags;
		/// @brief GUID of resource
		GUID guid;
		/// @brief Hash of resource
		std::string hash;
		/// @brief Type of resource
		ResourceType resourceType;
	};

	/**
	 * @brief Resource base class
	 * @details This class provides methods for encoding/parsing the various schemes used to compress/encrypt resources.
	 */
	class StandardResource {

		public:

			/**
			 * @brief Construct a StandardResource object
			 */
			StandardResource();

			/**
			 * @brief Construct a StandardResource object
			 * @param stream Stream to load data from
			 */
			StandardResource(std::iostream& stream);

			/**
			 * @brief Deconstruct a StandardResource object
			 */
			~StandardResource();


			/**
			 * @brief Get resource load state
			 * @returns Wether the resource is loaded
			 */
			bool isLoaded(){return _loaded;}

			/**
			 * @brief Load resource from stream
			 * @param stream Stream to load data from
			 */
			void load(std::iostream& stream);

			/**
			 * @brief Write resource to stream
			 * @param stream Stream to write data to
			 */
			void write(std::iostream& stream);


			/**
			 * @brief Get resource revision
			 * @returns Resource revision
			 */
			uint32_t getRevision(){return _revision;}

			/**
			 * @brief Set resource revision
			 * @param revision Resource revision
			 */
			void setRevision(uint32_t revision){_revision=revision;}

			/**
			 * @brief Get number of dependencies
			 * @returns Number of dependencies
			 */
			uint32_t getDependencyCount(){return _dependencies.size();}

		protected:

			/// @brief Wether resource is loaded
			bool _loaded;

			/// @brief Resource type (magic)
			char8_t _type[4];
			/// @brief Resource revision
			uint32_t _revision;
			/// @brief Resource branch ID
			uint16_t _branchId;
			/// @brief Resource branch revision
			uint16_t _branchRevision;
			/// @brief Resource compression flags
			uint8_t _compressionFlags;
			/// @brief Wether the resource is compressed
			bool _compressed;

			/// @brief List of dependencies
			std::vector<ResourceReference> _dependencies;

			/// @brief Length of resource data buffer
			size_t _dataLength;
			/// @brief Resource data buffer
			uint8_t* _data;

	};

};

#endif