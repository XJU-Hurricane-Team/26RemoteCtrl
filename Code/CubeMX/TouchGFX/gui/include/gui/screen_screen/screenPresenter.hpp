#ifndef SCREENPRESENTER_HPP
#define SCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class screenView;

class screenPresenter : public touchgfx::Presenter, public ModelListener {
  public:
    screenPresenter(screenView &v);

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

    virtual ~screenPresenter() {}
    virtual void onKeyValueChanged(int8_t key);
    virtual void onIrdaMsgNumChanged(uint8_t msgnum);
    virtual void onCtrlKeyValueChanged(int8_t ctrl_key, int8_t choose);
    virtual void onVoltageChanged(int8_t voltage);
    virtual void onJoystickChanged(int8_t rsL_x, int8_t rsL_y, int8_t rsR_x,
                                   int8_t rsR_y);
    virtual void onR1StateChanged(int16_t xSpeed, int16_t ySpeed,
                                  int16_t wSpeed, uint8_t status, uint8_t state, float accel,
                                  float leftPos, float rightPos, uint8_t leftAdsorbed, uint8_t rightAdsorbed,
                                  uint8_t sendMsg, uint8_t recMsg);
    virtual void R1StateChanged(uint8_t status, uint8_t state, float accel, uint8_t source);
    // virtual void onR2StateChanged(int16_t xSpeed, int16_t ySpeed, int16_t angle,
    //                               uint8_t status);

  private:
    screenPresenter();

    screenView &view;
};

#endif // SCREENPRESENTER_HPP
