#if !defined(SERIAL_MESSAGER)
#define SERIAL_MESSAGER

#include "WString.h"
class SerialMessager {
public:
   String heading_esp;
   String heading_ard;
   String color;   //... color json
   String waiter;  //... desk rfid
   String attic;   // ... 0~1
   String automatic_attic; // 0~1
   String log;     // ... log json
public:
  SerialMessager();
   String send(String);
   String read(String);
   String readLog(String);
};

#endif  // SERIAL_MESSAGER
