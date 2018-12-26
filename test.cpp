#include "gtest/gtest.h"
#include "psr.h"

using namespace std;

TEST(PsrTestCases, SanityTest) {
	CPSR cpsr;
	vector<PairedPlayerRating> team1;
	vector<PairedPlayerRating> team2;

	//sample from http://dota.eurobattle.net/la/forum/index.php?action=gameinfo;sa=game;gid=5917726

	team1.push_back(make_pair("hempshie.", 1759-0));		
	team1.push_back(make_pair("djez^ba^posho", 1658-8));
	team1.push_back(make_pair("pro^.blema", 1585-13));		
	team1.push_back(make_pair("feedfear", 1582-13));		
	team1.push_back(make_pair("loombardpl", 1510-18));		

	team2.push_back(make_pair("jing_jang", 1600+9));		
	team2.push_back(make_pair("vulepasulj", 1786+0));		
	team2.push_back(make_pair("vasaubica", 1528+13));		
	team2.push_back(make_pair("tedebe", 1530+13));			
	team2.push_back(make_pair("jaqenh^ghar", 1559+12));	

	cpsr.CalculatePSR(team1, team2);

	cout << "Team 1 gain/lose" << endl;
	for (int i = 0; i < team1.size(); i++) {
		cout << cpsr.getTeam1gainLose()[i].first << " / " << cpsr.getTeam1gainLose()[i].second << endl;
	}

	cout << "Team 2 gain/lose" << endl;
	for (int i = 0; i < team2.size(); i++) {
		cout << cpsr.getTeam2gainLose()[i].first << " / " << cpsr.getTeam2gainLose()[i].second << endl;
	}

	EXPECT_EQ(1, 1);
}

int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	cin.get();
}