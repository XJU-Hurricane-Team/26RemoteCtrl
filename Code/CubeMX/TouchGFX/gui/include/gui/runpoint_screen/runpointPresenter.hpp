#ifndef RUNPOINTPRESENTER_HPP
#define RUNPOINTPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class runpointView;

class runpointPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    runpointPresenter(runpointView& v);

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
    virtual void onKeyValueChanged(int8_t key);
    virtual ~runpointPresenter() {}

private:
    runpointPresenter();

    runpointView& view;
};

#endif // RUNPOINTPRESENTER_HPP
