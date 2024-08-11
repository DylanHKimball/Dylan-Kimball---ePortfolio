
// Dylan Kimball - IT-312 - Final Project: Farkle Game

// This file contains the main function that will display the menu and create the game object.

# include "Game.h"

# include <iostream>
# include <iomanip>
# include <Windows.h>

using namespace std;


// Receives user input and verifies to ensure only correct inputs are entered for integers
int InputVerificationInt()
{
	int userInput = 0;
	do {
		// Display menu options
		system("CLS");
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
		cout << "               ||          - 1: Exit                          ||" << endl;
		cout << "               " << setfill('_') << setw(50) << " " << endl;

		// Trys to get grade value from user and displays error messages for invalid inputs
		try {
			string inputString; // String input is used to account for anything the user inputs
			cin >> inputString;


			userInput = stoi(inputString); //Converts string to integer

			// GAME SETUP - PLAYERS: If the input is 1 or less, out put error message to display
			// that there must be at least 2 players
			if (userInput > -1 || userInput < 2)
			{
				return userInput;
			}
			// If all checks pass, then return the input
			else
			{
				cout << "Not a valid option!" << endl << endl;
				Sleep(1000); // wait 1 second
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


void main()
{
	int userChoice = -1;
	// Used to see how many games the user plays in a session
	// Maybe add a scoreboard in the future for different game sessions
	int loopCount = 1;

	do {
		userChoice = InputVerificationInt();

		// If user choice is 1 then start a new game
		if (userChoice == 0)
		{
			Game newGame(loopCount);
			newGame.GameplayLoop();

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
			break;
		}
	} while (userChoice != 1);

	system("CLS");
	cout << "Have a good day!!" << endl;
	Sleep(2000);
}