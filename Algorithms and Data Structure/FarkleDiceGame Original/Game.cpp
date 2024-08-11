// Dylan Kimball - IT-312 - Final Project: Farkle Game

// This file contains all of the game functions that will allow for the game to actually work.
// The gameplay loop is called from the main function and this loop access all other game methods
// as needed.

# include "Game.h"
# include "Player.h"
# include "PlayerOptions.h"

#using<system.dll>
# include <array>
# include <cstdlib>
# include <iomanip>
# include <iostream>
# include <string>
# include <vector>
# include <Windows.h>

using namespace std;
using namespace System;
using namespace System::IO;


Game::Game(int number)
{
	gameNumber = number;
}


Game::~Game()
{
}


bool Game::GetGameOver() const
{
	return gameOver;
}

// Receives user input and verifies to ensure only correct inputs are entered for integers
// Function will verify input for different menu options based on options value 
// passed through.
// option: 1 = Player Setup - option: 2 - Storing Dice
int Game::InputVerificationInt(int options)
{
	int userInput = 0;
	do {
		system("CLS");
		// Displays prompt to enter number of players
		if (options == 1)
		{
			cout << setfill('_') << setw(50) << " " << endl;
			cout << "||       - Enter the number of players -       ||" << endl;
			cout << setfill('_') << setw(50) << " " << endl;
		}

		// Displays whos turn, the dice values, and the keep dice prompt
		if (options == 2 || options == 3)
		{
			cout << setfill('_') << setw(50) << " " << endl;
			cout << " - Current Turn: Player " << currentPlayer + 1 << " - " << playerList[currentPlayer].GetPlayerName();
			cout << " - Score: " << playerList[currentPlayer].GetPlayerScore() << endl;
			cout << setfill('_') << setw(50) << " " << endl;

			DisplayDice();
			// Displays storing dice prompt
			if (options == 2)
			{
				cout << setfill('_') << setw(50) << " " << endl;
				cout << endl << "Choose a dice to store by entering its number (1-6)" << endl;
				cout << " ...Enter 0 to return." << endl;
			}
			// Displays removing dice prompt
			if (options == 3)
			{
				cout << setfill('_') << setw(50) << " " << endl;
				cout << endl << "Choose a dice to remove by entering its number (1-6)" << endl;
				cout << " ...Enter 0 to return." << endl;
			}
		}

		// Trys to get grade value from user and displays error messages for invalid inputs
		try {
			string inputString; // String input is used to account for anything the user inputs
			cin >> inputString;

	
			userInput = stoi(inputString); //Converts string to integer

			// GAME SETUP - PLAYERS: If the input is 1 or less, out put error message to display
			// that there must be at least 2 players
			if (userInput < 2 && options == 1)
			{
				cout << "There must be at least 2 player!" << endl;
				Sleep(1000); // wait 1 second
			}

			// If user is storing dice, then output and error if the number is not 1 through 6
			else if (userInput < 0 || userInput > 6 && options == 2)
			{
				cout << "Number must be between 0 and 6!" << endl;
				Sleep(1000); // wait 1 second
			}

			// If all checks pass, then return the input
			else
			{
				return userInput;
			}
		}
		// If input is not an integer then output an error message
		catch (exception e) {
			cout << "Please enter a number!" << endl;
			Sleep(1000); // wait 1 second
		}
	} while (true);

	return 0;
}


