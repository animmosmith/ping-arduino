static char _debug_buffer[100];

#include <pingmessage_all.h>
#include "parser_ping.h"

HardwareSerial& pingSerial = Serial;
HardwareSerial& debugSerial = Serial3;

#define debug(fmt, args ...)  do {sprintf(_debug_buffer, "[%s:%d]: " fmt "\n\r", __FUNCTION__, __LINE__, ## args); debugSerial.print(_debug_buffer);} while(0)

void setup() {
  pingSerial.begin(115200);
  debugSerial.begin(115200);
  pinMode(13, OUTPUT);
  debugSerial.println("sup");
}

void loop() {
    debugSerial.println("sup1");

  static PingParser p;
  static uint8_t counter = 0;
  static Ping1DNamespace::msg_ping1D_id requestIds[] = {
    Ping1DNamespace::Voltage_5,
    Ping1DNamespace::Processor_temperature,
    Ping1DNamespace::Fw_version,
    Ping1DNamespace::Profile,
  };
  static int requestIdsSize = sizeof(requestIds)/sizeof(requestIds[0]);
  counter++;
  counter = counter%requestIdsSize;

  ping_msg_ping1D_empty m;
  m.set_id(requestIds[counter]);
  m.updateChecksum();
  pingSerial.write(m.msgData, m.msgDataLength());
  delay(300);

  while(pingSerial.available()) {
    if(p.parseByte(pingSerial.read()) == PingParser::NEW_MESSAGE) {
      switch(p.rxMsg.message_id()) {
//
//        case Ping1DNamespace::Voltage_5: {
//          ping_msg_ping1D_pcb_temperature m(p.rxMsg);
//          debug("> Pcb voltage: %d",m.temp());
//          break;
//        }
//
//        case Ping1DNamespace::Nack: {
//          ping_msg_ping1D_nack m(p.rxMsg);
//          debug("> Nack text: %s", m.err_msg());
//          break;
//        }
//
//        case Ping1DNamespace::Ascii_text: {
//          ping_msg_ping1D_ascii_text m(p.rxMsg);
//          debug("> Ascii text: %s", m.msg());
//          break;
//        }
//
//        case Ping1DNamespace::Processor_temperature: {
//          ping_msg_ping1D_processor_temperature m(p.rxMsg);
//          debug("> Pcb temp: %d", m.temp());
//          break;
//        }
//
//        case Ping1DNamespace::Fw_version: {
//          ping_msg_ping1D_fw_version m(p.rxMsg);
//          debug("> type: %d", m.device_type());
//          debug("> model: %d", m.device_model());
//          debug("> fw version: %d.%d", m.fw_version_major(), m.fw_version_minor());
//          break;
//        }
//
//        case Ping1DNamespace::Profile: {
//          ping_msg_ping1D_profile m(p.rxMsg);
//          debug("> distance: %d", m.distance());
//          debug("> confidence: %d", m.confidence());
//          debug("> pulse_usec: %d", m.pulse_usec());
//          debug("> ping_number: %d", m.ping_number());
//          debug("> start_mm: %d", m.start_mm());
//          debug("> length_mm: %d", m.length_mm());
//          debug("> gain_index: %d", m.gain_index());
//          debug("> num_points: %d", m.num_points());
//          debug("> gain_index: %d", m.gain_index());
//          for(int i =0; i < m.num_points(); i++) {
//            debug("> data: %d", m.data()[i]);
//          }
//          break;
//        }

        default:
          debug("> id: %d\t Length: %d\t parsed: %d\t errors: %u", p.rxMsg.message_id(), p.rxMsg.payload_length(), p.parsed, p.errors);
          break;
      }

      digitalWrite(13, !digitalRead(13));
    }
  }
  while(1);
  //delay(1000);
}