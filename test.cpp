#include "gtest/gtest.h"
#include "psr.h"
#include "TestSample.h"

using namespace std;

TestSamples testSamples;

//Test cases

TEST(CompareOldAndNewBalance, SanityTest) {
	return;
	int sampleNum = 1;

	CPSR cpsr = testSamples.generateCPSRFromSample(sampleNum);

	testSamples.displayTeamsFromCPSRPreBalance(cpsr);

	cpsr.balance();

	cout << "<CPSR.balance()>" << std::endl;
	testSamples.displayTeamsFromCPSRPostBalance(cpsr);

	cpsr = testSamples.generateCPSRFromSample(sampleNum);

	cpsr.balanceNew();

	cout << "<CPSR.balanceNew()>" << std::endl;
	testSamples.displayTeamsFromCPSRPostBalance(cpsr);
}

TEST(CompareOldBalanceAndPSRvsNewBalanceAndPSR, SanityTest) {
	int sampleNum = 1;

	//Old Balance & Old PSR calculation based on team average PSR

	CPSR cpsr = testSamples.generateCPSRFromSample(sampleNum);

	testSamples.displayTeamsFromCPSRPreBalance(cpsr);

	cpsr.balance();

	cout << "<CPSR.balance()>" << std::endl;
	testSamples.displayTeamsFromCPSRPostBalance(cpsr);

	pair<vector<PairedPlayerRating>, vector<PairedPlayerRating>> teams = testSamples.GetTeamsWithRatingFromCPSR(cpsr);
	vector<PairedPlayerRating> team1 = teams.first;
	vector<PairedPlayerRating> team2 = teams.second;

	cpsr.CalculatePSR(team1, team2);

	cout << "<CPSR.CalculatePSR(team1, team2)>" << endl;

	testSamples.displayWinLosePercOld(cpsr);

	testSamples.displayTeamsGainLoseFromCPSR(cpsr);

	cout << endl;

	//New Balance & New PSR calculation based on game average PSR

	cpsr = testSamples.generateCPSRFromSample(sampleNum);

	cpsr.balanceNew();

	cout << "<CPSR.balanceNew()>" << std::endl;
	testSamples.displayTeamsFromCPSRPostBalance(cpsr);

	pair<vector<TupledPlayerValue>, vector<TupledPlayerValue>> teamsNew = testSamples.GetTeamsWithBalanceValueFromCPSR(cpsr);
	vector<TupledPlayerValue> team1New = teamsNew.first;
	vector<TupledPlayerValue> team2New = teamsNew.second;

	cpsr.CalculatePSRNew(team1New, team2New);

	cout << endl;
	cout << "<CPSR.CalculatPSRNew(team1New, team1New)>" << endl;

	testSamples.displayWinLosePercNew(cpsr);

	testSamples.displayTeamsGainLoseFromCPSR(cpsr);
}

TEST(OldBalancePsrTestCases, SanityTest) {
	return;
	cout << "<Old Balance>" << endl;
	CPSR cpsr = testSamples.generateCPSRFromSample(1);

	testSamples.displayTeamsFromCPSRPreBalance(cpsr);
	cpsr.balance();
	testSamples.displayTeamsFromCPSRPostBalance(cpsr);

	pair<vector<PairedPlayerRating>, vector<PairedPlayerRating>> teams = testSamples.GetTeamsWithRatingFromCPSR(cpsr);
	vector<PairedPlayerRating> team1 = teams.first;
	vector<PairedPlayerRating> team2 = teams.second;

	cpsr.CalculatePSR(team1, team2);

	cout << "<CPSR.CalculatePSR(team1, team2)>" << endl;

	testSamples.displayWinLosePercOld(cpsr);

	testSamples.displayTeamsGainLoseFromCPSR(cpsr);

	cout << endl;

	pair<vector<TupledPlayerValue>, vector<TupledPlayerValue>> teamsNew = testSamples.GetTeamsWithBalanceValueFromCPSR(cpsr);
	vector<TupledPlayerValue> team1New = teamsNew.first;
	vector<TupledPlayerValue> team2New = teamsNew.second;

	cpsr.CalculatePSRNew(team1New, team2New);

	cout << endl;
	cout << "<CPSR.CalculatPSRNew(team1New, team1New)>" << endl;

	testSamples.displayWinLosePercNew(cpsr);

	testSamples.displayTeamsGainLoseFromCPSR(cpsr);

	EXPECT_EQ(1, 1);
}

TEST(NewBalancePsrTestCases, SanityTest) {
	return;
	cout << "<New Balance>" << endl;
	CPSR cpsr = testSamples.generateCPSRFromSample(1);

	testSamples.displayTeamsFromCPSRPreBalance(cpsr);
	cpsr.balanceNew();
	testSamples.displayTeamsFromCPSRPostBalance(cpsr);

	pair<vector<PairedPlayerRating>, vector<PairedPlayerRating>> teams = testSamples.GetTeamsWithRatingFromCPSR(cpsr);
	vector<PairedPlayerRating> team1 = teams.first;
	vector<PairedPlayerRating> team2 = teams.second;

	cpsr.CalculatePSR(team1, team2);

	cout << "<CPSR.CalculatePSR(team1, team2)>" << endl;

	testSamples.displayWinLosePercOld(cpsr);

	testSamples.displayTeamsGainLoseFromCPSR(cpsr);

	pair<vector<TupledPlayerValue>, vector<TupledPlayerValue>> teamsNew = testSamples.GetTeamsWithBalanceValueFromCPSR(cpsr);
	vector<TupledPlayerValue> team1New = teamsNew.first;
	vector<TupledPlayerValue> team2New = teamsNew.second;

	cpsr.CalculatePSRNew(team1New, team2New);

	cout << endl;
	cout << "<CPSR.CalculatPSRNew(team1New, team1New)>" << endl;

	testSamples.displayWinLosePercNew(cpsr);

	testSamples.displayTeamsGainLoseFromCPSR(cpsr);

	EXPECT_EQ(1, 1);
}

