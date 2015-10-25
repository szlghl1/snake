#ifndef _snake_
#define _snake_

#include <vector>
#include "Point.h"
#include "Boundry.h"

enum Direction {toUp, toDown, toLeft, toRight};

class Snake
{
private:
	std::vector<Point> body;
	enum Direction direct;
	int length;
public:	
	Snake(const std::vector<Point>& v):body(v){length=v.size();}
	Point move();
	void grow();
	void changeDirection(Direction turnTo);
	std::vector<Point>& getSnakeBody();
	int getLength(){return length;}
};

Point Snake::move()
{	
	Point newHead(0,0);
	switch(direct)
	{
		case toUp: newHead.x = body[0].x;newHead.y = body[0].y - 1;break;
		case toDown: newHead.x = body[0].x;newHead.y = body[0].y + 1;break;
		case toLeft: newHead.x = body[0].x - 1;newHead.y = body[0].y;break;
		case toRight: newHead.x = body[0].x + 1,newHead.y = body[0].y;break;
	}
	body.insert(body.begin(),newHead);
	body.pop_back();
	return newHead;
}

void Snake::changeDirection(Direction turnTo)
{
	switch(turnTo)
	{	
		//you cannot turn a going up snake to going down
		case toUp: if (direct == toDown) return;break;
		case toDown: if (direct == toUp) return;break;
		case toLeft: if (direct == toRight) return;break;
		case toRight: if (direct == toLeft) return;break;
	}
	direct = turnTo;
	return;
}

std::vector<Point>& Snake::getSnakeBody()
{
	return body;
}

void Snake::grow()
{
	std::vector<Point>::iterator i = body.end() - 1;
	//if snake has only one node, growing direction is reverse to going direction
	if(length == 1)
	{
		length++;
		switch(direct)
		{
			case toUp: body.push_back(Point((*i).x,(*i).y+1));return;break;
			case toDown: body.push_back(Point((*i).x,(*i).y-1)); return;break;
			case toLeft: body.push_back(Point((*i).x+1,(*i).y)); return;break;
			case toRight: body.push_back(Point((*i).x-1,(*i).y)); return;break;
		}
	}
	else
	{
		length++;
		//two condition: tail is vertical or horizonal
		if ((*(i-1)).x == (*i).x)//vertical
		{
			if ((*(i-1)).y < (*i).y)
				body.push_back(Point((*i).x,(*i).y+1));
			else
				body.push_back(Point((*i).x,(*i).y-1));
		}
		else//horizonal
		{
			if ((*(i-1)).x < (*i).x)
				body.push_back(Point((*i).x+1,(*i).y));
			else
				body.push_back(Point((*i).x-1,(*i).y));
		}
	}
	return;
}

#endif