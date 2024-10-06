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
    progress.setText("Taste " + context.Channel + ": Teste IR-Code...");
    online.connect();

    var protocol = device.getParameterByName("IR_c" + context.Channel + "CodeProtocol").value;
    var address = device.getParameterByName("IR_c" + context.Channel + "CodeAddress").value;
    var command = device.getParameterByName("IR_c" + context.Channel + "CodeCommand").value;
    var bits = device.getParameterByName("IR_c" + context.Channel + "CodeBits").value;
    var extra = device.getParameterByName("IR_c" + context.Channel + "CodeExtra").value;

    // var resp = online.invokeFunctionProperty(0xA0, 2, [1, context.Channel - 1]);
    // if (resp[0] != 0) {
    //     throw new Error("Taste "  + context.Channel + ": Es ist ein unbekannter Fehler aufgetreten!");
    // }

    online.disconnect();
    progress.setText("Taste " + context.Channel + ": Test des IR-Codes wurde abgeschlossen.");
}

