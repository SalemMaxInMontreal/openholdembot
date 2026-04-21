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

#ifndef INC_CHANDHISTORYWRITER_H
#define INC_CHANDHISTORYWRITER_H

#include "CVirtualSymbolEngine.h"

const int kMaxLines = 256;
using namespace std;

class CHandHistoryWriter : public CVirtualSymbolEngine {
public:
	CHandHistoryWriter();
	~CHandHistoryWriter();
public:
	// Mandatory reset-functions
	void InitOnStartup();
	void UpdateOnConnection();
	void UpdateOnHandreset();
	void UpdateOnNewRound();
	void UpdateOnMyTurn();
	void UpdateOnHeartbeat();
	const string setDate();
	const string TornamentName();

public:
	// Public accessors
	bool EvaluateSymbol(const CString name, double* result, bool log = false);
	CString SymbolsProvided();
public:
	void AddMessage(CString message);
public:
	void PostsSmallBlind(int chair);
	void PostsBigBlind(int chair);
	void PostsAnte(int chair);

public:
	void Checks(int chair);
	void Folds(int chair);
	void Calls(int chair);
	void Raises(int chair);
	bool CheckUser();
	bool Winner();
	void Showdown();
	bool ShowAllinCards();
	bool DealPhase();
	bool PotVsTotalBet();



public:
	void WinsUncontested(int chair);
private:
	CString PlayerName(int chair);
	double PreviusBet[kMaxNumberOfPlayers];
	double MaxBet;
	long int HandNumber;
	int Actions[kMaxNumberOfPlayers];

	double MaxPlayerTotalBet;
	bool IsBigBlind[kMaxNumberOfPlayers];
	bool HasFolded[kMaxNumberOfPlayers];
	bool IsAllWin[kMaxNumberOfPlayers];
	bool IsSmallBlind[kMaxNumberOfPlayers];
	bool PlayerShowDown[kMaxNumberOfPlayers];
	CString PlayerCards[kMaxNumberOfPlayers];
	CString PlayerNames[kMaxNumberOfPlayers];
	bool PlayerStartRound[kMaxNumberOfPlayers];
	bool IsShowDown;
	bool ShowDownStep1;
	bool ShowDownFinish;
	bool FinalWinnerScan;
	bool Finish;
	bool MinWinnerShow;
	bool MaxWinnerShow;
	bool MiddleLowWinnerShow;
	bool MiddleHighWinnerShow;
	bool PlayerCollect[kMaxNumberOfPlayers];
	double PlayerTotalBet[kMaxNumberOfPlayers];
	double PlayerStack[kMaxNumberOfPlayers];
	bool PlayerPotShow[kMaxNumberOfPlayers];
	int MaxActions;
	int Round_First_Chair;
	int Round_Last_Chair;
	int QtyActionsRound;
	int QtyPlaying;
	int QtyAllwin;
	int QtyShowCards;
	int QtyPlayerCollect;
	int ActualChair;
	int UserChair;
	double TotalBetsOnGame;
	double WinnerPots;
	double MaxWinnerPots;
	double MinWinnerPots;
	double MiddleLowWinnerPots;
	double MiddleHighWinnerPots;
	int QtyWinnerPots;
	bool PlayerWinner[kMaxNumberOfPlayers];
private:
	void WriteHistory();

private:
	int _lines_collected;
	CString _handhistory_data[kMaxLines];
	CString _message;

};

extern CHandHistoryWriter* p_handhistory_writer;

#endif INC_CHANDHISTORYWRITER_H