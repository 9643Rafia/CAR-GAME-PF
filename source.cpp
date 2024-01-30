#include <iostream>
#include <conio.h>
#include <windows.h>
#include <fstream>
#include <iomanip>

using namespace std;

// Variables
const int ROWS = 21;
const int COLUMNS = 41;

const string SCORE_FILENAME = "highscores.txt";

static int CAR_X = 1, CAR_Y = 8;
static int SCORES = 0, GAIN = 5, LEVEL = 1;
static int FOOD_POINTS = 0, TOTAL_FOOD_POINTS = 60;

static int OPPONENT_CARS[4] = { 15, 1, 10, 10 }; //starting printing axis
static int OPPONENT_CAR_COUNT = 1; //level one= 1 opp car
static bool CAR_ACTIVE_MODE = false; //level 2-4 opp car is active

static int SLEEP_TIMER = 250;

static string GAME_STATUS = "NEW_GAME";
static string PLAYER_NAME = "Player1";

// Declare the matrix
static char GAMEBOARD[ROWS][COLUMNS];

bool readMatrixFromFile()
{
	string filename = "gameboard.txt";
	ifstream inputFile(filename);

	if (!inputFile.is_open())
	{
		cout << "File could not opened" << endl;
		return false;
	}

	char game_widgets;

	//read
	for (int row = 0; row < ROWS; row++)
	{
		for (int col = 0; col < COLUMNS; col++)
		{
			inputFile >> game_widgets;
			if (game_widgets == '#')
			{
				game_widgets = ' ';
			}
			GAMEBOARD[row][col] = game_widgets;
		}
	}

	inputFile.close();

	return true;
}

bool checkCarMove(int x, int y) 
{
	char widget = GAMEBOARD[x][y];

	if (widget == '-' || widget == '|')
	{
		return false;
	}
	else if (widget == '.')
	{
		GAMEBOARD[x][y] = ' ';
		SCORES += GAIN;
		FOOD_POINTS += 2;
	}

	CAR_X = x;
	CAR_Y = y;

	return true;
}

void handleKeyInput(int key)
{
	if (key == 0xE0 || key == 0xE1)
	{
		key = _getch();

		switch (key)
		{
			//up
		case 72:
			cout << "Up arrow key pressed" << endl;
			checkCarMove((CAR_X - 1 + ROWS) % ROWS, CAR_Y);
			break;
			//down
		case 80:
			cout << "Down arrow key pressed" << endl;
			checkCarMove((CAR_X + 1 + ROWS) % ROWS, CAR_Y);
			break;
			//left
		case 75:
			cout << "Left arrow key pressed" << endl;
			checkCarMove(CAR_X, (CAR_Y - 1 + COLUMNS) % COLUMNS);
			break;
			//right
		case 77:
			cout << "Right arrow key pressed" << endl;
			checkCarMove(CAR_X, (CAR_Y + 1 + COLUMNS) % COLUMNS);
			break;
			//error
		default:
			cout << "Invalid key" << endl;
			break;
		}
	}
	else if (key == 32) // Space bar
	{
		cout << "Space bar key pressed." << endl;
	}
	else if (key == 'p')
	{
		GAME_STATUS = "PAUSE";
	}
	else
	{
		cout << "Press correct key again!" << endl;
	}
}

bool printOpponentCar(int i, int j)
{

	for (int k = 0; k < OPPONENT_CAR_COUNT * 2; k += 2)
	{
		int x = OPPONENT_CARS[k];
		int y = OPPONENT_CARS[k + 1];

		if (i == x && y == j)
		{
			return true;
		}
	}
	return false;
}

void printGameBoard()
{

	system("CLS");
	system("Color F0");

	cout << setw(10) << LEVEL << setw(16) << SCORES << endl;

	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLUMNS; j++)
		{
			if (CAR_X == i && j == CAR_Y)
			{
				if (GAME_STATUS == "CAR_CRASHED")
				{
					cout << "@";
				}
				else
				{
					cout << "X";
				}
			}
			else if (printOpponentCar(i, j))
			{
				cout << "O";
			}
			else
			{
				cout << GAMEBOARD[i][j];
			}
		}
		cout << endl;
	}
}

void checkGameValidity()
{

	for (int i = 0; i < OPPONENT_CAR_COUNT * 2; i += 2)
	{
		if (CAR_X == OPPONENT_CARS[i] && CAR_Y == OPPONENT_CARS[i + 1])
		{
			GAME_STATUS = "CAR_CRASHED";
			return;
		}
	}

	if (FOOD_POINTS >= TOTAL_FOOD_POINTS)
	{
		GAME_STATUS = "NEXT_LEVEL";
	}

	if (LEVEL > 4)
	{
		GAME_STATUS = "GAME_OVER";
		LEVEL = 4;
	}
	else if (LEVEL > 2)
	{
		OPPONENT_CAR_COUNT = 2;
	}
	else if (LEVEL > 1)
	{
		CAR_ACTIVE_MODE = true;
		SLEEP_TIMER = 50;
	}

}

bool showHighScore(bool print = true)
{

	ifstream input(SCORE_FILENAME);

	if (input.is_open())
	{
		string name;
		int score;

		input >> name >> score;

		if (print)
		{
			cout << "===================== HIGH SCORE ========================" << endl;
			cout << "\n High Scores" << endl;
			cout << " Name: " << name << endl;
			cout << " Score: " << score << endl << endl;
		}

		input.close();

		if (SCORES > score)
		{
			return true;
		}
	}
	else
	{
		cout << "File could not be read" << endl;
	}

	return false;
}

