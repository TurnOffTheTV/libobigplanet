#include <fstream>
#include <gtest/gtest.h>
#include <TranslationTable.hpp>

TEST(TranslationTable,ParsesValidTrans){

	//LBP::TranslationTable parses valid .trans file properly and without errors

	EXPECT_NO_THROW({

		std::fstream introFile(std::string(FILES_DIR)+"/intro.trans",std::ios::binary|std::ios::in);
		LBP::TranslationTable introTable(introFile);

		EXPECT_EQ(introTable.getNumEntries(),14);
		EXPECT_EQ(introTable.getEntry("LBP1_INTRO_LINE5"),u"Where does it all go?");

		introFile.close();

	});

}

TEST(TranslationTable,WritesDuplicateFiles){

	//LBP::TranslationTable outputs identical files

	EXPECT_NO_THROW({

		std::fstream introFile(std::string(FILES_DIR)+"/intro.trans",std::ios::binary|std::ios::in);
		
		LBP::TranslationTable introTable(introFile);

		std::stringstream introSource;
		std::stringstream introResult;

		introFile.clear();
		introFile.seekg(0,std::ios::beg);
		introSource << introFile.rdbuf();

		introTable.write(introResult);

		introFile.close();

		EXPECT_EQ(introSource.str(),introResult.str());

	});

}