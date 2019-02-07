
#ifndef PSR_H
#define PSR_H

#include <vector>
#include <list>
#include <utility>
#include <string>
#include <algorithm>

using std::pair;
using std::tuple;
using std::string;
using std::vector;
using std::list;

typedef pair<string,double> PairedPlayerRating;
typedef tuple<string, double, double> TupledPlayerValue;

class CGameSlot;
class CBalanceSlot;
class CDIV1DotAPlayer;
class CDBDiv1DPS;

//
// CPSR
//

class CPSR
{
private:
	double m_BaseKFactor;
	double m_GammaCurveK;
	double m_GammaCurveRange;
	double m_GammaCurveTheta;
	double m_KFactorScale;
	double m_LogisticPredictionScale;
	double m_MaxKFactor;
	double m_MedianScalingRank;
	double m_MinKFactor;
	double m_TeamRankWeighting;

	double m_ConstWinGain;
	double m_ConstLoseGain;

	// if this is true we need to recalculate properties below before using them because they are not accurate anymore
	bool m_NeedRecalculate;

	vector<PairedPlayerRating> m_team1;
	vector<PairedPlayerRating> m_team2;

	double m_team1avgpsr;
	double m_team2avgpsr;
	double m_team1AverageValue;
	double m_team2AverageValue;
	double m_team1winPerc;
	double m_team2winPerc;
	vector<double> m_team1kfactors;
	vector<double> m_team2kfactors;
	vector< pair<double, double> > m_team1gainLose;
	vector< pair<double, double> > m_team2gainLose;

	

public:
	CPSR( );
	~CPSR( );

	vector<CGameSlot> m_Slots;
	list<CDIV1DotAPlayer *> m_DotAPlayers;

	void SetBaseKFactor( double nBaseKFactor )					{ m_BaseKFactor = nBaseKFactor; }

	double GetTeamAvgPSR( uint32_t team );
	double GetTeamAvgValue(uint32_t team);
	double GetTeamWinPerc( uint32_t team );
	double GetPlayerKFactor( string player );
	pair<double, double> GetPlayerGainLoss( string player );
	
	void balance();
	void BalanceSlots2();
	unsigned int BalanceSlots2Recursive(list<CBalanceSlot>& slots_to_balance);
	void SwapSlots(unsigned char SID1, unsigned char SID2);
	unsigned char GetSIDFromPID(unsigned char PID);
	string UTIL_ToString(double d, int digits);

	void CalculatePSR( vector<PairedPlayerRating> team1, vector<PairedPlayerRating> team2 );
	void CalculatePSR_New(vector<PairedPlayerRating> team1, vector<PairedPlayerRating> team2);

	vector< pair<double, double> > getTeam1gainLose();
	vector< pair<double, double> > getTeam2gainLose();
};

class CGameSlot
{
private:
	unsigned char m_PID;				// player id
	unsigned char m_SlotStatus;			// slot status (0 = open, 1 = closed, 2 = occupied)
	unsigned char m_Team;				// team

public:
	unsigned char m_Colour;				// colour

	CGameSlot(unsigned char nTeam, unsigned char nColour) {
		m_PID = nColour;
		m_SlotStatus = 0;
		m_Team = nTeam;
		m_Colour = nColour;
	}
	CGameSlot(unsigned char p, unsigned char nTeam, unsigned char nColour) {
		m_PID = p;
		m_SlotStatus = 0;
		m_Team = nTeam;
		m_Colour = nColour;
	}
	~CGameSlot() {};

	unsigned char GetPID() { return m_PID; }
	unsigned char GetSlotStatus() { return m_SlotStatus; }
	unsigned char GetTeam() { return m_Team; }
	unsigned char GetColour() { return m_Colour; }

	void SetPID(unsigned char nPID) { m_PID = nPID; }
	void SetSlotStatus(unsigned char nSlotStatus) { m_SlotStatus = nSlotStatus; }
	void SetTeam(unsigned char nTeam) { m_Team = nTeam; }
	void SetColour(unsigned char nColour) { m_Colour = nColour; }
};

