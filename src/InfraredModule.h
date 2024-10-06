#pragma once

#include "InfraredChannel.h"
#include "InfraredCode.h"
#include "OpenKNX.h"

class InfraredModule : public OpenKNX::Module
{

  protected:
    InfraredChannel *_channels[IR_ChannelCount];
    uint8_t _currentChannel = 0;
    InfraredCode _lastCode;

    void receiveIrCode();
    void processRelease();
    void processPress(InfraredCode &code);

  public:
    void setup(bool configured) override;
    void loop(bool configured) override;
    bool processCommand(const std::string command, bool diagnose) override;
    void showHelp() override;
    void processInputKo(GroupObject &ko) override;
    void transmitIrCode(InfraredCode &code);
    InfraredChannel *getChannel(uint8_t index);

    bool processFunctionProperty(uint8_t objectIndex, uint8_t propertyId, uint8_t length, uint8_t *data, uint8_t *resultData, uint8_t &resultLength) override;
    bool processFunctionPropertyState(uint8_t objectIndex, uint8_t propertyId, uint8_t length, uint8_t *data, uint8_t *resultData, uint8_t &resultLength) override;

    const std::string name() override;
    const std::string version() override;
};

extern InfraredModule openknxInfraredModule;

// #include <Arduino.h>
// #include "OpenKNX.h"
// #include "hardware.h"
// #include "IRremote.hpp"
// #include "knxprod.h"
// #include "LittleFS.h"

// class IrCodeModule : public OpenKNX::Module
// {
// 	public:
// 		void loop(bool configured) override;
// 		void setup(bool configured) override;
// 		void processInputKo(GroupObject &ko) override;
// 		void showHelp() override;

// 		const std::string name() override;
// 		const std::string version() override;
// 		// void writeFlash() override;
// 		// void readFlash(const uint8_t* data, const uint16_t size) override;
// 		// uint16_t flashSize() override;

// 	private:
// 		enum class FuncState {
// 			Idle,
// 			Receive,
// 			Verify,
// 			Success,
// 			Error
// 		};

// 		uint8_t _state = 0;
// 		FuncState _funcState = FuncState::Idle;
// 		IRrecv *rec;
// 		IRsend *send;
// 		IRData *_data;
// 		uint8_t _index;
// 		long _lastPress = -1;
// 		uint _pressInterval = 0;
// 		uint _pressState = 0;
// 		IRData* read(uint8_t index);
// 		void write(uint8_t index, IRData *data);
// 		void print(IRData *data, int16_t index);
// 		bool compare(uint8_t index, uint8_t *data);
// 		void handleCode(bool state);
// 		void checkPress();
// 		void executeCode(bool state);
// 		void sendCode(uint8_t index);
// 		bool isEnabled = false;
// 		long lastCode = 0;
// 		bool processFunctionProperty(uint8_t objectIndex, uint8_t propertyId, uint8_t length, uint8_t *data, uint8_t *resultData, uint8_t &resultLength) override;
// 		bool processFunctionPropertyState(uint8_t objectIndex, uint8_t propertyId, uint8_t length, uint8_t *data, uint8_t *resultData, uint8_t &resultLength) override;
// 		bool processCommand(const std::string cmd, bool diagnoseKo) override;
// 		void koHandleSwitch(GroupObject &ko, uint8_t index);
// 		void koHandleValue(GroupObject &ko, uint8_t index);
// 		void koHandleScene(GroupObject &ko, uint8_t index);
// 		void executeSwitch(bool state, uint8_t index);
// 		void executeValue();
// 		void executeScene();
// 		void executeDimm();
// 		void executeColor();
// 		void loopStateIdle();
// 		void loopStateVerify();
// 		void loopStateCheck();
// };

// //Give your Module a name
// //it will be desplayed when you use the method log("Hello")
// // -> Log     Hello
// const std::string IrCodeModule::name()
// {
//     return "IrCode";
// }

// //You can also give it a version
// //will be displayed in Command Infos
// const std::string IrCodeModule::version()
// {
//     return "";
// }

// //will be called once
// //only if knx.configured == true
// void IrCodeModule::setup(bool configured)
// {
//     rec = new IRrecv(IR_PIN_REC);
// 	rec->enableIRIn();

// 	//send = new IRsend(P21);
// 	//send->begin(P21);
// 	send = new IRsend();
// 	send->begin();
// 	send->enableIROut(IR_SEND_PIN);

// 	rec->resume();
// }

// void IrCodeModule::loop(bool configured)
// {
// 	switch(_state)
// 	{
// 		case 0:
// 		{
// 			if(configured)
// 				loopStateIdle();
// 			break;
// 		}

