/**
 * @file    screenPresenter.hpp
 * @brief   主遥控界面Presenter
 */
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
    virtual void onKeyValueChanged(int8_t key) override;           // 普通按键
    virtual void onIrdaMsgNumChanged(uint8_t msgnum) override;     // IRDA消息号
    virtual void onCtrlKeyChanged(int8_t ctrl_key) override;       // CTRL/SET 按键
    virtual void onChoosepointChanged(int8_t choose) override;     // 跑点高亮位置
    virtual void onVoltageChanged(int8_t voltage) override;       // 电池电压更新
    virtual void onJoystickChanged(int8_t rsL_x, int8_t rsL_y, int8_t rsR_x,
                                   int8_t rsR_y) override;        // 摇杆数据更新
    virtual void onR1StateChanged(int16_t xSpeed, int16_t ySpeed,
                                  int16_t wSpeed, uint8_t status, uint8_t state, float accel,
                                  float leftPos, float rightPos, uint8_t leftAdsorbed, uint8_t rightAdsorbed,
                                  uint8_t sendMsg, uint8_t recMsg,
                                  uint8_t source) override; // R1完整状态更新
    // virtual void onR2StateChanged(int16_t xSpeed, int16_t ySpeed, int16_t angle,
    //                               uint8_t status);

  private:
    screenPresenter();

    screenView &view;
};

#endif // SCREENPRESENTER_HPP
