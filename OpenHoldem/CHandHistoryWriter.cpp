//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: Part of the modular handhistory-generator.
//   Not really a symbol-engine but it is convenient
//   to make use of the symbol-engine-concepts,
//   Also used for the new summary in the logs.
//
// This module collects hh-data and writes it
//   at appropriate moments to disk, both for
//   real hand-histories and for the summary in the logs.
//
//******************************************************************************

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include "CHandHistoryWriter.h"

#include "CHandHistoryDealPhase.h"
#include "CHandHistoryUncontested.h"
#include "CSessionCounter.h"
#include "CVirtualSymbolEngine.h"
#include "CEngineContainer.h"
#include "CHandresetDetector.h"
#include "CSymbolEngineActiveDealtPlaying.h"
#include "CSymbolEnginePokerval.h"
#include "CSymbolEngineUserchair.h"
#include "CSymbolEngineTableLimits.h"
#include "CSymbolEngineDealerchair.h"
#include "CBetroundCalculator.h"
#include "CSymbolEngineMTTInfo.h"
#include "CScraper.h"
#include "CSymbolEngineChipAmounts.h"
#include "CTableState.h"
#include <sstream>
#include <ctime>
#include <iostream>
#include <Windows.h>
#include <iomanip>
#include <tchar.h> 
#include "CScrapedMoney.h"
#include <stdlib.h> 





using namespace std;



bool Step1;
bool Step2;
bool Step3;
bool Step4;
bool BigBlindFound;
bool SmallBlindFound;
long int torneio;
int NrTable;



CHandHistoryWriter* p_handhistory_writer = NULL;



CHandHistoryWriter::CHandHistoryWriter() {


}

CHandHistoryWriter::~CHandHistoryWriter() {
}

void CHandHistoryWriter::InitOnStartup() {
	NrTable = rand() % 25 + 1;
	torneio = rand() * 137 + 170045000;
	HandNumber = rand() * 144 + 1289840000;




}

void CHandHistoryWriter::UpdateOnConnection() {
}

void CHandHistoryWriter::UpdateOnHandreset() {

	if (!Preferences()->handhistory_generator_enable()) {
		return;
	}


	int nchairs = p_tablemap->nchairs();
	MaxBet = 0;
	MaxActions = 0;
	Step1 = false;
	Step2 = false;
	Step3 = false;
	Step4 = false;
	BigBlindFound = false;
	SmallBlindFound = false;
	QtyPlaying = 0;
	QtyAllwin = 0;
	HandNumber++;
	WinnerPots = 0;
	MaxWinnerPots = 0;
	MinWinnerPots = 0;
	MiddleLowWinnerPots = 0;
	MiddleHighWinnerPots = 0;
	MinWinnerShow = false;
	MaxWinnerShow = false;
	FinalWinnerScan = false;
	Finish = false;
	HasFolded[0] = false;
	HasFolded[1] = false;
	HasFolded[2] = false;
	HasFolded[3] = false;
	HasFolded[4] = false;
	HasFolded[5] = false;
	HasFolded[6] = false;
	HasFolded[7] = false;
	HasFolded[8] = false;
	HasFolded[9] = false;
	PlayerStartRound[0] = false;
	PlayerStartRound[1] = false;
	PlayerStartRound[2] = false;
	PlayerStartRound[3] = false;
	PlayerStartRound[4] = false;
	PlayerStartRound[5] = false;
	PlayerStartRound[6] = false;
	PlayerStartRound[7] = false;
	PlayerStartRound[8] = false;
	PlayerStartRound[9] = false;
	Actions[0] = 0;
	Actions[1] = 0;
	Actions[2] = 0;
	Actions[3] = 0;
	Actions[4] = 0;
	Actions[5] = 0;
	Actions[6] = 0;
	Actions[7] = 0;
	Actions[8] = 0;
	Actions[9] = 0;
	PreviusBet[0] = 0;
	PreviusBet[1] = 0;
	PreviusBet[2] = 0;
	PreviusBet[3] = 0;
	PreviusBet[4] = 0;
	PreviusBet[5] = 0;
	PreviusBet[6] = 0;
	PreviusBet[7] = 0;
	PreviusBet[8] = 0;
	PreviusBet[9] = 0;
	PlayerWinner[0] = false;
	PlayerWinner[1] = false;
	PlayerWinner[2] = false;
	PlayerWinner[3] = false;
	PlayerWinner[4] = false;
	PlayerWinner[5] = false;
	PlayerWinner[6] = false;
	PlayerWinner[7] = false;
	PlayerWinner[8] = false;
	PlayerWinner[9] = false;
	IsAllWin[0] = false;
	IsAllWin[1] = false;
	IsAllWin[2] = false;
	IsAllWin[3] = false;
	IsAllWin[4] = false;
	IsAllWin[5] = false;
	IsAllWin[6] = false;
	IsAllWin[7] = false;
	IsAllWin[8] = false;
	IsAllWin[9] = false;
	IsSmallBlind[0] = false;
	IsSmallBlind[1] = false;
	IsSmallBlind[2] = false;
	IsSmallBlind[3] = false;
	IsSmallBlind[4] = false;
	IsSmallBlind[5] = false;
	IsSmallBlind[6] = false;
	IsSmallBlind[7] = false;
	IsSmallBlind[8] = false;
	IsSmallBlind[9] = false;
	IsBigBlind[0] = false;
	IsBigBlind[1] = false;
	IsBigBlind[2] = false;
	IsBigBlind[3] = false;
	IsBigBlind[4] = false;
	IsBigBlind[5] = false;
	IsBigBlind[6] = false;
	IsBigBlind[7] = false;
	IsBigBlind[8] = false;
	IsBigBlind[9] = false;
	TotalBetsOnGame = 0;
	PlayerPotShow[0] = false;
	PlayerPotShow[1] = false;
	PlayerPotShow[2] = false;
	PlayerPotShow[3] = false;
	PlayerPotShow[4] = false;
	PlayerPotShow[5] = false;
	PlayerPotShow[6] = false;
	PlayerPotShow[7] = false;
	PlayerPotShow[8] = false;
	PlayerPotShow[9] = false;
	PlayerShowDown[0] = false;
	PlayerShowDown[1] = false;
	PlayerShowDown[2] = false;
	PlayerShowDown[3] = false;
	PlayerShowDown[4] = false;
	PlayerShowDown[5] = false;
	PlayerShowDown[6] = false;
	PlayerShowDown[7] = false;
	PlayerShowDown[8] = false;
	PlayerShowDown[9] = false;
	PlayerNames[0] = "";
	PlayerNames[1] = "";
	PlayerNames[2] = "";
	PlayerNames[3] = "";
	PlayerNames[4] = "";
	PlayerNames[5] = "";
	PlayerNames[6] = "";
	PlayerNames[7] = "";
	PlayerNames[8] = "";
	PlayerNames[9] = "";
	PlayerCards[0] = "";
	PlayerCards[1] = "";
	PlayerCards[2] = "";
	PlayerCards[3] = "";
	PlayerCards[4] = "";
	PlayerCards[5] = "";
	PlayerCards[6] = "";
	PlayerCards[7] = "";
	PlayerCards[8] = "";
	PlayerCards[9] = "";
	PlayerCollect[0] = false;
	PlayerCollect[1] = false;
	PlayerCollect[2] = false;
	PlayerCollect[3] = false;
	PlayerCollect[4] = false;
	PlayerCollect[5] = false;
	PlayerCollect[6] = false;
	PlayerCollect[7] = false;
	PlayerCollect[8] = false;
	PlayerCollect[9] = false;
	






}

