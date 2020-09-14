#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <conio.h>
#define HEIGHT 30
#define WIDTH 60
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
//available directions for the Movement 
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

// 1 Length sneak drawing 
int sneeek_draw(char** playground, sneeekpos pos, Direction dir, int &length_change)
{
	//draw Front position atm 
	playground[pos.y][pos.x] = 'o';

	return 0;
}

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
	char old_input = 100;
	sneeekpos pos;

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD positioncursor = { 0 , 0 };
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 0;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(hConsole, &info);

	clear_field(playground);
	generate_food(foodx, foody, playground);

	//Baseline direction at the start of the game
	Direction dir = right;

	//history for the Tail of the sneak declaration 
	position* history = (position*)malloc(sizeof(position) * HEIGHT * WIDTH);


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
		}

		sneeek_tail(pos, history, playground, foodx,foody);


		//Death check if you hit the wall
		if (death_check(playground, pos, history))
		{
			//gameover screen

			printf("### Game Over - Thanks for playing Sneeek - Press any key### \n# Your Score: %d", pos.length);
			Sleep(1000);
			_getch();
			system("cls");
			break;
			
		}
		
		draw_field(playground);

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