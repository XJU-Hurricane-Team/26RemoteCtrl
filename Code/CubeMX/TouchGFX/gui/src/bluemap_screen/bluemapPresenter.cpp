#include <gui/bluemap_screen/bluemapView.hpp>
#include <gui/bluemap_screen/bluemapPresenter.hpp>

bluemapPresenter::bluemapPresenter(bluemapView& v)
    : view(v)
{

}

void bluemapPresenter::activate()
{

}

void bluemapPresenter::deactivate()
{

}

void bluemapPresenter::onCtrlKeyValueChanged(int8_t ctrl_Key, int8_t choose, int8_t send, int8_t mode){
    view.point = send;
    view.choosekey = choose;
    view.handleKeyEvent(ctrl_Key);
}
