#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <ctime>
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>
#include <math.h>

using namespace std;

#ifdef _WIN32

#elif __linux__

struct termios orig_termios;

/* Returns to original terminal */
void reset_terminal_mode()
{
	tcsetattr(0, TCSANOW, &orig_termios);
}

/* Sets special terminal */
void set_conio_terminal_mode()
{
	struct termios new_termios;

	/* take two copies - one for now, one for later */
	tcgetattr(0, &orig_termios);
	memcpy(&new_termios, &orig_termios, sizeof(new_termios));

	/* register cleanup handler, and set the new terminal mode */
	atexit(reset_terminal_mode);
	cfmakeraw(&new_termios);
	tcsetattr(0, TCSANOW, &new_termios);
}

/* Ckecks if keyboard was hit */
int kbhit()
{
	struct timeval tv = { 0L, 0L };
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(0, &fds);
	return select(1, &fds, NULL, NULL, &tv);
}

/* Gets char from keyboard */
int getch()
{
	int r;
	unsigned char c;
	if ((r = read(0, &c, sizeof(c))) < 0) {
		return r;
	} else {
		return c;
	}
}

#endif

#define ZERO_DELAY 0.05
#define HEIGHT 17
#define WIDTH 70
#define PI 3.1415926

/* Returns time now */
double time_now()
{
	return double (clock())/CLOCKS_PER_SEC;
}

/* Returns number of digits form n */
int digits (int n)
{
	if (n <= 0) return 1;
	int d = 0;
	while (n) {
		d++;
		n /= 10;
	}
	return d;
}

/* Prints scores s1 and s2 */
void printScore (int s1, int s2)
{
	int d1 = digits(s1);
	int d2 = digits(s2);

	cout << s1;
	for (int i = 0; i < WIDTH + 2 -(d1 + d2); i++) cout << ' ';
	cout << s2 << endl;
}

/* Prints title, centralized */
void printTitle ()
{
	for (int i = 0; i <= WIDTH/2 - 3; i++) cout << ' ';
	cout << "P O N G";
	for (int i = 0; i <= WIDTH/2 - 4; i++) cout << ' ';
	cout << endl;
}

/* Prints a line of '_' with size WIDTH */
void printLine ()
{
	for (int i = 0; i <= WIDTH+1; i++ ) cout << '_';
	cout << endl;
}

/* Converts angle x to radians */
double rad (double x)
{
	return x*PI/180;
}

/* Returns an integer in range [min, max] */
int rand_range (int min, int max)
{
	srand(clock());
	return rand()*1.0/(RAND_MAX-1) * (max+1-min) + min;
}

/* Returns true or false, based on a probability of x */
bool prob (double x)
{
	srand(clock());
	return x > (double)rand() / ((double)RAND_MAX + 1);
}

/* Returns signal (+1 or -1) of x */
int signal (double x)
{
	if (x >= 0) return 1;
	else return -1;
}

#include "paddle.cpp"
#include "ball.cpp"

int main ()
{
	int s1 = 0, s2 = 0;

	bool roundOver = false;
	bool gameOver = false;

	double t1 = time_now();
	double t2 = t1;
	double delay = ZERO_DELAY;

	char input = ' ';

	Ball ball(WIDTH/2, HEIGHT/2);
	ball.reset();

	Paddle leftP (HEIGHT/2 - 2, 0);
	Paddle rightP (HEIGHT/2 - 2, WIDTH-1);

	while (!gameOver) {
		/* SETUP TERMINAL */
		set_conio_terminal_mode();
		
		while (!kbhit()  && t1-t2 < delay) t1 = time_now();
		if (t1-t2 < delay) input = getch();
		t2 = t1;
		
		reset_terminal_mode();
		system ("clear");
		/*----------------*/

		/* INPUT */
		switch (input) {
			case 'a':
			case 'A':
				leftP.move(-1);
				break;
			case 'z':
			case 'Z':
				leftP.move(1);
				break;
			case 's':
			case 'S':
				rightP.move(-1);
				break;
			case 'x':
			case 'X':
				rightP.move(1);
				break;
			case 'q':
				gameOver = true;
				break;
		}
		input = ' ';
		/*-------*/

		/* LOGIC AND MOVEMENT */
		if (delay == 1) {
			delay = ZERO_DELAY;
			ball.reset();
		}

		if(ball.checkPass()) {
			delay = 1;
			if (floor(ball.x) == 0)	s2++;
			else 					s1++;
		}
		else {
			ball.checkEdge();
			ball.checkPaddle(&leftP, 'L');
			ball.checkPaddle(&rightP, 'R');
		}
		
		ball.move();
		/*--------------------*/

		/* DRAW */
		printTitle();
		printScore (s1, s2);
		
		printLine();
		for (int j = 0; j < HEIGHT; j++) {
			cout << '|';
			for (int i = 0; i < WIDTH; i++) {
				if (i == floor(ball.x) && j == floor(ball.y)) cout << 'O';
				else if (i == WIDTH/2) cout << '.';
				else if (i == leftP.x && j >= leftP.posTop && j <= leftP.posBottom) cout << '!';
				else if (i == rightP.x && j >= rightP.posTop && j <= rightP.posBottom) cout << '!';
				else cout << ' ';
			}
			cout << '|' << endl;
		}
		printLine();
		/*------*/
		/* cout << "BALL:" << endl;
		ball.printStats();
		cout << "LEFT:" << endl;
		leftP.printStats();
		cout << "RIGHT:" << endl;
		rightP.printStats(); */
	}
	return 0;
}