// Receives user input and verifies to ensure only correct inputs are entered for strings
// Function will verify input for different menu options based on options value 
// passed through.
// Optoin 1: Player Setup - Option 2: Storing Dice
string Game::InputVerificationString(int options)
{
	string userInput = "";
	int playerSize = playerList.size();

	do {
		// Display the player names and get confirmation from player
		if (options == 1)
		{
			system("CLS");
			cout << setfill('_') << setw(50) << " " << endl;
			cout << "||                 - Players -                 ||" << endl;
			cout << setfill('_') << setw(50) << " " << endl;
			for (int i = 0; i < playerSize; ++i)
			{
				cout << "Player " << i + 1 << ": " << playerList[i].GetPlayerName() << endl;
			}
			cout << endl << " ...Continue with these players? (Y / N)" << endl;
		}

		// Trys to get grade value from user and displays error messages for invalid inputs
		try {
			string inputString; // String input is used to account for anything the user inputs
			cin >> inputString;

			// If choice is Y or N then return the input
			if (inputString.compare("Y") == 0 || inputString.compare("y") == 0 ||
				inputString.compare("N") == 0 || inputString.compare("n") == 0)
			{
				return inputString;
			}

			// If choice is not Y or N then output an error message
			else
			{
				cout << "Please enter Y or N!" << endl;
				Sleep(1000); // wait 1 secod
			}
		}
		// If input is not an integer then output an error message
		catch (exception e) {
			cout << "Not a valid option!" << endl;
			Sleep(1000); // wait 1 second
		}
	} while (true);

	return userInput;
}


// Verifies user input for turn options and returns chosen option based on PlayerOptions class
PlayerOptions Game::turnOptionVerification()
{
	// choseOption stores users option for different checks
	PlayerOptions chosenOption = PlayerOptions::None;

	do {
		// Display turn options on screen
		TurnOptionsMenu();

		// Trys to get grade value from user and displays error messages for invalid inputs
		try {
			string inputString; // String input is used to account for anything the user inputs
			cin >> inputString;

			// chosen option is to roll the dice
			if (inputString.compare("0") == 0)
			{
				chosenOption = PlayerOptions::Roll;
			}

			// Chosen option is to keep Dice
			else if (inputString.compare("1") == 0)
			{
				chosenOption = PlayerOptions::KeepDice;
			}

			// Chosen option is the Remove dice from the kept dice
			else if (inputString.compare("2") == 0)
			{
				chosenOption = PlayerOptions::RemoveDice;
			}

			// Chosen option is to end the turn
			else if (inputString.compare("3") == 0)
			{
				chosenOption = PlayerOptions::EndTurn;
			}

			// Chosen option is to view the rules
			else if (inputString.compare("4") == 0)
			{
				chosenOption = PlayerOptions::Rules;
			}

			// Chosen option is to exit the game
			else if (inputString.compare("5") == 0)
			{
				chosenOption = PlayerOptions::Exit;
			}

			else
			{
				cout << "Not a valid option!" << endl << endl;
				Sleep(1000); // wait 1 second
			}

			// If input is a valid choice then return the choice.
			if (chosenOption != PlayerOptions::None)
			{
				return chosenOption;
			}
		}
		// If input is not an integer then output an error message
		catch (exception e) {
			cout << "Not a valid option!" << endl;
			Sleep(1000); // wait 1 second
		}
	} while (true);
}


// Sets up the game by requesting the number of players and calling the function
// set up the players.
void Game::GameSetup()
{
	int playerCount;
	string userChoice;

	// Displays rules and waits for user to enter something before continuing
	system("CLS");
	cout << setfill('_') << setw(50) << " " << endl;
	cout << "||           - Welcome to Farkle! -            ||" << endl;
	cout << setfill('_') << setw(50) << " " << endl;
	Sleep(2000); // wait 2 second

	system("CLS");
	GameRules();

	cin >> userChoice;
	userChoice = "N";

	// while the user hasn't confirmed to continue with selected players
	do
	{
		// Get the total number of players for the game
		playerCount = InputVerificationInt(1);

		// Call funciton to set up each player
		SetUpPlayers(playerCount);

		// Get input from user to either continue with or redu player selection
		userChoice = InputVerificationString(1);

	} while (userChoice.compare("N") == 0 || userChoice.compare("n") == 0);

	// Initializes the arrays for rollable dice to -1 and storedDice to 0
	for (int i = 0; i < 6; ++i)
	{
		rollableDice[i] = -1;
		storedDice[i] = 0;
	}
}