class CDBDiv1DPS
{
private:
	double m_Rating;
	double m_HighestRating;
	uint32_t m_TotalGames;			// total number of dota games played
	uint32_t m_TotalWins;			// total number of dota games won
	uint32_t m_TotalLosses;			// total number of dota games lost
	uint32_t m_TotalKills;			// total number of hero kills
	uint32_t m_TotalDeaths;			// total number of deaths
	uint32_t m_TotalCreepKills;		// total number of creep kills
	uint32_t m_TotalCreepDenies;	// total number of creep denies
	uint32_t m_TotalAssists;		// total number of assists
	uint32_t m_TotalNeutralKills;	// total number of neutral kills
	uint32_t m_TotalTowerKills;	// total number of tower kills
	uint32_t m_TotalRaxKills;		// total number of rax kills
	//uint32_t m_TotalCourierKills;	// total number of courier kills

public:
	CDBDiv1DPS(double nRating, double nHighestRating, uint32_t nTotalGames, uint32_t nTotalWins, uint32_t nTotalLosses, uint32_t nTotalKills, uint32_t nTotalDeaths, uint32_t nTotalCreepKills, uint32_t nTotalCreepDenies, uint32_t nTotalAssists, uint32_t nTotalNeutralKills)
		: m_Rating(nRating), m_HighestRating(nHighestRating), m_TotalGames(nTotalGames), m_TotalWins(nTotalWins), m_TotalLosses(nTotalLosses), m_TotalKills(nTotalKills), m_TotalDeaths(nTotalDeaths), m_TotalCreepKills(nTotalCreepKills), m_TotalCreepDenies(nTotalCreepDenies), m_TotalAssists(nTotalAssists), m_TotalNeutralKills(nTotalNeutralKills) { }
	CDBDiv1DPS(double nRating, uint32_t nTotalGames, uint32_t nTotalWins, uint32_t nTotalLosses, uint32_t nTotalKills, uint32_t nTotalDeaths, uint32_t nTotalAssists, uint32_t nTotalCreepKills, uint32_t nTotalCreepDenies, uint32_t nTotalNeutralKills, uint32_t nTotalTowerKills, uint32_t nTotalRaxKills)
		: m_Rating(nRating), m_TotalGames(nTotalGames), m_TotalWins(nTotalWins), m_TotalLosses(nTotalLosses), m_TotalKills(nTotalKills), m_TotalDeaths(nTotalDeaths), m_TotalCreepKills(nTotalCreepKills), m_TotalCreepDenies(nTotalCreepDenies), m_TotalAssists(nTotalAssists), m_TotalNeutralKills(nTotalNeutralKills), m_TotalTowerKills(nTotalTowerKills), m_TotalRaxKills(nTotalRaxKills) { }
	
	CDBDiv1DPS(int p, double nRating, uint32_t nTotalGames, uint32_t nTotalWins, uint32_t nTotalLosses, float avKills, float avDeaths, float avAssists, float avCreepKills, float avCreepDenies, float avNeutralKills, float avTowerKills, float avRaxKills) {
		m_Rating = nRating;
		m_TotalGames = nTotalGames;
		m_TotalWins = nTotalWins;
		m_TotalLosses = nTotalLosses;

		uint32_t seasonGames = m_TotalWins + m_TotalLosses;

		m_TotalKills = avKills * seasonGames;
		m_TotalDeaths = avDeaths * seasonGames;
		m_TotalAssists = avAssists * seasonGames;
		m_TotalCreepKills = avCreepKills * seasonGames;
		m_TotalCreepDenies = avCreepDenies * seasonGames;
		m_TotalNeutralKills = avNeutralKills * seasonGames;
		m_TotalTowerKills = avTowerKills * seasonGames;
		m_TotalRaxKills = avRaxKills * seasonGames;
	}

	~CDBDiv1DPS() { }

	uint32_t GetTotalGames() { return m_TotalGames; }
	uint32_t GetTotalWins() { return m_TotalWins; }
	uint32_t GetTotalLosses() { return m_TotalLosses; }
	uint32_t GetTotalKills() { return m_TotalKills; }
	uint32_t GetTotalDeaths() { return m_TotalDeaths; }
	uint32_t GetTotalCreepKills() { return m_TotalCreepKills; }
	uint32_t GetTotalCreepDenies() { return m_TotalCreepDenies; }
	uint32_t GetTotalAssists() { return m_TotalAssists; }
	uint32_t GetTotalNeutralKills() { return m_TotalNeutralKills; }
	uint32_t GetTotalTowerKills( )	{ return m_TotalTowerKills; }
	uint32_t GetTotalRaxKills( )		{ return m_TotalRaxKills; }
	//uint32_t GetTotalCourierKills( )	{ return m_TotalCourierKills; }
	float GetAvgKills() { return (m_TotalWins + m_TotalLosses) > 0 ? (float)m_TotalKills / (m_TotalWins + m_TotalLosses) : 0; }
	float GetAvgDeaths() { return (m_TotalWins + m_TotalLosses) > 0 ? (float)m_TotalDeaths / (m_TotalWins + m_TotalLosses) : 0; }
	float GetAvgCreepKills() { return (m_TotalWins + m_TotalLosses) > 0 ? (float)m_TotalCreepKills / (m_TotalWins + m_TotalLosses) : 0; }
	float GetAvgCreepDenies() { return (m_TotalWins + m_TotalLosses) > 0 ? (float)m_TotalCreepDenies / (m_TotalWins + m_TotalLosses) : 0; }
	float GetAvgAssists() { return (m_TotalWins + m_TotalLosses) > 0 ? (float)m_TotalAssists / (m_TotalWins + m_TotalLosses) : 0; }
	float GetAvgNeutralKills() { return (m_TotalWins + m_TotalLosses) > 0 ? (float)m_TotalNeutralKills / (m_TotalWins + m_TotalLosses) : 0; }
	float GetAvgTowerKills( ) { return ( m_TotalWins + m_TotalLosses ) > 0 ? (float)m_TotalTowerKills / ( m_TotalWins + m_TotalLosses ) : 0; }
	float GetAvgRaxKills( ) { return ( m_TotalWins + m_TotalLosses ) > 0 ? (float)m_TotalRaxKills / ( m_TotalWins + m_TotalLosses ) : 0; }
	//float GetAvgCourierKills( )		{ return ( m_TotalWins + m_TotalLosses ) > 0 ? (float)m_TotalCourierKills / ( m_TotalWins + m_TotalLosses ) : 0; }
	double GetRating() { return m_Rating; }
	double GetHighestRating() { return m_HighestRating; }
};

