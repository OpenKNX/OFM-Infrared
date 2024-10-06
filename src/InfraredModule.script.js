function readIrCode(device, online, progress, context) {
    progress.setText("Taste " + context.Channel + ": Auslesen des letzten empfangen IR-Code...");
    online.connect();

    var resp = online.invokeFunctionProperty(0xA0, 0, [1]);
    if (resp[0] == 1) {
        throw new Error("Taste " + context.Channel + ": Es wurde noch keine IR-Code empfangen!");
    }
    else if (resp[0] != 0) {
        throw new Error("Taste " + context.Channel + ": Es ist ein unbekannter Fehler aufgetreten!");
    }

    var protocol = resp[1];
    var address = (resp[2] << 8) | resp[3];
    var command = (resp[4] << 8) | resp[5];
    var bits = (resp[6] << 8) | resp[7];
    var extra = (resp[8] << 8) | resp[9];

    device.getParameterByName("IR_c" + context.Channel + "CodeProtocol").value = protocol;
    device.getParameterByName("IR_c" + context.Channel + "CodeAddress").value = address;
    device.getParameterByName("IR_c" + context.Channel + "CodeCommand").value = command;
    device.getParameterByName("IR_c" + context.Channel + "CodeBits").value = bits;
    device.getParameterByName("IR_c" + context.Channel + "CodeExtra").value = extra;

    online.disconnect();
    progress.setText("Taste " + context.Channel + ": IR-Code wurde ausgelesen.");
}

function testIrCode(device, online, progress, context) {
    progress.setText("Taste " + context.Channel + ": Sende IR-Code...");
    online.connect();

    var protocol = device.getParameterByName("IR_c" + context.Channel + "CodeProtocol").value;
    var address = device.getParameterByName("IR_c" + context.Channel + "CodeAddress").value;
    var command = device.getParameterByName("IR_c" + context.Channel + "CodeCommand").value;
    var bits = device.getParameterByName("IR_c" + context.Channel + "CodeBits").value;
    var extra = device.getParameterByName("IR_c" + context.Channel + "CodeExtra").value;

    if (protocol == 0 || address == 0 || command == 0 || bits == 0) {
        throw new Error("Taste " + context.Channel + ": Der IR-Code ist ungültig und konnte nicht gesendet werden.");
    }

    data = [];
    data[0] = 2;
    data[1] = protocol;
    data[2] = (address >> 8) & 0xFF;
    data[3] = address & 0xFF;
    data[4] = (command >> 8) & 0xFF;
    data[5] = command & 0xFF;
    data[6] = (bits >> 8) & 0xFF;
    data[7] = bits & 0xFF;
    data[8] = (extra >> 8) & 0xFF;
    data[9] = extra & 0xFF;

    var resp = online.invokeFunctionProperty(0xA0, 0, data);
    if (resp[0] != 0) {
        throw new Error("Taste " + context.Channel + ": Es ist ein unbekannter Fehler aufgetreten!");
    }

    online.disconnect();
    progress.setText("Taste " + context.Channel + ": IR-Code wurde erfolgreich gesendet.");
}

