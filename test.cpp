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

TEST(BalanceNeutralTestCase, SanityTest) {
	CPSR cpsr1;

	//Define players
	//Sentinel team 0, 1-5
	CDIV1DotAPlayer p1(1600, "p1", 1, &cpsr1);
	CDIV1DotAPlayer p2(1600, "p2", 2, &cpsr1);
	CDIV1DotAPlayer p3(1600, "p3", 3, &cpsr1);
	CDIV1DotAPlayer p4(1600, "p4", 4, &cpsr1);
	CDIV1DotAPlayer p5(1600, "p5", 5, &cpsr1);

	//Scourge team 1, 7-11
	CDIV1DotAPlayer p6(1600, "p6", 7, &cpsr1);
	CDIV1DotAPlayer p7(1600, "p7", 8, &cpsr1);
	CDIV1DotAPlayer p8(1600, "p8", 9, &cpsr1);
	CDIV1DotAPlayer p9(1600, "p9", 10, &cpsr1);
	CDIV1DotAPlayer p10(1600, "p10", 11, &cpsr1);

	cpsr1.m_DotAPlayers.push_back(&p1);
	cpsr1.m_DotAPlayers.push_back(&p2);
	cpsr1.m_DotAPlayers.push_back(&p3);
	cpsr1.m_DotAPlayers.push_back(&p4);
	cpsr1.m_DotAPlayers.push_back(&p5);
	cpsr1.m_DotAPlayers.push_back(&p6);
	cpsr1.m_DotAPlayers.push_back(&p7);
	cpsr1.m_DotAPlayers.push_back(&p8);
	cpsr1.m_DotAPlayers.push_back(&p9);
	cpsr1.m_DotAPlayers.push_back(&p10);

	//Define slots
	CGameSlot s1(0, 1);
	CGameSlot s2(0, 2);
	CGameSlot s3(0, 3);
	CGameSlot s4(0, 4);
	CGameSlot s5(0, 5);

	CGameSlot s6(1, 7);
	CGameSlot s7(1, 8);
	CGameSlot s8(1, 9);
	CGameSlot s9(1, 10);
	CGameSlot s10(1, 11);

	cpsr1.m_Slots.push_back(s1);
	cpsr1.m_Slots.push_back(s2);
	cpsr1.m_Slots.push_back(s3);
	cpsr1.m_Slots.push_back(s4);
	cpsr1.m_Slots.push_back(s5);
	cpsr1.m_Slots.push_back(s6);
	cpsr1.m_Slots.push_back(s7);
	cpsr1.m_Slots.push_back(s8);
	cpsr1.m_Slots.push_back(s9);
	cpsr1.m_Slots.push_back(s10);

	cpsr1.balance();

	EXPECT_EQ(1, 1);
}

TEST(BalanceTestCase, SanityTest) {
	CPSR cpsr1;

	//Define players
	//Sentinel team 0, 1-5
	CDIV1DotAPlayer p1(1600, "p1", 1, &cpsr1);
	CDIV1DotAPlayer p2(1610, "p2", 2, &cpsr1);
	CDIV1DotAPlayer p3(1620, "p3", 3, &cpsr1);
	CDIV1DotAPlayer p4(1630, "p4", 4, &cpsr1);
	CDIV1DotAPlayer p5(1640, "p5", 5, &cpsr1);

	//Scourge team 1, 7-11
	CDIV1DotAPlayer p6(1650, "p6", 7, &cpsr1);
	CDIV1DotAPlayer p7(1660, "p7", 8, &cpsr1);
	CDIV1DotAPlayer p8(1670, "p8", 9, &cpsr1);
	CDIV1DotAPlayer p9(1680, "p9", 10, &cpsr1);
	CDIV1DotAPlayer p10(1690, "p10", 11, &cpsr1);

	cpsr1.m_DotAPlayers.push_back(&p1);
	cpsr1.m_DotAPlayers.push_back(&p2);
	cpsr1.m_DotAPlayers.push_back(&p3);
	cpsr1.m_DotAPlayers.push_back(&p4);
	cpsr1.m_DotAPlayers.push_back(&p5);
	cpsr1.m_DotAPlayers.push_back(&p6);
	cpsr1.m_DotAPlayers.push_back(&p7);
	cpsr1.m_DotAPlayers.push_back(&p8);
	cpsr1.m_DotAPlayers.push_back(&p9);
	cpsr1.m_DotAPlayers.push_back(&p10);

	//Define slots
	CGameSlot s1(0, 1);
	CGameSlot s2(0, 2);
	CGameSlot s3(0, 3);
	CGameSlot s4(0, 4);
	CGameSlot s5(0, 5);

	CGameSlot s6(1, 7);
	CGameSlot s7(1, 8);
	CGameSlot s8(1, 9);
	CGameSlot s9(1, 10);
	CGameSlot s10(1, 11);

	cpsr1.m_Slots.push_back(s1);
	cpsr1.m_Slots.push_back(s2);
	cpsr1.m_Slots.push_back(s3);
	cpsr1.m_Slots.push_back(s4);
	cpsr1.m_Slots.push_back(s5);
	cpsr1.m_Slots.push_back(s6);
	cpsr1.m_Slots.push_back(s7);
	cpsr1.m_Slots.push_back(s8);
	cpsr1.m_Slots.push_back(s9);
	cpsr1.m_Slots.push_back(s10);

	cpsr1.balance();

	EXPECT_EQ(1, 1);
}

int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	cin.get();
}