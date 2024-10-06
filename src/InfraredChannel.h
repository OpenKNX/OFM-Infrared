#pragma once
#include "OpenKNX.h"
#include "InfraredCode.h"

class InfraredChannel : public OpenKNX::Channel
{

  protected:
    bool _pressed = false;

  public:
    InfraredChannel(uint8_t index);

    // void setup() override;
    // void loop() override;
    const std::string name() override;

    void processInputKo(GroupObject &ko);
    void processPress(InfraredCode &code);
    void processRelease();
};
