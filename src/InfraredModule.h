#pragma once

#include "InfraredChannel.h"
#include "InfraredCode.h"
#include "OpenKNX.h"

class InfraredModule : public OpenKNX::Module
{

  protected:
    InfraredChannel *_channels[IR_ChannelCount];
    uint8_t _currentChannel = 0;
    InfraredCode _lastReceviedCode;
    uint32_t _lastReceviedTime = 0;

    void receiveIrCode();
    void processRelease();
    void processPress(InfraredCode &code);

  public:
    void setup(bool configured) override;
    void loop(bool configured) override;
    void processInputKo(GroupObject &ko) override;
    bool transmitIrCode(InfraredCode &code);
    InfraredChannel *getChannel(uint8_t index);

    bool processFunctionProperty(uint8_t objectIndex, uint8_t propertyId, uint8_t length, uint8_t *data, uint8_t *resultData, uint8_t &resultLength) override;
    bool processFunctionPropertyState(uint8_t objectIndex, uint8_t propertyId, uint8_t length, uint8_t *data, uint8_t *resultData, uint8_t &resultLength) override;

    const std::string name() override;
    const std::string version() override;
};

extern InfraredModule openknxInfraredModule;
