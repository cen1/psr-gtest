#include "gtest/gtest.h"
#include "psr.h"
#include "TestSample.h"
#include <iostream>
#include <fstream>

using namespace std;

TestSamples testSamples;

//Test cases

void test_CompareOldAndNewPSR(int sampleNum) {

	streambuf* oldCoutStreamBuf = cout.rdbuf();

	std::ofstream out("test_CompareOldAndNewPSR_Sample_"+std::to_string(sampleNum)+".txt");
	std::streambuf *coutbuf = std::cout.rdbuf();
	std::cout.rdbuf(out.rdbuf());

	CPSR cpsr;

	cout << "Sample Nr. " << sampleNum << endl << endl;

	cout << testSamples.getSampleDescription(sampleNum) << endl << endl;

	//Old

	cpsr = testSamples.generateCPSRFromSample(sampleNum);

	pair<vector<PairedPlayerRating>, vector<PairedPlayerRating>> teams_0 = testSamples.GetTeamsWithRatingFromCPSR(cpsr);
	vector<PairedPlayerRating> team1_0 = teams_0.first;
	vector<PairedPlayerRating> team2_0 = teams_0.second;

	cpsr.CalculatePSR(team1_0, team2_0);

	cout << "<Old>" << endl;

	testSamples.displayWinLosePercOld(cpsr);

	testSamples.displayTeamsAndPSRFromCPSR(cpsr);

	cout << endl;
	
	//New Balance & New PSR calculation based on game average PSR

	cpsr = testSamples.generateCPSRFromSample(sampleNum);

	cout << "<New>" << std::endl;

	pair<vector<PairedPlayerRating>, vector<PairedPlayerRating>> teams_1 = testSamples.GetTeamsWithRatingFromCPSR(cpsr);
	vector<PairedPlayerRating> team1_1 = teams_1.first;
	vector<PairedPlayerRating> team2_1 = teams_1.second;

	cpsr.CalculatePSR_New(team1_1, team2_1);

	testSamples.displayWinLosePercOld(cpsr);

	testSamples.displayTeamsAndPSRFromCPSR(cpsr);

	cout.rdbuf(oldCoutStreamBuf);

}

//Samples

