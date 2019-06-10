# DataBaseServer
simple linux socket database

COMPILATION AND RUNNING

[MAKEFILE]
compile: make dbserver
clean: make clean
run: ./dbserver

[CONSOLE]
compile: g++ -o dbserver server.cc movieTable.cc
run: ./dbserver

ctrl-c to exit process:
- this interupt is correctly handled by program and will close socket and free memory allocated to database
