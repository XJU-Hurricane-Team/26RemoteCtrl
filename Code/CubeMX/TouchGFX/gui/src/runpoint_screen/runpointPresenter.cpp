#include <gui/runpoint_screen/runpointView.hpp>
#include <gui/runpoint_screen/runpointPresenter.hpp>

runpointPresenter::runpointPresenter(runpointView& v)
    : view(v)
{

}

void runpointPresenter::activate()
{

}

void runpointPresenter::deactivate()
{

}

void runpointPresenter::onKeyValueChanged(int8_t key){
    view.keyState = key;
}

void runpointPresenter::onCtrlKeyValueChanged(int8_t ctrl_Key, int8_t choose, int8_t send, int8_t mode){
    view.choosekey = choose;
    view.point = send;
    view.Mode2 = mode;
    view.handleKeyEvent(ctrl_Key);
}
