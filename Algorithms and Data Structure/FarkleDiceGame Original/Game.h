// Dylan Kimball - IT-312 - Final Project: Farkle Game

// This file contains class definition for the Game object and its methods

#pragma once

# include "Player.h"
# include "PlayerOptions.h"

# include <vector>

using namespace std;

class Game {

	int gameNumber, max = 6, min = 1;
	// CurrentScore stores the total score with all stored point dice,
	// storedScore stores the score of the totaled point dice from all
	// six dice being stored for points and adds that to the total.
	int currentPlayer = 0, currentScore = 0, storedScore = 0;
	bool pointDiceInSet = false, gameOver = false;
	vector<Player> playerList;
	int rollableDice[6];
	int storedDice[6];

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
	bool CheckPointDice(int dice);
	bool CheckThreePointMatch(int dice, int array);
	void CalculateScore();
	bool KeepDice();
	void RemoveDice();
	void TurnEnd();

public:

	Game(int number);
	~Game();

	// Declares gameplay loop function
	// This is the main core for the turn to turn gameplay
	void GameplayLoop();
	bool GetGameOver() const;
};