// For each player, a name will be requested and then an player object will be
// created an initialized based on that name.
void Game::SetUpPlayers(int playerCount)
{
	string playerName = "";

	for (int i = 0; i < playerCount; ++i)
	{
	system("CLS");
	cout << setfill('_') << setw(50) << " " << endl;
	cout << "||        - Enter A Name For Player " << i + 1 << " -        ||" << endl;
	cout << setfill('_') << setw(50) << " " << endl;
	
	// Get player name from user
	cin >> playerName;

	// Create new player object using the entered name and add the the player vector
	playerList.emplace_back(i+1, playerName);
	}
}


// Displays the rules of the game
int Game::GameRules()
{
	// Stores the file name that will be read
	String^ rules = "FarkleRules.txt";

	// Tries to open and read the file and if it is unable to then it displays an error.
	try
	{
		// Opens the file to prep it for reading
		Console::WriteLine("trying to open file {0}...", rules);
		system("CLS");
		StreamReader^ din = File::OpenText(rules);

		// String that will store each line on the file so it can be displayed
		String^ str;

		// While there is still lines in the file, store the line and display it on the console.
		while ((str = din->ReadLine()) != nullptr)
		{
			Console::WriteLine(str);
		}
	}
	catch (Exception^ e)
	{
		// If the file name can't be found then an error is displayed
		if (dynamic_cast<FileNotFoundException^>(e))
			Console::WriteLine("file '{0}' not found", rules);
		// If something went wrong when reading the file then an error is displayed
		else
			Console::WriteLine("problem reading file '{0}'", rules);
	}
	return 0;
}


// Displays the turn options
void Game::TurnOptionsMenu()
{
	int count = 0;

	// Displays whos turn it is
	system("CLS");
	cout << setfill('_') << setw(50) << " " << endl;
	cout << " - Current Turn: Player " << currentPlayer + 1 << " - " << playerList[currentPlayer].GetPlayerName();
	cout << " - Score: " << playerList[currentPlayer].GetPlayerScore() << endl;
	cout << setfill('_') << setw(50) << " " << endl;

	DisplayDice(); // Displays current dice both rolled and stored
	
	// Display options
	cout << setfill('_') << setw(50) << " " << endl;
	cout << "||         - Choose an Option Below -          ||" << endl;
	cout << setfill('_') << setw(50) << " " << endl;
	cout << " - 0: Roll Dice" << endl;
	cout << " - 1: Keep Dice" << endl;
	cout << " - 2: Remove Dice" << endl;
	cout << " - 3: End Turn" << endl;
	cout << " - 4: Rules" << endl;
	cout << " - 5: Exit Game" << endl;
	cout << setfill('_') << setw(50) << " " << endl;
}



void Game::DisplayDice()
{
	// Outputs the current roll
	int count = 0;
	cout << " - Rolled Dice: " << endl;
	for (int i = 0; i < 6; ++i)
	{
		// If the dice value is -1 then it is the first loop of a turn and will prompt user to roll the dice
		if (rollableDice[i] == -1)
		{
			cout << "    ...Roll the Dice!" << endl;
			break;
		}
		// If the dice value is 0 then it is stored and won't display on rolled dice
		else if (rollableDice[i] == 0) {}

		// If the dice is not stored then display the rolled value
		else
		{
			cout << " Dice" << i + 1 << ": " << rollableDice[i];
			if (CheckPointDice(rollableDice[i]))
			{
				cout << "  -  " << "Point Dice!";
			}
			if (CheckThreePointMatch(rollableDice[i], 1) && count < 3)
			{
				cout << " - " << "Three Dice Match!";
				++count;
			}
			cout << endl;
		}
	}

	// Outputs the current stored dice, if there are no stored dice then nothing is output
	cout << endl << " - Stored Dice: " << endl;
	for (int i = 0; i < 6; ++i)
	{
		// If dice value is not 0 then it is currently stored and will be displayed
		if (storedDice[i] != 0)
		{
			cout << " Dice" << i + 1 << ": " << storedDice[i];

			if (CheckThreePointMatch(storedDice[i], 1)) // Outputs matching dice if there is a three of a kind match
			{
				cout << " - " << "Three Dice Match!";
			}
			cout << endl;
		}
	}
	// Display the current stored score
	cout << endl << "Stored Score: " << currentScore << endl;
}


