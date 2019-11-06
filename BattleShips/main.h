#ifndef _MAIN_H
#define _MAIN_H

//this file contains all functions, the structure and the enum declarations with comments.

enum Direction //used to set the direction the ship pieces are placed in (relative to the first ship piece)
{
	Right,
	Left,
	Up,
	Down

};

struct Ship //an array of 5 Ships is later created to represent each ship
{
	char Name[20]; //name of the ship
	int Xpos[5]; // Ship X positions for all the ship pieces
	int Ypos[5]; // Ship Y positions for all the ship pieces
	int Size; //Size of the ship
	int Sank; //counter for how many ship pieces sank/got hit
	bool shipSank = false; //if the whole ship Sank or not
	Direction Dir; //the direction the ship pieces are placed in (relative to the first ship piece)
	
};


void MapInit(char _mapPlayer[10][10], char _mapComp[10][10]); //initializes both arrays to '#'

void MapDraw(char _mapPlayer[10][10], char _mapComp[10][10], bool _Lost); /*Draws the player array. 
															  The computer array is printed for testing.*/

void ShipInit(Ship _Ships[5]); /*Initialises most of the "Ships" array's contents to 0. 
							   It initializes the names and sizes appropriately*/

void ShipPlace(Ship _Ships[5], char _mapComp[10][10]); /*Places the ships randomly on the grid.
													   This function is where all collision checks for ship placement
													   and random functions are*/

void Fire(char _mapPlayer[10][10], char _mapComp[10][10], Ship _Ships[5]); /*asks for and validates coordinates
																		   from the user. Updates the grids suitably*/

void GameLoop(bool &demoMode); /*Creates both 2D arrays for the player and Computer grids.
				 Creates an array of size 5 of the structure Ship.
				 40 turn game loop. Calls the fire functions to ask for coordinates and fire in them,
				 sinking or missing ships. The mapDraw function is called to update the grid.
				 Also contains code for both win and loss states.*/

void Menu(); //main menu with options to start, exit and see instructions

#endif