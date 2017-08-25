# Simple-Server-Client-Socket

The idea is to implement a client and a server which adhere to the specifications given below.

Objectives:-
1) Basic message passing through sockets.
2) Gaining familiarity with Socket API

Server:-
1. Listens for connections from clients.
2. Receives username and password from client.
3. Checks if there is an entry with the client’s username in users.txt
4. If Yes, then check the password,
     4.1. If Matched: then send “Hello <username>”.
     4.2. Else send message “Authentication Failure!!!”, and terminates the
            connection.
5. Incase of 4.1, the connection stays open, and at this point if a client requests for a file, the
    server checks for the availability of the file and
      5.1. Sends the file if it is present.
      5.2. Else the message “File Not Found”.
6. After one request of the client is serviced, the server terminates the connection with that
    client.

Note:
users.txt is a file which you can create and populate with some dummy values.

Client:-
1. The client sends a username and password to the server.
2. If it is authenticated, the client will send the name of the file which it requires.
3. Client exits once the file download completes or if the server does not have that file.
4. The Client program should be executed as follows:-
./<client_exe> <username:password@serverip>

Optional:-
1. You can implement the server as a concurrent server.
2. Allow functionality such as creating users (instead of using predefined dummy values from
    users.txt).
3. Enable option to change password, for a existing user.
