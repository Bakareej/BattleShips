#include <iostream>
#include <iomanip>  //for the strcpy function
#include <windows.h>  //for the sleep function
#include <Mmsystem.h> //for the BGM "Farewell Stack Exchange"'s code
#include <mciapi.h>
#pragma comment(lib, "Winmm.lib")
#include "main.h"



int main()
{
	PlaySound("Automation.wav", GetModuleHandle(NULL), SND_FILENAME | SND_ASYNC | SND_LOOP); //for the BGM "Farewell Stack Exchange"'s code

	while (true)
	{
		Menu(); //go to main menu .. only way to exit the game is by entering 4 in main menu
	}

	return 0;
}


void Menu()
{
	int Option = 0;
	bool validOption = true;
	bool Demo = false; //used to enable and disable demo mode

	system("CLS"); //printing menu options
	std::cout << "1: Start\n";
	std::cout << "2: Demo Mode\n";
	std::cout << "3: Instructions\n";
	std::cout << "4: Exit\n";

	std::cout << std::endl;

	//validating input
	do
	{
		std::cout << std::endl;
		std::cout << "Choose an option: ";
		std::cin >> Option;

		if (std::cin.fail()) // has a previous extraction failed?
		{
			validOption = false;

			std::cin.clear(); // clear error flag
			std::cin.ignore(32767, '\n'); // remove the invalid input from the buffer
		}
		else
		{
			validOption = true;
		}

	} while (Option < 1 || Option > 4 || validOption == false); //validating input


	switch (Option) //starting the game or exitting the console if the user chooses to
	{
	case 1:
	{
		Demo = false;
		GameLoop(Demo); //start the game
		break;
	}
	case 2:
	{
		Demo = true;
		GameLoop(Demo); //start the game in demo mode
		break;
	}
	case 3: //prints the instructions
	{
		std::cout << std::endl;
		std::cout << "The computer places 5 ships on the 10x10 grid randomly.\nEnter X and Y coordinates to aim at a position and fire.\nWin the game by sinking all ships.\nYou only have 40 shots in normal mode to sink all ships, otherwise u lose.\n";
		std::cout << "Choose start for normal mode(40 turns to win)\nOr Demo Mode for inifite ammo/turns.\n";
		std::cout << std::endl;
		std::cout << "Choose another option!";
		std::cout << std::endl;
		system("pause");
		break;
	}
	case 4:
	{
		exit(0); //close the console
		break;
	}

	}
}

void MapInit(char _mapPlayer[10][10], char _mapComp[10][10])
{
	//initialiing both arrays to '#'

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			_mapPlayer[i][j] = 35;
			_mapComp[i][j] = 35;
		}	
	}
}

void MapDraw(char _mapPlayer[10][10], char _mapComp[10][10], bool _Lost)
{

	system("CLS");

	if (_Lost == false) //displays the player grid unless the player lost
	{
		//printing the character grid for the coordinates

		std::cout << "    ";  //just to snap the character header to the rest of the grid

		for (char g = 65; g < 75; g++)
		{
			std::cout << std::setw(4) << g;
		}

		std::cout << std::endl;

		//printing the player grid
		for (int i = 0; i < 10; i++)
		{
			std::cout << std::setw(4) << i + 1; //printing the number grid

			for (int j = 0; j < 10; j++)
			{
				std::cout << std::setw(4) << _mapPlayer[i][j];
			}

			std::cout << std::endl;
		}

		std::cout << std::endl;
	}
	else if (_Lost == true) // displays the computer grid if the player loses
	{
		//printing the character grid for the coordinates

		std::cout << "    ";

		for (char g = 65; g < 75; g++)
		{
			std::cout << std::setw(4) << g;
		}

		std::cout << std::endl;

		//printing the computer grid
		for (int l = 0; l < 10; l++)
		{
			std::cout << std::setw(4) << l + 1; //printing the number grid

			for (int m = 0; m < 10; m++)
			{
				std::cout << std::setw(4) << _mapComp[l][m];
			}

			std::cout << std::endl;
		}

		std::cout << std::endl;
	}
/*
	//printing the computer grid just for testing (commenting this out)

	//printing the character grid for the coordinates

	std::cout << "    ";

	for (char g = 65; g < 75; g++)
	{
		std::cout << std::setw(4) << g;
	}

	std::cout << std::endl;

	for (int l = 0; l < 10; l++)
	{
		std::cout << std::setw(4) << l + 1; //printing the number grid

		for (int m = 0; m < 10; m++)
		{
			std::cout << std::setw(4) << _mapComp[l][m];
		}

		std::cout << std::endl;
	}
	
	std::cout << std::endl;
*/
}

