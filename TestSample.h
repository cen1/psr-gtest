#ifndef TEST_H
#define TEST_H

#include "psr.h"

using std::cout;
using std::endl;

class TestSamples
{
private:
	vector<vector<pair<string, CDBDiv1DPS*>>> m_arrayOfSamples;
	vector<string> descriptions;

public:
	TestSamples() {}
	~TestSamples() {}

	void addSample(vector<pair<string, CDBDiv1DPS*>> players, string description) {
		m_arrayOfSamples.push_back(players);
		descriptions.push_back(description);
	}

	string getSampleDescription(int i) {
		if (i > 0 && i < descriptions.size()) {
			return descriptions[i];
		}
		else {
			return "";
		}
	}

	vector<pair<string, CDBDiv1DPS*>> getPlayerList(int index) {
		vector<pair<string, CDBDiv1DPS*>> players;
		for (pair<string, CDBDiv1DPS*> player : m_arrayOfSamples[index]) {
			players.push_back(make_pair(player.first + " (" + std::to_string(index) + ")", player.second));
		}
		return players;
	}

	int size() {
		return m_arrayOfSamples.size();
	}

	//Returns a newly created cpsr object using the test sample i 
	CPSR generateCPSRFromSample(int i) {
		CPSR cpsr1;
		CDIV1DotAPlayer *p;

		vector<unsigned char> pids = { 1, 2, 3, 4, 5, 7, 8, 9, 10, 11 };

		for (int j = 0; j < 10; j++) {
			p = new CDIV1DotAPlayer(m_arrayOfSamples[i][j].first, pids[j], &cpsr1, m_arrayOfSamples[i][j].second);

			//Hardcode : player 1 is locked (host)
			if (j == 0) {
				p->SetLocked(true);
			}

			cpsr1.m_DotAPlayers.push_back(p);

		}

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

		return cpsr1;
	}