void CHandHistoryWriter::UpdateOnNewRound() {
	if (!Preferences()->handhistory_generator_enable() || Finish) {
		return;
	}

	if (Step1 && !Winner()) {
		int dealerchair = p_engine_container->symbol_engine_dealerchair()->dealerchair();
		int	userchair = p_engine_container->symbol_engine_userchair()->userchair();
		int nchairs = p_tablemap->nchairs();
		int betround = p_betround_calculator->betround();
		int nplayeractive = p_engine_container->symbol_engine_active_dealt_playing()->nplayersactive();
		double sblind = p_engine_container->symbol_engine_tablelimits()->sblind();
		double bblind = p_engine_container->symbol_engine_tablelimits()->bblind();
		double ante = p_engine_container->symbol_engine_tablelimits()->ante();
		int nopponentsdealt = p_engine_container->symbol_engine_active_dealt_playing()->nopponentsdealt();
		double pot = p_engine_container->symbol_engine_chip_amounts()->pot();


		ofstream outfile;
		CreateDirectory("handhistory", NULL);
		stringstream ss;
		ss << "handhistory/" << TornamentName() << ".txt";
		string s = ss.str() + " ";
		outfile.open(s.c_str(), fstream::app);
		outfile.precision(2);
		outfile.setf(ios::fixed, ios::floatfield);

		//Vericar Check e Fold sobrando;
		int last_chair = (2 * nchairs) + (dealerchair);
		int first_chair = (nchairs + dealerchair + 1);
		for (int i = first_chair; i <= last_chair; ++i) {
			double balance = p_table_state->Player(i % nchairs)->_balance.GetValue();
			double currentbet = PlayerStack[i % nchairs] - balance + PreviusBet[i % nchairs];
			// CALL ULTIMO JOGADOR
			if (!HasFolded[i % nchairs] && PlayerStartRound[i % nchairs] && PlayerStack[i % nchairs] > balance && PreviusBet[i % nchairs] != MaxBet && !IsAllWin[i % nchairs]) {
				TotalBetsOnGame += PlayerStack[i % nchairs] - balance;
				outfile << PlayerNames[i % nchairs] << " calls [" << PlayerStack[i % nchairs] - balance << "] - NewRound" << " PreviusBet=" << PreviusBet[i % nchairs] << " TotalBetonGame=" << TotalBetsOnGame << " Pot=" << pot << " PlayerStack=" << PlayerStack[i % nchairs] << " Balance=" << balance << " QtyPlayer=" << QtyPlaying << " PotVsTotalBet=" << PotVsTotalBet() << endl;
				PreviusBet[i % nchairs] = currentbet;
				PlayerStack[i % nchairs] = balance;
				Actions[i % nchairs]++;
				QtyActionsRound++;
			}
			// FOLD LAST PLAYER
			else if (!HasFolded[i % nchairs] && PlayerStartRound[i % nchairs] && !p_table_state->Player(i % nchairs)->HasAnyCards() && !IsAllWin[i % nchairs] && QtyPlaying >= 1 && PreviusBet[i % nchairs] != MaxBet && MaxBet > 0 && MaxActions > 0) {
				HasFolded[i % nchairs] = true;
				outfile << PlayerNames[i % nchairs] << " folds - Last Player" << " PreviusBet=" << PreviusBet[i % nchairs] << " TotalBetonGame=" << TotalBetsOnGame << " Pot=" << pot << " PlayerStack=" << PlayerStack[i % nchairs] << " Balance=" << balance << " QtyPlayer=" << QtyPlaying << " PotVsTotalBet=" << PotVsTotalBet() << endl;
				QtyPlaying--;
			}
			// ALLWIN ULTIMO JOGADOR
			else if (!HasFolded[i % nchairs] && (balance == 0) && PlayerStartRound[i % nchairs] && !IsAllWin[i % nchairs] && currentbet > PreviusBet[i % nchairs] && PlayerStack[i % nchairs] > 0) {
				TotalBetsOnGame += PlayerStack[i % nchairs] - balance;
				outfile << PlayerNames[i % nchairs] << " is all-In [" << PlayerStack[i % nchairs] - balance << "] - NewRound" << " PreviusBet=" << PreviusBet[i % nchairs] << " TotalBetonGame=" << TotalBetsOnGame << " Pot=" << pot << " PlayerStack=" << PlayerStack[i % nchairs] << " Balance=" << balance << " QtyPlayer=" << QtyPlaying << " PotVsTotalBet=" << PotVsTotalBet() << endl;
				PreviusBet[i % nchairs] = currentbet;
				PlayerStack[i % nchairs] = balance;
				QtyAllwin++;
				IsAllWin[i % nchairs] = true;
				Actions[i % nchairs]++;
				QtyActionsRound++;
				if (currentbet > MaxBet) {
					MaxBet = currentbet;
				}

				if (Actions[i % nchairs] > MaxActions) {
					MaxActions = Actions[i % nchairs];
				}
			}
			// CHECK ULTIMO JOGADOR PREFLOP
			else if (!HasFolded[i % nchairs] && PlayerStartRound[i % nchairs] && (PreviusBet[i % nchairs] == MaxBet && betround == 0) && !IsAllWin[i % nchairs] && Actions[i % nchairs] < MaxActions) {
				outfile << PlayerNames[i % nchairs] << " checks - NewRound" << " PreviusBet=" << PreviusBet[i % nchairs] << " TotalBetonGame=" << TotalBetsOnGame << " Pot=" << pot << " PlayerStack=" << PlayerStack[i % nchairs] << " Balance=" << balance << " QtyPlayer=" << QtyPlaying << " PotVsTotalBet=" << PotVsTotalBet() << endl;
				Actions[i % nchairs]++;
				QtyActionsRound++;
			}

			// CHECK ALL PLAYERS
			else if (!ShowAllinCards() && MaxBet == 0 && !HasFolded[i % nchairs] && MaxActions == 0 && PlayerStartRound[i % nchairs]) {
				outfile << PlayerNames[i % nchairs] << " checks - all check NewRound" << " PreviusBet=" << PreviusBet[i % nchairs] << " TotalBetonGame=" << TotalBetsOnGame << " Pot=" << pot << " PlayerStack=" << PlayerStack[i % nchairs] << " Balance=" << balance << " QtyPlayer=" << QtyPlaying << " PotVsTotalBet=" << PotVsTotalBet() << endl;
				Actions[i % nchairs]++;
				QtyActionsRound++;
			}


		}

		MaxActions = 0;
		QtyActionsRound = 0;
		MaxBet = 0;
		PreviusBet[0] = 0;
		PreviusBet[1] = 0;
		PreviusBet[2] = 0;
		PreviusBet[3] = 0;
		PreviusBet[4] = 0;
		PreviusBet[5] = 0;
		PreviusBet[6] = 0;
		PreviusBet[7] = 0;
		PreviusBet[8] = 0;
		PreviusBet[9] = 0;
		Actions[0] = 0;
		Actions[1] = 0;
		Actions[2] = 0;
		Actions[3] = 0;
		Actions[4] = 0;
		Actions[5] = 0;
		Actions[6] = 0;
		Actions[7] = 0;
		Actions[8] = 0;
		Actions[9] = 0;
		IsBigBlind[0] = false;
		IsBigBlind[1] = false;
		IsBigBlind[2] = false;
		IsBigBlind[3] = false;
		IsBigBlind[4] = false;
		IsBigBlind[5] = false;
		IsBigBlind[6] = false;
		IsBigBlind[7] = false;
		IsBigBlind[8] = false;
		IsBigBlind[9] = false;
		IsSmallBlind[0] = false;
		IsSmallBlind[1] = false;
		IsSmallBlind[2] = false;
		IsSmallBlind[3] = false;
		IsSmallBlind[4] = false;
		IsSmallBlind[5] = false;
		IsSmallBlind[6] = false;
		IsSmallBlind[7] = false;
		IsSmallBlind[8] = false;
		IsSmallBlind[9] = false;





		// INICIO DO FLOP
		if (betround >= kBetroundFlop && p_table_state->CommonCards(0)->IsKnownCard() && !p_table_state->CommonCards(3)->IsKnownCard()) {
			outfile << "** Dealing Flop ** [";
			outfile << " " << p_table_state->CommonCards(0)->ToString() << " ,";
			outfile << " " << p_table_state->CommonCards(1)->ToString() << " ,";
			outfile << " " << p_table_state->CommonCards(2)->ToString() << " ]" << endl;


		}

		// INICIO DO TURN
		if (betround >= kBetroundTurn && p_table_state->CommonCards(3)->IsKnownCard() && !p_table_state->CommonCards(4)->IsKnownCard()) {
			outfile << "** Dealing Turn ** [ " << p_table_state->TurnCard()->ToString() << " ]" << endl;

		}

		// INICIO DO RIVER
		if (betround >= kBetroundRiver && p_table_state->CommonCards(4)->IsKnownCard()) {
			outfile << "** Dealing River ** [ " << p_table_state->RiverCard()->ToString() << " ]" << endl;

		}

	}
}

