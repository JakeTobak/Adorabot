#include "Adorabot.h"
#include "User.h"

using namespace std;

int main() {
	User* user = new User((char*)"Adorabot", (char*)"Adorabot");
	Adorabot* myBot = new Adorabot(user);
	myBot->start();
	return 0;
}