// 		case 1:
// 		{
// 			loopStateCheck();
// 			break;
// 		}

// 		case 2:
// 		{
// 			loopStateVerify();
// 			break;
// 		}
// 	}
// }

// void IrCodeModule::loopStateIdle()
// {
// 	if(rec->decode())
// 	{
// 		if(!(rec->decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT) && lastCode == 0)
// 		{
// 			logDebugP("IR Empfang");
// 			logIndentUp();
// 			handleCode(true);
// 		}
// 		rec->resume();
// 	}
// 	if(lastCode != 0 && delayCheckMillis(lastCode, 150))
// 	{
// 		logDebugP("IR Button losgelassen");
// 		//handleCode(false);
// 		executeCode(false);
// 		lastCode = 0;
// 	}
// }

// void IrCodeModule::loopStateCheck()
// {
// 	if(rec->decode())
// 	{
// 		//skip invalid codes
// 		if(rec->decodedIRData.protocol == 0 || rec->decodedIRData.address == 0 || rec->decodedIRData.numberOfBits == 0) return;
// 		//skip if code is repeated (like holding button)
// 		if(rec->decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT) return;

// 		_data = new IRData();
// 		_data->protocol = rec->decodedIRData.protocol;
// 		_data->address = rec->decodedIRData.address;
// 		_data->command = rec->decodedIRData.command;
// 		_data->numberOfBits = rec->decodedIRData.numberOfBits;
// 		_data->flags = rec->decodedIRData.flags;
// 		_data->extra = rec->decodedIRData.extra;

// 		print(_data, -1);

// 		_state = 2;
// 		_funcState = FuncState::Verify;
// 		logInfoP("IR Code empfangen. Zum verifizieren erneut senden");
// 		rec->resume();
// 	}
// }

// void IrCodeModule::loopStateVerify()
// {
// 	if(rec->decode())
// 	{
// 		rec->resume();
// 		//skip invalid codes
// 		if(rec->decodedIRData.protocol == 0 || rec->decodedIRData.address == 0 || rec->decodedIRData.numberOfBits == 0) return;
// 		//skip if code is repeated (like holding button)
// 		if(rec->decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT) return;

// 		if(rec->decodedIRData.protocol != _data->protocol)
// 		{
// 			logErrorP("Protokoll unterschiedlich");
// 			_state = 0;
// 			_funcState = FuncState::Error;
// 			return;
// 		}
// 		if(rec->decodedIRData.address != _data->address)
// 		{
// 			logErrorP("Adresse unterschiedlich");
// 			_state = 0;
// 			_funcState = FuncState::Error;
// 			return;
// 		}
// 		if(rec->decodedIRData.numberOfBits != _data->numberOfBits)
// 		{
// 			logErrorP("NumberOfBits unterschiedlich");
// 			_state = 0;
// 			_funcState = FuncState::Error;
// 			return;
// 		}
// 		if(rec->decodedIRData.command != _data->command)
// 		{
// 			logErrorP("Command unterschiedlich");
// 			_state = 0;
// 			_funcState = FuncState::Error;
// 			return;
// 		}

// 		this->write(_index, _data);
// 		this->print(_data, _index);
// 		logInfoP("IR Code erhalten und gespeichert");

// 		delete _data;

// 		_state = 0;
// 		_funcState = FuncState::Success;
// 	}
// }

// void IrCodeModule::handleCode(bool state)
// {
// 	if(rec->decodedIRData.protocol == 0 && rec->decodedIRData.address == 0) return;

// 	print(&rec->decodedIRData, -1);

// 	lastCode = millis();
// 	if(lastCode == 0) lastCode++;
// 	bool flag = false;

// 	File f;
// 	if(!LittleFS.exists("/ircodes.txt"))
// 	{
// 		logDebugP("Gedrückt (keine txt)");
// 		return;
// 	} else {
// 		f = LittleFS.open("/ircodes.txt", "r+");
// 		if(f.size() < CODE_COUNT*CODE_SIZE)
// 		{
// 			f.seek(f.size());
// 			for(int i = 0; i < (CODE_COUNT*CODE_SIZE - f.size()); i++)
// 				f.write((uint8_t)0x00);
// 		}
// 	}

// 	for(int i = 0; i < CODE_COUNT; i++)
// 	{
// 		f.seek(i * CODE_SIZE);
// 		uint8_t *data = new uint8_t[5];
// 		f.readBytes((char *)data, 5);
// 		bool compared = compare(i, data);
// 		delete[] data;