void CHandHistoryWriter::UpdateOnMyTurn() {

}
void CHandHistoryWriter::UpdateOnHeartbeat() {
	if (!Preferences()->handhistory_generator_enable() || Finish) {
		return;
	}

	int betround = p_betround_calculator->betround();
	int	userchair = p_engine_container->symbol_engine_userchair()->userchair();
	int dealerchair = p_engine_container->symbol_engine_dealerchair()->dealerchair();
	int nchairs = p_tablemap->nchairs();
	int nplayeractive = p_engine_container->symbol_engine_active_dealt_playing()->nplayersactive();
	double sblind = p_engine_container->symbol_engine_tablelimits()->sblind();
	double bblind = p_engine_container->symbol_engine_tablelimits()->bblind();
	double ante = p_engine_container->symbol_engine_tablelimits()->ante();
	int nopponentsdealt = p_engine_container->symbol_engine_active_dealt_playing()->nopponentsdealt();
	int last_chair = (2 * nchairs) + (dealerchair);
	int first_chair = (nchairs + dealerchair + 1);
	double pot = p_engine_container->symbol_engine_chip_amounts()->pot();


	ofstream outfile;
	CreateDirectory("handhistory", NULL);
	stringstream ss;
	ss << "handhistory/" << TornamentName() << ".txt";
	string s = ss.str() + " ";
	outfile.open(s.c_str(), fstream::app);
	outfile.precision(2);
	outfile.setf(ios::fixed, ios::floatfield);
	if ((!Step1) && DealPhase() && (betround >= kBetroundPreflop) && (p_table_state->Player(dealerchair)->hole_cards(1)->IsAnyCard()) && p_table_state->Player(dealerchair)->name() != "") {

		outfile << "Game #" << HandNumber << " starts." << endl; // << p_handreset_detector->GetHandNumber()
		outfile << endl;
		outfile << "#Game No :" << HandNumber << endl; // p_handreset_detector->GetHandNumber()
		outfile << "***** Hand History for Game " << HandNumber << " *****" << endl;  // p_handreset_detector->GetHandNumber() << " *****" << endl;
		outfile << "NL Texas Hold'em $1.10 USD Buy-in Trny:" << torneio << " Level:7  " << "Blinds-Antes(" << sblind << "/" << bblind << " -" << ante << ") - " << setDate() << endl;                   //setDate() << endl; << " Monday, October 14, 22:59:22 BRT 2019 - "
		outfile << TornamentName() << " (Real Money)" << endl;

		for (int i = 0; i < p_tablemap->nchairs(); i++) {
			if (p_table_state->Player(i)->dealer()) {
				outfile << "Seat " << (i + 1) << " is the button" << endl;

			}
		}

		outfile << "Total number of players : " << nplayeractive << "/" << nchairs << endl;
		for (int i = 0; i < nchairs; ++i) {
			int chair = i;
			double balance = p_table_state->Player(i)->_balance.GetValue();
			if (p_table_state->Player(i)->seated()) {
				if (IsBigBlind[i]) {
					outfile << "Seat " << (i + 1) << ": " << p_table_state->Player(chair)->name() << " ( " << balance + bblind + ante << " ) " << " QtyPlayer=" << QtyPlaying << endl;
					PlayerStack[i] = balance + bblind + ante;
					PlayerNames[i] = p_table_state->Player(chair)->name();
					
					
				}
				else if (IsSmallBlind[i]) {
					outfile << "Seat " << (i + 1) << ": " << p_table_state->Player(chair)->name() << " ( " << balance + sblind + ante << " ) " << " QtyPlayer=" << QtyPlaying << endl;
					PlayerStack[i] = balance + sblind + ante;
					PlayerNames[i] = p_table_state->Player(chair)->name();
					
					
				}
				else if(!IsSmallBlind[i] && !IsBigBlind[i])
				outfile << "Seat " << (i + 1) << ": " << p_table_state->Player(chair)->name() << " ( " << balance + ante << " ) " << " QtyPlayer=" << QtyPlaying << endl;
				PlayerStack[i] = balance + ante;
				PlayerNames[i] = p_table_state->Player(chair)->name();
				
				
			}
		}
		outfile << "Trny:" << torneio << " Level:7 " << endl;
		outfile << "Blinds-Antes(" << sblind << "/" << bblind << " -" << ante << ")" << endl;

		for (int i = 0; i < nchairs; ++i) {
			double balance = p_table_state->Player(i)->_balance.GetValue();
			int chair = i;
			if (p_table_state->Player(i)->seated() && p_engine_container->symbol_engine_tablelimits()->ante() > 0) {
				TotalBetsOnGame += ante;
				PlayerStack[i] -= ante;
				outfile << PlayerNames[chair] << " posts ante [" << ante << "]" << " PreviusBet=" << PreviusBet[i % nchairs] << " TotalBetonGame=" << TotalBetsOnGame << " Pot=" << pot << " PlayerStack=" << PlayerStack[i % nchairs] << " Balance=" << balance << " QtyPlayer=" << QtyPlaying << " PotVsTotalBet=" << PotVsTotalBet() << endl;
				QtyPlaying++;
				PlayerStartRound[i] = true; 
			}

		}


		Step1 = true;
	}
	if ((Step1) && (!Step2) && (p_table_state->Player(dealerchair)->hole_cards(1)->IsAnyCard())) {
		ofstream outfile;
		CreateDirectory("handhistory", NULL);
		stringstream ss;
		ss << "handhistory/" << TornamentName() << ".txt";
		string s = ss.str() + " ";
		outfile.open(s.c_str(), fstream::app);
		outfile.precision(2);
		outfile.setf(ios::fixed, ios::floatfield);
		for (int i = first_chair; i <= last_chair; ++i) {
			double balance = p_table_state->Player(i % nchairs)->_balance.GetValue();
			double currentbet = p_table_state->Player(i % nchairs)->_bet.GetValue();
			if (IsSmallBlind[i % nchairs]) {
				TotalBetsOnGame += sblind;
				PreviusBet[i % nchairs] = sblind;
				IsSmallBlind[i % nchairs] = true;
				outfile << PlayerNames[i % nchairs] << " posts small blind [" << sblind << "]." << " PreviusBet=" << PreviusBet[i % nchairs] << " TotalBetonGame=" << TotalBetsOnGame << " Pot=" << pot << " PlayerStack=" << PlayerStack[i % nchairs] << " Balance=" << balance << " QtyPlayer=" << QtyPlaying << " PotVsTotalBet=" << PotVsTotalBet() << endl;



			}
			if (IsBigBlind[i % nchairs]) {
				TotalBetsOnGame += bblind;
				MaxBet = bblind;
				BigBlindFound = true;
				PreviusBet[i % nchairs] = bblind;
				IsBigBlind[i % nchairs] = true;
				outfile << PlayerNames[i % nchairs] << " posts big blind [" << bblind << "]." << " PreviusBet=" << PreviusBet[i % nchairs] << " TotalBetonGame=" << TotalBetsOnGame << " Pot=" << pot << " PlayerStack=" << PlayerStack[i % nchairs] << " Balance=" << balance << " QtyPlayer=" << QtyPlaying << " PotVsTotalBet=" << PotVsTotalBet() << endl;




			}
		}
		if (SmallBlindFound && BigBlindFound) {
			outfile << "** Dealing down cards **" << endl;
			if (CheckUser()) {
				outfile << "Dealt to " << p_table_state->Player(UserChair)->name() << " [  " << p_table_state->Player(UserChair)->hole_cards(0)->ToString() << " " << p_table_state->Player(UserChair)->hole_cards(1)->ToString() << " ]" << endl;
				PlayerCards[UserChair] = " [ " + p_table_state->Player(UserChair)->hole_cards(0)->ToString() + " , " + p_table_state->Player(UserChair)->hole_cards(1)->ToString() + " ]";
				
			}
			Step2 = true;
		}
	}
	// SCAN PLAYERS
	if (Step2) {
		for (int i = first_chair; i <= last_chair; ++i) {
			double balance = p_table_state->Player(i % nchairs)->_balance.GetValue();
			double currentbet = PlayerStack[i % nchairs] - balance + PreviusBet[i % nchairs];
			Showdown();
			//WINNER
			if (Winner()) {
				if (!FinalWinnerScan) {
					for (int i = first_chair; i <= last_chair; ++i) {
						double balance = p_table_state->Player(i)->_balance.GetValue();
						double currentbet = p_table_state->Player(i % nchairs)->_bet.GetValue();
						// CALL ULTIMO JOGADOR
						if (!HasFolded[i % nchairs] && PlayerStartRound[i & nchairs] && (pot - TotalBetsOnGame == MaxBet - PreviusBet[i % nchairs]) && !IsAllWin[i % nchairs] && MaxActions > 0 && Actions[i % nchairs] <= MaxActions && PreviusBet[i % nchairs] != MaxBet) {
							TotalBetsOnGame += MaxBet - PreviusBet[i % nchairs];
							outfile << PlayerNames[i % nchairs] << " calls [" << MaxBet - PreviusBet[i % nchairs] << "] - Winner Call" << " PreviusBet=" << PreviusBet[i % nchairs] << " TotalBetonGame=" << TotalBetsOnGame << " Pot=" << pot << " PlayerStack=" << PlayerStack[i % nchairs] << " Balance=" << balance << " QtyPlayer=" << QtyPlaying << " PotVsTotalBet=" << PotVsTotalBet() << endl;
							PreviusBet[i % nchairs] = currentbet;
							PlayerStack[i % nchairs] = balance;
							Actions[i % nchairs]++;
							QtyActionsRound++;

						}
						// ALLWIN ULTIMO JOGADOR
						else if (!HasFolded[i % nchairs] && (p_table_state->Player(i % nchairs)->_balance.GetValue() == 0) && PlayerStartRound[i % nchairs] && !IsAllWin[i % nchairs] && currentbet > PreviusBet[i % nchairs] && PlayerStack[i % nchairs]>0) {
							TotalBetsOnGame += PlayerStack[i % nchairs];
							outfile << PlayerNames[i % nchairs] << " is all-In [" << PlayerStack[i % nchairs] << "] - Winner All-in" << " PreviusBet=" << PreviusBet[i % nchairs] << " TotalBetonGame=" << TotalBetsOnGame << " Pot=" << pot << " PlayerStack=" << PlayerStack[i % nchairs] << " Balance=" << balance << " QtyPlayer=" << QtyPlaying << " PotVsTotalBet=" << PotVsTotalBet() << endl;
							PreviusBet[i % nchairs] = currentbet;
							PlayerStack[i % nchairs] = balance;
							QtyAllwin++;
							Actions[i % nchairs]++;
							QtyActionsRound++;

						}
						// CHECK ULTIMO JOGADOR PREFLOP
						else if (!HasFolded[i % nchairs] && PlayerStartRound[i % nchairs] && (PreviusBet[i % nchairs] == MaxBet && betround == 0) && !IsAllWin[i % nchairs] && Actions[i % nchairs] < MaxActions) {
							outfile << PlayerNames[i % nchairs] << " checks - Winner Check" << " PreviusBet=" << PreviusBet[i % nchairs] << " TotalBetonGame=" << TotalBetsOnGame << " Pot=" << pot << " PlayerStack=" << PlayerStack[i % nchairs] << " Balance=" << balance << " QtyPlayer=" << QtyPlaying << " PotVsTotalBet=" << PotVsTotalBet() << endl;
							Actions[i % nchairs]++;
							QtyActionsRound++;
						}
						// CHECK ALL PLAYERS
						else if (!ShowAllinCards() && MaxBet == 0 && !HasFolded[i % nchairs] && MaxActions == 0 && PlayerStartRound[i % nchairs]) {
							outfile << PlayerNames[i % nchairs] << " checks - Winner all check" << " PreviusBet=" << PreviusBet[i % nchairs] << " TotalBetonGame=" << TotalBetsOnGame << " Pot=" << pot << " PlayerStack=" << PlayerStack[i % nchairs] << " Balance=" << balance << " QtyPlayer=" << QtyPlaying << " PotVsTotalBet=" << PotVsTotalBet() << endl;
							Actions[i % nchairs]++;
							QtyActionsRound++;
						}
						// FOLD LAST PLAYER
						else if (!HasFolded[i % nchairs] && PlayerStartRound[i % nchairs] && !p_table_state->Player(i % nchairs)->HasAnyCards() && !IsAllWin[i % nchairs] && PreviusBet[i % nchairs] != MaxBet && MaxBet > 0) {
							HasFolded[i % nchairs] = true;
							outfile << PlayerNames[i % nchairs] << " folds - Winner fold" << " PreviusBet=" << PreviusBet[i % nchairs] << " TotalBetonGame=" << TotalBetsOnGame << " Pot=" << pot << " PlayerStack=" << PlayerStack[i % nchairs] << " Balance=" << balance << " QtyPlayer=" << QtyPlaying << " PotVsTotalBet=" << PotVsTotalBet() << endl;
							QtyPlaying--;
						}

					}
					FinalWinnerScan = true;
				}
				if (QtyWinnerPots == 1) {
					for (int i = first_chair; i <= last_chair; ++i) {
						double currentbet = p_table_state->Player(i % nchairs)->_bet.GetValue();
						if (!HasFolded[i % nchairs] && !PlayerShowDown[i % nchairs] && PlayerCollect[i % nchairs]) {
							outfile << PlayerNames[i % nchairs] << " shows "<< PlayerCards[i % nchairs] << "  chips with  a straight flush, Nine to King." << endl;
							PlayerShowDown[i % nchairs] = true;
							
						}

					}
					for (int i = first_chair; i <= last_chair; ++i) {
						double currentbet = p_table_state->Player(i % nchairs)->_bet.GetValue();
						if (!HasFolded[i % nchairs] && PlayerStartRound[i % nchairs] && PlayerWinner[i % nchairs] && !PlayerPotShow[i % nchairs]) {
							outfile << PlayerNames[i % nchairs] << " wins " << TotalBetsOnGame << " chips from the side main pot with  a straight flush, Nine to King." << " Pot=" << pot << " MaxBet=" << MaxBet << " PreviusBet=" << PreviusBet[i % nchairs] << " PotVsTotalBet=" << PotVsTotalBet() << "Qtypots=" << QtyWinnerPots << endl;
							PlayerPotShow[i % nchairs] = true;
						}

					}
					Finish = true;
				}
				if (QtyWinnerPots == 2) {
					for (int i = first_chair; i <= last_chair; ++i) {
						double currentbet = p_table_state->Player(i % nchairs)->_bet.GetValue();
						if (!HasFolded[i % nchairs] && !PlayerShowDown[i % nchairs] && PlayerCollect[i % nchairs]) {
							outfile << PlayerNames[i % nchairs] << " shows " << PlayerCards[i % nchairs] << "  chips with  a straight flush, Nine to King." << endl;
							PlayerShowDown[i % nchairs] = true;

						}
					}
					for (int i = first_chair; i <= last_chair; ++i) {
						double currentbet = p_table_state->Player(i % nchairs)->_bet.GetValue();
						if (!HasFolded[i % nchairs] && PlayerStartRound[i % nchairs] && !MinWinnerShow && PlayerWinner[i % nchairs] && currentbet == MinWinnerPots && !PlayerPotShow[i % nchairs]) {
							outfile << PlayerNames[i % nchairs] << " wins " << MinWinnerPots << " chips from the side pot 1 with  a straight flush, Nine to King." << "QtyPots=" << QtyWinnerPots << endl;
							MinWinnerShow = true;
							PlayerPotShow[i % nchairs] = true;
						}
						else if (!HasFolded[i % nchairs] && PlayerStartRound[i % nchairs] && !MaxWinnerShow && PlayerWinner[i % nchairs] && currentbet == MaxWinnerPots && !PlayerPotShow[i % nchairs]) {
							outfile << PlayerNames[i % nchairs] << " wins " << MaxWinnerPots << " chips from the main pot with  a straight flush, Nine to King." << endl;
							MaxWinnerShow = true;
							PlayerPotShow[i % nchairs] = true;
						}

					}
					Finish = true;
				}
				if (QtyWinnerPots == 3) {
					for (int i = first_chair; i <= last_chair; ++i) {
						double currentbet = p_table_state->Player(i % nchairs)->_bet.GetValue();
						if (!HasFolded[i % nchairs] && !PlayerShowDown[i % nchairs] && PlayerCollect[i % nchairs]) {
							outfile << PlayerNames[i % nchairs] << " shows " << PlayerCards[i % nchairs] << "  chips with  a straight flush, Nine to King." << endl;
							PlayerShowDown[i % nchairs] = true;
						}
					}
					for (int i = first_chair; i <= last_chair; ++i) {
						double currentbet = p_table_state->Player(i % nchairs)->_bet.GetValue();
						if (!HasFolded[i % nchairs] && PlayerStartRound[i % nchairs] && !PlayerPotShow[i % nchairs] && !MinWinnerShow && PlayerWinner[i % nchairs] && currentbet == MinWinnerPots) {
							outfile << PlayerNames[i % nchairs] << " wins " << MinWinnerPots << " chips from the side pot 1 with  a straight flush, Nine to King." << endl;
							MinWinnerShow = true;
							PlayerPotShow[i % nchairs] = true;
						}
						else if (!HasFolded[i % nchairs] && PlayerStartRound[i % nchairs] && !PlayerPotShow[i % nchairs] && !MiddleLowWinnerShow && PlayerWinner[i % nchairs] && currentbet == MiddleLowWinnerPots) {
							outfile << PlayerNames[i % nchairs] << " wins " << MiddleLowWinnerPots << " chips from the side pot 2 with  a straight flush, Nine to King." << endl;
							MiddleLowWinnerShow = true;
							PlayerPotShow[i % nchairs] = true;
						}
						else if (!HasFolded[i % nchairs] && PlayerStartRound[i % nchairs] && !PlayerPotShow[i % nchairs] && !MaxWinnerShow && PlayerWinner[i % nchairs] && currentbet == MaxWinnerPots) {
							outfile << PlayerNames[i % nchairs] << " wins " << MaxWinnerPots << " chips from the main pot with  a straight flush, Nine to King." << endl;
							MaxWinnerShow = true;
							PlayerPotShow[i % nchairs] = true;
						}


					}
					Finish = true;
				}
				if (QtyWinnerPots == 4) {
					for (int i = first_chair; i <= last_chair; ++i) {
						double currentbet = p_table_state->Player(i % nchairs)->_bet.GetValue();
						if (!HasFolded[i % nchairs] && !PlayerShowDown[i % nchairs] && PlayerCollect[i % nchairs]) {
							outfile << PlayerNames[i % nchairs] << " shows " << PlayerCards[i % nchairs] << "  chips with  a straight flush, Nine to King." << endl;
							PlayerShowDown[i % nchairs] = true;
						}
					}
					for (int i = first_chair; i <= last_chair; ++i) {
						double currentbet = p_table_state->Player(i % nchairs)->_bet.GetValue();
						if (!HasFolded[i % nchairs] && PlayerStartRound[i % nchairs] && !PlayerPotShow[i % nchairs] && !MinWinnerShow && PlayerWinner[i % nchairs] && currentbet == MinWinnerPots) {
							outfile << PlayerNames[i % nchairs] << " wins " << MinWinnerPots << " chips from the side pot 1" << endl;
							MinWinnerShow = true;
							PlayerPotShow[i % nchairs] = true;
						}
						else if (!HasFolded[i % nchairs] && PlayerStartRound[i % nchairs] && !PlayerPotShow[i % nchairs] && !MiddleLowWinnerShow && PlayerWinner[i % nchairs] && currentbet == MiddleLowWinnerPots) {
							outfile << PlayerNames[i % nchairs] << " wins " << MiddleLowWinnerPots << " chips from the side pot 2" << endl;
							MiddleLowWinnerShow = true;
							PlayerPotShow[i % nchairs] = true;
						}
						else if (!HasFolded[i % nchairs] && PlayerStartRound[i % nchairs] && !PlayerPotShow[i % nchairs] && !MiddleHighWinnerShow && PlayerWinner[i % nchairs] && currentbet == MiddleHighWinnerPots) {
							outfile << PlayerNames[i % nchairs] << " wins " << MiddleHighWinnerPots << " chips from the side pot 3" << endl;
							MiddleHighWinnerShow = true;
							PlayerPotShow[i % nchairs] = true;
						}
						else if (!HasFolded[i % nchairs] && PlayerStartRound[i % nchairs] && !PlayerPotShow[i % nchairs] && !MaxWinnerShow && PlayerWinner[i % nchairs] && currentbet == MaxWinnerPots) {
							outfile << PlayerNames[i % nchairs] << " wins " << MaxWinnerPots << " chips from the main pot" << endl;
							MaxWinnerShow = true;
							PlayerPotShow[i % nchairs] = true;
						}

					}
					Finish = true;
				}
			}
			//SCAN - FOLD
			else if (!ShowAllinCards() && !HasFolded[i % nchairs] && PlayerStartRound[i % nchairs] && !p_table_state->Player(i % nchairs)->HasAnyCards() && !IsAllWin[i % nchairs] && QtyPlaying >= 1 && PreviusBet[i % nchairs] != MaxBet && MaxBet > 0) {
				HasFolded[i % nchairs] = true;
				outfile << PlayerNames[i % nchairs] << " folds - Scan Fold" << " PreviusBet=" << PreviusBet[i % nchairs] << " TotalBetonGame=" << TotalBetsOnGame << " Pot=" << pot << " PlayerStack=" << PlayerStack[i % nchairs] << " Balance=" << balance << " QtyPlayer=" << QtyPlaying << " PotVsTotalBet=" << PotVsTotalBet() << endl;
				QtyPlaying--;
			}
			//SCAN - ALLWIN
			else if (!ShowAllinCards() && !HasFolded[i % nchairs] && (balance == 0) && PlayerStartRound[i % nchairs] && !IsAllWin[i % nchairs] && currentbet > PreviusBet[i % nchairs] && PlayerStack[i % nchairs] > 0) {
				ActualChair = i;
				for (i = first_chair; i < ActualChair; ++i) {
					if (!HasFolded[i % nchairs] && PlayerStartRound[i % nchairs] && !IsAllWin[i % nchairs] && Actions[i % nchairs] == Actions[ActualChair % nchairs] && !IsSmallBlind[i % nchairs] && !IsBigBlind[i % nchairs]) {
						outfile << PlayerNames[i % nchairs] << " checks - Scan Allwin" << " PreviusBet=" << PreviusBet[i % nchairs] << " TotalBetonGame=" << TotalBetsOnGame << " Pot=" << pot << " PlayerStack=" << PlayerStack[i % nchairs] << " Balance=" << balance << " QtyPlayer=" << QtyPlaying << " PotVsTotalBet=" << PotVsTotalBet() << endl;
						Actions[i % nchairs]++;
						QtyActionsRound++;
					}

				}
				TotalBetsOnGame += PlayerStack[i % nchairs] - balance;
				outfile << PlayerNames[i % nchairs] << " is all-In [" << PlayerStack[i % nchairs] << "]" << " PreviusBet=" << PreviusBet[i % nchairs] << " TotalBetonGame=" << TotalBetsOnGame << " Pot=" << pot << " PlayerStack=" << PlayerStack[i % nchairs] << " Balance=" << balance << " QtyPlayer=" << QtyPlaying << " PotVsTotalBet=" << PotVsTotalBet() << endl;
				PreviusBet[i % nchairs] = currentbet;
				PlayerStack[i % nchairs] = balance;
				IsAllWin[i % nchairs] = true;
				QtyAllwin++;
				Actions[i % nchairs]++;
				QtyActionsRound++;
				if (currentbet > MaxBet) {
					MaxBet = currentbet;
				}

				if (Actions[i % nchairs] > MaxActions) {
					MaxActions = Actions[i % nchairs];
				}
			}
			//SCAN - CALL
			else if (!ShowAllinCards() && !HasFolded[i % nchairs] && (currentbet != PreviusBet[i % nchairs]) && currentbet != 0 && (currentbet == MaxBet) && PlayerStartRound[i % nchairs]) {
				ActualChair = i;
				for (i = first_chair; i < ActualChair; ++i) {
					if (!HasFolded[i % nchairs] && PlayerStartRound[i % nchairs] && !IsAllWin[i % nchairs] && Actions[i % nchairs] == Actions[ActualChair % nchairs] && !IsSmallBlind[i % nchairs] && !IsBigBlind[i % nchairs]) {
						outfile << PlayerNames[i % nchairs] << " checks - Scan Call" << " PreviusBet=" << PreviusBet[i % nchairs] << " TotalBetonGame=" << TotalBetsOnGame << " Pot=" << pot << " PlayerStack=" << PlayerStack[i % nchairs] << " Balance=" << balance << " QtyPlayer=" << QtyPlaying << " PotVsTotalBet=" << PotVsTotalBet() << endl;
						Actions[i % nchairs]++;
						QtyActionsRound++;
					}

				}
				TotalBetsOnGame += MaxBet - PreviusBet[i % nchairs];
				outfile << PlayerNames[i % nchairs] << " calls [" << MaxBet - PreviusBet[i % nchairs] << "]" << " PreviusBet=" << PreviusBet[i % nchairs] << " TotalBetonGame=" << TotalBetsOnGame << " Pot=" << pot << " PlayerStack=" << PlayerStack[i % nchairs] << " Balance=" << balance << " QtyPlayer=" << QtyPlaying << " PotVsTotalBet=" << PotVsTotalBet() << endl;
				PreviusBet[i % nchairs] = currentbet;
				PlayerStack[i % nchairs] = balance;
				Actions[i % nchairs]++;
				QtyActionsRound++;
				if (Actions[i % nchairs] > MaxActions) {
					MaxActions = Actions[i % nchairs];
				}
			}
			//SCAN - RAISES
			else if (!ShowAllinCards() && !HasFolded[i % nchairs] && (currentbet != PreviusBet[i % nchairs]) && currentbet > MaxBet && currentbet > bblind && !IsAllWin[i % nchairs] && PlayerStartRound[i % nchairs]) {
				ActualChair = i;
				for (i = first_chair; i < ActualChair; ++i) {
					if (!HasFolded[i % nchairs] && PlayerStartRound[i % nchairs] && !IsAllWin[i % nchairs] && Actions[i % nchairs] == Actions[ActualChair % nchairs] && !IsSmallBlind[i % nchairs] && !IsBigBlind[i % nchairs]) {
						outfile << PlayerNames[i % nchairs] << " checks - Scan Raises" << " PreviusBet=" << PreviusBet[i % nchairs] << " TotalBetonGame=" << TotalBetsOnGame << " Pot=" << pot << " PlayerStack=" << PlayerStack[i % nchairs] << " Balance=" << balance << " QtyPlayer=" << QtyPlaying << " PotVsTotalBet=" << PotVsTotalBet() << endl;
						Actions[i % nchairs]++;
						QtyActionsRound++;
					}

				}
				TotalBetsOnGame += PlayerStack[i % nchairs] - balance;
				outfile << PlayerNames[i % nchairs] << " raises [" << PlayerStack[i % nchairs] - balance << "]" << " PreviusBet=" << PreviusBet[i % nchairs] << " TotalBetonGame=" << TotalBetsOnGame << " Pot=" << pot << " PlayerStack=" << PlayerStack[i % nchairs] << " Balance=" << balance << " QtyPlayer=" << QtyPlaying << " PotVsTotalBet=" << PotVsTotalBet() << endl;
				PreviusBet[i % nchairs] = currentbet;
				PlayerStack[i % nchairs] = balance;
				MaxBet = currentbet;
				Actions[i % nchairs]++;
				QtyActionsRound++;
				if (Actions[i % nchairs] > MaxActions) {
					MaxActions = Actions[i % nchairs];
				}
			}




		}
	}
}

