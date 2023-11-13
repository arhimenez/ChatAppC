#include "message.h"
#include "gui.h"

#define DEBUG 1

void FlushMessage(Message* message_p) {
	message_p->length = 0;
	message_p->text[0] = '\0';
}

void GetUserMessage(SDL_Event event, Message* message_p) {
	char key_val = event.text.text[0];
	printf("\n%d\n", key_val);
	if (key_val == 1 && message_p->length > 0) { // BACKSPACE INPUT
		if (DEBUG) {
			printf("you son of bitch, im in\n");
		}
		message_p->text[--message_p->length] = '\0';
	}
	else if (key_val != 1) { // REGULAR INPUT
		message_p->text[message_p->length++] = key_val;
		message_p->text[message_p->length] = '\0';
	}
	printf("length: %d", message_p->length);
}