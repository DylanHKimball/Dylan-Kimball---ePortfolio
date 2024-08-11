/*
*    ||  -- Farkle Dice Game -- ||
*    |  Programmers Name: Dylan Kimball
*    |  Date: 7/28/2024
*    |  Version: 2.0
*
*    The player options class is an enum class that contains a list of the actions that a player 
*	 is able to take during their turn. Through the use of this class, identifing the action that 
*	 the player has taken is much simpler and adding more actions is also straightforward.
* 
*    NO CHANGES FROM ORIGINAL
*/

#pragma once

enum PlayerOptions {
	Roll,
	KeepDice,
	RemoveDice,
	EndTurn,
	Rules,
	Exit,
	None
};