# ChatAppC
Creating a chat application with C. client-server model. user will have an SDL2 interface, 

*   in order to handle newlines (\n) in the user's message when he presses Enter key, i need to change the 'text' parameter
    in the Messege struct to a 2-dim array, while the rows are purely lines and the columms are the characters.
    (using 2-dim array and not an array of pointers because im limiting the length of a lines to the size of the client window)
    after doing that, need to change 'DrawText()' function as needed (as it seems adding a for loop, iterating over the number of lines, and inserting each row to TTF function)

*   create a clickable button to send the message onward to the server -> initilizing DrawButton(), ButtonLogic(). (sockets used in Button Logic)
