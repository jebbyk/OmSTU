#include "stdafx.h"
# include <iostream>
# include <windows.h>
# include <time.h>
# include <conio.h>
#include "stdafx.h"
#include  "math.h"
#include <stdio.h>
#include "locale.h"
#include <string>
#include <cstring>
#include "graphics.h"
using namespace std;

HANDLE hStdin;
HANDLE hStdout;
DWORD fdwSaveOldMode;
DWORD cNumRead, fdwMode, i;
INPUT_RECORD irInBuf[128];

VOID ErrorExit(LPSTR);
VOID KeyEventProc(KEY_EVENT_RECORD);
VOID MouseEventProc(MOUSE_EVENT_RECORD);
VOID ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD);

int sleepTime = 1000/30;

bool leftMouseButtonPressed = false;
bool rightMouseButtonPressed = false;

int buttonsList[6][7];

char button1Name[] = "Task 1";
char button2Name[] = "Task 2";
char button3Name[] = "Task 3";
char button4Name[] = "Astro-fighter";
char button5Name[] = "About Author";
char button6Name[] = "Quite";

short selecktedMenu = 0;

int buttonsStartY = 0;
int buttonsStartX = 0;
int buttonsCount = 6;
int intervalBetweenButtons = 1;
int buttonsHeight = 30;
int buttonsWidth = 120;

POINT mousePosition;

HWND hwnd;
HDC hdc;
RECT consoleRect;

COLORREF backColor = RGB(10, 10, 10);
COLORREF buttonsColor = RGB(100, 100, 100);
COLORREF buttonsActiveColor = RGB(150, 150, 150);
COLORREF buttonsSelecktedColor = RGB(255, 255, 255);
COLORREF greenCellColor = RGB(100, 255, 100);
COLORREF redCellColor = RGB(255, 100, 100);

short bestScore = 0;
//creating menu buttons
void StartMenu()
{
	for (int i = 0; i < buttonsCount; i++)
	{
		buttonsList[i][1] = buttonsStartX + (buttonsWidth*i) + (intervalBetweenButtons*i);
		buttonsList[i][2] = buttonsStartY;
		buttonsList[i][3] = buttonsWidth;
		buttonsList[i][4] = buttonsHeight;
		buttonsList[i][5] = 0;
		buttonsList[i][6] = 0;
		buttonsList[i][7] = 0;
	}
}
//updating buttons state
void MenuButonsInput()
{
	for (int i = 0; i < buttonsCount; i++)
	{
		buttonsList[i][5] = 0;
		if (mousePosition.x  > buttonsList[i][1] && mousePosition.x  < buttonsList[i][1] + buttonsList[i][3] && mousePosition.y  > buttonsList[i][2] && mousePosition.y < buttonsList[i][2] + buttonsList[i][4])
		{
			//button lighting
			if (buttonsList[i][6] <= 0)//if not lighted
				buttonsList[i][5] = 1;//make litle lighted
			if (leftMouseButtonPressed)
			{
				
				for (int i = 0; i < 6; i++)
				{
					buttonsList[i][6] = 0;//unlighting all buttons
				}
				buttonsList[i][6] = 1;//lighting only selected
				selecktedMenu = i + 1;//changing menu selsection
			}
		}
	}
}
//getting all discriptors
void SetScreen()
{
	hStdin = GetStdHandle(STD_INPUT_HANDLE);
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdin == INVALID_HANDLE_VALUE)
		ErrorExit("GetStdHandle");
	GetConsoleMode(hStdout, &fdwMode);
	if (!GetConsoleMode(hStdin, &fdwSaveOldMode))
		ErrorExit("GetConsoleMode");
	fdwMode = ENABLE_MOUSE_INPUT;
	if (!SetConsoleMode(hStdin, fdwMode))
		ErrorExit("SetConsoleMode");

	hwnd = GetConsoleWindow();
	SetWindowPos(hwnd, HWND_TOP, -8, -30, 1000, 600, NULL);
	hdc = GetDC(hwnd);
	GetClientRect(hwnd, &consoleRect);
}
//i dont know how it works, Im just have copied it, and i know - it is for mouse input ))))))))))))))))))))))
void InputProc()
{
	// Wait for the events. 
	GetNumberOfConsoleInputEvents(hStdin, &cNumRead);
	if (cNumRead > 0)
	{
		ReadConsoleInput(hStdin, irInBuf, 128, &cNumRead);

		for (i = 0; i < cNumRead; i++)
		{
			switch (irInBuf[i].EventType)
			{
			case KEY_EVENT: // keyboard input 
				KeyEventProc(irInBuf[i].Event.KeyEvent);
				break;

			case MOUSE_EVENT: // mouse input 
				MouseEventProc(irInBuf[i].Event.MouseEvent);
				break;

			case WINDOW_BUFFER_SIZE_EVENT: // scrn buf. resizing 
				ResizeEventProc(irInBuf[i].Event.WindowBufferSizeEvent);
				break;

			case FOCUS_EVENT:  // disregard focus events 

			case MENU_EVENT:   // disregard menu events 
				break;

			default:
				ErrorExit("Unknown event type");
				break;
			}
		}
	}

}

