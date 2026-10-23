#include <fstream>
#include <gtest/gtest.h>
#include <TranslationTable.hpp>

TEST(TranslationTable,ParsesValidTrans){

	//LBP::TranslationTable parses valid .trans file properly and without errors

	EXPECT_NO_THROW({

		std::fstream introFile(std::string(FILES_DIR)+"/intro.trans",std::ios::binary|std::ios::in);
		LBP::TranslationTable introTable(introFile);

		EXPECT_EQ(introTable.getNumEntries(),14);
		EXPECT_EQ(introTable.getEntry("LBP1_INTRO_LINE2"),u"...'The Orb of Dreamers'.");

		introFile.close();

	});

}

TEST(TranslationTable,ParsesValidJson){

	//LBP::TranslationTable parses valid Toolkit translation json file properly and without errors

	EXPECT_NO_THROW({

		std::fstream introFile(std::string(FILES_DIR)+"/intro.trans.json",std::ios::binary|std::ios::in);
		std::stringstream introJson;
		introJson << introFile.rdbuf();
		introFile.close();

		LBP::TranslationTable introTable(introJson.str());

		EXPECT_EQ(introTable.getNumEntries(),14);
		EXPECT_EQ(introTable.getEntry("LBP1_INTRO_LINE2"),u"...'The Orb of Dreamers'.");

		

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