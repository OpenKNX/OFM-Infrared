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

void InfraredChannel::processInputKo(GroupObject &ko)
{
    if (ParamIR_cMode != 2) return; // Not in send mode

    if (IR_KoCalcIndex(ko.asap()) == IR_KocControl)
    {
        logTraceP("processInputKo %i", (bool)ko.value(DPT_Bool));
        if ((bool)ko.value(DPT_Bool))
        {
            // Send IR
            InfraredCode code;
            code.protocol = ParamIR_cCodeProtocol;
            code.address = ParamIR_cCodeAddress;
            code.command = ParamIR_cCodeCommand;
            code.numberOfBits = ParamIR_cCodeBits;
            code.extra = ParamIR_cCodeExtra;
            logInfoP("Send IR-Code: %i/%i/%i/%i/%i", code.protocol, code.address, code.command, code.numberOfBits, code.extra);
            openknxInfraredModule.transmitIrCode(code);
        }
        else
        {
            // Ignore
            // TODO/FEAT: Support IR repeat - then stop repeating
        }
    }
}

void InfraredChannel::processPress(InfraredCode &code)
{
    if (ParamIR_cMode != 1) return; // Not in receive mode
    if (!ParamIR_cCodeProtocol) return;
    if (!ParamIR_cCodeAddress) return;
    if (!ParamIR_cCodeCommand) return;
    if (!ParamIR_cCodeBits) return;
    if (ParamIR_cCodeProtocol != code.protocol) return;
    if (ParamIR_cCodeAddress != code.address) return;
    if (ParamIR_cCodeCommand != code.command) return;
    if (ParamIR_cCodeBits != code.numberOfBits) return;
    if (ParamIR_cCodeExtra != code.extra) return;

    logTraceP("processPress");

    logIndentUp();
    // stop previouse if running
    processRelease();
    logIndentDown();

    KoIR_cControl.value(true, DPT_Bool);
    _pressed = true;
}

void InfraredChannel::processRelease()
{
    if (ParamIR_cMode != 1) return; // Not in receive mode
    if (!_pressed) return;          // no press is active

    logTraceP("processRelease");

    KoIR_cControl.value(false, DPT_Bool);
    _pressed = false;
}