//Samples

void createSamples() {
	pair<string, CDBDiv1DPS*> p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, averagePlayer, newPlayer, newStrongPlayer, 
		badPlayer, badHighPlayer, strongHighPlayer, badLowPlayer, veryStrongPlayer;
	vector<pair<string, CDBDiv1DPS*>> players;

	//Rating, TotalGames, TotalWins, TotalLoses, TotalKills, TotalDeaths, TotalAssists, TotalCreepKills, TotalDenies, 
	//TotalNeutrals, TowerKills, RaxKills
	
	//Preset players
	averagePlayer = make_pair("Average Player", new CDBDiv1DPS(1550, 500, 250, 250, 5000, 5000, 7500, 50000, 2000, 20000, 250, 150));
	newPlayer = make_pair("New Player", new CDBDiv1DPS(1500, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
	newStrongPlayer = make_pair("New Strong Player", new CDBDiv1DPS(1525, 1, 1, 0, 16, 4, 10, 200, 10, 30, 1, 0.5));
	badPlayer = make_pair("Bad Player", new CDBDiv1DPS(1550, 300, 130, 170, 300*5.5, 300*6.8, 300*9.8, 300*90, 300*2, 300*40, 300*0.3, 300*0.2));
	badLowPlayer = make_pair("Bad Low PSR Player", new CDBDiv1DPS(1450, 300, 130, 170, 300 * 5.5, 300 * 6.8, 300 * 9.8, 300 * 90, 300 * 2, 300 * 40, 300 * 0.3, 300 * 0.2));
	badHighPlayer = make_pair("Bad High PSR Player", new CDBDiv1DPS(1700, 2500, 1200, 1300, 2500*6, 2500*8, 2500*15, 2500*100, 2500*3, 2500*50, 2500*0.5, 2500*0.2));
	strongHighPlayer = make_pair("Strong High PSR Player", new CDBDiv1DPS(1800, 500, 400, 100, 5000, 2500, 7500, 50000, 2500, 25000, 250, 150));



	//Sample 0
	//Sentinel team
	p1 = make_pair("smokin-^", new CDBDiv1DPS(1659 - 5, 43, 25, 13, 205, 271, 364, 5384, 186, 1649, 38, 18));
	p2 = make_pair("serdarr_m", new CDBDiv1DPS(1536 - 16, 1037, 130, 147, 1676, 2396, 2877, 58679, 220, 20993, 132, 48));
	p3 = make_pair("s.t.a.r.s", new CDBDiv1DPS(1533 - 16, 15, 7, 5, 158, 62, 140, 2238, 106, 494, 10, 6));
	p4 = make_pair("hypopo", new CDBDiv1DPS(1722 - 1, 125, 74, 44, 1001, 1025, 1578, 22700, 1393, 5791, 202, 105));
	p5 = make_pair("mirat", new CDBDiv1DPS(1448 + 17, 45, 19, 21, 218, 352, 328, 6626, 126, 761, 51, 21));

	//Scourge team
	p6 = make_pair("pizon_fcrs", new CDBDiv1DPS(1460 + 18, 141, 50, 60, 763, 952, 1375, 11807, 1002, 3459, 64, 19));
	p7 = make_pair("echo", new CDBDiv1DPS(1692 + 1, 1249, 62, 53, 702, 735, 1663, 14579, 555, 4515, 79, 51));
	p8 = make_pair("god..of..war", new CDBDiv1DPS(1700 + 1, 1588, 59, 43, 575, 637, 1355, 10925, 535, 620, 51, 28));
	p9 = make_pair("ivosanader", new CDBDiv1DPS(1517 + 14, 3, 2, 1, 23, 19, 40, 336, 10, 48, 5, 1));
	p10 = make_pair("ss-ballantines", new CDBDiv1DPS(1469 + 17, 2, 0, 1, 1, 12, 6, 228, 10, 44, 0, 0));

	players = { p1, p2, p3, p4, p5, p6, p7, p8, p9, p10 };

	testSamples.addSample(players);

	//Sample 1 (Common situation: strong high psr player + strong low psr player + high psr noob + averages)
	//Sentinel team
	p1 = badLowPlayer;
	p2 = averagePlayer;
	p3 = averagePlayer;
	p4 = averagePlayer;
	p5 = averagePlayer;

	//Scourge team
	p6 = badPlayer;
	p7 = newStrongPlayer;
	p8 = badHighPlayer;
	p9 = strongHighPlayer;
	p10 = strongHighPlayer;

	players = { p1, p2, p3, p4, p5, p6, p7, p8, p9, p10 };

	testSamples.addSample(players);
}

int main(int argc, char** argv) {
	createSamples();
	testing::InitGoogleTest(&argc, argv);
	::testing::GTEST_FLAG(filter) = "CompareOldBalanceAndPSRvsNewBalanceAndPSR*";
	RUN_ALL_TESTS();
	cin.get();
}
