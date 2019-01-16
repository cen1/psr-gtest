
//#include "ghost.h"
#include "psr.h"
#include <utility>
#include <iostream>
#include <set>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <tuple>

using std::make_pair;
using std::make_tuple;
using std::cout;
using std::set;
using std::get;
using std::stringstream;
using std::list;

//
// CPSR
//

CPSR :: CPSR( )
{
	m_BaseKFactor = 20.0;
	m_GammaCurveK = 18.0;
	m_GammaCurveRange = 200;
	m_GammaCurveTheta = 5.0000;
	m_KFactorScale = 8.0;
	m_LogisticPredictionScale = 80.0000;
	m_MaxKFactor = 40.0;
	m_MedianScalingRank = 1600.0;
	m_MinKFactor = 10.0;
	m_TeamRankWeighting = 6.5000;

	m_ConstWinGain = 5.0;
	m_ConstLoseGain = 5.0;

	m_team1avgpsr = 0.0;
	m_team2avgpsr = 0.0;
	m_team1winPerc = 0.0;
	m_team2winPerc = 0.0;

	m_team1kfactors = vector<double>( 5, 0.0 );
	m_team2kfactors = vector<double>( 5, 0.0 );
	m_team1gainLose = vector< pair<double, double> >( 5, make_pair( 0.0, 0.0 ) );
	m_team2gainLose = vector< pair<double, double> >( 5, make_pair( 0.0, 0.0 ) );
}

CPSR :: ~CPSR( )
{

}

double CPSR :: GetTeamAvgPSR( unsigned team )
{
	// team, sentinel = 0, scourge = 1

	if( team == 0 )
		return m_team1avgpsr;
	else
		return m_team2avgpsr;
}

double CPSR::GetTeamAvgValue(unsigned team)
{
	// team, sentinel = 0, scourge = 1

	if (team == 0)
		return m_team1AverageValue;
	else
		return m_team2AverageValue;
}

double CPSR :: GetTeamWinPerc( unsigned team )
{
	// team, sentinel = 0, scourge = 1

	if( team == 0 )
		return m_team1winPerc;
	else
		return m_team2winPerc;
}

double CPSR :: GetPlayerKFactor( string name )
{
	for( unsigned int i = 0; i < m_team1.size( ); ++i )
	{
		if( m_team1[i].first == name )
			return m_team1kfactors[i];
	}

	for( unsigned int i = 0; i < m_team2.size( ); ++i )
	{
		if( m_team2[i].first == name )
			return m_team2kfactors[i];
	}

	return 0.0;
}

pair<double, double> CPSR :: GetPlayerGainLoss( string name )
{
	for( unsigned int i = 0; i < m_team1.size( ); ++i )
	{
		if( m_team1[i].first == name )
			return m_team1gainLose[i];
	}

	for( unsigned int i = 0; i < m_team2.size( ); ++i )
	{
		if( m_team2[i].first == name )
			return m_team2gainLose[i];
	}

	return make_pair( 0.0, 0.0 );
}

