function readIrCode(device, online, progress, context) {
    progress.setText("Taste "  + context.Channel + ": Auslesen des letzten empfangen IR-Code...");
    online.connect();

    // var resp = online.invokeFunctionProperty(0xA0, 2, [1, context.Channel - 1]);
    // if (resp[0] != 0) {
    //     throw new Error("Taste "  + context.Channel + ": Es ist ein unbekannter Fehler aufgetreten!");
    // }

/*    var counterSigned = resp[1];
    var counter = (resp[2] << 24) | (resp[3] << 16) | (resp[4] << 8) | resp[5];
    var referenceSigned = resp[6];
    var reference = (resp[7] << 24) | (resp[8] << 16) | (resp[9] << 8) | resp[10];*/

    device.getParameterByName("IR_c" + context.Channel + "CodeProtocol").value = 200;
    device.getParameterByName("IR_c" + context.Channel + "CodeAddress").value = 65001;
    device.getParameterByName("IR_c" + context.Channel + "CodeCommand").value = 65002;
    device.getParameterByName("IR_c" + context.Channel + "CodeBits").value = 65003;
    device.getParameterByName("IR_c" + context.Channel + "CodeExtra").value = 65004;

    online.disconnect();
    progress.setText("Taste "  + context.Channel + ": IR-Code wurde ausgelesen.");
}

function testIrCode(device, online, progress, context) {
    progress.setText("Taste "  + context.Channel + ": Teste IR-Code...");
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
    progress.setText("Taste "  + context.Channel + ": Test des IR-Codes wurde abgeschlossen.");
}

