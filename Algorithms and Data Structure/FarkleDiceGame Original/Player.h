// Dylan Kimball - IT-312 - Final Project: Farkle Game

// This file contains class definition for the player objects and their methods

#pragma once

# include <string>

using namespace std;

// Player class to store the name and score for each player
class Player {

private:
	string playerName;
	int playerScore;
	bool enteredGame;
	bool sixPointDice;
	bool winning;

public:
	Player(int playerNumber, string name); // Constructor
	~Player(); // Destructor

	// Setter Functions declarations to set/add the players score
	void SetPlayerName(string name);
	void SetPlayerScore(int turnScore);
	void SetEnteredGame(bool over1000);
	void SetSixPointDice(bool pointDice);
	void SetWinning(bool leader);

	// Getter functions to return the player name and score
	string GetPlayerName() const;
	int GetPlayerScore() const;
	bool GetEnteredGame() const;
	bool GetSixPointDice() const;
	bool GetWinning() const;
};