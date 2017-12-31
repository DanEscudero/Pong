class Paddle
{
public:
	int len = 4;
	int posTop;
	int posBottom;
	int x;

	Paddle (int posTop, int x)
	{
		this->posTop = posTop;
		posBottom = posTop + len;
		this->x = x;
	}

	~Paddle ()
	{

	}

	/* Moves Paddle up (dir = -1) or down (dir = +1) */
	void move (int dir)
	{
		if (dir == -1 && posTop > 0) {
			posTop += dir;
			posBottom += dir;
		}

		if (dir == 1 && posBottom < HEIGHT - 1) {
			posTop += dir;
			posBottom += dir;
		}
	}

	/* Prints stats about the paddle */
	void printStats ()
	{
		cout << "posTop: " << posTop << endl;
		cout << "posBottom: " << posBottom << endl;
		cout << "x: " << x << endl;
	}
};