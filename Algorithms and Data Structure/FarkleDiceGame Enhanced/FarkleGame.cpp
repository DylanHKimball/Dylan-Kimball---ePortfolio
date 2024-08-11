/*
*    ||  -- Farkle Dice Game -- ||
*    |  Programmers Name: Dylan Kimball
*    |  Date: 7/28/2024
*    |  Version: 2.0
* 
*	 This code was created to mimic the farkle dice game and allow for individuals or groups to play Farkle on the computer.
*	 The goal was to make this program easy to use and understand for anyone that wants to play. This game includes the 
*	 majority of functionality and scoring options of the classic Farkle game. The program utilizes the .NET framework for
*    its functionality. This file is the main starting file that will display the main menu and allow the users to either
*	 start a game, change settings, or exit the program. If a user chooses to start the game then this file will create a 
*	 game object and call the function that starts the game loop.
* 
*	 ENHANCEMENTS TO WHOLE PROGRAM:
*		- Changed to data structure from a double array system to an array of dice objects.
*			 - ADDED: Dice.h & dice.cpp
*   	- Reworked entire code to work with the new dice object array system vs the original
*		    dual array system. (Nearly every function called the dual arrays in some way, so
*    	    nearly every funciton has been slightly modified)
*		- Completely reworked check for point dice function to include new scoring options 
*		- Completely reworked calculate score function to include new scoring options
*	    - Added check matches function to help check point dice and calculate score funciton
* 			- New scoring options (Stright, Three pairs, Four, Five, & Six of a kind match)
*	    - Added settings to main menu to allow the user to change the score requirements for
*			entering and winning the game.
* 
*	 ENHANCEMENTS TO THIS FILE:
*		- Added settings to main menu to allow the user to change the score requirements for
*			entering and winning the game.
*/
# include "Game.h"

# include <iostream>
# include <iomanip>
# include <Windows.h>

using namespace std;


// ** ADDED FUNCTION ** //
// Displays prompt for user to enter new score requirement values for entering and winning the game
void displayScoreChange(int settingOption)
{
	system("CLS");
	cout << setfill('_') << setw(80) << " " << endl;
	cout << setfill('_') << setw(80) << " " << endl;
	if (settingOption == 3)
	{
		cout << "||               - Enter new score requirement to enter game -               ||" << endl;
	}
	else
	{
		cout << "||               - Enter new score requirement to win game -                 ||" << endl;
	}
	cout << setfill('_') << setw(80) << " " << endl;
	cout << setfill('_') << setw(80) << " " << endl;
}


// ** ADDED FUNCTION ** //
// Displays setting options for user
void displaySettings(int enterGameScore, int winGameScore)
{
	system("CLS");
	cout << setfill('_') << setw(80) << " " << endl;
	cout << setfill('_') << setw(80) << " " << endl;
	cout << "||                               - Settings -                                ||" << endl;
	cout << setfill('_') << setw(80) << " " << endl;
	cout << setfill('_') << setw(80) << " " << endl;
	cout << "          ||         - Choose a setting to change below -         ||" << endl;
	cout << "          " << setfill('_') << setw(59) << " " << endl;
	cout << "                      - 0: Score required to enter game: " << enterGameScore << endl;
	cout << "                      - 1: Score required to win game: " << winGameScore << endl;
	cout << "                      - 2: Return to menu " << endl;
	cout << "          " << setfill('_') << setw(59) << " " << endl;
}


// ** MOVED FROM INPUT VERIFICATION TO ITS OWN FUNCTION ** //
// Displays menu options for user
void displayMenu()
{
	system("CLS");
	cout << setfill('_') << setw(80) << " " << endl;
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
	cout << setfill('_') << setw(80) << " " << endl;
	cout << "               ||         - Choose an Option Below -          ||" << endl;
	cout << "               " << setfill('_') << setw(50) << " " << endl;
	cout << "               ||          - 0: Start Game                    ||" << endl;
	cout << "               ||          - 1: Settings                      ||" << endl;
	cout << "               ||          - 2: Exit                          ||" << endl;
	cout << "               " << setfill('_') << setw(50) << " " << endl;
}