// 		if(compared)
// 		{
// 			_index = i;
// 			logDebugP("Gedrückt (Index %i)", i);
// 			int type = ParamIR_inOutTypeIndex(i);
// 			if(type == 1)
// 			{
// 				f.close();
// 				executeCode(state);
// 				return;
// 			}
// 			logDebugP("Aktion (keine)");
// 			flag = true;
// 		}
// 	}

// 	f.close();
// 	if(flag) return;

// 	logDebugP("Gedrückt (Unbekannt)");
// }

// void IrCodeModule::executeCode(bool state)
// {
// 	int type = ParamIR_inTypeIndex(_index);
// 	switch(type)
// 	{
// 		case PT_ir_receive_virtual1:
// 		{
// 			logDebugP("Aktion (Schaltfunktion 1 %s)", state ? "AN" : "AUS");
// 			executeSwitch(state, 0);
// 			break;
// 		}

// 		case PT_ir_receive_virtual2:
// 		{
// 			logDebugP("Aktion (Schaltfunktion 2 %s)", state ? "AN" : "AUS");
// 			executeSwitch(state, 1);
// 			break;
// 		}

// 		case PT_ir_receive_color:
// 		{
// 			if(!state) return;
// 			logDebugP("Aktion (Farbe)");
// 			executeColor();
// 			break;
// 		}

// 		default:
// 			logDebugP("Aktion (Unbekannt %i)", type);
// 			break;
// 	}
// }

// void IrCodeModule::executeSwitch(bool state, uint8_t index)
// {
// 	logDebugP("Execute Virtual Switch");
// 	uint8_t btnIndex = ParamIR_inBtnNumberIndex(_index)-1;

// 	uint16_t koNum = BTN_Templ_KoOffset + BTN_Templ_KoBlockSize * btnIndex + 1 + index;
// 	if(koNum > MAIN_MaxKoNumber) {
// 		logErrorP("Virtual Switch ungültige KO-Nr. %i", koNum);
// 		return;
// 	}
// 	logDebugP("Virtual Switch %i -> %s (KO: %i)", btnIndex+1, state ? "An" : "Aus", koNum);
// 	knx.getGroupObject(koNum).value(state, DPT_Switch);
// }

// void IrCodeModule::executeColor()
// {
// 	logDebugP("Execute Color");
// 	uint8_t *color;
// 	if(ParamIR_inColorTypeIndex(_index))
// 		color = ParamIR_inColor_HSVIndex(_index);
// 	else
// 		color = ParamIR_inColor_RGBIndex(_index);
// 	logDebugP("#%.2X%.2X%.2X", color[0], color[1], color[2]);
// 	uint32_t xcolor = color[2];
// 	xcolor |= color[1] << 8;
// 	xcolor |= color[0] << 16;
// 	KoIR_co_n1Index(_index).value(xcolor, DPT_Colour_RGB);
// }

// void IrCodeModule::sendCode(uint8_t index)
// {
// 	logDebugP("Sending %i", index);
// 	IRData *data = this->read(index);
// 	this->print(data, index);
// 	rec->disableIRIn();
// 	send->write(data);
// 	rec->enableIRIn();
// }

// void IrCodeModule::print(IRData *data, int16_t index)
// {
// 	if(index >= 0)
// 		logDebugP("IR Code %i", index);
// 	logIndentUp();
// 	logDebugP("Protokoll %.2X", data->protocol);
// 	logDebugP("Address %.4X", data->address);
// 	logDebugP("Command %.4X", data->command);
// 	logDebugP("Number %.4X", data->numberOfBits);
// 	logDebugP("Extra %.4X", data->extra);
// 	logDebugP("Flags %.2X", data->flags);
// 	logIndentDown();
// }

// bool IrCodeModule::compare(uint8_t index, uint8_t *data)
// {
// 	if(rec->decodedIRData.protocol != (decode_type_t)data[0]) return false;

// 	int temp = data[1] << 8;
// 	temp = temp | data[2];
// 	if(rec->decodedIRData.address != temp) return false;

// 	temp = data[3] << 8;
// 	temp = temp | data[4];
// 	if(rec->decodedIRData.command != temp) return false;

// 	return true;
// }

// IRData* IrCodeModule::read(uint8_t index)
// {
// 	File f;
// 	IRData *data = new IRData();
// 	data->protocol = decode_type_t::UNKNOWN;

// 	if(!LittleFS.exists("/ircodes.txt"))
// 	{
// 		logErrorP("ircodes file doesn't exist");
// 		return data;
// 	} else {
// 		f = LittleFS.open("/ircodes.txt", "r+");
// 		if(f.size() < CODE_COUNT*CODE_SIZE)
// 		{
// 			f.seek(f.size());
// 			for(int i = 0; i < (CODE_COUNT*CODE_SIZE - f.size()); i++)
// 				f.write((uint8_t)0x00);
// 		}
// 	}