void ClearWindow()
{
	HBRUSH brush = CreateSolidBrush(backColor);
	SelectObject(hdc, brush);
	FillRect(hdc, &consoleRect, brush);
	DeleteObject(brush);
}

void DrawButton(short x, short y, short w, short h, short active, short pressed, char text[])
{
	HBRUSH brush;
	if (active == 0)
	{
		brush = CreateSolidBrush(buttonsColor);
		RECT rect;
		rect.left = x;
		rect.top = y;
		rect.right = x + w;
		rect.bottom = y + h;
		FillRect(hdc, &rect, brush);
		SetBkColor(hdc, buttonsColor);
		TextOutA(hdc, x - strlen(text) * 8 / 2 + w / 2, y - 8 + h / 2, text, strlen(text));
		DeleteObject(brush);
	}
	else
	{
		brush = CreateSolidBrush(buttonsActiveColor);
		RECT rect;
		rect.left = x;
		rect.top = y;
		rect.right = x + w;
		rect.bottom = y + h;
		FillRect(hdc, &rect, brush);
		SetBkColor(hdc, buttonsActiveColor);
		TextOutA(hdc, x - strlen(text) * 8 / 2 + w / 2, y - 8 + h / 2, text, strlen(text));
		DeleteObject(brush);
	}

	if (pressed == 1)
	{
		brush = CreateSolidBrush(buttonsSelecktedColor);
		RECT rect;
		rect.left = x;
		rect.top = y;
		rect.right = x + w;
		rect.bottom = y + h;
		FillRect(hdc, &rect, brush);
		SetBkColor(hdc, buttonsSelecktedColor);
		TextOutA(hdc, x - strlen(text) * 8 / 2 + w / 2, y - 8 + h / 2, text, strlen(text));
		DeleteObject(brush);
	}


}

void DrawButtons()
{
	string s = to_string(bestScore);
	char *cs = &s[0u];
	TextOutA(hdc, 750, 8, "Best score: ", 12);
	TextOutA(hdc, 825, 8, cs, 5);

	for (int i = 0; i < buttonsCount; i++)
	{
		//some pease of..... bydlodkod
		if (i == 0)
			DrawButton(buttonsList[i][1], buttonsList[i][2], buttonsList[i][3], buttonsList[i][4], buttonsList[i][5], buttonsList[i][6], button1Name);
		if (i == 1)
			DrawButton(buttonsList[i][1], buttonsList[i][2], buttonsList[i][3], buttonsList[i][4], buttonsList[i][5], buttonsList[i][6], button2Name);
		if (i == 2)
			DrawButton(buttonsList[i][1], buttonsList[i][2], buttonsList[i][3], buttonsList[i][4], buttonsList[i][5], buttonsList[i][6], button3Name);
		if (i == 3)
			DrawButton(buttonsList[i][1], buttonsList[i][2], buttonsList[i][3], buttonsList[i][4], buttonsList[i][5], buttonsList[i][6], button4Name);
		if (i == 4)
			DrawButton(buttonsList[i][1], buttonsList[i][2], buttonsList[i][3], buttonsList[i][4], buttonsList[i][5], buttonsList[i][6], button5Name);
		if (i == 5)
			DrawButton(buttonsList[i][1], buttonsList[i][2], buttonsList[i][3], buttonsList[i][4], buttonsList[i][5], buttonsList[i][6], button6Name);

		
	}
}

float t1y[16]; float t1z[16]; float t1x[16];
short t1columnW = 128;
short t1rowH = 20;
short t1tabTop = 128;
short t1tabLeft = 32;

float t1a = 2; float t1b = 4; float t1n = 16;
float t1step = (t1b - t1a) / 15; float t1X = t1a;
short t1minZi = 0; short t1minYi = 0; short t1maxZi = 0; short t1maxYi = 0;
float t1minZ = 10000.0; float t1minY = 100000.0; float t1maxZ = -1000000.0; float t1maxY = -1000000.0;
int drawPointsCount = 0;

