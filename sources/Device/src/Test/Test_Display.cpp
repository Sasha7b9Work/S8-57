#include "defines.h"
#include "Display/Display.h"
#include "Display/Painter.h"
#include "Display/Primitives.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Queue.h"
#include "Test/Test.h"
#include "Utils/Values.h"


Queue<String *> *queue = nullptr;


void Test::Display::Init()
{
    queue = new Queue<String *>();
}


void Test::Display::DeInit()
{
    while (queue->Size())
    {
        delete queue->Back();
    }

    delete queue;
}


static void Update()
{
    Painter::BeginScene(Color::BACK);

    Color::FILL.SetAsCurrent();

    int start = queue->Size() - 24;
    if (start < 0)
    {
        start = 0;
    }

    for (int i = start; i < queue->Size(); i++)
    {
        Text((*queue)[i]->CString()).Draw(10, (i - start) * 10);
    }

    Painter::EndScene();
}


static String *CreateMessage(char *message)
{
    return new String("%d %s", Timer::TimeMS() / 1000, message);
}


void Test::Display::StartTest(char *nameTest)
{
    queue->Push(CreateMessage(nameTest));

    Update();
}


int Test::Display::AddMessage(char *message, int num)
{
    int result = num;

    if (num == -1)
    {
        queue->Push(CreateMessage(message));
        result = queue->Size() - 1;
    }
    else
    {
        delete (*queue)[num];
        (*queue)[num] = CreateMessage(message);
    }

    Update();

    return result;
}