// 	f.seek(index * CODE_SIZE);
// 	data->protocol = (decode_type_t)f.read();
// 	int temp = f.read() << 8;
// 	data->address = temp | f.read();

// 	temp = f.read() << 8;
// 	data->command = temp | f.read();
// 	temp = f.read() << 8;
// 	data->numberOfBits = temp | f.read();
// 	temp = f.read() << 8;
// 	data->extra = temp | f.read();
// 	return data;
// }

// void IrCodeModule::write(uint8_t index, IRData *data)
// {
// 	File f;
// 	if(!LittleFS.exists("/ircodes.txt"))
// 	{
// 		f = LittleFS.open("/ircodes.txt", "w+");
// 		for(int i = 0; i < CODE_COUNT*CODE_SIZE; i++)
// 			f.write((uint8_t)0x00);
// 		f.flush();
// 	} else {
// 		f = LittleFS.open("/ircodes.txt", "r+");
// 		if(f.size() < CODE_COUNT*CODE_SIZE)
// 		{
// 			f.seek(f.size());
// 			for(int i = 0; i < (CODE_COUNT*CODE_SIZE - f.size()); i++)
// 				f.write((uint8_t)0x00);
// 		}
// 	}

// 	f.seek(index * CODE_SIZE);
// 	f.write((uint8_t)data->protocol);
// 	f.write((uint8_t)(data->address >> 8));
// 	f.write((uint8_t)(data->address & 0xFF));
// 	f.write((uint8_t)(data->command >> 8));
// 	f.write((uint8_t)(data->command & 0xFF));
// 	f.write((uint8_t)(data->numberOfBits >> 8));
// 	f.write((uint8_t)(data->numberOfBits & 0xFF));
// 	f.write((uint8_t)(data->extra >> 8));
// 	f.write((uint8_t)(data->extra & 0xFF));

// 	f.flush();
// 	f.close();
// }

// void IrCodeModule::processInputKo(GroupObject& iKo)
// {
// 	return;
//     uint8_t index = floor((iKo.asap() - 1) / 2);
// 	logDebugP("got KO %i is index %i", iKo.asap(), index);

// 	int type = ParamIR_inOutTypeIndex(index);
// 	if(type != PT_inout_out)
// 	{
// 		logDebugP("KO ist nicht zum Empfangen gedacht");
// 		return;
// 	}

// 	type = ParamIR_outTypeIndex(index);
// 	switch(type)
// 	{
// 		case PT_ir_send_switch: //Switch
// 		{
// 			koHandleSwitch(iKo, index);
// 			break;
// 		}

// 		case PT_ir_send_value: //Value
// 		{
// 			koHandleValue(iKo, index);
// 			break;
// 		}

// 		case PT_ir_send_scene: //Scene
// 		{
// 			koHandleScene(iKo, index);
// 			break;
// 		}
// 	}
// }

// void IrCodeModule::koHandleSwitch(GroupObject &iKo, uint8_t index)
// {
// 	logDebugP("Switch");
// 	int stype = ParamIR_outSwitchIndex(index);
// 	switch(stype)
// 	{
// 		case PT_switch_type_out_any: //Always
// 		{
// 			logDebugP("Send Always");
// 			sendCode(index);
// 			break;
// 		}

// 		case PT_switch_type_out_on: //On
// 		{
// 			logDebugP("Send On");
// 			if(iKo.value(DPT_Switch))
// 				sendCode(index);
// 			break;
// 		}

// 		case PT_switch_type_out_off: //Off
// 		{
// 			logDebugP("Send Off");
// 			if(!iKo.value(DPT_Switch))
// 				sendCode(index);
// 			break;
// 		}
// 	}
// }

// void IrCodeModule::koHandleValue(GroupObject &iKo, uint8_t index)
// {
// 	logDebugP("Value");
// 	int svalue = ParamIR_outValueIndex(index);
// 	int ivalue = (uint8_t)iKo.value(DPT_Scaling);
// 	if(svalue == ivalue)
// 		sendCode(index);
// }