void CHandHistoryWriter::AddMessage(CString message) {

}

void CHandHistoryWriter::PostsSmallBlind(int chair) {
}

void CHandHistoryWriter::PostsBigBlind(int chair) {
}

void CHandHistoryWriter::PostsAnte(int chair) {
}

void CHandHistoryWriter::Checks(int chair) {
}

void CHandHistoryWriter::Folds(int chair) {
}

void CHandHistoryWriter::Calls(int chair) {
}

void CHandHistoryWriter::Raises(int chair) {
}

void CHandHistoryWriter::WinsUncontested(int chair) {
}

CString CHandHistoryWriter::PlayerName(int chair) {
	assert(chair >= 0);
	assert(chair <= kLastChair);
	return p_table_state->Player(chair)->name();
}



// Should be called
// * when a hand is over
// * when it is my turn (summary in the log)
void CHandHistoryWriter::WriteHistory() {


}

bool CHandHistoryWriter::EvaluateSymbol(const CString name, double* result, bool log /* = false */) {
	// No symbols provided
	return false;
}

CString CHandHistoryWriter::SymbolsProvided() {
	// No symbols provided
	return "";
}

const string CHandHistoryWriter::setDate()

{

	time_t curr_time;
	tm* curr_tm;
	char date_string[100];
	char time_string[100];

	stringstream ss;

	string s;

	time(&curr_time);
	curr_tm = localtime(&curr_time);

	strftime(date_string, 50, "%A, %B %d,", curr_tm);
	strftime(time_string, 50, " %T BRT %Y", curr_tm);

	ss << date_string << time_string;

	s = ss.str();

	return s;

}