// Dislpays the winner of the game as well as second and thrid place if there are more than 2 players
void Game::DisplayWinner(int winner, int second, int third)
{
	int playerCount = playerList.size();
	string userContinue;

	system("CLS");
	cout << setfill('_') << setw(50) << " " << endl;
	cout << "||                  - WINNER -                 ||" << endl;
	cout << setfill('_') << setw(50) << " " << endl;
	cout << " - Player " << winner + 1 << ": " << playerList[winner].GetPlayerName() << " Won!! - " << endl;
	cout << " - Final Score: " << playerList[winner].GetPlayerScore() << " - " << endl;

	cout << setfill('_') << setw(50) << " " << endl;
	cout << " - Player " << second + 1 << ": " << playerList[second].GetPlayerName() << " came in second!! - " << endl;
	cout << " - Final Score: " << playerList[second].GetPlayerScore() << " - " << endl;

	if (playerCount > 2)
	{
		cout << " - Player " << third + 1<< ": " << playerList[third].GetPlayerName() << " came in third!! - " << endl;
		cout << " - Final Score: " << playerList[third].GetPlayerScore() << " - " << endl;
		cout << setfill('_') << setw(50) << " " << endl;
	}
	cout << endl << "   Press any key to contiue..." << endl;

	// Waits for user to enter something before continuing
	cin >> userContinue;
}


// Displays message when player gets a Farkle
void Game::DisplayFarkle()
{
	system("CLS");
	cout << setfill('_') << setw(80) << " " << endl;
	cout << "||  8888888888     d8888 8888888b.  888    d8P  888      8888888888 888 888  ||" << endl;
	cout << "||  888           d88888 888   Y88b 888   d8P   888      888        888 888  ||" << endl;
	cout << "||  888          d88P888 888    888 888  d8P    888      888        888 888  ||" << endl;
	cout << "||  8888888     d88P 888 888   d88P 888d88K     888      8888888    888 888  ||" << endl;
	cout << "||  888        d88P  888 8888888P'  8888888b    888      888        888 888  ||" << endl;
	cout << "||  888       d88P   888 888 T88b   888  Y88b   888      888        888 888  ||" << endl;
	cout << "||  888      d8888888888 888  T88b  888   Y88b  888      888                 ||" << endl;
	cout << "||  888     d88P     888 888   T88b 888    Y88b 88888888 8888888888 888 888  ||" << endl;
	cout << setfill('_') << setw(80) << " " << endl;
	Sleep(2000);
}


// Displays that the final round has now been entered
void Game::GameEnd()
{
	int playerSize = playerList.size();
	int winning = currentPlayer;
	int second = 0;
	int third = 0;

	// Loops through the players and checks to see if they have more points then the current winner
	// If they do then they are set as the current winner and the checks continue to the last player
	for (int i = 0; i < playerSize; ++i)
	{
		if (i != winning)
		{
			// If a player has a higher score then they are changed to the current winning player and the previous
			// player is moved to second, if there was a player in second, they are moved to third.
			if (playerList[i].GetPlayerScore() > playerList[winning].GetPlayerScore())
			{
				playerList[winning].SetWinning(false);
				playerList[i].SetWinning(true);
				third = second;
				second = winning;
				winning = i;
			}
			// If the score is higher then second place then the player becomes second place and second place is moved
			// to third place
			else if (playerList[i].GetPlayerScore() > playerList[second].GetPlayerScore())
			{
				third = second;
				second = i;
			}
			// If the score is higher then thrid place, than that place is put as third place.
			else if (playerList[i].GetPlayerScore() > playerList[third].GetPlayerScore() && i != second)
			{
				third = i;
			}
		}

	}
	DisplayWinner(winning, second, third);
}