// void IrCodeModule::koHandleScene(GroupObject &iKo, uint8_t index)
// {
// 	logDebugP("Scene");
// 	uint8_t ivalue = (uint8_t)iKo.value(DPT_SceneNumber);
// 	if(ParamIR_outSceneActive1Index(index))
// 	{
// 		int svalue = ParamIR_outScene1Index(index) - 1;
// 		if(svalue == ivalue)
// 		{
// 			sendCode(index);
// 			return;
// 		}
// 	}
// 	if(ParamIR_outSceneActive2Index(index))
// 	{
// 		int svalue = ParamIR_outScene2Index(index) - 1;
// 		if(svalue == ivalue)
// 		{
// 			sendCode(index);
// 			return;
// 		}
// 	}
// 	if(ParamIR_outSceneActive3Index(index))
// 	{
// 		int svalue = ParamIR_outScene3Index(index) - 1;
// 		if(svalue == ivalue)
// 		{
// 			sendCode(index);
// 			return;
// 		}
// 	}
// 	if(ParamIR_outSceneActive4Index(index))
// 	{
// 		int svalue = ParamIR_outScene4Index(index) - 1;
// 		if(svalue == ivalue)
// 		{
// 			sendCode(index);
// 			return;
// 		}
// 	}
// }

// bool IrCodeModule::processFunctionProperty(uint8_t objectIndex, uint8_t propertyId, uint8_t length, uint8_t *data, uint8_t *resultData, uint8_t &resultLength)
// {
// 	if(objectIndex != 0xA0 || propertyId != 0) return false;

// 	switch(data[0])
// 	{
// 		case 0x01:
// 		{
// 			logDebugP("Lerne index %i", data[1]);
// 			logDebugP("Bitte IR Code senden");
// 			_index = data[1];
// 			_state = 1;
// 			_funcState = FuncState::Receive;
// 			resultData[0] = 0x00;
// 			resultLength = 1;
// 			break;
// 		}

// 		case 0x02:
// 		{
// 			logDebugP("Entferne index %i", data[1]);
// 			resultData[0] = 0x00;
// 			resultLength = 1;
// 			_data = new IRData();
// 			_data->protocol = decode_type_t::UNKNOWN;
// 			_data->address = 0;
// 			_data->command = 0;
// 			this->write(data[1], _data);
// 			delete _data;
// 			break;
// 		}

// 		default:
// 		{
// 			logErrorP("Unbekanntes Kommando");
// 			resultData[0] = 0xFF;
// 			resultLength = 1;
// 			break;
// 		}
// 	}

// 	return true;
// }

// bool IrCodeModule::processFunctionPropertyState(uint8_t objectIndex, uint8_t propertyId, uint8_t length, uint8_t *data, uint8_t *resultData, uint8_t &resultLength)
// {
// 	if(objectIndex == 0xA0 && propertyId == 0) return false;

// 	switch(data[0])
// 	{
// 		case 0x01:
// 		{
// 			resultData[0] = (uint8_t)_funcState;
// 			resultLength = 1;
// 			return true;
// 		}
// 	}

// 	return false;
// }

// bool IrCodeModule::processCommand(const std::string cmd, bool diagnoseKo)
// {
// 	if(diagnoseKo) return false;

// 	std::size_t pos = cmd.find(' ');
// 	std::string command;
// 	std::string arg;
// 	if(pos != -1)
// 	{
// 		command = cmd.substr(0, pos);
// 		arg = cmd.substr(pos+1, cmd.length() - pos - 1);
// 	} else {
// 		command = cmd;
// 		arg = "";
// 	}

// 	if(command == "learnIR" && arg.length() > 0)
// 	{
// 		_index = std::stoi(arg);
// 		if(_index == 0)
// 		{
// 			logErrorP("Code 0 wird nicht unterstuetzt");
// 			return true;
// 		}
// 		logInfoP("Lerne code %i (index %i)", _index, --_index);
// 		logInfoP("Bitte IR Code senden");
// 		rec->resume();
// 		_state = 1;
// 		return true;
// 	}
// 	if(command == "deleteIR" && arg.length() > 0)
// 	{
// 		_index = std::stoi(arg);
// 		if(_index == 0)
// 		{
// 			logErrorP("Code 0 wird nicht unterstuetzt");
// 			return true;
// 		}
// 		logInfoP("Entferne code %i (index %i)", _index, --_index);
// 		_data = new IRData();
// 		_data->protocol = decode_type_t::UNKNOWN;
// 		_data->address = 0;
// 		_data->command = 0;
// 		this->write(_index, _data);
// 		delete _data;
// 		return true;
// 	}

// 	return false;
// }

// void IrCodeModule::showHelp()
// {
// 	openknx.console.printHelpLine("learnIR <00>", "Learn a new IR-Code");
//     openknx.console.printHelpLine("deleteIR <00>", "Delete an existing IR-Code");
// }

// IrCodeModule openknxIrCodeModule;
// extern IrCodeModule openknxIrCodeModule;