	pair<vector<PairedPlayerRating>, vector<PairedPlayerRating>>GetTeamsWithRatingFromCPSR(CPSR cpsr1) {
		vector<PairedPlayerRating> team1;
		vector<PairedPlayerRating> team2;

		for (auto i =cpsr1.m_Slots.begin(); i != cpsr1.m_Slots.end(); i++) {
			for (auto j = cpsr1.m_DotAPlayers.begin(); j != cpsr1.m_DotAPlayers.end(); j++) {
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

		return make_pair(team1, team2);
	}

	pair<vector<PairedPlayerRating>, vector<PairedPlayerRating>>GetTeamsWithPlayerValueFromCPSR(CPSR cpsr1) {
		vector<PairedPlayerRating> team1;
		vector<PairedPlayerRating> team2;

		for (auto i = cpsr1.m_Slots.begin(); i != cpsr1.m_Slots.end(); i++) {
			for (auto j = cpsr1.m_DotAPlayers.begin(); j != cpsr1.m_DotAPlayers.end(); j++) {
				if ((*i).GetPID() == (*j)->GetPID()) {
					string Name = (*j)->GetName();

					if ((*i).GetTeam() == 0) {
						team1.push_back(make_pair(Name, (*j)->GetPlayerValue()));
					}
					else if ((*i).GetTeam() == 1) {
						team2.push_back(make_pair(Name, (*j)->GetPlayerValue()));
					}
				}
			}
		}

		return make_pair(team1, team2);
	}

	pair<vector<TupledPlayerValue>, vector<TupledPlayerValue>>GetTeamsWithBalanceValueFromCPSR(CPSR cpsr1) {
		vector<TupledPlayerValue> team1;
		vector<TupledPlayerValue> team2;

		for (auto i = cpsr1.m_Slots.begin(); i != cpsr1.m_Slots.end(); i++) {
			for (auto j = cpsr1.m_DotAPlayers.begin(); j != cpsr1.m_DotAPlayers.end(); j++) {
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

		return make_pair(team1, team2);
	}

	//Print teams from cpsr
	void displayTeamsFromCPSR(CPSR cpsr) {
		int slot = 1;
		double balanceValue;
		cout << "<Team 1>" << std::endl;
		for (auto i = cpsr.m_Slots.begin(); i != cpsr.m_Slots.end(); i++) {
			for (auto j = cpsr.m_DotAPlayers.begin(); j != cpsr.m_DotAPlayers.end(); j++) {
				if ((*i).GetPID() == (*j)->GetPID()) {
					if (slot == 6) { cout << "<Team 2>" << std::endl; }
					balanceValue = ((*j)->GetRating() + (*j)->GetPlayerValue()) / 2;
					cout << (int)(*i).m_Colour << ": " << (*j)->GetName() << " - " << (*j)->GetRating() << " (" << (*j)->GetPlayerValue() << ") - mean: " << balanceValue << std::endl;
				}
			}
			slot++;
		}
	}

	void displayTeamsFromCPSRPreBalance(CPSR cpsr) {
		cout << "PRE-BALANCE" << std::endl;
		displayTeamsFromCPSR(cpsr);
		cout << std::endl;
	}

	void displayTeamsFromCPSRPostBalance(CPSR cpsr) {
		cout << "POST-BALANCE" << std::endl;
		displayTeamsFromCPSR(cpsr);
		cout << std::endl;
	}

	void displayTeamsGainLoseFromCPSR(CPSR cpsr) {
		cout << "Team 1 gain/lose" << endl;
		vector<pair<double, double>> team1GainLose = cpsr.getTeam1gainLose();
		for (int i = 0; i < team1GainLose.size(); i++) {
			cout << team1GainLose[i].first << " / " << team1GainLose[i].second << endl;
		}

		cout << "Team 2 gain/lose" << endl;
		vector<pair<double, double>> team2GainLose = cpsr.getTeam2gainLose();
		for (int i = 0; i < team2GainLose.size(); i++) {
			cout << team2GainLose[i].first << " / " << team2GainLose[i].second << endl;
		}
	}

	void displayWinLosePercOld(CPSR cpsr) {
		cout << endl;
		cout << "Sentinel avg. PSR: " + cpsr.UTIL_ToString(cpsr.GetTeamAvgPSR(0), 0) 
			+ "(" + cpsr.UTIL_ToString(cpsr.GetTeamWinPerc(0), 0) + "% to win), Scourge avg. PSR: " 
			+ cpsr.UTIL_ToString(cpsr.GetTeamAvgPSR(1), 0) + "(" + cpsr.UTIL_ToString(cpsr.GetTeamWinPerc(1), 0) 
			+ "% to win)" << std::endl;
		cout << endl;
	}

	void displayWinLosePercNew(CPSR cpsr) {
		cout << endl;
		cout << "Sentinel avg. value: " + cpsr.UTIL_ToString(cpsr.GetTeamAvgValue(0), 0) 
			+ "(" + cpsr.UTIL_ToString(cpsr.GetTeamWinPerc(0), 0) + "% to win), Scourge avg. value: " 
			+ cpsr.UTIL_ToString(cpsr.GetTeamAvgValue(1), 0) + "(" + cpsr.UTIL_ToString(cpsr.GetTeamWinPerc(1), 0) 
			+ "% to win)" << ", avrg game PSR: " << cpsr.UTIL_ToString((cpsr.GetTeamAvgPSR(0)+cpsr.GetTeamAvgPSR(1)) / 2, 0)
			<< std::endl;
		cout << endl;
	}

	//Display teams + psr gain/lose
	void displayTeamsAndPSRFromCPSR(CPSR cpsr) {

		vector<pair<double, double>> gainLose;

		vector<pair<double, double>> team1GainLose = cpsr.getTeam1gainLose();
		for (int i = 0; i < team1GainLose.size(); i++) {
			gainLose.push_back(team1GainLose[i]);
		}

		vector<pair<double, double>> team2GainLose = cpsr.getTeam2gainLose();
		for (int i = 0; i < team2GainLose.size(); i++) {
			gainLose.push_back(team2GainLose[i]);
		}

		int slot = 1;
		double balanceValue;
		int index;
		cout << "<Team 1>" << std::endl;
		for (auto i = cpsr.m_Slots.begin(); i != cpsr.m_Slots.end(); i++) {
			for (auto j = cpsr.m_DotAPlayers.begin(); j != cpsr.m_DotAPlayers.end(); j++) {
				if ((*i).GetPID() == (*j)->GetPID()) {
					if (slot == 6) { cout << "<Team 2>" << std::endl; }
					balanceValue = ((*j)->GetRating() + (*j)->GetPlayerValue()) / 2;
					index = i - cpsr.m_Slots.begin();
					cout << (int)(*i).m_Colour << ": " << (*j)->GetName() << " - " << (*j)->GetRating() << " - gain/lose: " << gainLose[index].first << " / " << gainLose[index].second << std::endl;
				}
			}
			slot++;
		}
	}

	//Display teams + psr gain/lose
	void displayTeamsAndPSRFromCPSR_Value(CPSR cpsr) {

		vector<pair<double, double>> gainLose;

		vector<pair<double, double>> team1GainLose = cpsr.getTeam1gainLose();
		for (int i = 0; i < team1GainLose.size(); i++) {
			gainLose.push_back(team1GainLose[i]);
		}

		vector<pair<double, double>> team2GainLose = cpsr.getTeam2gainLose();
		for (int i = 0; i < team2GainLose.size(); i++) {
			gainLose.push_back(team2GainLose[i]);
		}

		int slot = 1;
		double balanceValue;
		int index;
		cout << "<Team 1>" << std::endl;
		for (auto i = cpsr.m_Slots.begin(); i != cpsr.m_Slots.end(); i++) {
			for (auto j = cpsr.m_DotAPlayers.begin(); j != cpsr.m_DotAPlayers.end(); j++) {
				if ((*i).GetPID() == (*j)->GetPID()) {
					if (slot == 6) { cout << "<Team 2>" << std::endl; }
					balanceValue = ((*j)->GetRating() + (*j)->GetPlayerValue()) / 2;
					index = i - cpsr.m_Slots.begin();
					cout << (int)(*i).m_Colour << ": " << (*j)->GetName() << " - " << (*j)->GetRating() << " (" << (*j)->GetPlayerValue() << ") - mean: " << balanceValue << " - gain/lose: " << gainLose[index].first << " / " << gainLose[index].second << std::endl;
				}
			}
			slot++;
		}
	}


};

#endif