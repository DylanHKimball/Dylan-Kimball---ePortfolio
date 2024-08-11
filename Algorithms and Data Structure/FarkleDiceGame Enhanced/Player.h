/*
*    ||  -- Farkle Dice Game -- ||
*    |  Programmers Name: Dylan Kimball
*    |  Date: 7/28/2024
*    |  Version: 2.0
*
*    The player class was created to store all the information needed for each player. This 
*	 contains their name, score, whether they have entered the game, if they have six stored 
*	 dice, and if they are winning. Farkle can have multiple players, so having each player 
*	 stored as its own player object makes keeping track of each players information much simpler.
* 
*    NO CHANGES FROM ORIGINAL
*/

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