class CDIV1DotAPlayer {
private:
	double m_Rating;
	bool m_RecvNegativePSR;
	bool m_Locked;
	string m_Name;
	unsigned char m_PID;
	unsigned char m_NewColor;
	CPSR * m_Game;
	CDBDiv1DPS * m_Stats;

public:
	//Original
	CDIV1DotAPlayer(double r, string n, unsigned char p, CPSR * game) {
		m_Rating = r;
		m_Name = n;
		m_PID = p;
		m_NewColor = p;
		m_Game = game;
		m_Stats = 0;
	}
	//Original + stats
	CDIV1DotAPlayer(double r, string n, unsigned char p, CPSR * game, CDBDiv1DPS * d) {
		m_Rating = r;
		m_Name = n;
		m_PID = p;
		m_NewColor = p;
		m_Game = game;
		m_Stats = d;
		m_Locked = false;
	}
	//New
	CDIV1DotAPlayer(string n, unsigned char p, CPSR * game, CDBDiv1DPS * d) {
		m_Rating = d->GetRating();
		m_Name = n;
		m_PID = p;
		m_NewColor = p;
		m_Game = game;
		m_Stats = d;
		m_Locked = false;
	}

	/*

	Player's stats value which approximately determines player's skill in the sense of being able to carry the game

	*/
	double GetPlayerValue() {

		//TODO Move somewhere else
		//Constants

		//Expected max values, calculated using 10%-trimmed mean of top 30 players (18.01.19)
		const float c_MaxAverageWL = 2.05; //67%
		const float c_MaxAverageKD = 2.21;
		const float c_MaxAverageAD = 2.04;
		const float c_MaxAverageCreepKills = 202.06;
		const float c_MaxAverageNeutralKills = 51.88;
		const float c_MaxAverageCreepDenies = 9.53;
		const float c_MaxAverageRaxKills = 0.87;
		const float c_MaxAverageTowerKills = 1.7;

		//Weights, 1 stands for 100% consideration
		const float c_WLWeight = 1.5;
		const float c_KDWeight = 1;
		const float c_ADWeight = 1;
		const float c_CreepKillsWeight = 1;
		const float c_CreepDeniesWeight = 1;
		const float c_NeutralKillsWeight = 0.8;
		const float c_RaxKillsWeight = 1;
		const float c_TowerKillsWeight = 1;

		//End constants

		//Get stats from the current player
		uint32_t totalWins = m_Stats->GetTotalWins();
		uint32_t totalLosses = m_Stats->GetTotalLosses();
		float averageKills = m_Stats->GetAvgKills();
		float averageDeaths = m_Stats->GetAvgDeaths();
		float averageAssists = m_Stats->GetAvgAssists();

		uint32_t totalGames = m_Stats->GetTotalGames();

		//If losses or deaths == 0, replace with 1
		float averageWL = totalLosses > 0 ? totalWins / totalLosses : totalWins;
		float averageKD = averageDeaths > 0 ? averageKills / averageDeaths : averageKills;
		float averageAD = averageDeaths > 0 ? averageAssists / averageDeaths : averageAssists;
		

		float averageCreepKills = m_Stats->GetAvgCreepKills();
		float averageCreepDenies = m_Stats->GetAvgCreepDenies();
		float averageNeutralKills = m_Stats->GetAvgNeutralKills();

		float averageTowerKills = m_Stats->GetAvgTowerKills();
		float averageRaxKills = m_Stats->GetAvgRaxKills();

		//Limit stats to cut off extremes
		averageWL = std::min(averageWL, c_MaxAverageWL);
		averageKD = std::min(averageKD, c_MaxAverageKD);
		averageAD = std::min(averageAD, c_MaxAverageAD);
		averageCreepKills = std::min(averageCreepKills, c_MaxAverageCreepKills);
		averageNeutralKills = std::min(averageNeutralKills, c_MaxAverageNeutralKills);
		averageCreepDenies = std::min(averageCreepDenies, c_MaxAverageCreepDenies);
		averageTowerKills = std::min(averageTowerKills, c_MaxAverageTowerKills);
		averageRaxKills = std::min(averageRaxKills, c_MaxAverageRaxKills);

		//Standartized stat values factorized with their weights. Expected : [0, 2]
		float WLValue = (averageWL / c_MaxAverageWL) * c_WLWeight;
		float KDValue = (averageKD / c_MaxAverageKD) * c_KDWeight;
		float ADValue = (averageAD / c_MaxAverageAD) * c_ADWeight;
		float creepKillsValue = (averageCreepKills / c_MaxAverageCreepKills) * c_CreepKillsWeight;
		float creepDeniesValue = (averageCreepDenies / c_MaxAverageCreepDenies) * c_CreepDeniesWeight;
		float neutralKillsValue = (averageNeutralKills / c_MaxAverageNeutralKills) * c_NeutralKillsWeight;
		float raxKillsValue = (averageRaxKills / c_MaxAverageRaxKills) * c_RaxKillsWeight;
		float towerKillsValue = (averageTowerKills / c_MaxAverageTowerKills) * c_TowerKillsWeight;

		//Sum of Weights
		float sumWeights = c_WLWeight + c_KDWeight + c_ADWeight + c_CreepKillsWeight
			+ c_CreepDeniesWeight + c_NeutralKillsWeight + c_RaxKillsWeight + c_TowerKillsWeight;

		//Normalizing multiplier
		float multiplier = -1 + 2 * (WLValue + KDValue + ADValue + creepKillsValue 
			+ creepDeniesValue + neutralKillsValue + raxKillsValue + towerKillsValue) / sumWeights;

		//Disperse totalValue around base value (1500) in [1200, 1800] interval
		double totalValue = 1500 + 300 * multiplier;

		//Set medium value for new player, since skill unknown
		if (!(totalWins + totalLosses + averageKills + averageDeaths + averageAssists + averageCreepKills + averageCreepDenies + averageNeutralKills + averageRaxKills + averageTowerKills)) {
			totalValue = 1500;
		}

		//Hard limits for totalValue : [1200, 1800]
		totalValue = std::max(totalValue, 1200.00);
		totalValue = std::min(totalValue, 1800.00);

		return totalValue;
	}


