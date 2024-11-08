#include <Bluepad32.h>

#define TXD_PIN 17
#define RXD_PIN 16
#define LED 2

ControllerPtr myControllers[BP32_MAX_GAMEPADS];

const uint8_t ALLOWED_BT_ADDRESS[] = {0x84, 0x30, 0x95, 0x39, 0x23, 0xA5};

bool isBTAddressAllowed(const uint8_t* btAddr) {
    for (int i = 0; i < 6; i++) {
        if (btAddr[i] != ALLOWED_BT_ADDRESS[i]) {
            return false;
        }
    }
    return true;
}

uint8_t processThrottle(uint16_t throttleValue) {
    // Simple mapping to 1 bit for throttle control
    return throttleValue > 512 ? 1 : 0;  // 1 bit for throttle
}

void onConnectedController(ControllerPtr ctl) {
    ControllerProperties properties = ctl->getProperties();
    const uint8_t* btAddr = properties.btaddr;
    
    Serial.printf("Controller attempting to connect - Bluetooth address: %02X:%02X:%02X:%02X:%02X:%02X\n",
                  btAddr[0], btAddr[1], btAddr[2], btAddr[3], btAddr[4], btAddr[5]);

    if (!isBTAddressAllowed(btAddr)) {
        Serial.println("Controller not allowed. Disconnecting.");
        ctl->disconnect();
        return;
    }

    bool foundEmptySlot = false;
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myControllers[i] == nullptr) {
            Serial.printf("CALLBACK: Allowed controller connected, index=%d\n", i);
            Serial.printf("Controller model: %s, VID=0x%04x, PID=0x%04x\n", 
                          ctl->getModelName().c_str(), properties.vendor_id, properties.product_id);
            myControllers[i] = ctl;
            foundEmptySlot = true;
            break;
        }
    }
    
    if (!foundEmptySlot) {
        Serial.println("CALLBACK: Controller connected, but could not find empty slot. Disconnecting.");
        ctl->disconnect();
    }
}

void onDisconnectedController(ControllerPtr ctl) {
    bool foundController = false;

    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myControllers[i] == ctl) {
            Serial.printf("CALLBACK: Controller disconnected from index=%d\n", i);
            myControllers[i] = nullptr;
            foundController = true;
            break;
        }
    }

    if (!foundController) {
        Serial.println("CALLBACK: Controller disconnected, but not found in myControllers");
    }
}

void dumpGamepad(ControllerPtr ctl) {
    Serial.printf(
        "axis X: %4d, axis Y: %4d, throttle: %4d, buttons: 0x%04x, misc: 0x%02x, dpad: 0x%02x\n",
        ctl->axisX(),       // Joystick X axis
        ctl->axisY(),       // Joystick Y axis
        ctl->throttle(),    // Throttle
        ctl->buttons(),     // Button bitmask
        ctl->miscButtons(), // Misc button bitmask
        ctl->dpad()         // Dpad
    );
}


void processGamepad(ControllerPtr ctl) {
    dumpGamepad(ctl);  // Print out control values

    // Prepare to send data via UART
    int16_t x = ctl->axisX();
    int16_t y = -ctl->axisY();  
    uint8_t throttle = processThrottle(ctl->throttle());
    
    uint8_t command = 0x00;

    // Define command regions based on x and y values
    if (y > 256 && abs(x) < 128) {              
        command |= 0x01;
        Serial.println("FORWARD");
    } else if (y < -384 && abs(x) < 256) {    
        command |= 0x02;
        Serial.println("REVERSE");
    } else if (y > 128 && x < -128 && x > -416) { // Left Turn 1 (Gentle Left, 0.8 diff)
        command |= 0x03;
        Serial.println("FORWARD GENTLE LEFT TURN");
    } else if (y > 128 && x > 128 && x < 416) {   // Right Turn 1 (Gentle Right, 0.8 diff)
        command |= 0x04;
        Serial.println("FORWARD GENTLE RIGHT TURN");
    } else if (y > 128 && x < -416) {             // Left Turn 2 (Moderate Left, 0.5 diff)
        command |= 0x05;
        Serial.println("FORWARD MODERATE LEFT TURN");
    } else if (y > 128 && x > 416) {              // Right Turn 2 (Moderate Right, 0.5 diff)
        command |= 0x06;
        Serial.println("FORWARD MODERATE RIGHT TURN");
    } else if (y > -384 && x < -384) {              // Left Turn 3 (Sharp Left, 0.3 diff)
        command |= 0x07;
        Serial.println("FORWARD SHARP LEFT TURN");
    } else if (y > -384 && x > 384) {               // Right Turn 3 (Sharp Right, 0.3 diff)
        command |= 0x08;
        Serial.println("FORWARD SHARP RIGHT TURN");
    } else if (ctl->l1()) {                       // Hard Left Curve
        command |= 0x09;
        Serial.println("LEFT CURVE");
    } else if (ctl->r1()) {                       // Hard Right Curve
        command |= 0x0A;
        Serial.println("RIGHT CURVE");
    } else if (ctl->dpad() == DPAD_LEFT) {        // Spin Left
        command |= 0x0B;
        Serial.println("SPIN LEFT ON THE SPOT");
    } else if (ctl->dpad() == DPAD_RIGHT) {       // Spin Right
        command |= 0x0C;
        Serial.println("SPIN RIGHT ON THE SPOT");
    } else if (ctl->a()) {                        // Run End
        command |= 0x0D;
        Serial.println("RUN END");
    } else {
        Serial.println("NEUTRAL");
    }

    // Include throttle in command if active
    if (throttle) {
        command |= (1 << 4);
        Serial.println("Throttle pressed");
    }

    // Send the command via UART
    Serial2.write(command);
    Serial.printf("Command sent: 0x%02X\n", command);
}

void processControllers() {
    for (auto myController : myControllers) {
        if (myController && myController->isConnected()) {  // Removed hasData() check
            if (myController->isGamepad()) {
                processGamepad(myController);
            } else {
                Serial.println("Unsupported controller");
            }
        }
    }
}

void setup() {
    pinMode(LED, OUTPUT);
    Serial.begin(115200);
    Serial.printf("Firmware: %s\n", BP32.firmwareVersion());
    const uint8_t* addr = BP32.localBdAddress();
    Serial.printf("BD Addr: %2X:%2X:%2X:%2X:%2X:%2X\n", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
    Serial2.begin(9600, SERIAL_8N1, RXD_PIN, TXD_PIN);

    BP32.setup(&onConnectedController, &onDisconnectedController);
    BP32.forgetBluetoothKeys();

    // Removed enableVirtualDevice as it doesn't exist
    BP32.enableVirtualDevice(false);
}

void loop() {
    BP32.update();  // Removed assignment to dataUpdated
    processControllers();
    delay(150);
}
