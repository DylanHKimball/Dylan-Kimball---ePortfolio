/*
*    ||  -- Farkle Dice Game -- ||
*    |  Programmers Name: Dylan Kimball
*    |  Date: 7/28/2024
*    |  Version: 2.0
*
*    The game class contains all of the main functionality of the game, every function is private
*	 other than the main gameplayloop function and can only be called by the main gameplayloop.
*	 This class allows for the user to perform everything they need to to play the game and will
*	 set up the game, display options and player/dice data, calculate the score, determine the winner
*	 etc. The goal of this class was to try to keep each game isolated in its own area, so that in the
*	 future it will be easy to take each games information and store it in a database or something
*	 similar.
*
*	 ENHANCEMENTS TO THIS FILE:
*/

#pragma once

# include "Dice.h"
# include "Player.h"
# include "PlayerOptions.h"

# include <vector>

using namespace std;

class Game {

	int gameNumber, max = 6, min = 1;
	int enterScoreReq = 0, winScoreReq = 0;
	// CurrentScore stores the total score with all stored point dice,
	// storedScore stores the score of the totaled point dice from all
	// six dice being stored for points and adds that to the total.
	int currentPlayer = 0, currentScore = 0, storedScore = 0;
	bool pointDiceInSet = false, gameOver = false;
	vector<Player> playerList;
	Dice DiceArray[6];

private:

	// Verifies input based on which menu is active
	int InputVerificationInt(int options);
	string InputVerificationString(int options);
	PlayerOptions turnOptionVerification();

	// Game setup functions
	void GameSetup();
	void SetUpPlayers(int playerCount);

	// functions to display messages, options, and rules
	int GameRules();
	void TurnOptionsMenu();
	void DisplayDice();
	void DisplayWinner(int winner, int second, int thrid);
	void DisplayFarkle();
	void GameEnd();

	// Gameplay functions
	void RollDice();
	void CheckForPointDice();
	string CheckMatches(int countArray[6]);
	void CalculateScore();
	bool KeepDice();
	void RemoveDice();
	void TurnEnd();

public:

	Game(int number);
	~Game();

	// Declares gameplay loop function
	// This is the main core for the turn to turn gameplay
	void GameplayLoop(int enterGameScore, int winGameScore);
	bool GetGameOver() const;
};