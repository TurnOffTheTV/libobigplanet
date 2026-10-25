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

#include <fstream>
#include <gtest/gtest.h>
#include <lbp/Fart.hpp>
#include <lbp/Map.hpp>
#include <lbp/StandardResource.hpp>
#include <lbp/TranslationTable.hpp>

TEST(Fart,ParsesValidFARC){

	//LBP::Fart parses valid FARC file properly and without errors

	EXPECT_NO_THROW({

		std::fstream farcFile(std::string(FILES_DIR)+"/container/intro.farc",std::ios::binary|std::ios::in);
		LBP::Fart farc(farcFile);

		EXPECT_EQ(farc.getNumEntries(),2);

		farcFile.close();

	});

}

TEST(Fart,ParsesValidFAR4){

	//LBP::Fart parses valid FAR4 save file properly and without errors

	EXPECT_NO_THROW({

		std::fstream far4File(std::string(FILES_DIR)+"/container/bigfart69",std::ios::binary|std::ios::in);
		LBP::Fart far4(far4File);

		EXPECT_EQ(far4.getNumEntries(),1);

		far4File.close();

	});

}

TEST(Fart,WritesDuplicateFiles){

	//LBP::Fart outputs identical files

	EXPECT_NO_THROW({

		std::fstream farcFile(std::string(FILES_DIR)+"/container/intro.farc",std::ios::binary|std::ios::in);
		std::fstream far4File(std::string(FILES_DIR)+"/container/bigfart69",std::ios::binary|std::ios::in);

		LBP::Fart farc(farcFile);
		LBP::Fart far4(far4File);
		
		std::stringstream farcSource;
		std::stringstream far4Source;
		std::stringstream farcResult;
		std::stringstream far4Result;

		farcFile.clear();
		far4File.clear();
		farcFile.seekg(0,std::ios::beg);
		far4File.seekg(0,std::ios::beg);
		farcSource << farcFile.rdbuf();
		far4Source << far4File.rdbuf();

		farc.write(farcResult);
		far4.write(far4Result);

		farcFile.close();
		far4File.close();

		EXPECT_EQ(farcSource.str(),farcResult.str());
		EXPECT_EQ(far4Source.str(),far4Result.str());

	});

}

TEST(Map,ParsesValidMAP){

	//LBP::Map parses valid MAP file properly and without errors

	EXPECT_NO_THROW({

		std::fstream mapFile(std::string(FILES_DIR)+"/container/intro.map",std::ios::binary|std::ios::in);
		LBP::Map map(mapFile);

		EXPECT_EQ(map.getNumEntries(),3);

		mapFile.close();

	});

}

TEST(Map,WritesDuplicateMAP){

	//LBP::Map outputs identical files

	EXPECT_NO_THROW({

		std::fstream mapFile(std::string(FILES_DIR)+"/container/intro.map",std::ios::binary|std::ios::in);

		LBP::Map map(mapFile);

		std::stringstream mapSource;
		std::stringstream mapResult;

		mapFile.clear();
		mapFile.seekg(0,std::ios::beg);
		mapSource << mapFile.rdbuf();

		map.write(mapResult);

		mapFile.close();

		EXPECT_EQ(mapSource.str(),mapResult.str());

	});

}

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

		std::fstream planFile(std::string(FILES_DIR)+"/things/palette_primitives_372.plan",std::ios::binary|std::ios::in);
		LBP::StandardResource plan(planFile);

		EXPECT_EQ(plan.getRevision(),0x222);
		EXPECT_EQ(plan.getDependencyCount(),5);

		planFile.close();

	});

}

TEST(StandardResource,ParsesValidLevel){

	//LBP::StandardResource parses valid level file properly and without errors

	EXPECT_NO_THROW({

		std::fstream lvlFile(std::string(FILES_DIR)+"/things/intro_level.bin",std::ios::binary|std::ios::in);
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

		std::fstream musFile(std::string(FILES_DIR)+"/text/i_english_g.mus",std::ios::binary|std::ios::in);
		LBP::StandardResource mus(musFile);

		musFile.close();

	});

}