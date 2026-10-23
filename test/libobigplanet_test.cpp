#include <fstream>
#include <gtest/gtest.h>
#include <StandardResource.hpp>
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

TEST(StandardResource,ParsesValidPlan){

	//LBP::StandardResource parses valid .plan file properly and without errors

	EXPECT_NO_THROW({

		std::fstream planFile(std::string(FILES_DIR)+"/palette_primitives_372.plan",std::ios::binary|std::ios::in);
		LBP::StandardResource plan(planFile);

		EXPECT_EQ(plan.getRevision(),0x222);
		EXPECT_EQ(plan.getDependencyCount(),5);

		planFile.close();

	});

}

TEST(StandardResource,ParsesValidLevel){

	//LBP::StandardResource parses valid level file properly and without errors

	EXPECT_NO_THROW({

		std::fstream lvlFile(std::string(FILES_DIR)+"/intro_level.bin",std::ios::binary|std::ios::in);
		LBP::StandardResource lvl(lvlFile);

		EXPECT_EQ(lvl.getRevision(),0x235);
		EXPECT_EQ(lvl.getDependencyCount(),240);

		lvlFile.close();

	});

}

TEST(StandardResource,ParsesValidIPR){

	//LBP::StandardResource parses valid encrypted local profile file properly and without errors

	EXPECT_NO_THROW({

		std::fstream iprFile(std::string(FILES_DIR)+"/test.ipr",std::ios::binary|std::ios::in);
		LBP::StandardResource ipr(iprFile);

		EXPECT_EQ(ipr.getRevision(),0x272);
		EXPECT_EQ(ipr.getDependencyCount(),3619);

		iprFile.close();

	});

}

TEST(StandardResource,ParsesValidText){

	//LBP::StandardResource parses valid text-based file properly and without errors

	EXPECT_NO_THROW({

		std::fstream musFile(std::string(FILES_DIR)+"/i_english_g.mus",std::ios::binary|std::ios::in);
		LBP::StandardResource mus(musFile);

		musFile.close();

	});

}