void CalculateTask1()
{
	for (int i = 0; i < 16; i++)
	{
		t1x[i] = t1X;
		t1y[i] = t1X * cos(t1X / 2.0);
		if (t1y[i] < t1minY)
		{
			t1minYi = i;
			t1minY = t1y[i];
		}
		if (t1y[i] > t1maxY)
		{
			t1maxYi = i;
			t1maxY = t1y[i];
		}
		t1z[i] = pow(t1X, 1 / 3.0) + (sqrt(2.0) * exp(-t1X));
		if (t1z[i] < t1minZ)
		{
			t1minZi = i;
			t1minZ = t1z[i];
		}
		if (t1z[i] > t1maxZ)
		{
			t1maxZi = i;
			t1maxZ = t1z[i];
		}
		t1X += t1step;
	}

}

void DrawTask1()
{

	HBRUSH grayBrush = CreateSolidBrush(buttonsColor);
	HBRUSH greenBrush = CreateSolidBrush(greenCellColor);
	HBRUSH redBrush = CreateSolidBrush(redCellColor);
	HBRUSH darkGrayBrush = CreateSolidBrush(RGB(50, 50, 50));
	RECT r;



	string step = to_string(t1step);
	char *cstep = &step[0u];
	SetBkColor(hdc, buttonsActiveColor);
	TextOutA(hdc, 32, 42, "function step = ", 16);
	TextOutA(hdc, 128, 42, cstep, 5);

	r.top = t1tabTop - t1rowH;
	r.left = t1tabLeft;
	r.right = t1tabLeft + t1columnW * 3;
	r.bottom = r.top + t1rowH;

	SetBkColor(hdc, buttonsSelecktedColor);
	TextOutA(hdc, r.left, r.top, "                X                                Y                                Z                .", 92);
	
	for (int i = 0; i < 16; i++)
	{
		string s = to_string(t1x[i]);
		char *cs = &s[0u];

		r.top = t1tabTop + i * t1rowH;
		r.left = t1tabLeft;
		r.right = t1tabLeft + t1columnW;
		r.bottom = t1tabTop + t1rowH + t1rowH * i;


		FillRect(hdc, &r, grayBrush);
		SetBkColor(hdc, buttonsActiveColor);
		TextOutA(hdc, r.left, r.top, cs, 5);

		s = to_string(t1y[i]);
		cs = &s[0u];
		r.top = t1tabTop + i * t1rowH;
		r.left = t1tabLeft + t1columnW;
		r.right = t1tabLeft + t1columnW * 2;
		r.bottom = t1tabTop + t1rowH + t1rowH * i;

		if (i != t1minYi && i != t1maxYi)
		{
			SetBkColor(hdc, buttonsActiveColor);
			FillRect(hdc, &r, grayBrush);
			TextOutA(hdc, r.left, r.top, cs, 5);

		}
		else {
			if (i == t1minYi)
			{
				SetBkColor(hdc, greenCellColor);
				FillRect(hdc, &r, greenBrush);
				TextOutA(hdc, r.left, r.top, cs, 5);
			}
			if (i == t1maxYi)
			{
				SetBkColor(hdc, redCellColor);
				FillRect(hdc, &r, redBrush);
				TextOutA(hdc, r.left, r.top, cs, 5);
			}
		}

		s = to_string(t1z[i]);
		cs = &s[0u];
		r.top = t1tabTop + i * t1rowH;
		r.left = t1tabLeft + t1columnW * 2;
		r.right = t1tabLeft + t1columnW * 3;
		r.bottom = t1tabTop + t1rowH + t1rowH * i;

		if (i != t1minZi && i != t1maxZi)
		{
			SetBkColor(hdc, buttonsActiveColor);
			FillRect(hdc, &r, grayBrush);
			TextOutA(hdc, r.left, r.top, cs, 5);
		}
		else {
			if (i == t1minZi)
			{
				SetBkColor(hdc, greenCellColor);
				FillRect(hdc, &r, greenBrush);
				TextOutA(hdc, r.left, r.top, cs, 5);
			}
			if (i == t1maxZi)
			{
				SetBkColor(hdc, redCellColor);
				FillRect(hdc, &r, redBrush);
				TextOutA(hdc, r.left, r.top, cs, 5);
			}

		}



	}
	int t1graphStartX = 512;
	int t1graphStartY = 280;
	float t1graphScaleX = 64;

	r.top = t1graphStartY - 164;
	r.bottom = t1graphStartY + 164;
	r.left = t1graphStartX - 72;
	r.right = t1graphStartX + 340;

	FillRect(hdc, &r, darkGrayBrush); 

	HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	SelectObject(hdc, pen);



	MoveToEx(hdc, t1graphStartX + t1x[0] * t1graphScaleX, t1graphStartY - t1y[0] * t1graphScaleX, NULL);

	for (int i = 1; i < drawPointsCount; i++)
	{
		LineTo(hdc, t1graphStartX + t1x[i] * t1graphScaleX, t1graphStartY - t1y[i] * t1graphScaleX);
	}


	pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	SelectObject(hdc, pen);
	MoveToEx(hdc, t1graphStartX + t1x[0] * t1graphScaleX, t1graphStartY - t1z[0] * t1graphScaleX, NULL);
	for (int i = 1; i < drawPointsCount; i++)
	{
		LineTo(hdc, t1graphStartX + t1x[i] * t1graphScaleX, t1graphStartY - t1z[i] * t1graphScaleX);
	}

	pen = CreatePen(PS_SOLID, 2, RGB(150, 150, 150));
	SelectObject(hdc, pen);
	MoveToEx(hdc, t1graphStartX - 64, t1graphStartY, NULL);
	LineTo(hdc, t1graphStartX + t1x[15] * t1graphScaleX + 64, t1graphStartY);

	SetBkColor(hdc, buttonsColor);
	TextOutA(hdc, t1graphStartX + t1x[15] * t1graphScaleX + 64, t1graphStartY + 8, "X", 1);

	MoveToEx(hdc, t1graphStartX, t1graphStartY + 128, NULL);
	LineTo(hdc, t1graphStartX, t1graphStartY - 128);
	TextOutA(hdc, t1graphStartX + 8, t1graphStartY - 128 - 32, "Y", 1);

	for (int i = 1; i < t1x[15]; i++)
	{
		MoveToEx(hdc, t1graphStartX + i * t1graphScaleX, t1graphStartY + 4, NULL);
		LineTo(hdc, t1graphStartX + i * t1graphScaleX, t1graphStartY - 4);
		string s = to_string(i);
		char *cs = &s[0u];
		TextOutA(hdc, t1graphStartX + i * t1graphScaleX, t1graphStartY - 24, cs, 2);

	}

	for (int i = 1; i < t1maxY || i < t1maxZ; i++)
	{
		string s = to_string(i);
		char *cs = &s[0u];
		MoveToEx(hdc, t1graphStartX - 4, t1graphStartY - i * t1graphScaleX, NULL);
		LineTo(hdc, t1graphStartX + 4, t1graphStartY - i * t1graphScaleX);
		TextOutA(hdc, t1graphStartX + 8, t1graphStartY - i * t1graphScaleX, cs, 2);

		s = to_string(-i);
		cs = &s[0u];
		MoveToEx(hdc, t1graphStartX - 4, t1graphStartY + i * t1graphScaleX, NULL);
		LineTo(hdc, t1graphStartX + 4, t1graphStartY + i * t1graphScaleX);
		TextOutA(hdc, t1graphStartX + 8, t1graphStartY + i * t1graphScaleX, cs, 2);
	}

	pen = CreatePen(PS_SOLID, 1, RGB(150, 50, 50));
	SelectObject(hdc, pen);
	MoveToEx(hdc, t1graphStartX + 2 * t1graphScaleX, t1graphStartY + 128, NULL);
	LineTo(hdc, t1graphStartX + 2 * t1graphScaleX, t1graphStartY - 128);
	MoveToEx(hdc, t1graphStartX + 4 * t1graphScaleX, t1graphStartY + 128, NULL);
	LineTo(hdc, t1graphStartX + 4 * t1graphScaleX, t1graphStartY - 128);

	DeleteObject(pen);

}