void ShipInit(Ship _Ships[5]) //initialising each ship in the array
{
	for (int i = 0; i < 5; i++)
	{
		_Ships[i].Size = 0;
		_Ships[i].Sank = 0;
		_Ships[i].shipSank = false;

		for (int j = 0; j < 5; j++)
		{
			_Ships[i].Xpos[j] = 0;
			_Ships[i].Ypos[j] = 0;
		}
	}
	strcpy_s(_Ships[0].Name, "Aircraft Carrier"); //setting the name and size for each ship in the array
	_Ships[0].Size = 5;
	strcpy_s(_Ships[1].Name, "Battleship");
	_Ships[1].Size = 4;
	strcpy_s(_Ships[2].Name, "Destroyer");
	_Ships[2].Size = 3;
	strcpy_s(_Ships[3].Name, "Submarine");
	_Ships[3].Size = 3;
	strcpy_s(_Ships[4].Name, "Cruiser");
	_Ships[4].Size = 2;
}

void ShipPlace(Ship _Ships[5], char _mapComp[10][10])
{
	//setting X & Y positions for each ship piece
	srand(time(NULL));
	bool Collided = true;
	int x = 0;
	int y = 0;
	Direction dir = Direction(0);

	for (int i = 0; i < 5; i++)
	{
		for (int k=0; k < _Ships[i].Size; k++)
		{
			while (Collided == true)
			{
				if (k == 0)
				{
					dir = Direction(rand() % 4);

					//generating random X and Y positions that do not go beyond the map array boundaries
					//a different random function was added for each ship size and direction (relative to the first ship piece)
					switch (_Ships[i].Size)
					{
					case 5:
					{
						if (dir == Right)
						{
							x = (rand() % 6);
							y = (rand() % 10);
						}
						else if (dir == Left)
						{
							x = (rand() % 6) + 4;
							y = (rand() % 10);
						}

						else if (dir == Up)
						{
							x = (rand() % 10);
							y = (rand() % 6) + 4;
						}
						else if (dir == Down)
						{
							x = (rand() % 10);
							y = (rand() % 6);
						}

						break;
					}

					case 4:
					{
						if (dir == Right)
						{
							x = (rand() % 7);
							y = (rand() % 10);
						}
						else if (dir == Left)
						{
							x = (rand() % 7) + 3;
							y = (rand() % 10);
						}
						else if (dir == Up)
						{
							x = (rand() % 10);
							y = (rand() % 7) + 3;
						}
						else if (dir == Down)
						{
							x = (rand() % 10);
							y = (rand() % 7);
						}

						break;
					}

					case 3:
					{
						if (dir == Right)
						{
							x = (rand() % 8);
							y = (rand() % 10);
						}
						else if (dir == Left)
						{
							x = (rand() % 8) + 2;
							y = (rand() % 10);
						}
						else if (dir == Up)
						{
							x = (rand() % 10);
							y = (rand() % 8) + 2;
						}
						else if (dir == Down)
						{
							x = (rand() % 10);
							y = (rand() % 8);
						}

						break;
					}

					case 2:
					{
						if (dir == Right)
						{
							x = (rand() % 9);
							y = (rand() % 10);
						}
						else if (dir == Left)
						{
							x = (rand() % 9) + 1;
							y = (rand() % 10);
						}
						else if (dir == Up)
						{
							x = (rand() % 10);
							y = (rand() % 9) + 1;
						}
						else if (dir == Down)
						{
							x = (rand() % 10);
							y = (rand() % 9);
						}

						break;
					}

					}
				}
					
				// x is column & y is row

				if (_mapComp[y][x] != '#')
				{
					Collided = true;
					k = 0;
				}
				else if (k == 0) //is this the first part of the ship/tip of it?
				{
					Collided = false;

					_Ships[i].Xpos[k] = x;
					_Ships[i].Ypos[k] = y;
					_Ships[i].Dir = dir;
				}

				else if (k == 1 || k == 2 || k == 3 || k == 4)/*if this isnt the first piece of the ship
															  place the rest of it in the direction that was generated*/
				{
					switch (_Ships[i].Dir)
					{
					case Right:
					{
						if (_mapComp[_Ships[i].Ypos[k - 1]][_Ships[i].Xpos[k - 1] + 1] != '#')
						{//is the place to the right of the previous piece not empty?
							Collided = true;
							k = 0;//if it isnt empty reset the entire loop
						}
						else
						{//if it is empty set the position of this piece to the right of the previous one
							Collided = false;

							_Ships[i].Xpos[k] = _Ships[i].Xpos[k - 1] + 1;
							_Ships[i].Ypos[k] = _Ships[i].Ypos[k - 1];

						}

						break;
					}

					case Left:
					{
						if (_mapComp[_Ships[i].Ypos[k - 1]][_Ships[i].Xpos[k - 1] - 1] != '#')
						{
							Collided = true;
							k = 0;
						}
						else
						{

							Collided = false;

							_Ships[i].Xpos[k] = _Ships[i].Xpos[k - 1] - 1;
							_Ships[i].Ypos[k] = _Ships[i].Ypos[k - 1];
						}

						break;
					}

					case Up:
					{
						if (_mapComp[_Ships[i].Ypos[k - 1] - 1][_Ships[i].Xpos[k - 1]] != '#')
						{
							Collided = true;
							k = 0;
						}
						else
						{

							Collided = false;

							_Ships[i].Xpos[k] = _Ships[i].Xpos[k - 1];
							_Ships[i].Ypos[k] = _Ships[i].Ypos[k - 1] - 1;
						}

						break;
					}

					case Down:
					{
						if (_mapComp[_Ships[i].Ypos[k - 1] + 1][_Ships[i].Xpos[k - 1]] != '#')
						{
							Collided = true;
							k = 0;
						}
						else
						{

							Collided = false;

							_Ships[i].Xpos[k] = _Ships[i].Xpos[k - 1];
							_Ships[i].Ypos[k] = _Ships[i].Ypos[k - 1] + 1;
						}

						break;
					}

					}
				}
			}

			Collided = true;

		}

		Collided = true;

		/*adding the ships to the comp array according to the position arrays & size*/

		for (int r = 0; r < _Ships[i].Size; r++)
		{
			_mapComp[_Ships[i].Ypos[r]][_Ships[i].Xpos[r]] = _Ships[i].Name[0];
		}
	}
}

