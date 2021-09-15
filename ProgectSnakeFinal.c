#define _CRT_SECURE_NO_WARNINGS
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

// The program generates a Snake game on a table (size 25X75) and let the User play until he eats 10 '$' (food) and wins or until he crashes with himeself or the table border and loses.
#define UP 72  // Up Arrow 
#define DOWN 80 // Down Arrow 
#define LEFT 75  // Left Arrow 
#define RIGHT 77  // Right Arrow 
#define ESC 27  // Escape Key
#define ROWS 25 // table length
#define COLS 75 // table hight
#define SNAKE_MAX_SIZE 21
#define GAMEPLAY_SPEED 300


typedef struct point_on_matrix                  //variable that containes (x,y) values on the board
{
	int xPoint;
	int yPoint;
}_POINT;


typedef struct snake                           // varieble that represent the snake's body on the board
{
	int x;
	int y;
}SNAKE;






void gotoxy(int y, int x);                  // function declarations.
int getKey();
_POINT randFood(SNAKE[], int size);
void initialize_array(char[][COLS]);
void print_array_full(char[][COLS]);
void print_menu();
bool gamePlay(char board[][COLS]);
void initialize_SNAKE(SNAKE snake[], int size, int x, int y);
void initialize_new_SNAKE(SNAKE snake[], int size, int x, int y, int cur_head);
void update_SNAKE(SNAKE snake[], int size, int x, int y, int cur_head);
void print_SNAKE(SNAKE snake[], int size);
void updateChar(_POINT rand_food, char food);
bool gameOver(SNAKE snake[], char board[][COLS], int x, int y, int size);
bool snakeEats(int x, int y, _POINT rand_food);
int previus_Head(int size, int cur_head);
void swapCells(SNAKE* first, SNAKE* second);





void main()
{
	bool result;
	int choice = -1;
	srand(time(NULL));


	char board[ROWS][COLS];
	initialize_array(board);
	while (1 == 1)
	{
		print_menu();

		while (choice != 1)
		{
			scanf("%d", &choice);
			while ((getchar()) != '\n');  // to avoid infinate loop if the User chooses something else than an integer.
			if (choice == 0)
				return;
			print_menu();
		}
		system("cls");



		print_array_full(board);


		result = gamePlay(board);
		system("cls");
		if (result == true)
		{
			printf("##################################\n");
			printf("           GAME OVER\n");
			printf("##################################\n");
		}
		else
		{
			printf("##################################\n");
			printf("           YOU WIN\n");
			printf("##################################\n");
		}
		choice = -1;

	}




}

bool gamePlay(char board[][COLS]) // the function gets a board and runs the game on the board until a lose or a win
{

	bool game_over = false;
	bool snake_ate = false;
	int size = 1;
	int cur_head = size - 1;
	int previus_head = -1;
	_POINT rand_food;
	char c;
	SNAKE snake[SNAKE_MAX_SIZE];
	int x = 40, y = 12;  // x for coulmn and y for line 

	// start from the middle of the screen
	gotoxy(x, y);
	// the direction is Right untill an arrow hit
	c = RIGHT;
	char temp = -1;
	rand_food = randFood(snake, size);
	updateChar(rand_food, '$');


	if (_kbhit()) // if any key was hit
		temp = getKey();
	while (!(temp == ESC || temp == RIGHT || temp == DOWN || temp == LEFT || temp == UP))
		if (_kbhit()) // if any key was hit
			temp = getKey();
	c = temp;




	while (game_over == false)
	{
		// sleeps for half a second before continuing to the next command
		Sleep(GAMEPLAY_SPEED);

		// only if a key was hit we read what key code it was
		if (_kbhit()) // if any key was hit
			temp = getKey();  // change direction
		// make a move in current direction if no change
		if (temp == ESC || temp == RIGHT || temp == DOWN || temp == LEFT || temp == UP)
			c = temp;

		switch (c)
		{
		case ESC:
			return true;   // to exit the program
			break;
		case RIGHT:
			x++;
			break;
		case DOWN:
			y++;
			break;
		case LEFT:
			x--;
			break;
		case UP:
			y--;
			break;


		}
		if (size == 21)
			return false;                       // win

		game_over = gameOver(snake, board, x, y, size);
		if (game_over == true)
			return true;                      // loss

		previus_head = previus_Head(size, cur_head);
		if ((snakeEats(snake[previus_head].x, snake[previus_head].y, rand_food)) == true)

		{

			size++;
			initialize_new_SNAKE(snake, size, x, y, cur_head);
			cur_head++;
			rand_food = randFood(snake, size);
			updateChar(rand_food, '$');

		}
		else
		{
			if (size < 11)
			{
				gotoxy(x, y);
				initialize_SNAKE(snake, size, x, y);
				print_SNAKE(snake, size);
				size++;
				cur_head++;

			}
			else
			{

				update_SNAKE(snake, size, x, y, cur_head);
				if (cur_head == size - 1)
					cur_head = 0;
				else
					cur_head++;

			}



		}


	}
	return true;

}

bool snakeEats(int x, int y, _POINT rand_food) // the function recives cuur x and y integers and a _POINT variable that containes the xPoint and Y point of the food and returns true if the current (x,y) is equal to the 
{                                               // xPoint and yPoint, else return false

	if ((x == rand_food.xPoint) && (y == rand_food.yPoint))
		return true;

	return false;

}




