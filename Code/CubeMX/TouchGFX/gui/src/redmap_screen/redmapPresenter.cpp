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

void redmapPresenter::onCtrlKeyValueChanged(int8_t ctrl_Key, int8_t choose, int8_t send, int8_t mode){
    view.point = send;
    view.choosekey = choose;
    view.handleKeyEvent(ctrl_Key);
    view.colortoggleEvent(choose);
}
