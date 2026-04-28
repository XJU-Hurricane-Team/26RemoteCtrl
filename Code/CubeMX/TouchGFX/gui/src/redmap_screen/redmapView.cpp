#include <gui/redmap_screen/redmapView.hpp>

redmapView::redmapView():choosekey(0),point(0)
{

}

void redmapView::setupScreen()
{
    redmapViewBase::setupScreen();
}

void redmapView::tearDownScreen()
{
    redmapViewBase::tearDownScreen();
}

void redmapView::handleKeyEvent(uint8_t key){
    redmapViewBase::handleKeyEvent(key);
}

void redmapView::update2(){
    Unicode::snprintf(redchoosepointBuffer, REDCHOOSEPOINT_SIZE, "%d", choosekey);
    Unicode::snprintf(redsendpointBuffer, REDSENDPOINT_SIZE, "%d", point);
    redchoosepoint.invalidate();
    redsendpoint.invalidate();
}