void createPracticalSamples() {
	pair<string, CDBDiv1DPS*> p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, averagePlayer, newPlayer, newStrongPlayer,
		badPlayer, badHighPSRPlayer, goodHighPSRPlayer, badLowPSRPlayer, goodLowPSRPlayer, veryStrongPlayer;
	vector<pair<string, CDBDiv1DPS*>> players;

	string description;

	//Sample 0
	
	description = "Game: http://dota.eurobattle.net/la/forum/index.php?action=gameinfo;sa=game;gid=6172527";

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

	testSamples.addSample(players, description);

	//Sample 1
	
	description = "Game: http://dota.eurobattle.net/la/forum/index.php?action=gameinfo;sa=game;gid=6175124, 1600+ high level game";

	//Sentinel team
	p1 = make_pair("c-hosen", new CDBDiv1DPS(1842 + 1, 207, 139, 51, 2838, 821, 2268, 45621, 2699, 12191, 382, 212));
	p2 = make_pair("edison", new CDBDiv1DPS(1750 + 4, 2025, 120, 92, 1619, 1293, 2417, 42420, 1096, 16917, 251, 176));
	p3 = make_pair("wardenhidua", new CDBDiv1DPS(1600 + 9, 366, 12, 6, 174, 121, 239, 2030, 60, 307, 11, 5));
	p4 = make_pair("noone-", new CDBDiv1DPS(1622 + 7, 51, 29, 20, 416, 392, 513, 9701, 123, 3317, 40, 23));
	p5 = make_pair("dr.h", new CDBDiv1DPS(1680 + 4, 634, 70, 53, 1240, 777, 1587, 22956, 1277, 4370, 135, 61));

	//Scourge team
	p6 = make_pair("meksikano", new CDBDiv1DPS(1786 - 5, 129, 80, 38, 1560, 711, 1321, 24979, 931, 5186, 246, 102));
	p7 = make_pair("bloodthorn", new CDBDiv1DPS(1753 - 5, 682, 90, 51, 1629, 783, 1629, 25953, 992, 7576, 159, 82));
	p8 = make_pair("7654321", new CDBDiv1DPS(1662 - 7, 32, 22, 8, 301, 152, 389, 3828, 133, 805, 37, 14));
	p9 = make_pair("-stopfeed-", new CDBDiv1DPS(1749 - 5, 66, 49, 12, 804, 353, 586, 15689, 194, 3256, 199, 82));
	p10 = make_pair("dontafkpls", new CDBDiv1DPS(1666 - 6, 54, 29, 19, 496, 283, 558, 8395, 110, 1881, 63, 36));

	players = { p1, p2, p3, p4, p5, p6, p7, p8, p9, p10 };

	testSamples.addSample(players, description);

	//Sample 2
	
	description = "Game: http://dota.eurobattle.net/la/forum/index.php?action=gameinfo;sa=game;gid=6175264, random game with most 1600+";
	
	//Sentinel team
	p1 = make_pair("marian_alecs", new CDBDiv1DPS(1674 + 5, 1033, 69, 57, 1070, 999, 1586, 15234, 436, 3875, 90, 48));
	p2 = make_pair("zmrd3c3k", new CDBDiv1DPS(1678 + 5, 932, 101, 112, 1401, 1811, 2366, 31477, 411, 6107, 234, 109));
	p3 = make_pair("dr.h", new CDBDiv1DPS(1694 + 4, 637, 71, 55, 1279, 794, 1609, 23488, 1321, 4489, 140, 64));
	p4 = make_pair("deadmanwalking", new CDBDiv1DPS(1632 + 8, 1359, 51, 47, 523, 489, 1166, 11327, 158, 2252, 72, 40));
	p5 = make_pair("topol-m", new CDBDiv1DPS(1417 + 21, 652, 29, 36, 313, 569, 506, 9663, 37, 2060, 99, 41));

	//Scourge team
	p6 = make_pair("fatandviolent", new CDBDiv1DPS(1678 - 6, 1998, 184, 214, 2705, 3100, 5132, 48148, 1558, 12047, 243, 125));
	p7 = make_pair("splasena.koza", new CDBDiv1DPS(1665 - 7, 269, 55, 40, 833, 592, 1343, 15273, 785, 4220, 110, 55));
	p8 = make_pair("kraghack", new CDBDiv1DPS(1565 - 15, 526, 25, 22, 330, 202, 431, 7558, 112, 2273, 45, 32));
	p9 = make_pair("7654321", new CDBDiv1DPS(1665 - 7, 33, 23, 8, 305, 156, 405, 4058, 143, 872, 38, 14));
	p10 = make_pair("crazy_drummer", new CDBDiv1DPS(1613 - 11, 28, 18, 8, 286, 180, 285, 5494, 231, 1700, 31, 20));

	players = { p1, p2, p3, p4, p5, p6, p7, p8, p9, p10 };

	testSamples.addSample(players, description);

	//Sample 3
	
	description = "Game: http://dota.eurobattle.net/la/forum/index.php?action=gameinfo;sa=game;gid=6175463), Sentinel dominance";

	//Sentinel team
	p1 = make_pair("sfa.geck0", new CDBDiv1DPS(1551 - 13, 19, 6, 3, 118, 50, 101, 1666, 99, 322, 15, 5));
	p2 = make_pair(".kajo.", new CDBDiv1DPS(1570 - 12, 121, 46, 44, 830, 839, 982, 9644, 102, 3459, 60, 37));
	p3 = make_pair(".set", new CDBDiv1DPS(1722 - 1, 1137, 67, 28, 1192, 770, 1428, 11800, 376, 1511, 90, 58));
	p4 = make_pair("izvicnik", new CDBDiv1DPS(1439 - 19, 120, 18, 24, 197, 381, 443, 3220, 63, 1303, 17, 6));
	p5 = make_pair("takeiteazy", new CDBDiv1DPS(1662 - 5, 1062, 51, 42, 798, 605, 1167, 12262, 550, 4298, 78, 44));

	//Scourge team
	p6 = make_pair("souvlaki", new CDBDiv1DPS(1507 + 13, 253, 104, 124, 1592, 1848, 2894, 34900, 762, 9846, 210, 105));
	p7 = make_pair("sup-reme", new CDBDiv1DPS(1658 + 3, 77, 36, 32, 532, 440, 687, 11370, 305, 3206, 59, 31));
	p8 = make_pair("_tiganj_", new CDBDiv1DPS(1500 + 14, 291, 2, 1, 11, 32, 26, 637, 0, 184, 7, 2));
	p9 = make_pair("nem3sis", new CDBDiv1DPS(1502 + 14, 3, 1, 2, 6, 16, 13, 208, 6, 68, 3, 0));
	p10 = make_pair("fatandviolent", new CDBDiv1DPS(1675 + 2, 2000, 184, 216, 2717, 3119, 5144, 48256, 1561, 12068, 243, 125));

	players = { p1, p2, p3, p4, p5, p6, p7, p8, p9, p10 };

	testSamples.addSample(players, description);
}

