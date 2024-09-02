Operating Systems - Assignment 4
By:
Shanny Abid 323858324 
yasmin Cohen 212733836

------- Introduction -------
In this assignment we created a Reactor (Reactor Design Pattern) that handles multiple client file descriptors (TCP socket) and handles them accordingly, using an handler function for each file descriptor.


------- Requirements -------
* Linux machine
* GNU C Compiler
* Make


------- Building -------
1) Building all the necessary files & the main program
make all

2) Export shared libraries.
export LD_LIBRARY_PATH="."


------- Running -------
Run the reactor server:
./react_server

------ How to connect ------
The assignment designed to work with any TCP client, but we'll show how to use it with Telnet client:
1) Open at least two terminals (one for each telnet client).
2) Type the following command: telnet 127.0.0.1 9034
3) Type in each window a message you want to send to the server.
4) The server will receive all the messages and print the in the following format: "client <clientID>: <message>", where the clientID is a reference number of the client, represeneted by the numeric file descriptor (starting from 4, as 0, 1, 2 are reserved for stdin, stdout and stderr and 3 is the file descriptor for the listening socket).
