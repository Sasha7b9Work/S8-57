#include "defines.h"
#include "Display/Display.h"
#include "Display/Painter.h"
#include "Display/Primitives.h"
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


void Test::Display::Update()
{
    Painter::BeginScene(Color::BACK);

    Color::FILL.SetAsCurrent();

    for (int i = 0; i < queue->Size(); i++)
    {
        Text((*queue)[i]->CString()).Draw(10, i * 10);
    }

    Painter::EndScene();
}


void Test::Display::StartTest(char *nameTest)
{
    queue->Push(new String(nameTest));
}


int Test::Display::AddMessage(char *message, int num)
{
    queue->Push(new String(message));
    return 0;
}
