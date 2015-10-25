#include <windows.h>
#include <iostream>
#include "SnakeView.h"
#include "Snake.h"
#include <pthread.h>
#include <random>
#include <string>

using namespace std;

enum status {welcome,playing,paused,dead};

const int movePerMs = 1000;
//4 parameters of Boundry() are topLine.y,bottomLine.y,leftLine.x,rightLine.x
const Boundry b = Boundry(1,10,1,30);
Point apple = Point(0,0);
enum Direction changeToDirect = toUp;
enum status nowStatus = welcome;

void *beginKeyCatch(void* snk);
void *refresh(void* input);

struct inputForRefresh
{
    Snake& snakeInstance;
    SnakeView& viewInstance;
    inputForRefresh(Snake& snk,SnakeView& v):snakeInstance(snk),viewInstance(v){}
};

int main()
{
    //initialize snakeBody
    Point p1 = Point((b.rightLine+b.leftLine)/2,(b.bottomLine+b.topLine)/2);//at the center
    std::vector<Point> v;
    v.push_back(p1);
    //parameter of constuction of Snake is a vector<Point>
    Snake snakeInstance = Snake(v);

    SnakeView viewInstance = SnakeView();

    inputForRefresh* refreshInput = new inputForRefresh(snakeInstance,viewInstance);
    Snake* keyCatchInput = &snakeInstance;

    pthread_t rend,keyCatch; 
    pthread_attr_t attrDefault;
    pthread_attr_init(&attrDefault);
    pthread_create(&rend, &attrDefault, refresh, refreshInput);
    pthread_create(&keyCatch, &attrDefault, beginKeyCatch, keyCatchInput);
    pthread_join(rend,NULL);
    pthread_join(keyCatch,NULL);

    return 0;
}

void *beginKeyCatch(void* snk)
{
    Snake* snakeInstance = (Snake*)snk;

    HANDLE hIn;
    HANDLE hOut;
    int KeyEvents = 0;
    bool Continue = TRUE;
    INPUT_RECORD InRec;
    DWORD NumRead;
 
    hIn = GetStdHandle(STD_INPUT_HANDLE);
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  
    DWORD  fdwMode, fdwSaveOldMode;
    GetConsoleMode(hIn, &fdwSaveOldMode);
  
    fdwMode =  fdwSaveOldMode&(~ENABLE_ECHO_INPUT)
       &(~ENABLE_LINE_INPUT);
    SetConsoleMode(hIn, fdwMode);

    while (Continue)
    {
        ReadConsoleInput(hIn,
                         &InRec,
                         1,
                         &NumRead);
  
        switch (InRec.EventType)
        {
        case KEY_EVENT:
            if(InRec.Event.KeyEvent.bKeyDown)
            {
                if(InRec.Event.KeyEvent.wVirtualKeyCode == VK_UP)
                {
                    changeToDirect = toUp;
                }
                else if(InRec.Event.KeyEvent.wVirtualKeyCode == VK_DOWN)
                {
                    changeToDirect = toDown;
                }
                else if(InRec.Event.KeyEvent.wVirtualKeyCode == VK_LEFT)
                {
                    changeToDirect = toLeft;
                }
                else if(InRec.Event.KeyEvent.wVirtualKeyCode == VK_RIGHT)
                {
                    changeToDirect = toRight;
                }
                else if(InRec.Event.KeyEvent.wVirtualKeyCode == VK_SPACE)
                {
                    if(nowStatus == playing)
                        nowStatus = paused;
                    else
                        nowStatus = playing;
                }
            }
            break;
        }
    }
}

void *refresh(void* input)
{
    //intialize random function and apple
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distributionX(b.leftLine + 1,b.rightLine - 1);
    std::uniform_int_distribution<int> distributionY(b.topLine + 1,b.bottomLine - 1);
    apple = Point(distributionX(generator),distributionY(generator));

    inputForRefresh *p = (inputForRefresh*)input;
    while (1)
    {
        Sleep(movePerMs);
        if (nowStatus == welcome)
        {
            p -> viewInstance.clrscr();
            p -> viewInstance.drawSentence(Point(20,10),string("Hello Snake!"));
            nowStatus = playing;
            Sleep(1000);
        }
        else if(nowStatus == paused)
        {
            p -> viewInstance.clrscr();
            p -> viewInstance.drawSentence(Point(20,10),string("Paused"));
        }
        else if(nowStatus == dead)
        {
            p -> viewInstance.clrscr();
            p -> viewInstance.drawSentence(Point(20,10),string("Oops, your snake is dead."));
        }
        else if(nowStatus == playing)
        {
            p -> snakeInstance.changeDirection(changeToDirect);
            Point newHead = p -> snakeInstance.move();
            //meet boundry
            if (newHead.x == b.leftLine || newHead.x == b.rightLine ||
                newHead.y == b.topLine || newHead.y == b.bottomLine)
            {
                nowStatus = dead;
                continue;
            }
            //meet apple
            else if(newHead.x == apple.x && newHead.y == apple.y)
            {
                p -> snakeInstance.grow();
                apple.x = distributionX(generator);
                apple.y = distributionY(generator);
            }
            //refresh screen
            p -> viewInstance.clrscr();
            p -> viewInstance.drawVectorPoint(p -> snakeInstance.getSnakeBody());
            p -> viewInstance.drawVerticalLine(Point(b.leftLine,b.topLine),Point(b.leftLine,b.bottomLine));
            p -> viewInstance.drawVerticalLine(Point(b.rightLine,b.topLine),Point(b.rightLine,b.bottomLine));
            p -> viewInstance.drawHorizonalLine(Point(b.leftLine,b.topLine),Point(b.rightLine,b.topLine));
            p -> viewInstance.drawHorizonalLine(Point(b.leftLine,b.bottomLine),Point(b.rightLine,b.bottomLine));
            p -> viewInstance.drawPoint(apple);
            //MinGW has a bug on to_string function, so I have to divide score into two part
            p -> viewInstance.drawSentence(Point(b.rightLine+1,1),string("Score:"));
            p -> viewInstance.drawInteger(Point(b.rightLine+7,1),p -> snakeInstance.getLength()-1);
        }
    }
}