void showHelp()
{

	cout << "===================== HELP ========================" << endl;
	cout << "1. To move the car, use the arrow keys on your keyboard. " <<
		"\n The left arrow key moves the car left, the right arrow key moves the car right, " <<
		"\n the up arrow key moves the car up, and the down arrow key moves the car down." << endl;

	cout << "2. Your goal is to eat all the food on the current level to advance to the next level." << endl;

	cout << "3. You can increase the speed of the car by pressing the space bar on your keyboard." <<
		"\n This can be helpful for avoiding obstacles or catching up to food boxes." << endl;

	cout << "4. Be careful not to crash into the other car, as this will end the game." << endl;

	cout << "5. You can pause the game at any time by pressing the 'P' key on your keyboard." << endl;

	cout << "\n Enjoy!" << endl;
}

void showMenu()
{

	int min = 49, max = 52;
	cout << "\n\n";
	cout << "================== MENU =======================" << endl;
	cout << "1.  Start a New Game" << endl;
	cout << "2.  See High Scores" << endl;
	cout << "3.  Help" << endl;
	cout << "4.  Exit" << endl;

	if (GAME_STATUS == "PAUSE")
	{
		cout << "5.  Continue Game" << endl;
		max = 53;
	}
	char key = ' ';

	cout << "\n Enter a key, (1-" << max - 48 << "): ";

	do
	{
		key = _getch();

	} while (key < min || key > max);

	cout << key << "\n\n";
	system("CLS");

	switch (key)
	{
	case '1':
		GAME_STATUS = "NEW_GAME";
		break;
	case '2':
		showHighScore();
		showMenu();
		break;
	case '3':
		showHelp();
		showMenu();
		break;
	case '4':
		GAME_STATUS = "EXIT";
		break;
	default:
		GAME_STATUS = "PLAYING";
		break;
	}

}

bool checkOpponentCarMove(int x, int y)
{
	srand((unsigned)time(NULL));
	char widget = GAMEBOARD[x][y];

	int ran = 0 + (rand() % 2);
	bool randomB = !!ran;
	if (widget == '-' || widget == '|' || randomB)
	{
		return false;
	}
	return true;
}


void handleOpponentCar()
{

	for (int i = 0; i < OPPONENT_CAR_COUNT * 2; i += 2)
	{
		int x = OPPONENT_CARS[i];
		int y = OPPONENT_CARS[i + 1];


		//x-axis
		if (checkOpponentCarMove((x - 1 + ROWS) % ROWS, y))
		{
			x = (x - 1 + ROWS) % ROWS;
		}
		else if (checkOpponentCarMove((x + 1) % ROWS, y))
		{
			x = (x + 1 + ROWS) % ROWS;
		}

		//y-axis
		if (CAR_ACTIVE_MODE && checkOpponentCarMove(x, (y - 1 + COLUMNS) % COLUMNS))
		{
			y = (y - 1 + COLUMNS) % COLUMNS;
		}
		else if (CAR_ACTIVE_MODE && checkOpponentCarMove(x, (y + 1) % COLUMNS))
		{
			y = (y + 1 + COLUMNS) % COLUMNS;
		}

		OPPONENT_CARS[i] = x;
		OPPONENT_CARS[i + 1] = y;
	}
}


void showScores()
{

	printGameBoard();
	cout << "- ================= GAME OVER ==================== -" << endl;
	cout << "\t Game Score: " << SCORES << endl;
	cout << "====================================================" << endl;

	bool saveScore = showHighScore(false);

	if (saveScore)
	{

		ofstream output(SCORE_FILENAME,ios::app);
		if (output)
		{
			output << PLAYER_NAME << " " << SCORES;
			output.close();
		}
	}

	GAME_STATUS = "NEW_GAME";
	showMenu();
}

void startPlay()
{

	while (GAME_STATUS == "PLAYING")
	{

		if (_kbhit())
		{
			int key = _getch();
			handleKeyInput(key);
		}
		for (int i = 0; i < OPPONENT_CAR_COUNT; i++)
		{
			handleOpponentCar();
			printGameBoard();
		}

		checkGameValidity();

		if (GAME_STATUS == "NEXT_LEVEL")
		{
			readMatrixFromFile();
			FOOD_POINTS = 0;
			LEVEL += 1;
			SCORES += 100;
			SCORES += GAIN;
			GAME_STATUS = "PLAYING";
		}
		else if (GAME_STATUS == "PAUSE")
		{
			showMenu();
		}
		else if (GAME_STATUS == "CAR_CRASHED" || GAME_STATUS == "GAME_OVER")
		{
			showScores();
		}

		Sleep(SLEEP_TIMER);
	}
}

int main()
{
	showMenu();

	while (GAME_STATUS == "NEW_GAME")
	{

		bool start = readMatrixFromFile();

		if (start)
		{

			CAR_X = 1, CAR_Y = 5;
			SCORES = 0, LEVEL = 1, FOOD_POINTS = 0;
			OPPONENT_CAR_COUNT = 1;
			CAR_ACTIVE_MODE = false;

			GAME_STATUS = "PLAYING";
			startPlay();
		}
		else
		{
			GAME_STATUS = "EXIT";
		}

	}

	cout << GAME_STATUS;
	return 0;
}
