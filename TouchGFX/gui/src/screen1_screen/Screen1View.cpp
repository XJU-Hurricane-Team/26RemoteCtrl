#include <gui/screen1_screen/Screen1View.hpp>
#include <touchgfx/Unicode.hpp>
#include <stdlib.h>
Screen1View::Screen1View()
{

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
    // 使用每个 wildcard 对应的缓冲区大小常量，避免写越界
    // 更新按键显示并刷新所有被修改的 text areas
    touchgfx::Unicode::snprintf(textArea2Buffer1, TEXTAREA2BUFFER1_SIZE, "%d", rsL_x);
    touchgfx::Unicode::snprintf(textArea2Buffer2, TEXTAREA2BUFFER2_SIZE, "%d", rsL_y);
    touchgfx::Unicode::snprintf(textArea3Buffer1, TEXTAREA3BUFFER1_SIZE, "%d", rsR_x);
    touchgfx::Unicode::snprintf(textArea3Buffer2, TEXTAREA3BUFFER2_SIZE, "%d", rsR_y);
    touchgfx::Unicode::snprintf(textArea4Buffer, TEXTAREA4_SIZE, "Key%d",keyState);
    touchgfx::Unicode::snprintf(textArea5Buffer, TEXTAREA5_SIZE, "%d",voltage);
    textArea2.invalidate();
    textArea3.invalidate();
    textArea4.invalidate();
    textArea5.invalidate();

    if (graphValue < 0)
    {
        graphValue = 0;
    }
    else if (graphValue > 100)
    {
        graphValue = 100;
    }

    graphValue = (rand() % 100) - 0.5f; // Random walk
      // Insert data point
      dynamicGraph1.addDataPoint(graphValue);

}