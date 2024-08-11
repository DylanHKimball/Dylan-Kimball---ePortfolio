/*
*    ||  -- Farkle Dice Game -- ||
*    |  Programmers Name: Dylan Kimbally
*    |  Date: 7/28/2024
*    |  Version: 2.0
*
*    The code containined in the game class allow for the entire games functionality. The
*	 first thing done is game setup where player count and names are chosen and the game is
*	 initilized with the information. From here the gameplay loop starts with player one and
*	 allows for them to either roll, keep dice, remove dice, view the rules, end their turn,
*	 or exit the game. This code contains functions to display information such as player
*	 or dice information, roll dice, check for point dice in a roll, store and unstore dice,
*	 calculate the players score, display the game rules, and determine the winner. The code
*	 also contains different error handling methods and input verification to ensure that
*	 only proper inputs are accepted while keeping the program from crashing.
*
*	 ENHANCEMENTS TO THIS FILE:
*		- Reworked entire code to work with the new dice object array system vs the original
*		    dual array system. (Nearly every function called the dual arrays in some way, so
*    	    nearly every funciton has been slightly modified)
*		- Completely reworked check for point dice function to include new scoring options
*		- Completely reworked calculate score function to include new scoring options
*	    - Added check matches function to help check point dice and calculate score funciton
* 			- New scoring options (Stright, Three pairs, Four, Five, & Six of a kind match)
*/

# include "Game.h"
# include "Dice.h"
# include "Player.h"
# include "PlayerOptions.h"

# using<system.dll>
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

/*
*  Receives user input and verifies to ensure only correct inputs are entered for integers
*  Function will verify input for different menu options based on options value 
*  passed through.
*  option: 1 = Player Setup - option: 2 - Storing Dice
*/
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


