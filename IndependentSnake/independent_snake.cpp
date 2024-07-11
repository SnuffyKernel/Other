#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <windows.h>

using namespace std;
using namespace chrono;

uint8_t random() {
    static uint8_t seed = system_clock::now().time_since_epoch().count();
    static ranlux24_base gen(seed);
    uniform_int_distribution<uint16_t> dis(1, 18);
    return dis(gen);
}

vector<string> field_generation() {
    vector<string> field(20, string(20, ' '));
    for (string& line : field) {
        for (char& columns : line) {
            columns = ' ';
        }
    }
    return field;
}

void print(vector<string>& field, uint16_t score) {
    COORD coord;
    coord.X = 0;
    coord.Y = 0;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

    bool check = true;
    for (string& line : field) {
        for (char& columns : line) {
            cout << columns;
        }
        if (check) cout << "   score: " << score;
        check = false;
        cout << endl;
        check = false;
    }
    Sleep(100);
}

void snake_generation(vector<string>& field) {
    field[random()][random()] = '0';
}

void apple_generation(vector<string>& field) {
    uint8_t x = random();
    uint8_t y = random();
    if (field[y][x] == '0' || field[y][x] == 'o') apple_generation(field);
    else field[y][x] = 'x';
}

vector<uint8_t> search(vector<string>& field, char symbol) {
    for (uint8_t line = 0; line < field.size(); line++) {
        for (uint8_t columns = 0; columns < field.size(); columns++ ) {
            if (field[line][columns] == symbol) return { line, columns };
        }
    }
}

void GameOver() {
    cout << "Game Over" << endl;
    exit(0); 
}

void tailMovement(vector<vector<uint8_t>>& snakeLong) {
    if (snakeLong.size() > 1) 
    {
        for (uint8_t i = snakeLong.size() - 1; i != 0; i--) 
        {
            //if (i != snakeLong.size() - 1 && snakeLong[i] == snakeLong[i - 1]) {
            //    GameOver();
            //}
            snakeLong[i] = snakeLong[i - 1];
        }
    }
}

void move(vector<string>& field, vector<vector<uint8_t>>& snakeLong, bool YmoveUp, bool XmoveUp, bool YmoveDown, bool XmoveDown ) {
    field[snakeLong[snakeLong.size() - 1][0]][snakeLong[snakeLong.size() - 1][1]] = ' ';
    tailMovement(snakeLong);

    if (YmoveUp) 
    {
        if (field[snakeLong[0][0] + 1][snakeLong[0][1]] != 'o' && snakeLong[0][0] + 1 != field.size()) snakeLong[0][0]++;
        else XmoveUp = true;
    }
    if (XmoveUp) 
    {
        if (field[snakeLong[0][0]][snakeLong[0][1] + 1] != 'o' && snakeLong[0][1] + 1 != field.size()) snakeLong[0][1]++;
        else YmoveDown = true;
    }
    if (YmoveDown) 
    {
        if (field[snakeLong[0][0] - 1][snakeLong[0][1]] != 'o' && snakeLong[0][0] - 1 != 0 ) snakeLong[0][0]--;
        else XmoveDown = true;
    }

    if (YmoveUp && XmoveUp && YmoveDown && XmoveDown) GameOver();

    if (XmoveDown) 
    {
        if (field[snakeLong[0][0]][snakeLong[0][1] - 1] != 'o' && snakeLong[0][1] - 1 != 0 ) snakeLong[0][1]--;
        else move(field, snakeLong, 0, 1, 0, 0);
    }
    field[snakeLong[0][0]][snakeLong[0][1]] = '0';
    for (uint8_t i = 1; i < snakeLong.size(); i++) 
    {
        field[snakeLong[i][0]][snakeLong[i][1]] = 'o';
    }
    print(field, snakeLong.size() - 1);
}

void logic(vector<string>& field, vector<vector<uint8_t>>& snakeLong) {
	vector<uint8_t> appleCoordinates = search(field, 'x');
	snakeLong[0] = search(field, '0');
	if (appleCoordinates.empty())
	{
		snakeLong.push_back(snakeLong[snakeLong.size() - 1]);
		apple_generation(field);
		appleCoordinates = search(field, 'x');
	}
    if (appleCoordinates[0] >= snakeLong[0][0] && appleCoordinates[1] >= snakeLong[0][1])
    {
        if (appleCoordinates[0] != snakeLong[0][0]) move(field, snakeLong, 1, 0, 0, 0);
        if (appleCoordinates[1] != snakeLong[0][1]) move(field, snakeLong, 0, 1, 0, 0);
    }
	if (appleCoordinates[0] >= snakeLong[0][0] && appleCoordinates[1] <= snakeLong[0][1])
	{
		if (appleCoordinates[0] != snakeLong[0][0]) move(field, snakeLong, 1, 0, 0, 0);
		if (appleCoordinates[1] != snakeLong[0][1]) move(field, snakeLong, 0, 0, 0, 1);
	}
	if (appleCoordinates[0] <= snakeLong[0][0] && appleCoordinates[1] >= snakeLong[0][1])
	{
		if (appleCoordinates[0] != snakeLong[0][0]) move(field, snakeLong, 0, 0, 1, 0);
		if (appleCoordinates[1] != snakeLong[0][1]) move(field, snakeLong, 0, 1, 0, 0);
	}
	if (appleCoordinates[0] <= snakeLong[0][0] && appleCoordinates[1] <= snakeLong[0][1])
	{
		if (appleCoordinates[0] != snakeLong[0][0]) move(field, snakeLong, 0, 0, 1, 0);
		if (appleCoordinates[1] != snakeLong[0][1]) move(field, snakeLong, 0, 0, 0, 1);
	}

}

int main()
{
    cout << "\033[?25l";
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    COORD bufferSize = { csbi.srWindow.Right + 1, csbi.srWindow.Bottom + 1 };
    SetConsoleScreenBufferSize(hConsole, bufferSize);

    vector<string> field = field_generation();
    vector<vector<uint8_t>> snakeLong(1);
    snake_generation(field);
    apple_generation(field);
    print(field, snakeLong.size() - 1);

    while (true)
    {
        logic(field, snakeLong);
    }

}