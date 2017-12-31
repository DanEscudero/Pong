class Ball
{
public:
	double x;
	double y;

	double xspeed;
	double yspeed;

	int ang_deg;
	double ang;	
	
	Ball(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	~Ball ()
	{
		
	}

	/* Resets ball speed, based on angle a and direction d */
	void setSpeed (int a, int dir)
	{
		this->ang = rad(a);

		this->xspeed = dir*cos(this->ang);
		this->yspeed = dir*sin(this->ang);
	}

	/* Resets ball stats */
	void reset ()
	{
		int d;
		
		this->x = WIDTH/2;
		this->y = HEIGHT/2;

		if (prob(0.5)) 	d = 1;
		else 			d = -1;

		this->setSpeed(rand_range(-45, 45), d);
	}

	/* Moves Ball */
	void move ()
	{
		ang = rad(ang_deg);
		this->x += this->xspeed;
		this->y += this->yspeed;	
	}

	/* Sorts a new angle based os relative postition of ball and paddle */
	int newAngle (Paddle *p)
	{
		switch ((int)(this->y - p->posTop)) {
			case 0:
				return rand_range(-45, -25);
				break;
			case 1:
				return rand_range(-25, -10);
				break;
			case 2:
				return rand_range(-10, 10);
				break;
			case 3:
				return rand_range(10, 25);
				break;
			case 4:
				return rand_range(25, 45);
				break;
		}
	}

	/* Checks if ball hits paddle */
	void checkPaddle(Paddle *p, char t)
	{
		if (t = 'R') {
			if (floor(this->x) == (p->x)-1
			 && floor(this->y) <= p->posBottom
			 && floor(this->y) >= p->posTop) {
				this->xspeed *= -1;
				this->ang_deg = this->newAngle(p);
				setSpeed(this->ang_deg, -1);
			}
		}
		
		if (t = 'L') {
			if (floor(this->x) == (p->x)+1
			 && floor(this->y) <= p->posBottom
			 && floor(this->y) >= p->posTop) {
				this->xspeed *= -1;
				this->ang_deg = this->newAngle(p);
				setSpeed(this->ang_deg, 1);
			}
		}
	}

	/* Checks if ball hits edge and makes it go down */
	void checkEdge ()
	{
		if (floor(this->y) == HEIGHT-1 || floor(this->y) == 0)
			this->yspeed *= -1;
	}

	/* Checks if ball passes the paddles */
	bool checkPass()
	{
		return floor(this->x) == 0 || floor(this->x) == WIDTH-1;
	}

	/* Prints Stats about the ball */
	void printStats()
	{
		cout << "ang_deg: " << ang_deg << endl;
		cout << "x: " << x << endl;
		cout << "y: " << y << endl;
		cout << "xspeed: " << xspeed << endl;
		cout << "yspeed: " << yspeed << endl;
	}	
};