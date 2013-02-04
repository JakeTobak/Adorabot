#include "Adorabot.h"
#include "User.h"

int main() {
	User* user = new User("Adorabot", "Adorabot")
	Adorabot myBot = Adorabot("NICK Adorabot\r\n","USER Adorabot 8 * :Adorabot\r\n");
	myBot.start();
	return 0;
}