double calcTask2(double x)
{
	return pow(2.0, pow(pow(x, 2.0) + 1, 1 / 3.0)) - (3 * sin(x)) - 5;
}

void DrawTask2()
{
	
	double a = -10, b = 10;

	string s = to_string(a);
	char *cs = &s[0u];

	TextOutA(hdc, 32, 64,"Решение уравнения 2^(куб.кор. из(x^2 + 1)) - 3sin(x) = 5 на интервале [1,10] методом бисекции",56);
	int i = 0;
	while (b - a > 0.001)
	{
		double c = a + (b - a) / 2.0;
		if (calcTask2(b) * calcTask2(c) > 0)
			b = c;
		else a = c;
	}
	s = to_string(a);
	cs = &s[0u];
	TextOutA(hdc, 32, 96, cs, 4);
	TextOutA(hdc, 64, 96, " - решение данного уравнения", 32);



}

double calculateTask3(double x)
{
	return cos(x) / (sin(x) + cos(x));
}

int t3i = 0;
void DrawTask3()
{

	
	double a, b;
	int n = 100;
	double summ = 0.0;
	TextOutA(hdc, 32, 64 + t3i*t1rowH*4,"Введите интервал [a, b]: ", 24);
	TextOutA(hdc, 0, 0, " ", 1);
	cin >> a;
	string s = to_string(a);
	char *cs = &s[0u];
	TextOutA(hdc, 32, 96 + t3i * t1rowH * 4, cs, 4);
	TextOutA(hdc, 0, 0, " ", 1);
	cin >> b;
	s = to_string(b);
	cs = &s[0u];
	TextOutA(hdc, 64, 96 + t3i * t1rowH * 4, cs, 4);
	TextOutA(hdc, 96, 96 + t3i * t1rowH * 4, "нажмите enter чтобы получить ответ", 34);
	TextOutA(hdc, 0, 0, " ", 1);
	_getch();
	if (a > b) 
	{
		double invert = a;
		a = b; b = invert;
	}
	double dx = double((b - a) / n);
	if (a != b) 
	{
		for (double x = a + dx; x < b + 0.00001; x += dx)
		{
			summ += calculateTask3(x - dx / 2);
		}
		summ *= dx;
		s = to_string(summ);
		cs = &s[0u];
		TextOutA(hdc, 32, 128 + t3i * t1rowH*4, "Ответ: ", 8);

		TextOutA(hdc, 32+64, 128 + t3i * t1rowH*4, cs, 6);
	}
	else TextOutA(hdc, 32, 128 + t3i * t1rowH*4, "0", 1);
	t3i++;
	TextOutA(hdc, 0, 0, " ", 1);
	_getch();
	selecktedMenu = 0;
}

