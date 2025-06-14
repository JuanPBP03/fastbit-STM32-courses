#include <avr/io.h>
#include <avr/interrupt.h>
#include <Arduino.h>

// === Pin & Command Definitions ===
#define LED_PIN 9
#define ACK  0xF5
#define NACK 0xA5

#define COMMAND_LED_CTRL    0x50
#define COMMAND_ID_READ     0x54

const char board_id[] = "ARDUINOUNO";

// === SPI State Machine ===
enum SPIState {
  SPI_WAIT_CMD,
  SPI_ACK,
  SPI_WAIT_ARG1,
  SPI_WAIT_ARG2,
  SPI_SENDING_ID,
  SPI_IDLE
};

volatile SPIState state = SPI_WAIT_CMD;

volatile uint8_t command = 0;
volatile uint8_t arg1 = 0;
volatile uint8_t arg2 = 0;
volatile uint8_t responseIndex = 0;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Set MISO as output
  pinMode(MISO, OUTPUT);

  // Enable SPI in slave mode, SPI interrupt
  SPCR |= _BV(SPE) | _BV(SPIE);
  SPDR = 0x00;  // preload response

  Serial.println("SPI Slave ready.");
}

// === SPI ISR ===
ISR(SPI_STC_vect) {
  uint8_t data = SPDR;

  switch (state) {
    case SPI_WAIT_CMD:
      command = data;
      if (command == COMMAND_LED_CTRL || command == COMMAND_ID_READ) {
        SPDR = ACK;
        state = SPI_ACK;
      } else {
        SPDR = NACK;
        state = SPI_WAIT_CMD;
      }
      break;

    case SPI_ACK:
      SPDR = 0x00;  // dummy byte back
      if (command == COMMAND_LED_CTRL) {
        state = SPI_WAIT_ARG1;  // expecting pin
      } else if (command == COMMAND_ID_READ) {
        responseIndex = 0;
        SPDR = board_id[responseIndex++];
        state = SPI_SENDING_ID;
      }
      break;

    case SPI_WAIT_ARG1:
      arg1 = data;  // pin
      SPDR = 0x00;
      state = SPI_WAIT_ARG2;
      break;

    case SPI_WAIT_ARG2:
      arg2 = data;  // value
      digitalWrite(arg1, arg2 ? HIGH : LOW);
      Serial.print("LED ");
      Serial.print(arg1);
      Serial.print(" = ");
      Serial.println(arg2);
      SPDR = 0x00;
      state = SPI_WAIT_CMD;
      break;

    case SPI_SENDING_ID:
      SPDR = board_id[responseIndex++];
      if (responseIndex >= strlen(board_id)) {
        state = SPI_WAIT_CMD;
      }
      break;

    default:
      SPDR = 0x00;
      state = SPI_WAIT_CMD;
      break;
  }
}

void loop() {
  // nothing needed — everything handled in ISR
}
