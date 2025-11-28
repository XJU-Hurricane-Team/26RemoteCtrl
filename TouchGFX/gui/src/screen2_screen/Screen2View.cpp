#include <gui/screen2_screen/Screen2View.hpp>
#include "stm32f4xx_hal.h"
#include "stdlib.h"

Screen2View::Screen2View()
{

}

void Screen2View::setupScreen()
{
    Screen2ViewBase::setupScreen();

    // 设置二维码内容：把下面字符串改为你想要指向的链接或任意文本。
    qrCode1.convertStringToQRCode("https://xju-hurricane-team.github.io/");
}

void Screen2View::tearDownScreen()
{
    Screen2ViewBase::tearDownScreen();
}

void Screen2View::handleTickEvent()
{
    tickCounter++;

    uint32_t now = HAL_GetTick();
    uint32_t totalSeconds =  now / 1000u;
    totalSeconds %= (24u * 3600u);

    digitalHours = totalSeconds / 3600u;
    digitalMinutes = (totalSeconds % 3600u) / 60u;
    digitalSeconds = totalSeconds % 60u;

    // Update the clock display
    digitalClock1.setTime24Hour(digitalHours, digitalMinutes, digitalSeconds);

 
    if (graphValue < 0)
    {
        graphValue = 0;
    }
    else if (graphValue > 100)
    {
        graphValue = 100;
    }
    if (tickCounter % 20 == 0)
    {
    graphValue = (rand() % 100) - 0.5f; // Random walk
      // Insert data point
      dynamicGraph1.addDataPoint(graphValue);
    }
    
}
