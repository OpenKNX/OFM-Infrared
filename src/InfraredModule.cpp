#include "InfraredModule.h"

// TODO move to hardware.h and validate define
#define IR_RECV_PIN 13
#define IR_SEND_PIN 14
#define NO_LED_FEEDBACK_CODE

#include <IRremote.hpp>

const std::string InfraredModule::name()
{
    return "IR";
}

const std::string InfraredModule::version()
{
    return MODULE_Infrared_Version;
}

void InfraredModule::setup(bool configured)
{
    IrReceiver.begin(IR_RECV_PIN);
    IrSender.begin();

    if (!configured) return;

    for (uint8_t i = 0; i < IR_ChannelCount; i++)
    {
        _channels[i] = new InfraredChannel(i);
        _channels[i]->setup();
    }
}

void InfraredModule::loop(bool configured)
{
    receiveIrCode();

    if (!configured) return;

    uint8_t processed = 0;
    do
        _channels[_currentChannel]->loop();

    while (openknx.freeLoopIterate(IR_ChannelCount, _currentChannel, processed));
}

void InfraredModule::processInputKo(GroupObject &ko)
{
    uint16_t asap = ko.asap();
    if (asap >= IR_KoOffset && asap < IR_KoOffset + IR_ChannelCount * IR_KoBlockSize)
    {
        uint8_t i = (asap - IR_KoOffset) / IR_KoBlockSize;
        _channels[i]->processInputKo(ko);
    }
}

bool InfraredModule::processCommand(const std::string command, bool diagnose)
{
    // TODO
    return false;
}

void InfraredModule::showHelp()
{
    // TODO
}

void InfraredModule::processPress(InfraredCode &code)
{
    for (uint8_t i = 0; i < IR_ChannelCount; i++)
    {
        _channels[i]->processPress(code);
    }
}

void InfraredModule::processRelease()
{
    for (uint8_t i = 0; i < IR_ChannelCount; i++)
    {
        getChannel(i)->processRelease();
    }
}

void InfraredModule::transmitIrCode(InfraredCode &code)
{
    // TODO Transmit IR Code
}

void InfraredModule::receiveIrCode()
{
    // TODO Receive IR Code

    // do
    // processPress(code);
    // or
    // processRelease();
}

bool InfraredModule::processFunctionProperty(uint8_t objectIndex, uint8_t propertyId, uint8_t length, uint8_t *data, uint8_t *resultData, uint8_t &resultLength)
{
    // TODO
    return false;
}

bool InfraredModule::processFunctionPropertyState(uint8_t objectIndex, uint8_t propertyId, uint8_t length, uint8_t *data, uint8_t *resultData, uint8_t &resultLength)
{
    // TODO
    return false;
}

InfraredModule openknxInfraredModule;