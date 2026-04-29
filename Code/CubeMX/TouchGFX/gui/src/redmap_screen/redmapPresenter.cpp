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
    // if (choose == 17|| choose == 18|| choose == 19||
    //     choose == 22|| choose == 23|| choose == 24|| 
    //     choose == 27|| choose == 28|| choose == 29|| 
    //     choose == 32|| choose == 33|| choose == 34)
    // {
    //     choose = 0;
    // }
    view.choosekey = choose;
    view.handleKeyEvent(ctrl_Key);
    view.colortoggleEvent(choose);
}
