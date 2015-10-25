#ifndef _Boundry_
#define _Boundry_

//boundry consist of four line
const int topInitial = 1;
const int bottomInitial = 20;
const int leftInitial = 1;
const int rightInitial = 50;

struct Boundry
{
	int topLine;      //y coordinate of top line
	int bottomLine;   //y
	int leftLine;     //x
	int rightLine;    //x
	Boundry(int topIn = topInitial,int bottomIn = bottomInitial,
			int leftIn = leftInitial, int rightIn = rightInitial)
			:topLine(topIn),bottomLine(bottomIn),leftLine(leftIn),
			rightLine(rightIn){}
};

#endif