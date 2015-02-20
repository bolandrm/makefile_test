#include <WProgram.h>

int main(void) {
	_init_Teensyduino_internal_();

	for (;;) {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
	}

  return EXIT_SUCCESS;
}
