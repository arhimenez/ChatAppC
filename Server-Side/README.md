
######################        DWORD WINAPI handleClient(int clientId)        ######################

Multi-threaded server (using windows api), that only broadcasts to all connected sockets it's received messages.
for each socket, a thread is initiated in order to listen for incoming messages.