void CPSR :: CalculatePSR( vector<PairedPlayerRating> team1, vector<PairedPlayerRating> team2 )
{
	if (team1.size() > 5 || team2.size() > 5) {
		return;
	}

	m_team1 = team1;
	m_team2 = team2;

	m_team1avgpsr = 0.0;
	m_team2avgpsr = 0.0;
	m_team1winPerc = 0.0;
	m_team2winPerc = 0.0;

	//m_team1kfactors.clear( );
	//m_team2kfactors.clear( );
	std::fill(m_team1kfactors.begin(), m_team1kfactors.end(), 0);
	std::fill(m_team2kfactors.begin(), m_team2kfactors.end(), 0);

	m_team1gainLose.clear( );
	m_team2gainLose.clear( );

	vector<double> team1weights( 5, 0.0 );
	vector<double> team2weights( 5, 0.0 );
	double team1weight = 0;
	double team2weight = 0;
	double team1totalpsr = 0;
	double team2totalpsr = 0;

	//Team 1 weight + total PSR
	for( unsigned int i = 0; i < m_team1.size( ); ++i )
	{
		team1weights[i] = pow( m_team1[i].second, m_TeamRankWeighting );
		team1weight += team1weights[i];
		team1totalpsr += m_team1[i].second;
	}
	//Team 2 weight + total PSR
	for( unsigned int i = 0; i < m_team2.size( ); ++i )
	{
		team2weights[i] = pow( m_team2[i].second, m_TeamRankWeighting );
		team2weight += team2weights[i];
		team2totalpsr += m_team2[i].second;
	}

	//Team 1 & 2 average PSR
	m_team1avgpsr = team1totalpsr / m_team1.size( );
	m_team2avgpsr = team2totalpsr / m_team2.size( );

	//?
	double team1rating = pow( team1weight, (double)1.0 / m_TeamRankWeighting );
	double team2rating = pow( team2weight, (double)1.0 / m_TeamRankWeighting );

	double diff = team1rating - team2rating;

	double E = 2.7182;

	//Team 1 & 2 win probabilities
	double team1winprobability = 1.0 / ( 1.0 + pow( E, ( ( -1.0 * diff ) / m_LogisticPredictionScale ) ) );
	double team2winprobability = 1.0 - team1winprobability;

	//Calculate Team 1 K Factors (?)
	for( unsigned int i = 0; i < m_team1.size( ); ++i )
	{
		//Calculate initial K Factor, cut off extremes
		m_team1kfactors[i] = ( ( m_MedianScalingRank - m_team1[i].second ) / m_KFactorScale ) + m_BaseKFactor;
		m_team1kfactors[i] = m_team1kfactors[i] > m_MaxKFactor ? m_MaxKFactor : m_team1kfactors[i];
		m_team1kfactors[i] = m_team1kfactors[i] > m_MinKFactor ? m_team1kfactors[i] : m_MinKFactor ;

		//Adjust K Factor for extremely standing out player
		double distancefromteamavgpsr = m_team1[i].second - 50 - m_team1avgpsr;

		if(distancefromteamavgpsr > 100 )
			distancefromteamavgpsr = 100;

		if( distancefromteamavgpsr > 0 && distancefromteamavgpsr <= 100 )
		{
			double implayingwithnewbiesfactor = ( 100.0 - distancefromteamavgpsr ) / 100;
			m_team1kfactors[i] = implayingwithnewbiesfactor * m_team1kfactors[i];
		}
	}

	//Calculate Team 2 K Factors
	for( unsigned int i = 0; i < m_team2.size( ); ++i )
	{
		m_team2kfactors[i] = ( ( m_MedianScalingRank - m_team2[i].second ) / m_KFactorScale ) + m_BaseKFactor;
		m_team2kfactors[i] = m_team2kfactors[i] > m_MaxKFactor ? m_MaxKFactor : m_team2kfactors[i];
		m_team2kfactors[i] = m_team2kfactors[i] > m_MinKFactor ? m_team2kfactors[i] : m_MinKFactor ;

		double distancefromteamavgpsr = m_team2[i].second - 50 - m_team2avgpsr;

		if(distancefromteamavgpsr > 100 )
			distancefromteamavgpsr = 100;

		if( distancefromteamavgpsr > 0 && distancefromteamavgpsr <= 100 )
		{
			double implayingwithnewbiesfactor = ( 100.0 - distancefromteamavgpsr ) / 100;
			m_team2kfactors[i] = implayingwithnewbiesfactor * m_team2kfactors[i];
		}
	}

	//Calculate Team 1 Win/Lose points
	for( unsigned int i = 0; i < m_team1.size( ); ++i ) {
		double im_win = ceil( team2winprobability * m_team1kfactors[i] );
		double im_lose = floor( team1winprobability * m_team1kfactors[i]);
		
		//lose points are also positive
		if (im_win < 1.0) {
			im_win = 1.0;
		}
		if (im_lose < 1.0) {
			im_lose = 1.0;
		}

		m_team1gainLose.push_back( make_pair( im_win, im_lose ) );
	}

	//Calculate Team 2 Win/Lose points
	for( unsigned int i = 0; i < m_team2.size( ); ++i ) {
		double im_win = ceil( team1winprobability * m_team2kfactors[i] );
		double im_lose = floor( team2winprobability * m_team2kfactors[i] );
		
		//lose points are also positive
		if (im_win < 1.0) {
			im_win = 1.0;
		}
		if (im_lose < 1.0) {
			im_lose = 1.0;
		}

		m_team2gainLose.push_back( make_pair( im_win, im_lose ) );
	}

	m_team1winPerc = ( team1winprobability * 100 );
	m_team2winPerc = 100 - m_team1winPerc;

	m_NeedRecalculate = false;

}

