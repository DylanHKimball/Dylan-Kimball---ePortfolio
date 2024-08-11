/*
*    ||  -- Farkle Dice Game -- ||
*    |  Programmers Name: Dylan Kimball
*    |  Date: 7/28/2024
*    |  Version: 2.0
*
*	 The code below initilizes, sets, and returns the different values of the player objects. 
*	 Storing each players information as an object allows for a large number of data to be 
*	 stored with a relativly small amount of code.
* 
*    NO CHANGES FROM ORIGINAL
*/

# include "Player.h"

# include <string>

using namespace std;


// New player constructor to set players name and intialize score as 0
Player::Player(int playerNumber, string name) 
{
	playerName = name;
	playerScore = 0;
	enteredGame = false;
	sixPointDice = false;
	winning = false;
}


// Player destructor
Player::~Player()
{
}


// Sets player name with the name passed through
void Player::SetPlayerName(string name)
{
	playerName = name;
}


// Sets player score by adding the int passed through to the current score
void Player::SetPlayerScore(int turnScore)
{
	playerScore = playerScore + turnScore;
}


// sets whether the player has over 1000 points
void Player::SetEnteredGame(bool over1000)
{
	enteredGame = over1000;
}


// sets whether all dice are stored as point dice
void Player::SetSixPointDice(bool pointDice)
{
	sixPointDice = pointDice;
}


// sets if the player is currenlty winning (over 10,000 points)
void Player::SetWinning(bool leader)
{
	winning = leader;
}


// Returns the player name
string Player::GetPlayerName() const
{
	return playerName;
}


// Returns the players current score
int Player::GetPlayerScore() const
{
	return playerScore;
}


// Returns whether the player has over 1000 points
bool Player::GetEnteredGame() const
{
	return enteredGame;
}


// Returns whether all dice are stored as point dice
bool Player::GetSixPointDice() const
{
	return sixPointDice;
}


// returns if the player is currenlty winning (over 10,000 points)
bool Player::GetWinning() const
{
	return winning;
}