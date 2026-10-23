#include <fstream>
#include <gtest/gtest.h>
#include <TranslationTable.hpp>

TEST(TranslationTable,ParsesValidTrans){

	//LBP::TranslationTable parses valid .trans file properly and without errors

	EXPECT_NO_THROW({

		std::fstream introFile(std::string(FILES_DIR)+"/intro.trans",std::ios::binary|std::ios::in);
		LBP::TranslationTable introTable(&introFile);

		EXPECT_EQ(introTable.getNumEntries(),14);

		introFile.close();

	});

}