vector<pair<double, double> > CPSR::getTeam1gainLose() {
	return m_team1gainLose;
}

vector<pair<double, double> > CPSR::getTeam2gainLose() {
	return m_team2gainLose;
}

void CPSR::balance() {
	//Redistribute players using PSR as criteria
	this->BalanceSlots2();

	//Assign players to teams, preparation for CalculatePSR, base on PSR only
	vector<PairedPlayerRating> team1;
	vector<PairedPlayerRating> team2;

	for (auto i = m_Slots.begin(); i != m_Slots.end(); i++) {
		for (auto j = m_DotAPlayers.begin(); j != m_DotAPlayers.end(); j++) {
			if ((*i).GetPID() == (*j)->GetPID()) {
				string Name = (*j)->GetName();

				if ((*i).GetTeam() == 0) {
					team1.push_back(make_pair(Name, (*j)->GetRating()));
				}
				else if ((*i).GetTeam() == 1) {
					team2.push_back(make_pair(Name, (*j)->GetRating()));
				}
			}
		}
	}

	//Calculate PSR Gain/Loss for all players based on win % and their PSR
	CalculatePSR(team1, team2);

	//Output for the players
	//cout << "Sentinel avg. PSR: " + UTIL_ToString(GetTeamAvgPSR(0), 0) + "(" + UTIL_ToString(GetTeamWinPerc(0), 0) + "% to win), Scourge avg. PSR: " + UTIL_ToString(GetTeamAvgPSR(1), 0) + "(" + UTIL_ToString(GetTeamWinPerc(1), 0) + "% to win)" << std::endl;
}

