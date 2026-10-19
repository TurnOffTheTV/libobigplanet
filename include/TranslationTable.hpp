#ifndef TRANSLATIONTABLE_HPP
#define TRANSLATIONTABLE_HPP

#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace LBP {

	class TranslationTable {

		public:
			TranslationTable();
			TranslationTable(char* buffer);
			TranslationTable(std::fstream* stream);

			void load(char* buffer);
			void load(std::fstream* stream);

			char* write(unsigned int& length);
			void write(std::fstream* stream);

			unsigned int getNumEntries(){return _keyIndices.size();}
			unsigned int getEntryKey(unsigned int index){return _keyIndices[index];}
			std::u16string getEntry(unsigned int entryKey);
			std::u16string getEntry(std::string translationTag){return getEntry(_transTagToKey(translationTag));}
			void setEntry(unsigned int entryKey,std::u16string entry);
			void setEntry(std::string translationTag,std::u16string entry){setEntry(_transTagToKey(translationTag),entry);}
			void removeEntry(unsigned int entryKey);
			void removeEntry(std::string translationTag){removeEntry(_transTagToKey(translationTag));}

		private:

			bool _loaded;
			std::vector<unsigned int> _keyIndices;
			std::unordered_map<unsigned int,std::u16string> _entries;

			unsigned int _transTagToKey(std::string tag);

	};

};

#endif