bool gameOver(SNAKE snake[], char board[][COLS], int x, int y, int size)      // the function recives a snake array, a matrix , the current x and y variables and the snake size and checks
{                                                                            // if the snake hits himself or the border, if so returns true, else false.
	if (board[y][x] == '#')
		return true;
	for (int i = 0; i < size; i++)
	{
		if ((snake[i].x == x) && (snake[i].y == y))
			return true;

	}
	return false;

}

void print_SNAKE(SNAKE snake[], int size)               // the function recives a snake array,and his size and prints '@' with his x,y variables on the board.
{
	for (int i = 0; i < size; i++)
	{
		gotoxy(snake[i].x, snake[i].y);
		printf("@");
		gotoxy(26, 81);

	}
}


void updateChar(_POINT rand_food, char food)          // the function recives a random food coordinates (as x and y) and prints '$' on those coordinates
{

	gotoxy(rand_food.xPoint, rand_food.yPoint);
	printf("%c", food);
}

_POINT randFood(SNAKE snake[], int size)             // the function recives a snake array,and his size and returns a random food coordinates (with random x and random y) that not outside the board or on the snake
{
	_POINT rand_food;
	rand_food.xPoint = 1 + rand() % (COLS - 2);
	rand_food.yPoint = 1 + rand() % (ROWS - 2);

	for (int i = 0; i < size; i++)
	{
		if ((rand_food.xPoint == snake[i].x) && (rand_food.yPoint == snake[i].y))
		{
			rand_food = randFood(&snake[0], size);
			break;
		}
	}

	return rand_food;

}


void update_SNAKE(SNAKE snake[], int size, int x, int y, int cur_head) // the function recives a snake array, his size, current x and y integers and cur_head and updates the snake with placing the new x and y on his 
{                                                                    // cur_head index, the function also prints on the new (x,y) a '@' (represnts the movment of the snake) and erases the last index that was updated.
	if (cur_head == size - 1)
	{

		snake[cur_head].x = x;
		snake[cur_head].y = y;
		gotoxy(snake[0].x, snake[0].y);
		printf(" ");
		gotoxy(26, 81);

	}
	else
	{
		snake[cur_head].x = x;
		snake[cur_head].y = y;
		gotoxy(snake[cur_head + 1].x, snake[cur_head + 1].y);
		printf(" ");
		gotoxy(26, 81);

	}

	gotoxy(snake[cur_head].x, snake[cur_head].y);
	printf("@");
	gotoxy(26, 81);



}

void initialize_SNAKE(SNAKE snake[], int size, int x, int y)   // the function recives a snake array, his size and current x and y integers and updates the final snake cell with the new x and y.
{

	snake[size - 1].x = x;
	snake[size - 1].y = y;
}

void initialize_new_SNAKE(SNAKE snake[], int size, int x, int y, int cur_head)     // the function recives a snake array, his size and current x and y integers and cur_head and updates the snake with the new x and y
{                                                                                 // the function is active only after the snake eats a '$' and has to add uninitialized cell the the snake array.
	snake[size - 1].x = x;
	snake[size - 1].y = y;
	for (int i = size - 1; i > cur_head; i--)
		swapCells(&snake[i], &snake[i - 1]);

	gotoxy(snake[cur_head].x, snake[cur_head].y);
	printf("@");
	gotoxy(26, 81);

}



void print_menu()              // the function prints the menu of the game
{
	printf("*******************************************************\n");
	printf("* Welcome To Snake!!, to exit press 0\n");
	printf("* $ To play the game press 1 $\n");
	printf("*******************************************************\n");

}

void print_array_full(char board[][COLS])        // the function recives a matrix and prints his value.
{
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
			printf("%c", board[i][j]);
		printf("\n");
	}

}

void initialize_array(char board[][COLS])        // the function recives a matrix and initialize his borders with '#', else initialize with ' '.
{
	for (int i = 0; i < COLS; i++)
	{
		board[0][i] = '#';
		board[ROWS - 1][i] = '#';

	}

	for (int i = 1; i < ROWS - 1; i++)
	{
		board[i][0] = '#';
		for (int j = 1; j < COLS - 1; j++)
			board[i][j] = ' ';
		board[i][COLS - 1] = '#';;
	}





}

int previus_Head(int size, int cur_head)   // the function recives the snake's size and his cur_head and returns his last cur_head
{
	if (cur_head == 0)
		return size - 1;
	else
		return (cur_head - 1);

}


void swapCells(SNAKE* first, SNAKE* second)  // the function recives two memmory addresess of the snakes cells and swaps their x and y value
{
	SNAKE temp = *first;
	*first = *second;
	*second = temp;
}






// This function moves the cursor to the line y and column x.
// (0,0) is the upper left corner of the screen.
void gotoxy(int y, int x) {
	printf("\x1b[%d;%df", x + 1, y + 1);
}



// This function gets keyCodes for special keys such as arrows , escape and others
// It returns the Key code such as 72 for arrow up, 27 for Escape... 
int getKey()
{
	char KeyStroke = _getch();
	if (KeyStroke == 0 || KeyStroke == -32)
		KeyStroke = _getch();
	return (KeyStroke);
}