/*

Input: 2 vectors of Triples for both teams of size < 5 consisting of
<0> Player's Name
<1> Player's BalanceValue = (Player PSR + Player Value) / 2
<2> Player's PSR

Win probability is calculated based on total BalanceValues of the both teams 
PSR gain & loss are calculated based on player's individual PSR, win probabilities and his PSR distance to game's average PSR
PSR gain for players whose distance is bigger than a certain amount is reduced
PSR loss for players whose distance is bigger than a certain amount is not reduced

Therefore, the new function should deliver exactly same PSR gain/loss numbers as the old one, except for the absence of PSR loss 
reduction for players with big distance to average game's PSR

*/
void CPSR::CalculatePSRNew(vector<TupledPlayerValue> p_team1, vector<TupledPlayerValue> p_team2)
{
	//Exit if teams invalid
	if (p_team1.size() > 5 || p_team2.size() > 5) {
		return;
	}

	vector<TupledPlayerValue> team1 = p_team1;
	vector<TupledPlayerValue> team2 = p_team2;

	//TODO Check if used by any other methods in code, not needed anymore
	m_team1avgpsr = 0.0;
	m_team2avgpsr = 0.0;

	double gameTotalPSR = 0.0;
	double gameAveragePSR = 0.0;

	double team1TotalValue = 0.0;
	double team2TotalValue = 0.0;

	m_team1AverageValue = 0.0;
	m_team2AverageValue = 0.0;

	m_team1winPerc = 0.0;
	m_team2winPerc = 0.0;

	std::fill(m_team1kfactors.begin(), m_team1kfactors.end(), 0);
	std::fill(m_team2kfactors.begin(), m_team2kfactors.end(), 0);

	m_team1gainLose.clear();
	m_team2gainLose.clear();

	vector<double> team1weights(5, 0.0);
	vector<double> team2weights(5, 0.0);
	double team1weight = 0;
	double team2weight = 0;

	//Win penalty factors, initialized with 1 (no effect) to lower psr gain for players with far higher psr than game's average
	std::vector<double> team1WinPenaltyFactors(m_team1kfactors.size());
	std::vector<double> team2WinPenaltyFactors(m_team2kfactors.size());
	std::fill(team1WinPenaltyFactors.begin(), team1WinPenaltyFactors.end(), 1);
	std::fill(team2WinPenaltyFactors.begin(), team2WinPenaltyFactors.end(), 1);


	//Lose reduction to prevent big psr lose for players with far higher value, disabled by default (set to 1)
	std::vector<double> team1LoseReductionFactors(m_team1kfactors.size());
	std::vector<double> team2LoseReductionFactors(m_team2kfactors.size());
	std::fill(team1LoseReductionFactors.begin(), team1LoseReductionFactors.end(), 1);
	std::fill(team2LoseReductionFactors.begin(), team2LoseReductionFactors.end(), 1);

	//Set weights of each player in team 1, sum its total Value and PSR
	for (unsigned int i = 0; i < team1.size(); ++i)
	{
		//Set player's weight based on his BalanceValue
		team1weights[i] = pow(get<1>(team1[i]), m_TeamRankWeighting);
		team1weight += team1weights[i];

		//Add player's PSR to game's total PSR
		gameTotalPSR += get<2>(team1[i]);

		//Add player's balanceValue to team total value
		team1TotalValue += get<1>(team1[i]);
	}

	//Set weights of each player in team 2, sum its total Value and PSR
	for (unsigned int i = 0; i < team2.size(); ++i)
	{
		//Set player's weight based on his BalanceValue
		team2weights[i] = pow(get<1>(team2[i]), m_TeamRankWeighting);
		team2weight += team2weights[i];

		//Add player's PSR to game's total PSR
		gameTotalPSR += get<2>(team2[i]);

		//Add player's balanceValue to team total value
		team2TotalValue += get<1>(team2[i]);
	}

	//Calculate game's average PSR
	gameAveragePSR = gameTotalPSR / (team1.size() + team1.size());

	//Calculate and set teams' average values
	m_team1AverageValue = team1TotalValue / team1.size();
	m_team2AverageValue = team2TotalValue / team2.size();

	//Calculate weighted rating of each team based on previously calculated team weights
	double team1rating = pow(team1weight, (double)1.0 / m_TeamRankWeighting);
	double team2rating = pow(team2weight, (double)1.0 / m_TeamRankWeighting);

	double diff = team1rating - team2rating;

	double E = 2.7182;

	//Calculate Team 1 & Team 2 win/lose probabilities
	double team1winprobability = 1.0 / (1.0 + pow(E, ((-1.0 * diff) / m_LogisticPredictionScale)));
	double team2winprobability = 1.0 - team1winprobability;

	//Calculate Team 1 K Factors
	for (unsigned int i = 0; i < team1.size(); ++i)
	{
		//Calculate initial K Factor using PSR for each player in team 1
		m_team1kfactors[i] = ((m_MedianScalingRank - get<2>(team1[i])) / m_KFactorScale) + m_BaseKFactor;

		//Cut off extreme values
		m_team1kfactors[i] = m_team1kfactors[i] > m_MaxKFactor ? m_MaxKFactor : m_team1kfactors[i];
		m_team1kfactors[i] = m_team1kfactors[i] > m_MinKFactor ? m_team1kfactors[i] : m_MinKFactor;

		//Calculate player's PSR distance to game's average PSR
		double distancefromteamavgpsr = get<2>(team1[i]) - 50 - gameAveragePSR;

		if (distancefromteamavgpsr > 100)
			distancefromteamavgpsr = 100;

		if (distancefromteamavgpsr > 0 && distancefromteamavgpsr <= 100)
		{
			//Calculate PSR gain/loss reduction factors for extremely standing out player based on his and game's average PSR
			double implayingwithnewbiesfactor = (100.0 - distancefromteamavgpsr) / 100;
			team1WinPenaltyFactors[i] = implayingwithnewbiesfactor;

			implayingwithnewbiesfactor = (100.0 - (distancefromteamavgpsr/2)) / 100;
			team1LoseReductionFactors[i] = implayingwithnewbiesfactor;
		}
	}

	//Calculate Team 2 K Factors
	for (unsigned int i = 0; i < team2.size(); ++i)
	{
		//Calculate initial K Factor using PSR for each player in team 2
		m_team2kfactors[i] = ((m_MedianScalingRank - get<2>(team2[i])) / m_KFactorScale) + m_BaseKFactor;

		//Cut off extreme values
		m_team2kfactors[i] = m_team2kfactors[i] > m_MaxKFactor ? m_MaxKFactor : m_team2kfactors[i];
		m_team2kfactors[i] = m_team2kfactors[i] > m_MinKFactor ? m_team2kfactors[i] : m_MinKFactor;

		//Calculate player's PSR distance to game's average PSR
		double distancefromteamavgpsr = get<2>(team2[i]) - 50 - gameAveragePSR;

		if (distancefromteamavgpsr > 100)
			distancefromteamavgpsr = 100;

		if (distancefromteamavgpsr > 0 && distancefromteamavgpsr <= 100)
		{
			//Calculate PSR gain/loss reduction factors for extremely standing out player based on his and game's average PSR
			double implayingwithnewbiesfactor = (100.0 - distancefromteamavgpsr) / 100;
			team2WinPenaltyFactors[i] = implayingwithnewbiesfactor;
			
			implayingwithnewbiesfactor = (100.0 - (distancefromteamavgpsr / 2)) / 100;
			team2LoseReductionFactors[i] = implayingwithnewbiesfactor;
		}
	}

	//Calculate Team 1 Win/Lose points (both positive)
	for (unsigned int i = 0; i < team1.size(); ++i) {

		//Reduce PSR gain/loss for extremely standing out players (relative to game's average PSR)
		double im_win = ceil(team2winprobability * (m_team1kfactors[i] * team1WinPenaltyFactors[i]));
		double im_lose = floor(team1winprobability * m_team1kfactors[i] * team1LoseReductionFactors[i]);

		//Minimum Gain/Loss is 1 PSR
		if (im_win < 1.0) {
			im_win = 1.0;
		}
		if (im_lose < 1.0) {
			im_lose = 1.0;
		}

		m_team1gainLose.push_back(make_pair(im_win, im_lose));
	}

	//Calculate Team 2 Win/Lose points (both positive)
	for (unsigned int i = 0; i < team2.size(); ++i) {

		//Reduce PSR gain/loss for extremely standing out players (relative to game's average PSR)
		double im_win = ceil(team1winprobability * (m_team2kfactors[i] * team2WinPenaltyFactors[i]));
		double im_lose = floor(team2winprobability * m_team2kfactors[i] * team2LoseReductionFactors[i]);

		//Minimum Gain/Loss is 1 PSR
		if (im_win < 1.0) {
			im_win = 1.0;
		}
		if (im_lose < 1.0) {
			im_lose = 1.0;
		}

		m_team2gainLose.push_back(make_pair(im_win, im_lose));
	}

	//Convert win/lose probabilities in % values
	m_team1winPerc = (team1winprobability * 100);
	m_team2winPerc = 100 - m_team1winPerc;

	//TODO Used anywhere?
	m_team1avgpsr = gameAveragePSR;
	m_team2avgpsr = gameAveragePSR;

	m_NeedRecalculate = false;
}