int AAx = 1000;
int AAy = 128;
int AAspeed = -3;
void DrawAboutAuthor()
{
	SetBkColor(hdc, RGB(100, 100, 100));
	TextOutA(hdc, AAx, AAy, "Данную расчетно графическую работу вывполнил студент первого курса группы ИВТ-172 Омского Государственного Технического Университета Зайцев Владимир Александрович", 256);
	AAx += AAspeed;
}

///////////////////////////////////////////////////THERE IS THE GAME BEGINS! (using DirectX 13, check your videocard for compability)//////////////////////////////////////////////////////////////////

void DrawPlayerShip(short x, short y, COLORREF color )
{
	HPEN pen = CreatePen(PS_SOLID, 1, color);
	SelectObject(hdc, pen);

	MoveToEx(hdc, x-4, y-32, NULL);
	LineTo(hdc, x + 4, y - 32);//nose of the ship
	LineTo(hdc, x + 5, y );// right side of nose
	LineTo(hdc, x + 24, y + 16); // right wing front
	LineTo(hdc, x + 22, y + 32); // right wing side
	LineTo(hdc, x + 6, y + 30);//right wing back
	LineTo(hdc, x + 5, y + 32);// ship back right
	LineTo(hdc, x - 5, y + 32); // ship back
	LineTo(hdc, x - 5, y + 30);//ship left back
	LineTo(hdc, x - 22, y + 32); //left wing back
	LineTo(hdc, x - 24, y + 16); //left wing sinde
	LineTo(hdc, x - 5, y); // left wing side foront
	LineTo(hdc, x - 4, y - 32); //left nose side
	MoveToEx(hdc, x - 16, y+16, NULL);//draw weapons
	LineTo(hdc, x - 16, y-12);
	MoveToEx(hdc, x + 16, y+16, NULL);
	LineTo(hdc, x + 16, y-12);
	MoveToEx(hdc, x + 3, y - 6, NULL);
	LineTo(hdc, x + 4, y + 6);//draw cockpit
	LineTo(hdc, x - 4, y  + 6);
	LineTo(hdc, x - 3, y - 6);
	LineTo(hdc, x + 3, y - 6);
	DeleteObject(pen);

}

short afStars[128][5];//0 - X, 1 - Y, 2 - previosX, 3 - prevY, 4 - speed
short afStarI = 0;
void StarProc()
{
	short x = rand() %  1000; 
	short y = 0;
	//creating only one new star particle every frame
	afStars[afStarI][0] = x; afStars[afStarI][1] = y; afStars[afStarI][2] = x; afStars[afStarI][3] = y; afStars[afStarI][4] = rand() % 4 +4;
	if (afStarI < 127)
	{
		afStarI++;
	}
	else
	{
		afStarI = 0;
	}

	HPEN pen = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
	SelectObject(hdc, pen);

	for (short i = 0; i < 127; i++)
	{
		//draw all star particles if it on the screen
		if (afStars[i][1] < 600)
		{
			MoveToEx(hdc, afStars[i][2], afStars[i][3], NULL);
			afStars[i][1] += afStars[i][4];
			LineTo(hdc, afStars[i][0], afStars[i][1]);
			afStars[i][3] = afStars[i][1];
		}
	}

	DeleteObject(pen);
}

