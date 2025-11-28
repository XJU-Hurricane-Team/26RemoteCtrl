#include <gui/screen1_screen/Screen1View.hpp>
#include <touchgfx/Unicode.hpp>
#include <stdlib.h>
Screen1View::Screen1View()
{

    myCounter = 0;

}

void Screen1View::setupScreen()
{
    Screen1ViewBase::setupScreen();
}

/* 每个渲染周期都会调用这个函数 */
void Screen1View::handleTickEvent()
{
    Screen1ViewBase::handleTickEvent();
}

void Screen1View::tearDownScreen()
{
    Screen1ViewBase::tearDownScreen();
}

    /* 这个函数用于更新文本的通配符 */
void Screen1View::InfoUpdate1()
{
    myCounter++;
    if(myCounter >= 100)
    {
        myCounter = 0;
    }
    // 使用每个 wildcard 对应的缓冲区大小常量，避免写越界
    touchgfx::Unicode::snprintf(textArea2Buffer1, TEXTAREA2BUFFER1_SIZE, "%d", myCounter);
    touchgfx::Unicode::snprintf(textArea2Buffer2, TEXTAREA2BUFFER2_SIZE, "%d", myCounter);
    touchgfx::Unicode::snprintf(textArea3Buffer1, TEXTAREA3BUFFER1_SIZE, "%d", myCounter);
    touchgfx::Unicode::snprintf(textArea3Buffer2, TEXTAREA3BUFFER2_SIZE, "%d", myCounter);

    // 更新按键显示并刷新所有被修改的 text areas
    touchgfx::Unicode::snprintf(textArea4Buffer, TEXTAREA4_SIZE, "Key%u", (unsigned)KeyState);
    textArea2.invalidate();
    textArea3.invalidate();
    textArea4.invalidate();
}