/*
Called when typing !balance

Balances players using their BalanceValue = (PSR + PlayerValue) / 2

*/
void CPSR::balanceNew() {
	//Redistribute players using their balanceValue as criteria
	this->BalanceSlots2New();

	//Preparation for CalculatePSR(...), Fill teams with <PlayerName, PlayerValue, PSR>
	vector<TupledPlayerValue> team1;
	vector<TupledPlayerValue> team2;

	for (auto i = m_Slots.begin(); i != m_Slots.end(); i++) {
		for (auto j = m_DotAPlayers.begin(); j != m_DotAPlayers.end(); j++) {
			if ((*i).GetPID() == (*j)->GetPID()) {
				string Name = (*j)->GetName();

				if ((*i).GetTeam() == 0) {
					team1.push_back(make_tuple(Name, (*j)->GetBalanceValue(), (*j)->GetRating()));
				}
				else if ((*i).GetTeam() == 1) {
					team2.push_back(make_tuple(Name, (*j)->GetBalanceValue(), (*j)->GetRating()));
				}
			}
		}
	}

	//PSR is given based on game's average PSR and win/lose probabilities,
	//probabilities calculated based on balanceValue
	CalculatePSRNew(team1, team2);

	//TODO Enable back
	//Display info for players
	//cout << "Sentinel avg. value: " + UTIL_ToString(GetTeamAvgValue(0), 0) + "(" + UTIL_ToString(GetTeamWinPerc(0), 0) + "% to win), Scourge avg. value: " + UTIL_ToString(GetTeamAvgValue(1), 0) + "(" + UTIL_ToString(GetTeamWinPerc(1), 0) + "% to win)" << std::endl;
}

