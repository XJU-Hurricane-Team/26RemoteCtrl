#include <gui/redmap_screen/redmapView.hpp>
#include <gui/redmap_screen/redmapPresenter.hpp>

redmapPresenter::redmapPresenter(redmapView& v)
    : view(v)
{

}

void redmapPresenter::activate()
{

}

void redmapPresenter::deactivate()
{

}

void redmapPresenter::onCtrlKeyValueChanged(int8_t ctrl_Key, int8_t choose){
    view.choosekey = choose;
    view.handleKeyEvent(ctrl_Key);
    view.colortoggleEvent(choose);
}

void redmapPresenter::R1StateChanged(uint8_t status){
    view.r1state = status;
}