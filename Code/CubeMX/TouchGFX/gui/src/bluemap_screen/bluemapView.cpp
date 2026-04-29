#include <gui/bluemap_screen/bluemapView.hpp>

bluemapView::bluemapView():choosekey(0),point(0)
{

}

void bluemapView::setupScreen()
{
    bluemapViewBase::setupScreen();
}

void bluemapView::tearDownScreen()
{
    bluemapViewBase::tearDownScreen();
}

void bluemapView::handleKeyEvent(uint8_t key){
    bluemapViewBase::handleKeyEvent(key);
}

void bluemapView::colortoggleEvent(uint8_t point){
    bluemapViewBase::colortoggleEvent(point);
}

void bluemapView::update2(){
    Unicode::snprintf(bluechoosepointBuffer, BLUECHOOSEPOINT_SIZE, "%d", choosekey);
    Unicode::snprintf(bluesendpointBuffer, BLUESENDPOINT_SIZE, "%d", point);
    bluechoosepoint.invalidate();
    bluesendpoint.invalidate();
}