// Takes all rollable dice and chooses a random number for each die.
// After the roll, the dice are sent to another function to check
// if there are any point dice.
void Game::RollDice()
{
	// Loops through the dice object array and rolls all dice that are not stored (dice with 0 value in rollable dice are currently stored)
	for (int i = 0; i < 6; ++i)
	{
		// If the dice is stored, then it is not rolled
		if (rollableDice[i] == 0) {}
		else 
		{
			rollableDice[i] = ((rand() % (max - min + 1)) + min);
		}
	}

	// Calls functions to check and see if there are any point dice in the rolled dice set
	for (int i = 0; i < 6; ++i)
	{
		// If dice value is 0 the it is stored and can be ignored
		if (rollableDice[i] == 0) {}

		// If at least one dice is a point dice, then point dice in set is true, if not it is false
		else if (CheckPointDice(rollableDice[i]))
		{
			pointDiceInSet = true;
			return;
		}
		else if (CheckThreePointMatch(rollableDice[i], 1))
		{
			pointDiceInSet = true;
			return;;
		}
	}
	pointDiceInSet = false;
}

// Function that checks whether number is a point value number (either 1, 5, or match of three)
bool Game::CheckPointDice(int dice)
{
	// If number is 1, 5, or a match of three then return true for point dice, if not then return false
	if (dice == 1 || dice == 5 || CheckThreePointMatch(dice, 1))
	{
		return true;
	}
	else
	{
		return false;
	}
}

// Function that checks whether there is a match of three in the rolled dice
bool Game::CheckThreePointMatch(int dice, int array)
{
	int count = 0; // Initialize count

	// For each dice in rolled dice, if the value of the dice matches the current dice, add 1 to the count
	// If the count reaches 3 it means there is a match of three and returns true, if not it returns false
	if (array == 1)
	{
		for (int i = 0; i < 6; ++i)
		{
			if (rollableDice[i] == dice)
			{
				++count;
				if (count == 3)
				{
					return true;
				}
			}
		}
		return false;
	}
	else
	{
		for (int i = 0; i < 6; ++i)
		{
			if (storedDice[i] == dice)
			{
				++count;
				if (count == 3)
				{
					return true;
				}
			}
		}
		return false;
	}
}

// Functions allows players add dice to their currently stored dice
bool Game::KeepDice()
{
	int userStoreDice, storedDiceValue = 0, count = 0;
	string userChoice = "N";
	bool diceStored = false;

	// while the user hasn't confirmed to continue with selected players
	do
	{
		count = 0;

		// Get dice value between 1 and 6 from user and minues 1 to match the vector values of 0-5
		userStoreDice = InputVerificationInt(2) - 1;

		// If the user chooses 0 then they will return to the turn options
		if (userStoreDice == -1)
		{
			// checks to see if all dice are currently stored
			for (int i = 0; i < 6; ++i)
			{
				// If they are, then sets value true to show that player has all six point dice stored
				// If not, then break and return as normal
				if (rollableDice[i] == 0) 
				{
					playerList[currentPlayer].SetSixPointDice(true);
				}
				else
				{
					playerList[currentPlayer].SetSixPointDice(false);
					break;
				}
			}
			return diceStored;
		}

		// If the dice the user chose is already stored then output message to user
		if (rollableDice[userStoreDice] == 0)
		{
			cout << "Dice " << userStoreDice + 1 << " is already stored..." << endl;
			Sleep(1000);
		}

		// If the dice the user chose is not a point dice then output message to user (either 1, 5, or three point match)
		else if (!CheckPointDice(rollableDice[userStoreDice]))
		{
			if (CheckThreePointMatch(rollableDice[userStoreDice], 1))	{}
			else
			{
				cout << "Dice " << userStoreDice + 1 << " is not a point dice..." << endl;
				Sleep(1000);
			}
		}

		// The dice is a point dice and not yet stored so checks to confirm that the user wants to store the dice
		else
		{
			cout << "Store dice " << userStoreDice + 1 << "? (Y / N)" << endl;
			userChoice = InputVerificationString(2);

			storedDiceValue = rollableDice[userStoreDice];
			// If the user confirms storing the dice then checks to see if it is a three of a kind match and stores all three
			// Dice that are stored have their values moved to the correct index in the stored dice array while the rollable
			// dice array value will switch to 0 to show that dice is stored
			if (userChoice == "Y" || userChoice == "y" && CheckThreePointMatch(rollableDice[userStoreDice], 1))
			{
				cout << endl << "Storing matching set!" << endl;
				for (int i = 0; i < 6; ++i)
				{
					if (rollableDice[i] == storedDiceValue)
					{
						storedDice[i] = storedDiceValue;
						rollableDice[i] = 0;
						++count;
					}
					// If three matching dice have been stored then break from the loop.
					if (count == 3)
					{
						break;
					}
				}
				Sleep(1000);
				diceStored = true;
			}

			// If it is not part of a match then only the one dice is stored
			else if (userChoice == "Y" || userChoice == "y")
			{
				storedDice[userStoreDice] = rollableDice[userStoreDice];
				rollableDice[userStoreDice] = 0;
				diceStored = true;
			}
		}
	} while (userStoreDice != -1);

	return diceStored;
}


