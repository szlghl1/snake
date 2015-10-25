#ifndef _SnakeView_
#define _SnakeView_

#include <iostream>
#include <vector>
#include "Point.h"
#include "Boundry.h"
#include <string>

class SnakeView
{
private:
    void gotoxy(int x,int y);
public:
    void drawVerticalLine(Point upPoint,Point downPoint);
    void drawHorizonalLine(Point leftPoint,Point rightPoint);
    void drawPoint(Point pnt);
    void drawSentence(Point pnt,std::string str);
    void drawVectorPoint(std::vector<Point>& v);
    void drawInteger(Point pnt,int i);
	void clrscr();
};

void SnakeView::clrscr(void)
{
    CONSOLE_SCREEN_BUFFER_INFO    csbiInfo;
    HANDLE    hConsoleOut;
    COORD    Home = {0,0};
    DWORD    dummy;
    hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hConsoleOut,&csbiInfo);
    FillConsoleOutputCharacter(hConsoleOut,' ',
        csbiInfo.dwSize.X * csbiInfo.dwSize.Y,Home,
        &dummy);
    csbiInfo.dwCursorPosition.X = 0;
    csbiInfo.dwCursorPosition.Y = 0;
    SetConsoleCursorPosition(hConsoleOut,csbiInfo.dwCursorPosition);
}
//locate cursor
void SnakeView::gotoxy(int x,int y)
{
    CONSOLE_SCREEN_BUFFER_INFO    csbiInfo;
    HANDLE    hConsoleOut;
    hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hConsoleOut,&csbiInfo);
    csbiInfo.dwCursorPosition.X = x;
    csbiInfo.dwCursorPosition.Y = y;
    SetConsoleCursorPosition(hConsoleOut,csbiInfo.dwCursorPosition);
}

void SnakeView::drawVerticalLine(Point upPoint,Point downPoint)
{
    if(upPoint.x == downPoint.x)
    {
        for (int i = 0; i <= downPoint.y - upPoint.y; i++)
        {
            gotoxy(upPoint.x,upPoint.y + i);
            std::cout << '|';
        }
        return;
    }
    else
        return;
}

void SnakeView::drawHorizonalLine(Point leftPoint,Point rightPoint)
{
    if(leftPoint.y == rightPoint.y)
    {
        gotoxy(leftPoint.x,leftPoint.y);
        for (int i = 0; i <= rightPoint.x - leftPoint.x; ++i)
        {
            std::cout << '-';
        }
        return;
    }
    else
        return;
}

void SnakeView::drawPoint(Point pnt)
{
    gotoxy(pnt.x,pnt.y);
    std::cout << '*';
    return;
}

void SnakeView::drawVectorPoint(std::vector<Point>& v)
{
    for(std::vector<Point>::iterator i = v.begin() ; i != v.end() ; i++)
    {
        gotoxy((*i).x,(*i).y);
        std::cout << '*';
    }
}

void SnakeView::drawSentence(Point pnt,std::string str)
{
    gotoxy(pnt.x,pnt.y);
    std::cout << str << std::endl; 
}

void SnakeView::drawInteger(Point pnt,int i)
{
    gotoxy(pnt.x,pnt.y);
    std::cout << i << std::endl; 
}
#endif