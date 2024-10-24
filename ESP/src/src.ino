#include <Bluepad32.h>

#define TXD_PIN 17
#define RXD_PIN 16
#define LED 2

ControllerPtr myControllers[BP32_MAX_GAMEPADS];

const uint8_t ALLOWED_BT_ADDRESS[] = {0x44, 0x16, 0x22, 0x94, 0x4E, 0xCD};

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

uint8_t processSpin(ControllerPtr ctl) {
    // Check if any misc button is pressed for spin
    return ctl->miscButtons() != 0 ? 1 : 0;  // Spin if misc buttons pressed
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
        "axis X: %4d, axis Y: %4d, throttle: %4d, buttons: 0x%04x, misc: 0x%02x\n",
        ctl->axisX(),       // Joystick X axis
        ctl->axisY(),       // Joystick Y axis
        ctl->throttle(),    // Throttle
        ctl->buttons(),     // Button bitmask
        ctl->miscButtons()  // Misc button bitmask
    );
}

void processGamepad(ControllerPtr ctl) {
    dumpGamepad(ctl);  // Print out control values
    
    // Prepare to send data via UART
    int16_t x = ctl->axisX();
    int16_t y = ctl->axisY();
    uint8_t throttle = processThrottle(ctl->throttle());
    uint8_t spin = processSpin(ctl);
    const int16_t threshold = 20;
    
    uint8_t command = 0x00;

    // Processing of data is done here (Basic 4 movement first)
    if (x > threshold) {
      command |= 0x01;
      Serial.println("RIGHT");
    } else if (x < -threshold) {
      command |= 0x02;
      Serial.println("LEFT");
    } else if (y > threshold) {
      command |= 0x03;
      Serial.println("UP");
    } else if (y < -threshold) {
      command |= 0x04;
      Serial.println("DOWN");
    } else {
      Serial.println("NEUTRAL");
    }

    if (throttle) {
      command |= (1 << 4);
      Serial.println("Throttle pressed");
    }

    if (spin) {
      command |= (1 << 5);
      Serial.println("Spin pressed");
    }

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
