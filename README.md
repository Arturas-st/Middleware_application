# Middleware_application

Instruction to run the program:

Dependencies (Linux/WSL).
Use asio for networking:
sudo apt install libasio-dev

Server should be run first.
#Terminal 1: Server (connects to middleware).
To start the server from the command prompt:
java -classpath SAAB.jar com.saabtech.server.SAABServer

#Terminal 2: Middleware (listens on port) and outputs binary data when server connects.
./middleware

#Terminal 3: Outputs human readable data.
Piping system must be included for successful program execution.
./middleware | ./client
