#ifndef BLUEMAPVIEW_HPP
#define BLUEMAPVIEW_HPP

#include <gui_generated/bluemap_screen/bluemapViewBase.hpp>
#include <gui/bluemap_screen/bluemapPresenter.hpp>

class bluemapView : public bluemapViewBase
{
public:
    bluemapView();
    virtual ~bluemapView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void update2();
    virtual void handleKeyEvent(uint8_t key);
    virtual void colortoggleEvent(uint8_t point);
    int8_t choosekey;
protected:
};

#endif // BLUEMAPVIEW_HPP
