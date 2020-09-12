#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <conio.h>
#define HEIGHT 30
#define WIDTH 60


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
//Draw Field
int draw_field(char** playground)
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
		output_string[a * (WIDTH + 1) + WIDTH ] = '\n';
	}
	output_string[(WIDTH + 1) * HEIGHT ] = '\0';
	printf("%s", output_string);
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

enum Direction
{
	up, down, left, right
};
//movement of sneek
int movement(sneeekpos& pos, Direction dir, char** playground)
{

		switch (dir)
		{
		case up:
			pos.y--;
			break;

		case left:
			pos.x--;
			break;

		case right:
			pos.x++;
			break;

		case down:
			pos.y++;
			break;

		default:
			break;
		}
	return 0;
}
//input from user
Direction get_input(char &old_input)
{
	while (true)
	{
		char input = _getch();

		switch (input)
		{
				// W & 8
		case 119:
		case 56:
			if (old_input == 115 || old_input == 50)
			{
				return down;
			}else
				old_input = input;
				return up;

			// S & 2
		case 115:
		case 50:
			if (old_input == 119 || old_input == 56)
			{
				return up;
			}
			old_input = input;
			return down;

			// A & 4
		case 97:
		case 52:
			if (old_input == 100 || old_input == 54)
			{
				return right;
			}
			else
			old_input = input;
			return left;

			// D & 6
		case 100:
		case 54:
			if (old_input == 97 || old_input == 52)
			{
				return left;
			}
			else
			old_input = input;
			return right;

		default:
			break;
		}
	}
}
//see if you failed
bool death_check(char** playground, sneeekpos pos)
{
	if (pos.x == 0 || pos.x == WIDTH - 1 || pos.y == 0 || pos.y == HEIGHT - 1)
	{
		//gameover
		return true;
	}
	return false;
}
// 1 Length sneak drawing 
int sneeek_draw(char** playground, sneeekpos pos, Direction dir, int &length_change)
{
	//draw Front position atm 
	playground[pos.y][pos.x] = 'o';
	//if length didnt change
	if (pos.length == length_change)
	{
		switch (dir)
		{
		case up:
			playground[pos.y + 1][pos.x] = ' ';
			break;
		case down:
			playground[pos.y - 1][pos.x] = ' ';
			break;
		case left:
			playground[pos.y][pos.x + 1] = ' ';
			break;
		case right:
			playground[pos.y][pos.x - 1] = ' ';
			break;
		default:
			break;
		}
	}

	length_change = pos.length;
	return 0;
}

struct position {
	int x;
	int y;
};



int main()
{								//// Before game starts \\\\
	//Create Playground
	char** playground = (char**)malloc(sizeof(char*) * HEIGHT);
	for (int a = 0; a < HEIGHT; a++)
	{
		playground[a] = (char*)malloc(sizeof(char) * WIDTH);
	}
	


	//Declarations 
	int foodx, foody, length_change = 1;
	char old_input = 100;
	sneeekpos pos;

	clear_field(playground);
	generate_food(foodx, foody, playground);


	//Start of Sneak
	playground[pos.y][pos.x] = 'o';

	//Baseline direction at the start of the game
	Direction dir = right;

								//// After game starts \\\\
	
	while (true)
	{


		//only on Keyboard press
		if (_kbhit()) {
			dir = get_input(old_input);
		}

		movement(pos, dir, playground);

		//Sneak hits the food finally 
		if (foodx == pos.x && foody == pos.y)
		{
			pos.length++;
			generate_food(foodx, foody, playground);
			//history of sneak, dependent from length of sneak
			position* history = (position*)malloc(sizeof(position) * pos.length);


		}

		if (death_check(playground, pos))
		{
			//gameover screen
			printf("you lose!");
			Sleep(5000);
		}




		sneeek_draw(playground, pos, dir, length_change);
		draw_field(playground);


		Sleep(100);
		//system("cls");
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD position = { 0 , 0 };
		SetConsoleCursorPosition(hConsole, position);
	}
	//Free Memory of Playground
	for (int a = 0; a < HEIGHT; a++)
	{
		free(playground[a]);
	}
	free(playground);

	return 0;
}