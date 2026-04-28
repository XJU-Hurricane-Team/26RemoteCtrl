#ifndef REDMAPPRESENTER_HPP
#define REDMAPPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class redmapView;

class redmapPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    redmapPresenter(redmapView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();
    virtual void onCtrlKeyValueChanged(int8_t ctrl_Key, int8_t choose, int8_t send, int8_t mode);

    virtual ~redmapPresenter() {}

private:
    redmapPresenter();

    redmapView& view;
};

#endif // REDMAPPRESENTER_HPP
