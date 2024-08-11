/*
*    ||  -- Farkle Dice Game -- ||
*    |  Programmers Name: Dylan Kimball
*    |  Date: 7/28/2024
*    |  Version: 2.0
* 
*	 The code below initilizes, sets, and returns the different values of the dice objects. 
*	 By storing the dice information in an object class, the game code requires less complex 
*	 checks which allows for more compact code.
* 
*	 ADDED CLASS
*/

# include "Dice.h"

using namespace std;

// Default Constructor
Dice::Dice()
{

}

// New dice constructor to set dice number and intialize
// value as 0 and point die as false
Dice::Dice(int number)
{
	diceNumber = diceNumber;
	diceValue = 0;
	isPointDie = false;
	pointMatch = false;
	isStored = false;
	matchType = "None";
}

// Dice destructor
Dice::~Dice()
{

}

// SETTER FUNCTIONS //

// Sets the number of the die
void Dice::SetDiceNumber(int number)
{
	diceNumber = number;
}

// Sets the value of the die
void Dice::SetDiceValue(int value)
{
	diceValue = value;
}

// Sets whether the die is a point die or not
void Dice::SetPointDie(bool pointDie)
{
	isPointDie = pointDie;
}

// Sets whetehr the die is part of a three dice match
void Dice::SetPointMatch(bool match)
{
	pointMatch = match;
}

// Sets whether the die is stored or not
void Dice::SetStoredValue(bool stored)
{
	isStored = stored;
}

// Sets what type of match this die is a part of
void Dice::SetMatchType(string type)
{
	matchType = type;
}

// GETTER FUNCTIONS //

// Returns the die number
int Dice::GetDiceNumber() const
{
	return diceNumber;
}

// Returns the value of the die
int Dice::GetDiceValue() const
{
	return diceValue;
}

// Returns whether the die is a point die or not
bool Dice::GetPointDie() const
{
	return isPointDie;
}

// Returns whether the die is part of a three dice match
bool Dice::GetPointMatch() const
{
	return pointMatch;
}

// Returns whether the die is stored or not
bool Dice::GetStoredValue() const
{
	return isStored;
}

// returns what type of match this die is sa part of
string Dice::GetMatchType() const
{
	return matchType;
}