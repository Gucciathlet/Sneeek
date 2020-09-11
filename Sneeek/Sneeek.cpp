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
	for (int a = 0; a < HEIGHT; a++)
	{
		for (int b = 0; b < WIDTH; b++)
		{
			printf("%c", playground[a][b]);
		}
		printf("\n");
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
Direction get_input()
{
	while (true)
	{
		char input = _getch();
		switch (input)
		{
			// W & 8
		case 119:
		case 56:
			return up;

			// S & 2
		case 115:
		case 50:
			return down;

			// A & 4
		case 97:
		case 52:
			return left;

			// D & 6
		case 100:
		case 54:
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





int main()
{
	//Create Playground
	char** playground = (char**)malloc(sizeof(char*) * HEIGHT);
	for (int a = 0; a < HEIGHT; a++)
	{
		playground[a] = (char*)malloc(sizeof(char) * WIDTH);
	}

	//declare
	int foodx, foody;
	sneeekpos pos;


	clear_field(playground);
	generate_food(foodx, foody, playground);

	//Baseline direction
	Direction dir = right;
	//only on Keyboard press
	if (_kbhit()) {
		dir = get_input();
	}
	movement(pos, dir, playground);


	if (death_check(playground, pos))
	{
		//gameover screen
		printf("you lose!");
	}




	draw_field(playground);
	//game loop 
	while (true)
	{



	}








	//Free Memory of Playground
	for (int a = 0; a < HEIGHT; a++)
	{
		free(playground[a]);
	}
	free(playground);

	return 0;
}