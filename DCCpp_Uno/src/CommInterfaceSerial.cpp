/*
 * CommInterfaceSerial.cpp
 *
 *  Created on: Mar 3, 2017
 *      Author: mdunston
 */

#include <Arduino.h>

#include "Config.h"
#include "CommInterfaceSerial.h"
#include "SerialCommand.h"

HardwareSerialInterface::HardwareSerialInterface(HardwareSerial &serial, long baud) : serialStream(serial), baud(baud), buffer(""), inCommandPayload(false) {
	serial.begin(baud);
	serial.flush();
}

void HardwareSerialInterface::process() {
	while(serialStream.available()) {
		char ch = serialStream.read();
		if (ch == '<') {
			inCommandPayload = true;
			buffer = "";
		} else if (ch == '>') {
			SerialCommand::parse(buffer.c_str());
			buffer = "";
			inCommandPayload = false;
		} else if(inCommandPayload) {
			buffer += ch;
		}
	}
}

void HardwareSerialInterface::showConfiguration() {
	DEBUG_INTERFACE.print("Hardware Serial - Speed:");
	DEBUG_INTERFACE.println(baud);
}

void HardwareSerialInterface::showInitInfo() {
	CommManager::printf("<N0:SERIAL>");
}

void HardwareSerialInterface::send(const char *buf) {
	serialStream.print(buf);
}