bool CHandHistoryWriter::ShowAllinCards() {
	int QtyShowCards = 0;
	int nchairs = p_tablemap->nchairs();
	if (QtyAllwin + 1 >= QtyPlaying) {
		for (int i = 0; i < nchairs; ++i) {
			if (PreviusBet[i % nchairs]==MaxBet && Actions[i%nchairs] == MaxActions && MaxActions !=0 || IsAllWin[i % nchairs]) {
				QtyShowCards++;
				}
			else return false;
			}

		if (QtyShowCards == QtyPlaying) {
			return true;
			}
		else return false;
		}
	else return false;		
}
	
void CHandHistoryWriter::Showdown() {
	
	int nchairs = p_tablemap->nchairs();
	for (int i = 0; i < nchairs; ++i) {
		if (!HasFolded[i % nchairs] && !PlayerCollect[i % nchairs] && !PlayerShowDown[i % nchairs] && p_table_state->Player(i % nchairs)->hole_cards(0)->IsKnownCard() && p_table_state->Player(i % nchairs)->hole_cards(1)->IsKnownCard()) {
			PlayerCards[i % nchairs] = " [ " + p_table_state->Player(i % nchairs)->hole_cards(0)->ToString() + " , " + p_table_state->Player(i % nchairs)->hole_cards(1)->ToString() + " ]";
			PlayerCollect[i % nchairs] = true;
		}
	}
}


