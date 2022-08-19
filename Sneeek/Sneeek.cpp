#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <conio.h>
#include <iostream>
#define HEIGHT 30
#define WIDTH 75
#define RUNTIME 100


//Reset Field 
int clear_field(char** playground)
{
	for (int a = 0; a < HEIGHT; a++)
	{
		for (int b = 0; b < WIDTH; b++)
		{
			if (a == 0 || a == HEIGHT - 1 || b == 0 || b == WIDTH - 1)
			{
				playground[a][b] = '#';
			}
			else
				playground[a][b] = ' ';
		}
	}

	return 0;
}
//Generate Food
int generate_food(int& foodx, int& foody, char** playground)
{
	srand(time(NULL));
	foodx = rand() % (WIDTH - 3) + 1;
	foody = rand() % (HEIGHT - 3) + 1;

	playground[foody][foodx] = '$';
	return 0;
}
//sneek infos
struct sneeekpos {
	int x = 1;
	int y = 1;
	int length = 1;
};
//Draw Field
int draw_field(char** playground, sneeekpos pos)
{
	char output_string[HEIGHT * (WIDTH + 1) + 1];
	int b = 0;
	int a = 0;

	for (int a = 0; a < HEIGHT; a++)
	{
		for (b = 0; b < WIDTH; b++)
		{
			output_string[a * (WIDTH + 1) + b] = playground[a][b];
		}
		output_string[a * (WIDTH + 1) + WIDTH] = '\n';
	}
	output_string[(WIDTH + 1) * HEIGHT] = '\0';
	printf("%s\nScore: %d", output_string, pos.length);
	return 0;
}
//available directions for the Movement 
enum class Direction
{
	up, down, left, right
};
//movement of sneek
int movement(sneeekpos& pos, Direction dir, char** playground)
{
		switch (dir)
		{
		case Direction::up:
			pos.y--;
			break;

		case Direction::left:
			pos.x--;
			break;

		case Direction::right:
			pos.x++;
			break;

		case Direction::down:
			pos.y++;
			break;

		default:
			break;
		}
	return 0;
}
//input from user
Direction get_input(Direction dir)
{
	while (true)
	{
		char input = _getch();

		switch (input)
		{
				// W & 8
		case 119:
		case 56:
		case 72:
			if (dir == Direction::down)
			{
				return Direction::down;
			}else
				return Direction::up;

			// S & 2
		case 115:
		case 50:
		case 80:
			if (dir == Direction::up)
			{
				return Direction::up;
			}
			return Direction::down;

			// A & 4
		case 97:
		case 52:
		case 75:
			if (dir == Direction::right)
			{
				return Direction::right;
			}
			else
			return Direction::left;

			// D & 6
		case 100:
		case 54:
		case 77:
			if (dir == Direction::left)
			{
				return Direction::left;
			}
			else
			return Direction::right;

		default:
			break;
		}
	}
}

// 1 Length sneak drawing 
int sneeek_draw(char** playground, sneeekpos pos, Direction dir, int &length_change)
{
	//draw Front position atm 
	playground[pos.y][pos.x] = 'o';

	return 0;
}
//History of Sneeek position
struct position {
	int x;
	int y;
};
//see if you failed
bool death_check(char** playground, sneeekpos pos, position* history)
{
	for (int a = 1; a <= pos.length; a++)
	{
		if (history[a].x == pos.x && history[a].y == pos.y)
		{
			return true;
		}

		if (pos.x == 0 || pos.x == WIDTH - 1 || pos.y == 0 || pos.y == HEIGHT - 1)
		{
			//gameover
			return true;
		}
	}
	return false;
}
//Draw the Sneeek and its tail
int sneeek_tail(sneeekpos pos, position *history, char** playground, int foodx, int foody)
{
	for (int a = pos.length; a >= 1; a--)
	{
		history[a].x = history[a - 1].x;
		history[a].y = history[a - 1].y;
	}
	
	history[0].x = pos.x;
	history[0].y = pos.y;

	clear_field(playground);
	playground[foody][foodx] = '$';

	for (int a = 0; a < pos.length; a++)
	{
		playground [history[a].y] [history[a].x] = 'o';
	}

	return 0;
}

int game()
{								//// Before game starts \\\\
	//Create Playground
	char** playground = (char**)malloc(sizeof(char*) * HEIGHT);
	for (int a = 0; a < HEIGHT; a++)
	{
		playground[a] = (char*)malloc(sizeof(char) * WIDTH);
	}

	//Declarations 
	int foodx, foody, length_change = 1;
	sneeekpos pos;

	//Set Cursor Position to Top Left
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD positioncursor = { 0 , 0 };
	//Try to hide Cursor (50/50)
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 0;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(hConsole, &info);

	clear_field(playground);
	generate_food(foodx, foody, playground);

	//Baseline direction at the start of the game
	Direction dir = Direction::right;

	//history for the Tail of the sneak declaration 
	position* history = (position*)malloc(sizeof(position) * HEIGHT * WIDTH);



								//// After game starts \\\\
	
	while (true)
	{
		//User input only on Keypress, without pressing Enter
		if (_kbhit()) {
			dir = get_input(dir);
		}

		movement(pos, dir, playground);

		//Sneak hits the food finally 
		if (foodx == pos.x && foody == pos.y)
		{
			pos.length++;
			generate_food(foodx, foody, playground);
		}

		sneeek_tail(pos, history, playground, foodx,foody);


		//Death check if you hit the wall
		if (death_check(playground, pos, history))
		{
			//gameover screen

			printf("# Game Over - Thanks for playing Sneeek \n# You scored %d from possible %d Points\n# Press any key to start a new game", pos.length, (HEIGHT-2)*(WIDTH-2));
			Sleep(5000);

			_getch();
			system("cls");
			break;
		}
		
		draw_field(playground,pos);

		//time between each move 
		Sleep(RUNTIME);
		//Cursor Position to top left, drastically improves the visual effect
		SetConsoleCursorPosition(hConsole, positioncursor);
	}


	//Free Memory of Playground
	for (int a = 0; a < HEIGHT; a++)
	{
		free(playground[a]);
	}
	free(playground);
	free(history);
	
	return 0;
}


int main()
{
	while (true)
	{
		game();
	}

	return 0;
}