short particles[256][8];//0 - x, 1 - y, 2 - dx, 3 - dy, 4 - life, 5 - prevX, 6 - prevY, 7 - width
short particleI = 0;
//creates new particle 
void CreateParticle(short x, short y, short sx, short sy, short l, short w)
{
		particles[particleI][0] = x; particles[particleI][1] = y; particles[particleI][2] = sx; particles[particleI][3] = sy; particles[particleI][4] = l;
		particles[particleI][5] = x; particles[particleI][6] = y; particles[particleI][7] = w;
		if (particleI < 255)
		{
			particleI++;
		}
		else
		{
			particleI = 0;
		}
	
}
//updates and draws all particles
void ParticlesProc()
{
	HPEN pen = CreatePen(PS_SOLID, 1, RGB(255, 255, 50));
	SelectObject(hdc, pen);

	for (int i = 0; i < 255; i++)
	{
		if (particles[i][7] == 0)
		{
			particles[i][4] -= 1;
			if (particles[i][4] < 0)//if died, clear all data of curent particle
			{
				particles[i][0] = 0;
				particles[i][1] = 0;
				particles[i][2] = 0;
				particles[i][3] = 0;
				particles[i][4] = 99999999;
			}
			else
			{
				//moving particle
				particles[i][0] += particles[i][2];
				particles[i][1] += particles[i][3];
				//drawing
				MoveToEx(hdc, particles[i][0], particles[i][1], NULL);//line from curent coord 
				LineTo(hdc, particles[i][5], particles[i][6]);// to previous cord
				//updating previous positions
				particles[i][5] = particles[i][0];
				particles[i][6] = particles[i][1];
			}
		}
	}

	DeleteObject(pen);

	pen = CreatePen(PS_SOLID, 3, RGB(255, 255, 50));
	SelectObject(hdc, pen);

	for (int i = 0; i < 255; i++)
	{
		if (particles[i][7] == 1)
		{
			particles[i][4] -= 1;
			if (particles[i][4] < 0)
			{
				particles[i][0] = 0;
				particles[i][1] = 0;
				particles[i][2] = 0;
				particles[i][3] = 0;
				particles[i][4] = 99999999;
			}
			else
			{

				particles[i][0] += particles[i][2];
				particles[i][1] += particles[i][3];

				MoveToEx(hdc, particles[i][0], particles[i][1], NULL);
				LineTo(hdc, particles[i][5], particles[i][6]);
				particles[i][5] = particles[i][0];
				particles[i][6] = particles[i][1];
			}
		}
	}
	DeleteObject(pen);
}

short afAsteroids[16][5];//0 - x, 1 - y, 2 - size, 3 - life, 4 - speed;
short afAsteroidI = 0;
void CreateAsteroid()
{
	afAsteroids[afAsteroidI][0] = rand() % 500 +250;//x
	afAsteroids[afAsteroidI][1] = 0;//y
	afAsteroids[afAsteroidI][2] = rand() % 16 +16;//size
	afAsteroids[afAsteroidI][3] = 4;//life
	afAsteroids[afAsteroidI][4] += 1;//speed
	
	if (afAsteroidI < 15)
	{
		afAsteroidI++;
	}
	else
	{
		afAsteroidI = 0;
	}
	
}

short afPlayerPoints = 0;
short afPlayerHealth = 100;
void AsteroidsProc()
{
	HPEN pen = CreatePen(PS_SOLID, 2, RGB(200, 200, 200));
	SelectObject(hdc, pen);

	for (int i = 0; i < 15; i++)
	{
		if (afAsteroids[i][3] < 0)
		{
			afPlayerPoints += afAsteroids[i][4]*10;
			//creating particles in random positions
			for (int m = 0; m < 32; m++)
			{
				CreateParticle(afAsteroids[i][0], afAsteroids[i][1], -8 + rand() % 16, -9 + rand() % 18, rand() % 64,1);
			}
			for (int m = 0; m < 32; m++)
			{
				CreateParticle(afAsteroids[i][0] + afAsteroids[i][2], afAsteroids[i][1] + afAsteroids[i][2], -9 + rand() % 18, -9 + rand() % 18, rand() % 64,1);
			}
			for (int m = 0; m < 32; m++)
			{
				CreateParticle(afAsteroids[i][0] - afAsteroids[i][2], afAsteroids[i][1] - afAsteroids[i][2], -7 + rand() % 18, -10 + rand() % 16, rand() % 64,1);
			}
			for (int m = 0; m < 32; m++)
			{
				CreateParticle(afAsteroids[i][0] - afAsteroids[i][2], afAsteroids[i][1] + afAsteroids[i][2], -4 + rand() % 18, -8 + rand() % 16, rand() % 64,1);
			}
			for (int m = 0; m < 32; m++)
			{
				CreateParticle(afAsteroids[i][0] + afAsteroids[i][2], afAsteroids[i][1] - afAsteroids[i][2], -8 + rand() % 14, -10 + rand() % 20, rand() % 64,1);
			}
			afAsteroids[i][0] = rand() % 1000;
			afAsteroids[i][1] = 0;
			afAsteroids[i][3] = 4;
			afAsteroids[i][4] += 1;
		}
		afAsteroids[i][1] += afAsteroids[i][4];
		if (afAsteroids[i][1] < 600)//life
		{
			//drawing asteroid rect
			MoveToEx(hdc, afAsteroids[i][0] - afAsteroids[i][2], afAsteroids[i][1] - afAsteroids[i][2], NULL);
			LineTo(hdc, afAsteroids[i][0] + afAsteroids[i][2], afAsteroids[i][1] - afAsteroids[i][2]);
			LineTo(hdc, afAsteroids[i][0] + afAsteroids[i][2], afAsteroids[i][1] + afAsteroids[i][2]);
			LineTo(hdc, afAsteroids[i][0] - afAsteroids[i][2], afAsteroids[i][1] + afAsteroids[i][2]);
			LineTo(hdc, afAsteroids[i][0] - afAsteroids[i][2], afAsteroids[i][1] - afAsteroids[i][2]);
		}
		else {
			afAsteroids[i][0] = rand() % 1000;
			afAsteroids[i][1] = 0;
			afAsteroids[i][3] = 4;
		}
	}
	DeleteObject(pen);
}

