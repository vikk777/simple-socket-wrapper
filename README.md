# simple-socket-wrapper
This is a first version of socket wrapper for Linux

Create listening socket very easy. Include sockets.h and declare an object of class <b>Sockets_Serv</b>. An argument of object is a listening port. <i>By default it is 7777.</i>

Client socket is created as object of class Sockets. An argument is a port to connect.

There are someone clients can connect to server. To disconnect from the server type command "exit".

At this moment server can perform one task only - it sends to client information about itself system.

But library Sockets is planned develop.
