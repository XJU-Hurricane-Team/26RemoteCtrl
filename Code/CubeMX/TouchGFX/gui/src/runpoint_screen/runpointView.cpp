#include <gui/runpoint_screen/runpointView.hpp>

runpointView::runpointView():keyState(0)
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
    keynum.invalidate();
}