short afBullets[32][6];//0 -x, 1 y y, 2 - prevX, 3 - prevY, 4 - dx, 5 - dy
short afBulletI = 0;
short afBulletSpeed = 32;
void CreateBullet(short x, short y)
{
	afBullets[afBulletI][0] = x; afBullets[afBulletI][1] = y; afBullets[afBulletI][2] = x; afBullets[afBulletI][3] = y;
	afBullets[afBulletI][4] = rand() % afBulletSpeed/10 -afBulletSpeed/20;
	afBullets[afBulletI][5] = afBulletSpeed + rand() % (afBulletSpeed/10);
	if (afBulletI < 31)
	{
		afBulletI++;
	}
	else
	{
		afBulletI = 0;
	}
}

void BulletsProc()
{
	HPEN pen = CreatePen(PS_SOLID, 2, RGB(255, 50, 50));
	SelectObject(hdc, pen);

	for (short i = 0; i < 31; i++)
	{
	
		if (afBullets[i][1] > 0)
		{
			MoveToEx(hdc, afBullets[i][2], afBullets[i][3], NULL);//begin drawing
			afBullets[i][0] -= afBullets[i][4];// mooving
			afBullets[i][1] -= afBullets[i][5];
			LineTo(hdc, afBullets[i][0], afBullets[i][1]);//end draving
			afBullets[i][3] = afBullets[i][1];// setting previous cords
			afBullets[i][2] = afBullets[i][0];

			for (int n = 0; n < 15; n++)
			{
				//checking collision
				if (afBullets[i][0] < afAsteroids[n][0] + afAsteroids[n][2] && afBullets[i][0] > afAsteroids[n][0] - afAsteroids[n][2] && afBullets[i][1] < afAsteroids[n][1] + afAsteroids[n][2] && afBullets[i][1] > afAsteroids[n][1] - afAsteroids[n][2])
				{
					//hit effect
					for (int m = 0; m < 32; m++)
					{
						CreateParticle(afBullets[i][0], afBullets[i][1], -4 + rand() % 8, -4 + rand() % 8, rand() % 16,0);
					}
					//clearing bullet data
					afBullets[i][0] = 0; afBullets[i][1] = 0; afBullets[i][2] = 0; afBullets[i][3] = 0;
					afAsteroids[n][3] -= 1;
				}
			}
		}
		
	}
	DeleteObject(pen);
}