bool CHandHistoryWriter::PotVsTotalBet() {
	double pot = p_engine_container->symbol_engine_chip_amounts()->pot();

	if (TotalBetsOnGame == pot) {
		return true;
	}
	else return false;

}

bool CHandHistoryWriter::DealPhase() {

	int dealerchair = p_engine_container->symbol_engine_dealerchair()->dealerchair();
	int nchairs = p_tablemap->nchairs();
	double sblind = p_engine_container->symbol_engine_tablelimits()->sblind();
	double bblind = p_engine_container->symbol_engine_tablelimits()->bblind();
	double ante = p_engine_container->symbol_engine_tablelimits()->ante();
	int last_chair = (2 * nchairs) + (dealerchair);
	int first_chair = (nchairs + dealerchair + 1);
	for (int i = first_chair; i <= last_chair; ++i) {
		double currentbet = p_table_state->Player(i % nchairs)->_bet.GetValue();
		if (currentbet == sblind) {
			SmallBlindFound = true;
			PreviusBet[i % nchairs] = sblind;
			IsSmallBlind[i % nchairs] = true;

		}
		if (currentbet == bblind) {
			BigBlindFound = true;
			PreviusBet[i % nchairs] = bblind;
			IsBigBlind[i % nchairs] = true;
			MaxBet = bblind;


		}

	}
	if ((SmallBlindFound == true) && (BigBlindFound = true)) {
		return true;
	}
	else return false;
}

