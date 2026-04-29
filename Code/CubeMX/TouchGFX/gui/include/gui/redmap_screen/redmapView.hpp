#ifndef REDMAPVIEW_HPP
#define REDMAPVIEW_HPP

#include <gui_generated/redmap_screen/redmapViewBase.hpp>
#include <gui/redmap_screen/redmapPresenter.hpp>

class redmapView : public redmapViewBase
{
public:
    redmapView();
    virtual ~redmapView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void update2();
    virtual void handleKeyEvent(uint8_t key);
    virtual void colortoggleEvent(uint8_t point);
    int8_t choosekey;
    int8_t point;

protected:
};

#endif // REDMAPVIEW_HPP