// Function allows players to remove dice from their currently stored dice
void Game::RemoveDice()
{
	int userRemoveDice, storedDiceValue = 0, count = 0, storedCount = 0;
	string userChoice = "N";

	do
	{
		// Determins how many dice are stored to ensure that at least one point dice is still stored at all times
		storedCount = 0;
		count = 0;
		for (int i = 0; i < 6; ++i)
		{
			// If value of array is not 0, then dice is stored
			if (storedDice[i] != 0)
			{
				++storedCount;
			}
		}

		// Get dice value between 1 and 6 from user and minues 1 to match the vector values of 0-5
		userRemoveDice = InputVerificationInt(3) - 1;

		// If the user chooses 0 then they will return to the turn options
		if (userRemoveDice == -1)
		{
			return;
		}

		// If the dice the user chose is not stored then output message to user (0 means dice is not stored)
		if (storedDice[userRemoveDice] == 0)
		{
			cout << "Dice " << userRemoveDice + 1 << " is not currenlty stored..." << endl;
			Sleep(1000);
		}

		// Checks to see if at least one point dice is still stored as one must be stored at all times.
		else
		{
			cout << "Remove stored dice " << userRemoveDice + 1 << "? (Y / N)" << endl;
			userChoice = InputVerificationString(2);
			storedDiceValue = storedDice[userRemoveDice];

			// If dice is only dice stored then it can't be removed
			if (storedCount == 1)
			{
				cout << endl << "Can't remove this dice, must have at least one point dice stored!" << endl;
				Sleep(2000);
			}

			// Else if the dice is part of a three point match that is the only match stored, then it can't be removed
			else if (storedCount == 3 && CheckThreePointMatch(storedDice[userRemoveDice], 2))
			{
				if (storedDice[userRemoveDice] == 1 || storedDice[userRemoveDice] == 5) {}
				else 
				{
					cout << endl << "Can't remove this set, must have at least one point dice stored!" << endl;
					Sleep(2000);
				}
			}

			// If the user confirms un storing the dice then checks to see if it is a three of a kind match and removes all three
			else if (userChoice == "Y" || userChoice == "y" && CheckThreePointMatch(storedDice[userRemoveDice], 2))
			{
				if (storedDice[userRemoveDice] == 1 || storedDice[userRemoveDice] == 5) 
				{
					rollableDice[userRemoveDice] = storedDice[userRemoveDice];
					storedDice[userRemoveDice] = 0;
				}
				else 
				{
					cout << endl << "Removing matching set!" << endl;
					for (int i = 0; i < 6; ++i)
					{
						// If the dice value matches than the dice value is moved to the matching rollable dice array and the stored dice
						// array is set to 0
						if (storedDice[i] == storedDiceValue)
						{
							rollableDice[i] = storedDiceValue;
							storedDice[i] = 0;
							++count;
						}
						// If three matching dice have been stored then break from the loop.
						if (count == 3)
						{
							break;
						}
					}
					Sleep(1000);
				}
			}

			// If it is not part of a match then only the one dice is removed
			else if (userChoice == "Y" || userChoice == "y")
			{
				rollableDice[userRemoveDice] = storedDice[userRemoveDice];
				storedDice[userRemoveDice] = 0;
			}
		}

	} while (userRemoveDice != -1);
}