/*
(*i)->GetBalanceValue() used instead of (*i)->GetRating()

Balancing is done used BalanceValue = (PSR + PlayerValue) / 2

*/
void CPSR::BalanceSlots2New() {
	if (m_DotAPlayers.size() < 2)
		return;

	list<CBalanceSlot> SlotsToBalance;
	set<unsigned char> TakenSlots;

	double balancePlayerValue;

	for (list<CDIV1DotAPlayer *> ::iterator i = m_DotAPlayers.begin(); i != m_DotAPlayers.end(); ++i)
	{
		//TODO Locked doesn't seem to affect anything
		SlotsToBalance.push_back(CBalanceSlot((*i)->GetLocked(), (*i)->GetPID(), (*i)->GetCurrentTeam(), (*i)->GetBalanceValue()));
	}

	this->BalanceSlots2Recursive(SlotsToBalance);

	for (list<CBalanceSlot> ::iterator i = SlotsToBalance.begin(); i != SlotsToBalance.end(); ++i)
	{
		if ((*i).m_Locked)
			continue;

		unsigned char j;
		unsigned char end;

		if ((*i).m_Team == 0)
		{
			j = 0;
			end = 5;
		}
		else if ((*i).m_Team == 1)
		{
			j = 5;
			end = 10;
		}

		for (; j < end; ++j)
		{
			if (TakenSlots.find(j) == TakenSlots.end())
			{
				SwapSlots(GetSIDFromPID((*i).m_PID), j);
				TakenSlots.insert(j);
				break;
			}
		}
	}
}

string CPSR::UTIL_ToString(double d, int digits) {
	string result;
	stringstream SS;
	SS << std::fixed << std::setprecision(digits) << d;
	SS >> result;
	return result;
}

void CPSR::BalanceSlots2() {
	if (m_DotAPlayers.size() < 2)
		return;

	list<CBalanceSlot> SlotsToBalance;
	set<unsigned char> TakenSlots;

	for (list<CDIV1DotAPlayer *> ::iterator i = m_DotAPlayers.begin(); i != m_DotAPlayers.end(); ++i)
	{
		SlotsToBalance.push_back(CBalanceSlot((*i)->GetLocked(), (*i)->GetPID(), (*i)->GetCurrentTeam(), (*i)->GetRating()));
	}

	this->BalanceSlots2Recursive(SlotsToBalance);

	for (list<CBalanceSlot> ::iterator i = SlotsToBalance.begin(); i != SlotsToBalance.end(); ++i)
	{
		if ((*i).m_Locked)
			continue;

		unsigned char j;
		unsigned char end;

		if ((*i).m_Team == 0)
		{
			j = 0;
			end = 5;
		}
		else if ((*i).m_Team == 1)
		{
			j = 5;
			end = 10;
		}

		for (; j < end; ++j)
		{
			if (TakenSlots.find(j) == TakenSlots.end())
			{
				SwapSlots(GetSIDFromPID((*i).m_PID), j);
				TakenSlots.insert(j);
				break;
			}
		}
	}
}

