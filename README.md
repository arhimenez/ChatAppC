# ChatAppC

<hr>

Simple chat application, while the user client interface has a live message typing box, and a clickable button to send messages (SDL2 implemented). <br> 
The server is multi-threaded and uses windows socket api for communication. For each new socket connection, a thread is created to listen for incoming messages <br> thus there can be unlimited users connected to chat (I limited to 10 on purpose)
