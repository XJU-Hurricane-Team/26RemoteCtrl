/**
 * @file    bluemapPresenter.hpp
 * @brief   蓝方地图Presenter
 */
#ifndef BLUEMAPPRESENTER_HPP
#define BLUEMAPPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class bluemapView;

class bluemapPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    bluemapPresenter(bluemapView& v);

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
    virtual void onCtrlKeyChanged(int8_t ctrl_Key) override;
    virtual void onChoosepointChanged(int8_t choose) override;
    virtual void onR1StateChanged(int16_t Pos_x, int16_t Pos_y, int16_t Pos_yaw,
                                  uint8_t status, uint8_t state, float accel,
                                  float leftPos, float rightPos,
                                  uint8_t leftAdsorbed, uint8_t rightAdsorbed,
                                  uint8_t sendMsg, uint8_t recMsg,
                                  uint8_t source, bool sendflag) override;
    virtual void onMapSubModeChanged(bool in_sub, uint8_t preset_idx) override;

    virtual ~bluemapPresenter() {}

private:
    bluemapPresenter();

    bluemapView& view;
};

#endif // BLUEMAPPRESENTER_HPP