void Fire(char _mapPlayer[10][10], char _mapComp[10][10], Ship _Ships[5])
{
	char cX = 0; //char used to get X coordinate as a letter
	int X = 0;
	int Y = 0;
	bool validY = false;


	do
	{
		//asking for coordinates and checking if input is within range
		do
		{
			std::cout << std::endl;
			std::cout << "Enter X coordinate (A-J) and Y coordinate (1-10): "; //asking for coordinate input
			std::cin >> cX;

			for (char t = 65; t < 75; t++)
			{
				if (cX == t)
				{
					X = t - 65; //changing it to be used as an index for collision checks in the array
				}
			}

			std::cin >> Y;

			Y--; //changing it to be used as an index for collision checks in the array

			if (std::cin.fail()) // has a previous extraction failed?
			{
				validY = false;

				std::cin.clear(); // clear error flag
				std::cin.ignore(32767, '\n'); // remove the invalid input from the buffer
			}
			else if ((Y < 0) || (Y > 9)) //checking if y is out of range
			{
				validY = false;
			}
			else if ((Y >= 0) || (Y <= 9))
			{
				validY = true;
			}
		} while (cX < 65 || cX > 74 || validY == false);


		if ((_mapPlayer[Y][X] != '#')) //asking for a different input if that position was targetted before
		{
			std::cout << std::endl;
			std::cout << "You have already hit this position!\n";
			std::cout << std::endl;
			Sleep(500);
		}
	
	//checking if the position entered was already hit before
	} while ((_mapPlayer[Y][X] != '#'));

	//checking whether a battleship was hit or missed

	if ((_mapComp[Y][X] != '#') && (_mapPlayer[Y][X] != 'H') && (_mapComp[Y][X] != 'M'))
	{
		std::cout << "Hit!";
		_mapPlayer[Y][X] = 'H';
		std::cout << std::endl;

		Sleep(500);
	}
	else if (_mapComp[Y][X] == '#')
	{
		std::cout << "Missed!";
		_mapComp[Y][X] = 'M';
		_mapPlayer[Y][X] = 'M';
		std::cout << std::endl;

		Sleep(500);
	}

	//updating the ships array by incrementing the counter for pieces that got hit for each ship

	for (int i = 0; i < 5; i++)
		for (int j = 0; j < _Ships[i].Size; j++)
		{
			if (_Ships[i].Xpos[j] == X && _Ships[i].Ypos[j] == Y)
			{
				_Ships[i].Sank = _Ships[i].Sank + 1;
			}
		}

	//checking if any ship sank and notifying the player which one it was!

	for (int i = 0; i < 5; i++)
	{
		if ((_Ships[i].Sank >= _Ships[i].Size) && (_Ships[i].shipSank == false))  /*making sure the msg wasnt 
																			displayed before for the same ship*/
		{
			_Ships[i].shipSank = true;

			std::cout << "The " << _Ships[i].Name << " Sank!";
			std::cout << std::endl;

			for (int j = 0; j < _Ships[i].Size; j++)
			{
				_mapPlayer[_Ships[i].Ypos[j]][_Ships[i].Xpos[j]] = _Ships[i].Name[0];
			}

			Sleep(2000);
		}
	}
}