short afPlayerShipX = 0;
short afPlayerShipY = 512;
short afPlayerShipSpeedDx = 8;
short afPlayerShipCurentSpeedX = 0;
short afPlayerShipMaxSpeed = 16;
short afWeaponsFreq = 1;
short afWeapons1Tick;
short afWeapons2Tick;
short afAsteroidsTick;
short afAsteroidsFreq = 128;
void AsteroidFighter()
{
	string s = to_string(afPlayerPoints);
	char *cs = &s[0u];
	TextOutA(hdc, 750, 8,"Score: ", 8);
	TextOutA(hdc, 800, 8, cs, 5);

	s = to_string(afPlayerHealth);
	cs = &s[0u];
	TextOutA(hdc, 750, 24, "Health: ", 9);
	TextOutA(hdc, 800, 24, cs, 5);

	afWeapons1Tick++;
	afWeapons2Tick++;
	afAsteroidsTick++;
	afPlayerShipCurentSpeedX = (mousePosition.x - afPlayerShipX)/ afPlayerShipSpeedDx;//speed control
	//speed clamping
	if (afPlayerShipCurentSpeedX > afPlayerShipMaxSpeed)
	{
		afPlayerShipCurentSpeedX = afPlayerShipMaxSpeed;
	}
	if (afPlayerShipCurentSpeedX < -afPlayerShipMaxSpeed)
	{
		afPlayerShipCurentSpeedX = -afPlayerShipMaxSpeed;
	}
	afPlayerShipX += afPlayerShipCurentSpeedX;//update player position
	DrawPlayerShip(afPlayerShipX, afPlayerShipY, RGB(50,255,50));


	//shooting
	if (afWeapons1Tick > afWeaponsFreq)
	{
		CreateBullet(afPlayerShipX - 16, afPlayerShipY);
		afWeapons1Tick = 0 - rand()% 4;
		for (int i = 0; i < 8; i++)
		{
			CreateParticle(afPlayerShipX - 16, afPlayerShipY, -2 + rand() % 4, -rand() % 16, rand() % 8,0);
		}
	}



	//shooting (second weapon)
	if (afWeapons2Tick > afWeaponsFreq)
	{
		CreateBullet(afPlayerShipX + 16, afPlayerShipY);
		afWeapons2Tick = 0 - rand() % 4;

		for (int i = 0; i < 8; i++)
		{
			CreateParticle(afPlayerShipX + 16, afPlayerShipY, -2 + rand() % 4, -rand() % 16, rand() % 8,0);
		}
	}


	//asteroid spawning
	if (afAsteroidsTick > afAsteroidsFreq)
	{
		afAsteroidsTick = 0;
		CreateAsteroid();
	}
	//player collisions with asteroids
	for (int c = 0; c < 15; c++)
	{
		if (afPlayerShipX < afAsteroids[c][0] + afAsteroids[c][2] && afPlayerShipX > afAsteroids[c][0] - afAsteroids[c][2] && afPlayerShipY - 32 < afAsteroids[c][1] + afAsteroids[c][2] && afPlayerShipY-32 > afAsteroids[c][1] - afAsteroids[c][2])
		{
			
			afPlayerHealth -= afAsteroids[c][4];
			afAsteroids[c][3] = -1;

		}

	}
	//checking health
	if (afPlayerHealth < 0)
	{
		if (afPlayerPoints > bestScore)
		{
			bestScore = afPlayerPoints;
		}
		afPlayerPoints = 0;
		afPlayerHealth = 100;
		for (int i = 0; i < 15; i++)
		{
			for (int n = 0; n < 5; n++)
			{
				afAsteroids[i][n] = 0;
			}
		}
		selecktedMenu = 5;
	}
	//updating all entities
	BulletsProc();
	StarProc();
	AsteroidsProc();
	ParticlesProc();
}

void Draw()
{
	DrawButtons();


	switch (selecktedMenu)
	{
	case 0:
		drawPointsCount = 0;
		AAx = 1000;
		break;
	case 1:
		AAx = 1000;
		DrawTask1();
		if (drawPointsCount < 16)
		{
			drawPointsCount++;
		}
		break;
	case 2:
		AAx = 1000;
		DrawTask2();
		drawPointsCount = 0;
		break;
	case 3:
		AAx = 1000;
		DrawTask3();
		drawPointsCount = 0;
		break;
	case 4:
		AAx = 1000;
		drawPointsCount = 0;
		AsteroidFighter();
		break;
	case 5:
		DrawAboutAuthor();
		drawPointsCount = 0;
		break;
	}

	HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
	RECT z;
	z.left = 0;
	z.top = 0;
	z.right = 1;
	z.bottom = 1;
	FillRect(hdc, &z, brush);
	DeleteObject(brush);
}

void main()
{
	SetScreen();
	StartMenu();
	CalculateTask1();

	while (true)
	{

		GetNumberOfConsoleInputEvents(hStdin, &cNumRead);
		
			ClearWindow();

		GetCursorPos(&mousePosition);
		InputProc();
		MenuButonsInput();
		Draw();
		Sleep(sleepTime);

		if (selecktedMenu == 6)
		{
			exit(0);
		}
		rightMouseButtonPressed = false;
		leftMouseButtonPressed = false;
	}

	// Restore input mode on exit.
	DeleteDC(hdc);
	SetConsoleMode(hStdin, fdwSaveOldMode);

}
// I dont know how it works too, but its for mouse input too
VOID ErrorExit(LPSTR lpszMessage)
{

	SetConsoleMode(hStdin, fdwSaveOldMode);

	ExitProcess(0);
}

VOID KeyEventProc(KEY_EVENT_RECORD ker)
{

}

VOID MouseEventProc(MOUSE_EVENT_RECORD mer)
{

	switch (mer.dwEventFlags)
	{
	case 0:

		if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			leftMouseButtonPressed = true;
		}
		else if (mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
		{
			rightMouseButtonPressed = true;
		}
		break;

	case MOUSE_MOVED:

		break;
	}
}

VOID ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD wbsr)
{

}