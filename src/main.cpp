#include "sweetiebot.h"

int main() {
	SweetieBot myBot = SweetieBot("NICK SweetieBot\r\n","USER botSweetie 8 * :SweetieBot\r\n");
	myBot.start();
	return 0;
}
