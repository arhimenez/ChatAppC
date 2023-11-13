#include <time.h>
#include "gui.h"

#ifndef MESSAGE_H
#define MESSAGE_H

typedef struct {
	int length;
	char text[1024];
	time_t timestamp;
} Message;

void FlushMessage(Message* message_p);
void GetUserMessage(SDL_Event event, Message* message_p);


#endif // !MESSAGE_H