bool CHandHistoryWriter::Winner() {
	int dealerchair = p_engine_container->symbol_engine_dealerchair()->dealerchair();
	int	userchair = p_engine_container->symbol_engine_userchair()->userchair();
	int nchairs = p_tablemap->nchairs();
	int last_chair = (2 * nchairs) + (dealerchair);
	int first_chair = (nchairs + dealerchair + 1);
	int nplayeractive = p_engine_container->symbol_engine_active_dealt_playing()->nplayersactive();
	double ante = p_engine_container->symbol_engine_tablelimits()->ante();
	double sblind = p_engine_container->symbol_engine_tablelimits()->sblind();
	double bblind = p_engine_container->symbol_engine_tablelimits()->bblind();
	double pot = p_engine_container->symbol_engine_chip_amounts()->pot();
	int betround = p_betround_calculator->betround();
	WinnerPots = 0;
	QtyWinnerPots = 0;
	MaxWinnerPots = 0;
	MinWinnerPots = pot;
	MiddleLowWinnerPots = pot;
	MiddleHighWinnerPots = 0;
	PlayerWinner[0] = false;
	PlayerWinner[1] = false;
	PlayerWinner[2] = false;
	PlayerWinner[3] = false;
	PlayerWinner[4] = false;
	PlayerWinner[5] = false;
	PlayerWinner[6] = false;
	PlayerWinner[7] = false;
	PlayerWinner[8] = false;
	PlayerWinner[9] = false;

	for (int i = first_chair; i <= last_chair; ++i) {
		double currentbet = p_table_state->Player(i % nchairs)->_bet.GetValue();
		double balance = p_table_state->Player(i % nchairs)->_balance.GetValue();
		if (currentbet > 0 && !PlayerWinner[i % nchairs]) {
			WinnerPots += currentbet;
			QtyWinnerPots++;
			PlayerWinner[i % nchairs] = true;
			if (currentbet >= MaxWinnerPots) {
				MaxWinnerPots = currentbet;
			}
			if (currentbet <= MinWinnerPots) {
				MinWinnerPots = currentbet;
			}
		}
//		if (currentbet == pot && !PlayerWinner[i % nchairs] || balance - PlayerStack[i % nchairs] == pot && !PlayerWinner[i % nchairs] || QtyPlaying == 1) {
//			QtyWinnerPots = 1;
//			PlayerWinner[i % nchairs] = true;
//			return true;
//		}
	}
	for (int i = first_chair; i <= last_chair; ++i) {
		double currentbet = p_table_state->Player(i % nchairs)->_bet.GetValue();
		if (PlayerWinner[i % nchairs] && currentbet != MinWinnerPots && currentbet != MaxWinnerPots) {
			//Qty3
			if (currentbet <= MiddleLowWinnerPots) {
				MiddleLowWinnerPots = currentbet;
			}
			//Qty4
			if (currentbet >= MiddleHighWinnerPots) {
				MiddleHighWinnerPots = currentbet;
			}
		}
	}
	if ((pot == WinnerPots && pot > (bblind + sblind + ante * nplayeractive) && pot > 0 && betround > 0) || (pot == WinnerPots && pot >= (bblind + sblind + ante * nplayeractive) && pot > 0 && QtyWinnerPots == 1)) {
		return true;
	}
	else return false;
}

bool CHandHistoryWriter::CheckUser() {
	int nchairs = p_tablemap->nchairs();
	bool d = false;
	for (int i = 0; i < nchairs; ++i) {
		if (p_table_state->Player(i % nchairs)->hole_cards(0)->IsKnownCard() && p_table_state->Player(i % nchairs)->hole_cards(1)->IsKnownCard()) {
			UserChair = (i % nchairs);
			d = true;
		}

	}
	if (d) {
		return true;
	}
	else return false;

}

const string CHandHistoryWriter::TornamentName() {

	time_t curr_time;
	tm* curr_tm;
	char weekday_string[100];
	stringstream ss;
	string s;
	time(&curr_time);
	curr_tm = localtime(&curr_time);
	strftime(weekday_string, 50, "%A", curr_tm);


	ss << "Table Main Event. $100 Gtd (" << torneio << ") Table #" << NrTable;

	s = ss.str();

	return s;

}