// ** REWORKED INPUT VERIFICATION TO WORK FOR MULTIPLE PURPOSES ** //
// Receives user input and verifies to ensure only correct inputs are entered for integers
int InputVerificationInt(int displayOption, int enterScore = 0, int winScore = 0)
{
	int userInput = 0;
	do {
		// If display option is 1 then the menu options will be shown
		if (displayOption == 1)
		{
			displayMenu();
		}
		// If display option is 2 then the settings options will be shown
		else if (displayOption == 2)
		{
			displaySettings(enterScore, winScore);
		}
		// If display option is 2 then the settings options will be shown
		else if (displayOption == 3 || displayOption == 4)
		{
			displayScoreChange(displayOption);
		}

		// Trys to get grade value from user and displays error messages for invalid inputs
		try {
			string inputString; // String input is used to account for anything the user inputs
			cin >> inputString;

			userInput = stoi(inputString); //Converts string to integer

			// For menue and settings options, the below checks will be performed on user input
			// GAME MENU - player can only choose between options 0-2, anything else is invalid
			if (displayOption == 1 || displayOption == 2)
			{
				if (userInput > -1 || userInput < 3)
				{
					return userInput;
				}
				else // If all checks pass, then return the input
				{
					cout << "Not a valid option!" << endl << endl;
					Sleep(1000); // wait 1 second
				}
			}
			// When user is updating the score requirement to enter the game, the below checks will be made on the user input
			// 3,000 is the max for entering the game, as anything higher would be nearly impossible with the scoring system
			if (displayOption == 3)
			{
				if (userInput < 0 || userInput > 3000)
				{
					cout << "Enter score requirement can't be less than 0 or greater than 3,000!" << endl << endl;
					Sleep(1000); // wait 1 second
				}
				else if (userInput >= winScore)
				{
					cout << "Enter score requirement can't be greater than the win score requirement!" << endl << endl;
					Sleep(1000); // wait 1 second
				}
				else // If all checks pass, then return the input
				{
					return userInput;
				}
			}
			// When user is updating the score requirement to win the game, the below checks will be made on the user input
			if (displayOption == 4)
			{
				if (userInput < 500 || userInput > 100000)
				{
					cout << "Win score requirement can't be less than 500 or greater then 100,000!" << endl << endl;
					Sleep(1000); // wait 1 second
				}
				else if (userInput <= enterScore)
				{
					cout << "Enter score requirement can't be greater than the win score requirement!" << endl << endl;
					Sleep(1000); // wait 1 second
				}
				else // If all checks pass, then return the input
				{
					return userInput;
				}
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


// ** ADDED FUNCTION ** //
// Function allows for user to change the values of the score requirement to enter the game and to
// win the game.
void updateSettings(int* enterScore, int* winScore)
{
	int userChoice = -1;

	// Code will display settings options until the user chooses the option to return to menu
	do {
		userChoice = InputVerificationInt(2, *enterScore, *winScore);

		// If user choice is 1 then update enter game score requirement
		if (userChoice == 0)
		{
			*enterScore = InputVerificationInt(3, *enterScore, *winScore);
		}
		// If user choice is 2 then update win game score requirement
		else if (userChoice == 1)
		{
			*winScore = InputVerificationInt(4, *enterScore, *winScore);
		}
		// If user choice is 2 then return to menu
		else if (userChoice == 2)
		{
			break;
		}
	} while (userChoice != 2);
}


// ** REWORKED FUNCTION TO INCLUDE SETTINGS OPTION ** //
// Main funciton that allow users to start game, go to settings, or exit the program
void main()
{
	int userChoice = -1;
	// Default values for enter and win game score requirements (can be adjusted in settings)
	int enterGameScore = 1000;
	int winGameScore = 10000;
	// Used to see how many games the user plays in a session
	// Maybe add a scoreboard in the future for different game sessions
	int loopCount = 1;

	do {
		userChoice = InputVerificationInt(1);

		// If user choice is 1 then start a new game
		if (userChoice == 0)
		{
			Game newGame(loopCount);
			newGame.GameplayLoop(enterGameScore, winGameScore);

			// If game over for the current game is set to true then exit the program
			if (newGame.GetGameOver())
			{
				userChoice = 1;
			}
			++loopCount;
		}
		// If user choice is 2 then exit the game
		else if (userChoice == 1)
		{
			updateSettings(&enterGameScore,&winGameScore);
		}
		// If user choice is 2 then exit the game
		else if (userChoice == 2)
		{
			break;
		}
	} while (userChoice != 2);

	system("CLS");
	cout << "Have a good day!!" << endl;
	Sleep(2000);
}