/* 
*  Receives user input and verifies to ensure only correct inputs are entered for strings
*  Function will verify input for different menu options based on options value 
*  passed through.
*  Optoin 1: Player Setup - Option 2: Storing Dice
*/
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
		playerList.clear(); // Clears vector to allow for new player count and information

		// Get the total number of players for the game
		playerCount = InputVerificationInt(1);

		// Call funciton to set up each player
		SetUpPlayers(playerCount);

		// Get input from user to either continue with or redu player selection
		userChoice = InputVerificationString(1);

	} while (userChoice.compare("N") == 0 || userChoice.compare("n") == 0);

	// sets up the array to hold all 6 dice objects
	for (int i = 0; i < 6; ++i)
	{
		DiceArray[i] = Dice(i+1);
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
	cout << " - Rolled Dice: " << endl;
	for (int i = 0; i < 6; ++i)
	{
		// If the first dice value is 0, then it is the first loop of the turn.
		// Prompts user to roll the dice when its the first loop.
		if (DiceArray[i].GetDiceValue() == 0)
		{
			cout << "    ...Roll the Dice!" << endl;
			break;
		}
		// If the dice is stored then the dice won't display on rolled dice
		else if (DiceArray[i].GetStoredValue()) {}

		// If the dice is not stored then display the rolled value
		else
		{
			cout << " Dice" << i + 1 << ": " << DiceArray[i].GetDiceValue();
			if (DiceArray[i].GetPointDie())
			{
				cout << "  -  " << "Point Dice!";
			}
			if (DiceArray[i].GetPointMatch())
			{
				cout << " - " << DiceArray[i].GetMatchType();
			}
			cout << endl;
		}
	}

	// Outputs the current stored dice, if there are no stored dice then nothing is output
	cout << endl << " - Stored Dice: " << endl;
	for (int i = 0; i < 6; ++i)
	{
		if (DiceArray[i].GetStoredValue())
		{
			cout << " Dice" << i + 1 << ": " << DiceArray[i].GetDiceValue();

			if (DiceArray[i].GetPointMatch()) // Outputs matching dice if there is a three of a kind match
			{
				cout << " - " << DiceArray[i].GetMatchType();
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
	cout << setfill('_') << setw(50) << " " << endl;

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
	int second = winning - 1;
	int third = winning - 2;

	// If second place and first are the same person then second is swapped to be a different player
	// to avoid first taking both places
	if (second < 0)
	{
		second = playerSize;
	}

	// If third place and first or second are the same person then third is swapped to be a different player
	// to avoid first or second taking both places
	if (third < 0)
	{
		third = playerSize - 1;
	}

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
			// If there are more than 2 players, then the third position will be checked as well
			else if (playerSize > 2)
			{
				// If the score is higher then thrid place, than that place is put as third place.
				if (playerList[i].GetPlayerScore() > playerList[third].GetPlayerScore() && i != second)
				{
					third = i;
				}
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
	// Loops through the dice object array and rolls all dice that are not stored
	for (int i = 0; i < 6; ++i)
	{
		// If the dice is stored, then it is not rolled
		if (DiceArray[i].GetStoredValue()) {}
		else 
		{
			DiceArray[i].SetDiceValue((rand() % (max - min + 1)) + min);
		}
	}

	// Call function to check if any point dice were rolled and return true or false whether
	// there are point dice in the set or not
	CheckForPointDice();
}


// ** COMPLETELY REWORKED FUNCTION ** //
void Game::CheckForPointDice()
{
	int countArray[6] = { 0 };
	int count = 0, matchCount = 0;
	bool matches = true;
	string matchType = "None";
	pointDiceInSet = false;

	// Loops through the dice array to add up how much of each number is stored to make calculations easier
	for (int i = 0; i < 6; ++i)
	{
		// If the dice is stored then it can be ignored, if not continue with the switch case
		if (!DiceArray[i].GetStoredValue())
		{
			// Checks what the value of the dice is and adds it to the corresponding index on the countArray
			//     I.E. countArray[0] is for ones, countArray[1] is for twos, and so on...
			//     If the dice value is 4, then the value of countArray[3] will be increased by one.
			switch (DiceArray[i].GetDiceValue())
			{
			case 1:
				countArray[0] += 1;
				break;
			case 2:
				countArray[1] += 1;
				break;
			case 3:
				countArray[2] += 1;
				break;
			case 4:
				countArray[3] += 1;
				break;
			case 5:
				countArray[4] += 1;
				break;
			case 6:
				countArray[5] += 1;
				break;
			}
		}
		// If the dice is not stored then matches is set to false as all 6 dice need to be stored in order for
		// a player to get a straight or three pairs (which is what the matches function checks)
		else
		{
			matches = false;
			break;
		}
	}

	// If matches is true, then there is chance that the player could have a straight or three pairs.
	if (matches)
	{
		matchType = CheckMatches(countArray); // Calls function to check for any matches and sets matchType

		// If the player has a straight or three pairs of twos then all dice in the array are set to point dice
		// with their corresponding match type and the function ends as all dice have been checked.
		if (matchType == "Straight" || matchType == "Three pairs of twos")
		{
			for (int i = 0; i < 6; ++i)
			{
				DiceArray[i].SetPointDie(true); // sets true for point dice
				DiceArray[i].SetPointMatch(true); // sets true for for part of a match
				DiceArray[i].SetMatchType(matchType); // sets what kind of match the die is
			}
			pointDiceInSet = true;
			return;
		}
	}

	// If there are no matches then the below code checks each individual dice to determine if there are three, four,
	// five, or six of a kind matches.
	for (int i = 0; i < 6; ++i)
	{
		// Initilize the temp array and count for each new dice being checked
		int tempArray[6] = {0};
		count = 0;

		// If the dice is already stored or listed as a point dice, then ignore it
		// If not, then continue with checking for matches
		if (DiceArray[i].GetStoredValue() || DiceArray[i].GetPointDie()) {}
		else
		{
			for (int j = 0; j < 6; ++j)
			{
				// If the dice is already stored or listed as a point dice, then ignore it
				if (DiceArray[i].GetStoredValue() || DiceArray[j].GetStoredValue()
					|| DiceArray[i].GetPointDie() || DiceArray[j].GetPointDie()) {}

				// Checks to see if two dice values are the same and if they are then it adds 1 to the count
				// If the count reaches three then it means there are three of a kind and those three dice will
				// be shown as point dice.
				else if (DiceArray[i].GetDiceValue() == DiceArray[j].GetDiceValue())
				{
					count = count + 1;

					if (count == 1) // If count is 1, then save array position int temp array
					{
						tempArray[0] = j;
					}
					else if (count == 2) // If count is 2, then save array position int temp array
					{
						tempArray[1] = j;
						matchType = "Pair";
					}
					else if (count == 3) // If count is 3, then save array position int temp array
					{
						tempArray[2] = j;
						matchType = "Three of a kind";
					}
					else if (count == 4) // If count is 4, then save array position int temp array
					{
						tempArray[3] = j;
						matchType = "Four of a kind";
					}
					else if (count == 5) // If count is 5, then save array position int temp array
					{
						tempArray[4] = j;
						matchType = "Five of a kind";
					}
					else if (count == 5) // If count is 6, then save array position int temp array
					{
						tempArray[5] = j;
						matchType = "Six of a kind";
					}
				}
			}
			// If there is a match of 2, 3, 4, 5, or 6, then all dice that are a part of that match will
			// be marked as point dice as well as a match with the match type they are.
			if (count > 1)
			{
				for (int j = 0; j < count; ++j)
				{
					DiceArray[tempArray[j]].SetPointDie(true); // sets true for point dice
					DiceArray[tempArray[j]].SetPointMatch(true); // sets true for for part of a match
					DiceArray[tempArray[j]].SetMatchType(matchType); // sets what kind of match the die is
				}
				pointDiceInSet = true;
			}
		}
	}

	// Checks the remaining dice to see if any 1s or 5s remain that are not in a three of a kind match
	for (int i = 0; i < 6; ++i)
	{
		// If the dice is already stored or marked as a point dice, then it is not checked
		if (DiceArray[i].GetStoredValue() || DiceArray[i].GetPointDie())
		{
		}
		else if (DiceArray[i].GetDiceValue() == 1 || DiceArray[i].GetDiceValue() == 5)
		{
			DiceArray[i].SetPointDie(true);
			pointDiceInSet = true;
		}
	}
}


// Functions allows players add dice to their currently stored dice
bool Game::KeepDice()
{
	int userStoreDice, count = 0;
	string userChoice = "N";
	bool diceStored = false;

	// while the user hasn't confirmed to continue with selected players
	do
	{
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
				if (DiceArray[i].GetStoredValue()) 
				{
					count = count + 1;
				}
			}

			if (count == 6)
			{
				playerList[currentPlayer].SetSixPointDice(true);
			}

			return diceStored;
		}

		// If the dice the user chose is already stored then output message to user
		if (DiceArray[userStoreDice].GetStoredValue())
		{
			cout << "Dice " << userStoreDice + 1 << " is already stored..." << endl;
			Sleep(1000);
		}

		// If the dice the user chose is not a point dice then output message to user
		else if (!DiceArray[userStoreDice].GetPointDie())
		{
			cout << "Dice " << userStoreDice + 1 << " is not a point dice..." << endl;
			Sleep(1000);
		}

		// The dice is a point dice and not yet stored so checks to confirm that the user wants to store the dice
		else
		{
			// If the dice is part of a matching set then the whole set is stored
			if (DiceArray[userStoreDice].GetPointMatch())
			{
				cout << endl << "Storing matching set!" << endl;
				for (int i = 0; i < 6; ++i)
				{
					// Checks the value and match of the dice the ensure only the same type are stored
					if (DiceArray[i].GetPointMatch() && DiceArray[i].GetDiceValue() == DiceArray[userStoreDice].GetDiceValue())
					{
						DiceArray[i].SetStoredValue(true);
					}
				}
				Sleep(1000);
				diceStored = true;
			}

			// If it is not part of a match then only the one dice is stored
			else
			{
				DiceArray[userStoreDice].SetStoredValue(true);
				diceStored = true;
			}
		}
	} while (userStoreDice != -1);

	return diceStored;
}


// Function allows players to remove dice from their currently stored dice
void Game::RemoveDice()
{
	int userRemoveDice, count = 0;
	string userChoice = "N";

	do
	{
		// Determins how many dice are stored to ensure that at least one point dice is still stored at all times
		count = 0;
		for (int i = 0; i < 6; ++i)
		{
			if (DiceArray[i].GetStoredValue())
			{
				++count;
			}
		}

		// Get dice value between 1 and 6 from user and minus 1 to match the vector values of 0-5
		userRemoveDice = InputVerificationInt(3) - 1;

		// If the user chooses 0 then they will return to the turn options
		if (userRemoveDice == -1)
		{
			return;
		}

		// If the dice the user chose is already stored then output message to user
		if (!DiceArray[userRemoveDice].GetStoredValue())
		{
			cout << "Dice " << userRemoveDice + 1 << " is not currenlty stored..." << endl;
			Sleep(1000);
		}

		// If the dice is a point dice and not yet removed, then checks to confirm that the user wants to removed the dice
		else
		{
			if (count == 1) // If only one dice is currently stored, then user can't remove the dice
			{
				cout << endl << "Can't remove this dice, must have at least one point dice stored!" << endl;
				Sleep(2000);
			}
			// If only two dice are stored and they're a pair, then user can't remove the dice
			else if (count == 2 && DiceArray[userRemoveDice].GetMatchType() == "Pair")
			{
				cout << endl << "Can't remove this set, must have at least one point dice stored!" << endl;
				Sleep(2000);
			}
			// If only three dice are stored and they're a match of three, then user can't remove the dice
			else if (count == 3 && DiceArray[userRemoveDice].GetMatchType() == "Three of a kind")
			{
				cout << "DOES THIS WORK " << DiceArray[userRemoveDice].GetMatchType() << endl;
				cout << endl << "Can't remove this set, must have at least one point dice stored!" << endl;
				Sleep(8000);
			}
			// If only four dice are stored and they're a match of four, then user can't remove the dice
			else if (count == 4 && DiceArray[userRemoveDice].GetMatchType() == "Four of a kind")
			{
				cout << endl << "Can't remove this set, must have at least one point dice stored!" << endl;
				Sleep(2000);
			}
			// If only five dice are stored and they're a match of five, then user can't remove the dice
			else if (count == 5 && DiceArray[userRemoveDice].GetMatchType() == "Five of a kind")
			{
				cout << endl << "Can't remove this set, must have at least one point dice stored!" << endl;
				Sleep(2000);
			}

			// If the user confirms removing the dice then checks to see if it is part of a match and removes all three
			else if (DiceArray[userRemoveDice].GetPointMatch())
			{
				cout << endl << "Removing matching set!" << endl;
				for (int i = 0; i < 6; ++i)
				{
					// Checks the value and match of the dice the ensure only the same type are removed
					if (DiceArray[i].GetPointMatch() && DiceArray[i].GetDiceValue() == DiceArray[userRemoveDice].GetDiceValue())
					{
						DiceArray[i].SetStoredValue(false);
					}
				}
				Sleep(1000);
			}

			// If it is not part of a match then only the one dice is removed
			else
			{
				DiceArray[userRemoveDice].SetStoredValue(false);
			}
		}

	} while (userRemoveDice != -1);
}


// Function will be called at the end of a players turn to calculate score
void Game::TurnEnd()
{
	CalculateScore(); // Calculate the players final score for the turn

	// If a players score hasn't reached the minimum score to enter the game, then they haven't entered the game and can't end their turn.
	if (currentScore < enterScoreReq)
	{
		cout << "Must get at least " << enterScoreReq << " points before you can end your turn!" << endl;
		Sleep(2000);
		return; // returns to continue players turn
	}
	else
	{
		// Adds turn score to the players total score
		playerList[currentPlayer].SetPlayerScore(currentScore);
		playerList[currentPlayer].SetEnteredGame(true);

		// If a player gets 10,000 points then they will be set as the current winner and the final round will begin
		if (playerList[currentPlayer].GetPlayerScore() >= winScoreReq)
		{
			playerList[currentPlayer].SetWinning(true);
			cout << " - Current Turn: Player " << currentPlayer + 1 << " - " << playerList[currentPlayer].GetPlayerName()
				<< " has reached " << winScoreReq << " points!!" << endl << "The final round has begun!!" << endl;
			Sleep(2000);
		}
	}
}


// ** ADDED FUNCTION ** //
// Checks the countArray from the calculate score function to determine if the player has a straight or three pairs.
string Game::CheckMatches(int countArray[6])
{
	int onesCount = 0, twosCount = 0;
	string matches = "None";

	// Loops through the count array the count of each dice value 1-6
	for (int i = 0; i < 6; ++i)
	{
		// If the value is 1 then add 1 to the ones count
		if (countArray[i] == 1)
		{
			onesCount += 1;
		}
		// If the value is 2 then add 1 to the twos count
		else if (countArray[i] == 2)
		{
			twosCount += 1;
		}

		// If twosCount is at three then all dice have been checked so the loop is no longer needed
		if (twosCount == 3)
		{
			break;
		}
	}
	// If the player has a striaght then set matches to Straight and return
	if (onesCount == 6)
	{
		return matches = "Straight";
	}
	// If the player has three pairs then set matches to Three pairs of two and return
	else if (twosCount == 3)
	{
		return matches = "Three pairs of two";
	}
	// Returns none if there are no straights or three pairs of twos.
	else
	{
		return matches;
	}
}


// ** COMPLETELY REWORKED FUNCTION ** //
// Takes the stored point dice and checks them against all possible scoring options to return the total turn score
void Game::CalculateScore()
{
	int countArray[6] = { 0 };
	int turnScore = 0;
	string matchType = "None";

	// Loops through the dice array to add up how much of each number is stored to make calculations easier
	for (int i = 0; i < 6; ++i)
	{
		// If the dice is stored then continue with the switch case, if its not then it can be ignored
		if (DiceArray[i].GetStoredValue())
		{
			// Checks what the value of the dice is and adds it to the corresponding index on the countArray
			//     I.E. countArray[0] is for ones, countArray[1] is for twos, and so on...
			//     If the dice value is 4, then the value of countArray[3] will be increased by one.
			switch (DiceArray[i].GetDiceValue())
			{
			case 1:
				countArray[0] += 1;
				break;
			case 2:
				countArray[1] += 1;
				break;
			case 3:
				countArray[2] += 1;
				break;
			case 4:
				countArray[3] += 1;
				break;
			case 5:
				countArray[4] += 1;
				break;
			case 6:
				countArray[5] += 1;
				break;
			}
		}
	}

	matchType = CheckMatches(countArray); // Calls function to check for any matches and sets matchType

	// If the match is for a Straight, then the player has a straight and 1,500 is added to their score
	// The funciton will then add the score to the current player score and exit as all 6 dice have been calculated
	if (matchType == "Straight")
	{
		turnScore += 1500;
		currentScore = storedScore + turnScore;
		return;
	}
	// If the match is for Three pairs of two, then the player has three pairs and 750 is added to their score
	// The funciton will then add the score to the current player score and exit as all 6 dice have been calculated
	else if (matchType == "Three pairs of two")
	{
		turnScore += 750;
		currentScore = storedScore + turnScore;
		return;
	}

	// If there are no matches, then the player does not have a staight or three pairs and the dice will be checked
	// against the remaining scoring options
	for (int i = 0; i < 6; ++i)
	{
		switch (countArray[i])
		{
		// Only 1's and 5's are able to meet this case and receive points
		case 1:
		case 2:
			if (i == 0) // Adds 200 to player score if only two 1's are stored
			{
				turnScore += countArray[i] * 100;
			}
			else if (i == 4) // Adds 100 to player score if only two 5's are stored
			{
				turnScore += countArray[i] * 50;
			}
			break;
		// If the count is 3, then there is a match of three
		case 3:
			if (i == 0) // If the dice value is 1, then add 1,000 to the player score
			{
				turnScore += 1000;
			}
			else // If the value is anything else, then add the value * 100 (I.E. 4 * 100 = 400)
			{
				turnScore += (i+1) * 100;
			}
			break;
		// If the count is 4, then there is a match of 4 and 2,000 will be added to the players score
		case 4:
			turnScore += 2000;
			break;
		// If the count is 5, then there is a match of 5 and 3,000 will be added to the players score
		case 5:
			turnScore += 3000;
			break;
		// If the count is 6, then there is a match of 6 and 5,000 will be added to the players score
		case 6:
			turnScore += 5000;
			break;
		}
	}
	currentScore = storedScore + turnScore;
}



// Gameplay loop will loop through each players turn calling all the necessary functions
// above when they are needed.
void Game::GameplayLoop(int enterGameScore, int winGameScore)
{
	// Set values of enter and win score requirements based on settings passed through
	enterScoreReq = enterGameScore;
	winScoreReq = winGameScore;

	// Sets up game by intilizing players
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
						for (int i = 0; i < 6; ++i)
						{
							if (!DiceArray[i].GetStoredValue())
							{
								DiceArray[i].SetPointDie(false);
								DiceArray[i].SetPointMatch(false);
							}
						}
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

					if (playerList[currentPlayer].GetSixPointDice())
					{
						firstRoll = false;
						CalculateScore();
						storedScore = currentScore; // Set stored score as the value of all 6 stored dice
						// Reset all the dice to their defaul for player to roll again
						for (int i = 0; i < 6; ++i)
						{
							DiceArray[i].SetDiceValue(0);
							DiceArray[i].SetPointDie(false);
							DiceArray[i].SetPointMatch(false);
							DiceArray[i].SetStoredValue(false);
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
					// If the player has already rolled once they may choose the end turn option
					if (firstRoll == true)
					{
						TurnEnd();
						// If the player has at a least the minimum amount of points set to enter the game, then they are free to end their turn
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
					DiceArray[i].SetDiceValue(0);
					DiceArray[i].SetPointDie(false);
					DiceArray[i].SetPointMatch(false);
					DiceArray[i].SetStoredValue(false);
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