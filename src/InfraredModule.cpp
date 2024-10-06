#include "InfraredModule.h"

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
    // TODO Setup IR?

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

void InfraredModule::processPress(uint8_t protocol, uint16_t address, uint16_t command, uint16_t bits, uint16_t extra)
{
    for (uint8_t i = 0; i < IR_ChannelCount; i++)
    {
        _channels[i]->processPress(protocol, address, command, bits, extra);
    }
}

void InfraredModule::processRelease()
{
    for (uint8_t i = 0; i < IR_ChannelCount; i++)
    {
        getChannel(i)->processRelease();
    }
}

void InfraredModule::transmitIrCode(uint8_t protocol, uint16_t address, uint16_t command, uint16_t bits, uint16_t extra)
{
    // TODO Transmit IR Code
}

void InfraredModule::receiveIrCode()
{
    // TODO Receive IR Code

    // do
    // processPress(uint8_t protocol, uint16_t address, uint16_t command, uint16_t bits, uint16_t extra);
    // or
    // processRelease();
}

InfraredModule openknxInfraredModule;