// Function will be called at the end of a players turn to calculate score
void Game::TurnEnd()
{
	// If the player has already entered the game then add up their score
	if (playerList[currentPlayer].GetEnteredGame())
	{
		// Adds turn score to the players total score
		playerList[currentPlayer].SetPlayerScore(currentScore);

		// If a player gets 10,000 points then they will be set as the current winner and the final round will begin
		if (playerList[currentPlayer].GetPlayerScore() >= 1500)
		{
			playerList[currentPlayer].SetWinning(true);
			cout << " - Current Turn: Player " << currentPlayer + 1 << " - " << playerList[currentPlayer].GetPlayerName()
				<< " has reached 10,000 points!!" << endl << "The final round has begun!!" << endl;
			Sleep(2000);
		}
	}

	// If a players score isn't 1,000 points then they haven't entered the game and can't end their turn.
	else if (currentScore < 1000)
	{
		cout << "Must get at least 1,000 points before you can end your turn!" << endl;
		Sleep(2000);
		return; // returns to continue players turn
	}
	else
	{
		// Adds turn score to the players total score
		playerList[currentPlayer].SetPlayerScore(currentScore);
		playerList[currentPlayer].SetEnteredGame(true);
	}
}


// Takes the stored point dice and gets the sum to return the total turn score
void Game::CalculateScore()
{
	int turnScore = 0; // Holds the score
	int count = 0, match1 = -1, match2 = -1;

	// Loops through all dice in the array to check which are stored and add up the points of
	// the stored dice.
	for (int i = 0; i < 6; ++i)
	{
		count = 0;
		// If the dice is stored then add its value, if not then ignore
		if (storedDice[i] != 0)
		{
			// If the point dice is a match then find its match and add the value times 100
			if (CheckThreePointMatch(storedDice[i], 2))
			{
				// If the point dice has not already been calculated as a match of three then continue
				if (i != match1)
				{
					if (i != match2)
					{
						// If the matching set is 1's then add 1,000 to the score
						if (storedDice[i] == 1)
						{
							turnScore = turnScore + 1000;
						}
						// If the matching set is other number then add that value * 100 to the score
						else
						{
							turnScore = turnScore + (storedDice[i] * 100);
						}

						// Loops through stored dice array to determine which dice were matches and already calcuated
						for (int j = 0; j < 6; ++j)
						{
							if (storedDice[i] == storedDice[j])
							{
								++count;
								// If a second match is found then its index value is stored
								if (count == 2)
								{
									match1 = j;
								}
								// If three matches were found then the index value is stored and the loop stops
								else if (count == 3)
								{
									match2 = j;
									break;
								}
							}
						}
					}
				}
			}
			// If the dice is not a match then just add the points to the score
			else if (storedDice[i] == 1)
			{
				turnScore = turnScore + 100;
			}

			else if (storedDice[i] == 5)
			{
				turnScore = turnScore + 50;
			}
		}
	}
	// Sets players curent score to the calculated score
	currentScore = storedScore + turnScore;
}


