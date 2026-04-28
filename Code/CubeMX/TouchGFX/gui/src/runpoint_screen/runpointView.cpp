#include <gui/runpoint_screen/runpointView.hpp>

runpointView::runpointView():keyState(0),choosekey(0),point(0),Mode2(0)
{

}

void runpointView::setupScreen()
{
    runpointViewBase::setupScreen();
}

void runpointView::tearDownScreen()
{
    runpointViewBase::tearDownScreen();
}

void runpointView::handleKeyEvent(uint8_t key){
    runpointViewBase::handleKeyEvent(key);
}

void runpointView::update2(){
    Unicode::snprintf(keynumBuffer, KEYNUM_SIZE, "%d", keyState);
    Unicode::snprintf(chooseBuffer, CHOOSE_SIZE, "%d", choosekey);
    Unicode::snprintf(sendpointBuffer, SENDPOINT_SIZE, "%d", point);
    Unicode::snprintf(mode2Buffer, MODE2_SIZE, "%d", Mode2);
    keynum.invalidate();
    choose.invalidate();
    sendpoint.invalidate();
    mode2.invalidate();
}
