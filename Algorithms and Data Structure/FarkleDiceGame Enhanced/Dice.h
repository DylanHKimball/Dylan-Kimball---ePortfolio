/*
*    ||  -- Farkle Dice Game -- ||
*    |  Programmers Name: Dylan Kimball
*    |  Date: 7/28/2024
*    |  Version: 2.0
*
*    The dice class was added to store information about each die with the intent to reduce
*	 the number of checks made with the previous double array system. The dice class is able
*	 to store the dice number, value, whether it is a point die, whether its part of a match,
*	 and the match type if applicable.
* 
*	 ADDED CLASS
*/

#pragma once

# include <string>

using namespace std;

// Player class to store the value and point die bool for each die
class Dice {

private:
	int diceNumber;
	int diceValue;
	bool isPointDie;
	bool pointMatch;
	bool isStored;
	string matchType;

public:
	Dice();
	Dice(int number); // Constructor
	~Dice(); // Destructor

	// Setter Functions declarations to set the dice value
	// & set if the dice is a point die
	void SetDiceNumber(int number);
	void SetDiceValue(int value);
	void SetPointDie(bool pointDie);
	void SetPointMatch(bool match);
	void SetStoredValue(bool stored);
	void SetMatchType(string type);

	// Getter functions to return the dice value and whether
	// it is a point die
	int GetDiceNumber() const;
	int GetDiceValue() const;
	bool GetPointDie() const;
	bool GetPointMatch() const;
	bool GetStoredValue() const;
	string GetMatchType() const;
};