// Gameplay loop will loop through each players turn calling all the necessary functions
// above when they are needed.
void Game::GameplayLoop()
{
	GameSetup();
	// Initialises number generator using time
	srand(time(0));

	// choseOption stores users option for different checks
	PlayerOptions chosenOption = PlayerOptions::None;
	string userContinue = "";
	int playerSize = playerList.size();
	int loopCount = 0;
	bool pointDiceStored = false, firstRoll = false;

	do
	{
		for (int i = 0; i < playerSize; ++i)
		{
			currentScore = 0, storedScore = 0, loopCount = 0;
			currentPlayer = i;
			pointDiceInSet = false, pointDiceStored = false, firstRoll = false;

			// If a player is currenlty set as the winner then the game will end and scores will be compared
			// When this is true it means all players have had one more turn after this player reached 10,000 points
			if (playerList[currentPlayer].GetWinning())
			{
				// Function that determins the winner and displays the end game info on the screen
				GameEnd();
				gameOver = true;
				break;
			}

			do // Turn will continue until there are no point dice or the player ends their turn
			{
				CalculateScore(); // Gets the current stored score to display on screen
				chosenOption = turnOptionVerification();

				// If chosen option is Roll - then call function to roll the dice
				if (chosenOption == PlayerOptions::Roll)
				{
					if (pointDiceStored == false && loopCount > 0) 
					{
						cout << "You must store at least one point dice to continue!!" << endl;
						Sleep(1000);
					}

					// If a player decided to roll again, then all dice not currently stored
					// have their point and match values reset to false
					else
					{
						// Rolls all non stored dice
						RollDice();
						if (firstRoll == false)
						{
							firstRoll = true;
						}
					}
				}

				// If chosen option is KeepDice - then call function to allow the player to keep certain dice and 
				// return whether a dice is stored or not
				else if (chosenOption == PlayerOptions::KeepDice)
				{
					pointDiceStored = KeepDice();

					// If the player has stored all 6 dice as point dice, then that score is added up and they can roll 6 dice again
					if (playerList[currentPlayer].GetSixPointDice())
					{
						firstRoll = false;
						CalculateScore();
						storedScore = currentScore; // Set stored score as the value of all 6 stored dice
						// Reset all the dice to their defaul for player to roll again
						for (int i = 0; i < 6; ++i)
						{
							rollableDice[i] = -1;
							storedDice[i] = 0;
						}
						playerList[currentPlayer].SetSixPointDice(false);
					}
				}

				// If chosen option is RemoveDice - then call function to allow the player to remove dice from 
				// their current kept dice selction
				else if (chosenOption == PlayerOptions::RemoveDice)
				{
					RemoveDice();
				}

				// If chosen option is EndTurn - then exit the turn loop and start next turn
				else if (chosenOption == PlayerOptions::EndTurn)
				{
					// If the player has already rolled once they may choose the nd turn option
					if (firstRoll == true)
					{
						TurnEnd();
						// If the player has at least 1,000 points then they are free to end their turn
						if (playerList[currentPlayer].GetEnteredGame())
						{
							break;
						}
					}
					else
					{
						cout << "Must roll dice at least once!" << endl;
						Sleep(1000);
					}
				}

				// If chosen option is Rules - then display the rules
				else if (chosenOption == PlayerOptions::Rules)
				{
					GameRules();
					cin >> userContinue; // When user enters something the game will continue
				}

				// If chosen option is Exit - then exit the game
				else if (chosenOption == PlayerOptions::Exit)
				{
					gameOver = true;
					break;
				}
				++loopCount;

			} while (pointDiceInSet == true);

			if (gameOver == false)
			{
				// If there were no point dice in the set, then display farkle
				if (pointDiceInSet == false)
				{
					DisplayFarkle();
					currentScore = 0;
				}

				// Displays points gathered this turn and total points before moving to the next players turn
				system("CLS");
					cout << setfill('_') << setw(50) << " " << endl;
				cout << "||         - Ending Turn for Player " << currentPlayer + 1 << " -        ||" << endl;
				cout << setfill('_') << setw(50) << " " << endl;
				cout << "Turn Score: " << currentScore << endl;
				cout << "Total Score: " << playerList[currentPlayer].GetPlayerScore() << endl;

				// Reset all the dice to their defaul values for the next turn
				for (int i = 0; i < 6; ++i)
				{
					rollableDice[i] = -1;
					storedDice[i] = 0;
				}
				Sleep(1000);
			}
			// If game over is set to true then break the for loop
			else
			{
				break;
			}
		}
	} while (gameOver == false);
}