void GameLoop(bool &demoMode)
{
	//creating and initialising both grids

	char mapPlayer[10][10];
	char mapComp[10][10];

	MapInit(mapPlayer, mapComp);

	//creating and initialzing the ships array (an array of struct ship)

	Ship Ships[5];

	ShipInit(Ships);

	//placing each ship on the computer grid

	ShipPlace(Ships, mapComp);

	bool gameLost = false;

	//initial map display

	MapDraw(mapPlayer, mapComp, gameLost);

	if (demoMode == false) //Ammo doesnt count in demo mode
	{
		std::cout << "You have 40 Turns!";
		std::cout << std::endl;
	}
	else if (demoMode == true) // notifying the player that demo mode is on
	{
		std::cout << "Demo Mode On! Infinite Ammo!";
		std::cout << std::endl;
	}

	int ammoUsed = 1;
	bool gameWon = false;
	int sankCntr = 0;


	//40 turn game loop 

	while (ammoUsed <= 40 && !(gameWon)) /*the first condition is disabled in demo mode										 by not incremenating the ammo used*/
	{
		//asking for coordinate input

		Fire(mapPlayer, mapComp, Ships);


		//updating the map

		MapDraw(mapPlayer, mapComp, gameLost);

		if (demoMode == false) //Ammo doesnt count in demo mode
		{
			//notifying players about the number of turns left
			std::cout << (40 - ammoUsed) << " Turns left!";
			std::cout << std::endl;
		}
		else if (demoMode == true) // notifying the player that demo mode is on
		{
			std::cout << "Demo Mode On! Infinite Ammo!";
			std::cout << std::endl;
		}

		sankCntr = 0;

		/*checking how many ships sank and ending the game if all ships sank*/
		for (int i = 0; i < 5; i++)
		{
			if (Ships[i].shipSank == true)
			{
				sankCntr++;

				if (sankCntr == 5)
				{
					gameWon = true;

					std::cout << std::endl;
					std::cout << "Congrats! You Won!\n";
					std::cout << std::endl;

					system("pause"); //waiting for intput to go back to main menu

				}
			}
;
		}

		if (demoMode == false) //Ammo doesnt count in demo mode so it is not incremented
		{
			ammoUsed++;
		}
	}

	//loss state
	//Demo mode never reaches this
	if (ammoUsed > 40)
	{
		gameLost = true;
		MapDraw(mapPlayer, mapComp, gameLost);
		std::cout << std::endl;
		std::cout << "You Lost..\n";
		std::cout << std::endl;

		system("pause"); //waiting for input to go back to main menu
	}

	//returns to main menu anyways
}