void createTheoreticalSamples() {
	pair<string, CDBDiv1DPS*> p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, averagePlayer, newPlayer, newStrongPlayer, 
		badPlayer, averagePlayer1, badHighPSRPlayer, goodHighPSRPlayer, badLowPSRPlayer, goodLowPSRPlayer, veryStrongPlayer;
	vector<pair<string, CDBDiv1DPS*>> players;
	double lowPSR, averagePSR, highPSR;

	string description;

	//Rating, TotalGames, TotalWins, TotalLoses, TotalKills, TotalDeaths, TotalAssists, TotalCreepKills, TotalDenies, 
	//TotalNeutrals, TowerKills, RaxKills
	
	//Preset players

	lowPSR = 1400;
	averagePSR = 1600;
	highPSR = 1800;

	newPlayer = make_pair("New Player", new CDBDiv1DPS(1500, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
	newStrongPlayer = make_pair("New Strong Player", new CDBDiv1DPS(1525, 1, 1, 0, 16, 4, 10, 200, 10, 30, 1, 0.5));
	badPlayer = make_pair("Bad Player", new CDBDiv1DPS(1550, 300, 130, 170, 300*5.5, 300*6.8, 300*9.8, 300*90, 300*2, 300*40, 300*0.3, 300*0.2));
	
	averagePlayer = make_pair("Average Player", new CDBDiv1DPS(averagePSR, 500, 250, 250, 5000, 5000, 7500, 50000, 2000, 20000, 250, 150));
	badLowPSRPlayer = make_pair("Bad Low PSR Player", new CDBDiv1DPS(lowPSR, 300, 130, 170, 300 * 5.5, 300 * 6.8, 300 * 9.8, 300 * 90, 300 * 2, 300 * 40, 300 * 0.3, 300 * 0.2));
	badHighPSRPlayer = make_pair("Bad High PSR Player", new CDBDiv1DPS(highPSR, 2500, 1200, 1300, 2500*6, 2500*8, 2500*15, 2500*100, 2500*3, 2500*50, 2500*0.5, 2500*0.2));
	goodLowPSRPlayer = make_pair("Strong High PSR Player", new CDBDiv1DPS(lowPSR, 500, 400, 100, 5000, 2500, 7500, 50000, 2500, 25000, 250, 150));
	goodHighPSRPlayer = make_pair("Strong High PSR Player", new CDBDiv1DPS(highPSR, 500, 400, 100, 5000, 2500, 7500, 50000, 2500, 25000, 250, 150));
	averagePlayer1 = make_pair("Average Player 2", new CDBDiv1DPS(0, 1503, 131, 56, 62, 5.5, 6.6, 11.7, 90.8, 0.4, 23.3, 0.4, 0.3));

	//Sample 0

	description = "Common situation: strong high psr player + strong low psr player + high psr noob + averages";
	
	//Sentinel team
	p1 = badHighPSRPlayer;
	p2 = averagePlayer;
	p3 = averagePlayer;
	p4 = averagePlayer;
	p5 = averagePlayer;

	//Scourge team
	p6 = badPlayer;
	p7 = newStrongPlayer;
	p8 = averagePlayer;
	p9 = goodHighPSRPlayer;
	p10 = goodLowPSRPlayer;

	players = { p1, p2, p3, p4, p5, p6, p7, p8, p9, p10 };

	testSamples.addSample(players, description);

	//Sample 1

	description = "Good High PSR player vs Bad High PSR player + averages";

	//Sentinel team
	p1 = badHighPSRPlayer;
	p2 = averagePlayer;
	p3 = averagePlayer;
	p4 = averagePlayer;
	p5 = averagePlayer;

	//Scourge team
	p6 = averagePlayer;
	p7 = averagePlayer;
	p8 = averagePlayer;
	p9 = goodHighPSRPlayer;
	p10 = averagePlayer;

	players = { p1, p2, p3, p4, p5, p6, p7, p8, p9, p10 };

	testSamples.addSample(players, description);

	//Sample 2

	description = "PSR test";

	pair<string, CDBDiv1DPS*> a1 = make_pair("New Player", new CDBDiv1DPS(1600, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));

	pair<string, CDBDiv1DPS*> a2 = make_pair("New Player", new CDBDiv1DPS(1500, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));

	pair<string, CDBDiv1DPS*> a3 = make_pair("New Player", new CDBDiv1DPS(1550, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));

	//Sentinel team
	p1 = a3;
	p2 = a2;
	p3 = a1;
	p4 = a2;
	p5 = a1;

	//Scourge team
	p6 = a3;
	p7 = a2;
	p8 = a2;
	p9 = a1;
	p10 = a1;

	players = { p1, p2, p3, p4, p5, p6, p7, p8, p9, p10 };

	testSamples.addSample(players, description);
}

int main(int argc, char** argv) {
	//Generate test samples from preset data

	//createTheoreticalSamples();
	createPracticalSamples();

	//Execute the test on all samples and print each to its own file
	for (int i = 0; i < testSamples.size(); i++) {
		test_CompareOldAndNewPSR(i);
	}
}
