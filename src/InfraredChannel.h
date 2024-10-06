#pragma once
#include "OpenKNX.h"

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
    void processPress(uint8_t protocol, uint16_t address, uint16_t command, uint16_t bits, uint16_t extra);
    void processRelease();
};
