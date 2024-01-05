
#include "SerialMessager.hpp"

SerialMessager::SerialMessager() {
  heading_esp = "esp-";
  color = "color:";     //... color json
  waiter = "waiter:";   //... desk rfid
  attic = "attic:";     // ... 0~1
  log = "log_request";  // ... log json
  automatic_attic = "automatic_attic:";
  heading_ard = "ard-";
}
String SerialMessager::send(String message) {
  return heading_esp + message;
}

// takes one line of serial port.
String SerialMessager::read(String _read) {
  if (_read.indexOf(heading_ard) != -1) {
    // probably a log line received.
    if (_read.indexOf(heading_ard + log) != -1) {
      String sub = _read.substring(heading_ard.length() + log.length() + 1);
      return sub;
    }
  }
  return "no return";
}

String SerialMessager::readLog(String _log) {
  if (_log.indexOf(heading_ard + log) != -1) {
    String sub = _log.substring(heading_ard.length() + log.length());
    return sub;
  } else
    return "";
}