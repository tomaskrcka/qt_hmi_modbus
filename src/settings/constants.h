#ifndef CONSTANTS_H
#define CONSTANTS_H

constexpr auto USER_PATH = "/jd/users";
constexpr auto USER_CAN_PRESS = "/canPress";

// TODO
constexpr auto TANK_SAVE_PRESS = "";


// registers
enum RegistersRead {
    oiRegister40001 = 0,
    oiPressure40002,
    oiTime1_40003,
    oiTime2_40004,
    oiPressure40005,
};

enum RegistersWrite {
    iiRegister40006 = 5
};

enum RegisterRead40001_Bits {
    oxPressingInProgress = 0,   // the pressing is in progress
    oxPressingOK,               // pressing finished OK
    oxPressingNOK,              // not used atm
    oxTakePicture,              // take a picture
    oxDefaultPosition,          // ready for the pressing
    oxService,                  // service screen
    oxSecurity,                 // emergency
    oxPushBtns,                  // push and hold buttons
    oxManualBtn                 // manual btn
};

enum RegisterWrite40006_Bits {
    ixReady,
    ixLoginScreen
};


#endif // CONSTANTS_H
