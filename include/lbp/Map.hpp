#ifndef MAP_HPP
#define MAP_HPP

#include <filesystem>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace LBP {

	struct MapEntry {

		std::filesystem::path path;
		uint64_t timestamp;
		uint32_t size;
		std::string hash;
		uint32_t guid;

	};

	class Map {

		public:

			static MapEntry constructEntry(std::filesystem::path path,uint64_t timestamp,uint32_t size,std::string hash,uint32_t guid);

			Map();
			Map(std::iostream& stream);

			bool isLoaded(){return _loaded;}

			void load(std::iostream& stream);

			void write(std::iostream& stream);

			uint32_t getNumEntries(){return _entries.size();}
			MapEntry getEntry(uint32_t index);
			void addEntry(MapEntry& entry);
			void removeEntry(uint32_t index);

			uint32_t getEntryIndexByGuid(uint32_t guid);
			uint32_t getEntryIndexByHash(std::string hash);

		private:

			bool _loaded;
			uint32_t _revision;

			std::vector<MapEntry> _entries;
			std::unordered_map<uint32_t,uint32_t> _guidIndices;
			std::unordered_map<std::string,uint32_t> _hashIndices;

	};

}

#endif