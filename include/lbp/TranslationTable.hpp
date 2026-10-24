#ifndef TRANSLATIONTABLE_HPP
#define TRANSLATIONTABLE_HPP

#include <cstdint>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace LBP {

	class TranslationTable {

		public:
			TranslationTable();
			TranslationTable(std::iostream& stream);
			TranslationTable(std::string json);

			void load(std::iostream& stream);
			void load(std::string json);

			void write(std::iostream& stream);
			std::string write();

			uint32_t getNumEntries(){return _keyIndices.size();}
			uint32_t getEntryKey(uint32_t index){return _keyIndices[index];}
			std::u16string getEntry(uint32_t entryKey);
			std::u16string getEntry(std::string translationTag){return getEntry(_transTagToKey(translationTag));}
			void setEntry(uint32_t entryKey,std::u16string entry);
			void setEntry(std::string translationTag,std::u16string entry){setEntry(_transTagToKey(translationTag),entry);}
			void removeEntry(uint32_t entryKey);
			void removeEntry(std::string translationTag){removeEntry(_transTagToKey(translationTag));}

		private:

			bool _loaded;
			std::vector<uint32_t> _keyIndices;
			std::unordered_map<uint32_t,std::u16string> _entries;

			uint32_t _transTagToKey(std::string tag);

	};

};

#endif