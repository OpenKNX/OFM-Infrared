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
        _channels[i]->processRelease();
    }
}

void InfraredModule::transmitIrCode(InfraredCode &code)
{
    // TODO Transmit IR Code
    IRData irTransitData;
    irTransitData.protocol = (decode_type_t)code.protocol;
    irTransitData.address = code.address;
    irTransitData.command = code.command;
    irTransitData.numberOfBits = code.numberOfBits;
    irTransitData.extra = code.extra;

    IrReceiver.disableIRIn();
    IrSender.write(&irTransitData);
    IrReceiver.enableIRIn();
}

void InfraredModule::receiveIrCode()
{
    // TODO Receive IR Code
    if (IrReceiver.decode())
    {
        if (IrReceiver.decodedIRData.protocol && IrReceiver.decodedIRData.address && IrReceiver.decodedIRData.numberOfBits)
        {
            _lastReceviedCode.protocol = IrReceiver.decodedIRData.protocol;
            _lastReceviedCode.address = IrReceiver.decodedIRData.address;
            _lastReceviedCode.command = IrReceiver.decodedIRData.command;
            _lastReceviedCode.numberOfBits = IrReceiver.decodedIRData.numberOfBits;
            _lastReceviedCode.extra = IrReceiver.decodedIRData.extra;

            // is not repeated
            if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT)
            {
                _lastReceviedTime = millis();
            }
            else
            {
                logDebugP("receiveIrCode: %i/%i/%i/%i/%i/%i",
                          IrReceiver.decodedIRData.protocol,
                          IrReceiver.decodedIRData.address,
                          IrReceiver.decodedIRData.command,
                          IrReceiver.decodedIRData.numberOfBits,
                          IrReceiver.decodedIRData.extra,
                          IrReceiver.decodedIRData.flags);

                logIndentUp();
                processPress(_lastReceviedCode);
                logIndentDown();
                _lastReceviedTime = millis();
            }
        }

        IrReceiver.resume(); // Enable receiving of the next value
    }

    if (_lastReceviedTime && delayCheck(_lastReceviedTime, 150))
    {
        logDebugP("releaseIrCode: %i/%i/%i/%i/%i",
                  _lastReceviedCode.protocol,
                  _lastReceviedCode.address,
                  _lastReceviedCode.command,
                  _lastReceviedCode.numberOfBits,
                  _lastReceviedCode.extra);

        _lastReceviedTime = 0;
        logIndentUp();
        processRelease();
        logIndentDown();
    }
}

bool InfraredModule::processFunctionProperty(uint8_t objectIndex, uint8_t propertyId, uint8_t length, uint8_t *data, uint8_t *resultData, uint8_t &resultLength)
{
    if (!knx.configured()) return false;
    if (objectIndex != 0xA0) return false;
    if (propertyId != 0) return false;

    logInfoP("processFunctionProperty %i", data[0]);

    switch (data[0])
    {
        case 1:
        {
            // never received a valid code
            if (!_lastReceviedCode.protocol || !_lastReceviedCode.address || !_lastReceviedCode.command)
            {
                resultData[0] = 1; // Reserved for ErrorCode
                resultLength = 1;
                return true;
            }

            resultData[0] = 0; // Reserved for ErrorCode
            resultData[1] = (uint8_t)_lastReceviedCode.protocol;
            resultData[2] = (uint8_t)(_lastReceviedCode.address >> 8);
            resultData[3] = (uint8_t)_lastReceviedCode.address;
            resultData[4] = (uint8_t)(_lastReceviedCode.command >> 8);
            resultData[5] = (uint8_t)_lastReceviedCode.command;
            resultData[6] = (uint8_t)(_lastReceviedCode.numberOfBits >> 8);
            resultData[7] = (uint8_t)_lastReceviedCode.numberOfBits;
            resultData[8] = (uint8_t)(_lastReceviedCode.extra >> 8);
            resultData[9] = (uint8_t)_lastReceviedCode.extra;
            resultLength = 10;
            return true;
        }
        case 2:
        {
        }
    }

    return false;
}

bool InfraredModule::processFunctionPropertyState(uint8_t objectIndex, uint8_t propertyId, uint8_t length, uint8_t *data, uint8_t *resultData, uint8_t &resultLength)
{
    return false;
}

InfraredModule openknxInfraredModule;