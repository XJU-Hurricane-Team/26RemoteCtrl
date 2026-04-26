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
    view.handleKeyEvent(key);
}