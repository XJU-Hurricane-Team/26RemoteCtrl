#ifndef RUNPOINTVIEW_HPP
#define RUNPOINTVIEW_HPP

#include <gui_generated/runpoint_screen/runpointViewBase.hpp>
#include <gui/runpoint_screen/runpointPresenter.hpp>

class runpointView : public runpointViewBase
{
public:
    runpointView();
    virtual ~runpointView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void update2();
    virtual void handleKeyEvent(uint8_t key);
    int8_t keyState;
    int8_t choosekey;
    int8_t point;
    int8_t Mode2;
protected:
};

#endif // RUNPOINTVIEW_HPP