	/*

	Return "mean value" based on player's PSR and skill value

	*/
	double GetBalanceValue() {	
		//Lower maxDistance : weaker effect, higher maxDistance : stronger effect
		double maxDistance = 200;

		double distance = GetPlayerValue() - m_Rating;

		//Limit maximum distance to reduce effect for extremely different PSR/PlayerValue pairs

		//Positive distance, skill > PSR, higher balanceValue, disadvantage
		distance = std::min(distance, maxDistance);

		//Negative distance, PSR > skill, lower balanceValue, advantage
		//distance = std::max(distance, -maxDistance);
		distance = std::max(distance, 0.0);

		double mean = m_Rating + (distance / 2);

		return mean;
	}

	double GetRating() { return m_Rating; }

	bool GetRecvNegativePSR() { return m_RecvNegativePSR; }
	bool GetLocked() { return m_Locked; }

	void SetRecvNegativePSR(bool nRecvNegativePSR) { m_RecvNegativePSR = nRecvNegativePSR; }
	void SetLocked(bool nLocked) { m_Locked = nLocked; }
	string GetName() { return m_Name; }
	unsigned char GetPID() { return m_PID; }
	CDBDiv1DPS* GetStats() { return m_Stats; }
	
	unsigned char GetCurrentTeam() {
		if (m_NewColor == 255)
		{
			// game is still in lobby

			unsigned char SID = m_Game->GetSIDFromPID(m_PID);

			if (SID < m_Game->m_Slots.size())
				return m_Game->m_Slots[SID].GetTeam();
		}
		else if (m_NewColor >= 1 && m_NewColor <= 5)
		{
			// sentinel player

			return 0;
		}
		else if (m_NewColor >= 7 && m_NewColor <= 11)
		{
			// scourge player

			return 1;
		}
		else
		{
			// observer

			return 12;
		}
	}
};

class CBalanceSlot
{
public:
	CBalanceSlot(bool nLocked, unsigned char nPID, unsigned char nTeam, double nRating) : m_Locked(nLocked), m_PID(nPID), m_Team(nTeam), m_Rating(nRating) { }
	~CBalanceSlot() { }

	bool m_Locked;
	unsigned char m_PID;
	unsigned char m_Team;
	double m_Rating;
};


#endif
