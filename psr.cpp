
//#include "ghost.h"
#include "psr.h"
#include <utility>

#include <cmath>

using std::make_pair;

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

	for( unsigned int i = 0; i < m_team1.size( ); ++i )
	{
		team1weights[i] = pow( m_team1[i].second, m_TeamRankWeighting );
		team1weight += team1weights[i];
		team1totalpsr += m_team1[i].second;
	}

	for( unsigned int i = 0; i < m_team2.size( ); ++i )
	{
		team2weights[i] = pow( m_team2[i].second, m_TeamRankWeighting );
		team2weight += team2weights[i];
		team2totalpsr += m_team2[i].second;
	}

	m_team1avgpsr = team1totalpsr / m_team1.size( );
	m_team2avgpsr = team2totalpsr / m_team2.size( );

	double team1rating = pow( team1weight, (double)1.0 / m_TeamRankWeighting );
	double team2rating = pow( team2weight, (double)1.0 / m_TeamRankWeighting );

	double diff = team1rating - team2rating;

	double E = 2.7182;

	double team1winprobability = 1.0 / ( 1.0 + pow( E, ( ( -1.0 * diff ) / m_LogisticPredictionScale ) ) );
	double team2winprobability = 1.0 - team1winprobability;

	for( unsigned int i = 0; i < m_team1.size( ); ++i )
	{
		m_team1kfactors[i] = ( ( m_MedianScalingRank - m_team1[i].second ) / m_KFactorScale ) + m_BaseKFactor;
		m_team1kfactors[i] = m_team1kfactors[i] > m_MaxKFactor ? m_MaxKFactor : m_team1kfactors[i];
		m_team1kfactors[i] = m_team1kfactors[i] > m_MinKFactor ? m_team1kfactors[i] : m_MinKFactor ;

		double distancefromteamavgpsr = m_team1[i].second - 50 - m_team1avgpsr;

		if(distancefromteamavgpsr > 100 )
			distancefromteamavgpsr = 100;

		if( distancefromteamavgpsr > 0 && distancefromteamavgpsr <= 100 )
		{
			double implayingwithnewbiesfactor = ( 100.0 - distancefromteamavgpsr ) / 100;
			m_team1kfactors[i] = implayingwithnewbiesfactor * m_team1kfactors[i];
		}
	}

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