unsigned int CPSR::BalanceSlots2Recursive(list<CBalanceSlot>& slots_to_balance) {
	static unsigned int call = 0;
	++call;
	unsigned int current_call = call;
	//cout << "call #" << call << endl;

	unsigned int team1_players_count = 0;
	unsigned int team2_players_count = 0;
	int team1_rating = 0;
	int team2_rating = 0;
	int rating_diff = 0;
	int next_rating_diff = 999999;

	// teams static variable must be resetted in the last function call execution
	static unsigned int teams = 0;
	unsigned int current_teams = teams++;

	int binary_position = 0;

	for (list<CBalanceSlot> ::iterator i = slots_to_balance.begin(); i != slots_to_balance.end(); ++i)
	{
		if ((current_teams & (int)pow(2.0, binary_position)) == 0)
		{
			++team1_players_count;
			team1_rating += (*i).m_Rating;
		}
		else
		{
			++team2_players_count;
			team2_rating += (*i).m_Rating;
		}

		//cout << "teams:" << teams << " pow(2, binary_position):" << pow(2.0, binary_position) << endl;

		++binary_position;
	}

	if (team1_players_count == 0 || team2_players_count == 0 || team1_players_count > 5 || team2_players_count > 5)
		rating_diff = 999999;
	else
		rating_diff = abs(team1_rating - team2_rating);

	//cout << "team2_players_count:" << team2_players_count << " slots_to_balance.size( ):" << slots_to_balance.size( ) << endl;

	if (team2_players_count < slots_to_balance.size())
	{
		next_rating_diff = BalanceSlots2Recursive(slots_to_balance);
	}
	else
	{
		//cout << "this should be the last call, call #" << call << endl;
		teams = 0; // this is the last function call execution, reset static variable
	}

	if (rating_diff < next_rating_diff)
	{
		binary_position = 0;

		//cout << "call #" << current_call << endl;
		//cout << "rating_diff:" << rating_diff << " next_rating_diff:" << next_rating_diff << endl;

		for (list<CBalanceSlot> ::iterator i = slots_to_balance.begin(); i != slots_to_balance.end(); ++i)
		{
			if ((current_teams & (int)pow(2.0, binary_position)) == 0)
			{
				if ((*i).m_Locked && (*i).m_Team == 1)
				{
					// this team layout is invalid
					// this player is locked to the scourge team, he can't be swaped to sentinel

					rating_diff = 999999;
					break;
				}

				(*i).m_Team = 0;
			}
			else
			{
				if ((*i).m_Locked && (*i).m_Team == 0)
				{
					// this team layout is invalid
					// this player is locked to the sentinel team, he can't be swaped to scourge

					rating_diff = 999999;
					break;
				}

				(*i).m_Team = 1;
			}

			++binary_position;
		}
	}

	return rating_diff < next_rating_diff ? rating_diff : next_rating_diff;
}

void CPSR::SwapSlots(unsigned char SID1, unsigned char SID2) {
	if (SID1 < m_Slots.size() && SID2 < m_Slots.size() && SID1 != SID2)
	{
		CGameSlot Slot1 = m_Slots[SID1];
		CGameSlot Slot2 = m_Slots[SID2];

		//m_Slots[SID1] = Slot2;
		//m_Slots[SID2] = Slot1;

		m_Slots[SID1] = CGameSlot(Slot2.GetPID(), Slot1.GetTeam(), Slot1.GetColour());
		m_Slots[SID2] = CGameSlot(Slot1.GetPID(), Slot2.GetTeam(), Slot2.GetColour());

	}
}

unsigned char CPSR::GetSIDFromPID(unsigned char PID) {
	if (m_Slots.size() > 255)
		return 255;

	for (unsigned char i = 0; i < m_Slots.size(); ++i)
	{
		if (m_Slots[i].GetPID() == PID)
			return i;
	}

	return 255;
}