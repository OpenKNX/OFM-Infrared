#include "InfraredChannel.h"
#include "InfraredModule.h"

InfraredChannel::InfraredChannel(uint8_t index)
{
    _channelIndex = index;
}

const std::string InfraredChannel::name()
{
    return "IR";
}

// void InfraredChannel::setup()
// {
// }

// void InfraredChannel::loop()
// {
// }

void InfraredChannel::processInputKo(GroupObject &ko)
{
    if (!ParamIR_cMode != 2) return; // Not in send mode

    if (IR_KoCalcIndex(ko.asap()) == IR_KocControl)
    {
        logTraceP("processInputKo %i", (bool)ko.value(DPT_Bool));
        if ((bool)ko.value(DPT_Bool))
        {
            // Send IR
            openknxInfraredModule.transmitIrCode(ParamIR_cCodeProtocol, ParamIR_cCodeAddress, ParamIR_cCodeCommand, ParamIR_cCodeBits, ParamIR_cCodeExtra);
        }
        else
        {
            // Ignore
            // TODO/FEAT: Support IR repeat - then stop repeating
        }
    }
}

void InfraredChannel::processPress(uint8_t protocol, uint16_t address, uint16_t command, uint16_t bits, uint16_t extra)
{
    if (!ParamIR_cMode != 1) return; // Not in receive mode
    if (!ParamIR_cCodeProtocol) return;
    if (!ParamIR_cCodeAddress) return;
    if (!ParamIR_cCodeCommand) return;
    if (!ParamIR_cCodeBits) return;

    logTraceP("processPress");

    KoIR_cControl.value(true, DPT_Bool);
    _pressed = true;
}

void InfraredChannel::processRelease()
{
    if (!ParamIR_cMode != 1) return; // Not in receive mode
    if (!_pressed) return;           // no press is active

    logTraceP("processRelease");

    KoIR_cControl.value(false, DPT_Bool);
    _pressed = false;
}
