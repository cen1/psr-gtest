
#ifndef PSR_H
#define PSR_H

#include <vector>
#include <list>
#include <utility>
#include <string>

using std::pair;
using std::string;
using std::vector;
using std::list;

typedef pair<string,double> PairedPlayerRating;

class CGameSlot;
class CBalanceSlot;
class CDIV1DotAPlayer;

//
// CPSR
//

class CPSR
{
private:
	/*
	var psf_baseKFactor = 20;
	var psf_gammaCurveK = 18;
	var psf_gammaCurveRange = 200;
	var psf_gammaCurveTheta = 5.0000;
	var psf_KFactorScale = 8;
	var psf_logisticPredictionScale = 80.0000;
	var psf_maxKFactor = 40;
	var psf_medianScalingRank = 1600;
	var psf_minKFactor = 10;
	var psf_teamRankWeighting = 6.5000;
	*/

	/*uint32_t m_BaseKFactor;
	uint32_t m_GammaCurveK;
	uint32_t m_GammaCurveRange;
	double m_GammaCurveTheta;
	uint32_t m_KFactorScale;
	double m_LogisticPredictionScale;
	uint32_t m_MaxKFactor;
	uint32_t m_MedianScalingRank;
	uint32_t m_MinKFactor;
	double m_TeamRankWeighting;*/

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

	bool m_NeedRecalculate;				// if this is true we need to recalculate properties below before using them because they are not accurate anymore

	vector<PairedPlayerRating> m_team1;
	vector<PairedPlayerRating> m_team2;

	double m_team1avgpsr;
	double m_team2avgpsr;
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

class CDIV1DotAPlayer {
private:
	double m_Rating;
	bool m_RecvNegativePSR;
	bool m_Locked;
	string m_Name;
	unsigned char m_PID;
	unsigned char m_NewColor;
	CPSR * m_Game;

public:

	CDIV1DotAPlayer(double r, string n, unsigned char p, CPSR * game) {
		m_Rating = r;
		m_Name = n;
		m_PID = p;
		m_NewColor = p;
		m_Game = game;
	}

	double GetRating() { return m_Rating; }
	bool GetRecvNegativePSR() { return m_RecvNegativePSR; }
	bool GetLocked() { return m_Locked; }

	void SetRecvNegativePSR(bool nRecvNegativePSR) { m_RecvNegativePSR = nRecvNegativePSR; }
	void SetLocked(bool nLocked) { m_Locked = nLocked; }
	string GetName() { return m_Name; }
	unsigned char GetPID() { return m_PID; }
	
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
