//
// Created by Daniel on 30/05/2019.
//

//build using makefile - make dbserver

#include "server.hh"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sstream>
#include <sys/wait.h>
#include <signal.h>

using namespace std;
using namespace table;

/**function triggered when console interrupt, ie ^C
 * goes through correct shutdown procedure, eliminating causes fo memory leaks and ensure socket is closed
 * exits process
 * */
void exithandler(int sig)
{
	delete data; //free database
	cout << "\ndatabase freed" << endl;
	wait(nullptr); //kills children
	cout << "server processes killed" << endl;
	cout << "shutting down..." << endl;
	close(socket_fd); //closes socket
	exit(0);
}

//implementation of socket creation func defined and described in included header "server.hh"
int createsocket()
{
	//attempts to create a socket
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		printf("ERROR creating socket");
		exit(1); //if fails end process
	}
	cout << "socket created" << endl; //output to mark success
	return sock;
}
//implementation of socket binding func defined and described in included header "server.hh"
void bindsocket(int sock, sockaddr_in serv)
{
	//attempts to bind socket to passed address
	if (bind(sock, (struct sockaddr *)&serv, sizeof(serv)) < 0) {
		cerr << "ERROR binding socket" << endl;
		exit(1); //if fails end process
	}
	cout << "socket bound at port: " << PORT << endl; //output to mark success
}

//implementation of prototype defined and described in included header "server.hh"
void listenforclient(int sock)
{
	//attempts to set into listening state
	if (listen(sock, SOMAXCONN) < 0) {
		cerr << "ERROR listening for connections" << endl;
		exit(1); //if fails end process
	}
	cout << "listening for connections..." << endl; //output to mark success
}

//implementation of prototype defined and described in included header "server.hh"
int accecptconnection(int sock, sockaddr_in client, socklen_t len)
{
	//attempt to accept and bind client
	int client_fd = accept(sock, (struct sockaddr *)&client, &len);
	if (client_fd < 0) {
		cerr << "ERROR accepting connection" << endl;
		exit(1); //if fails end process
	}
	cout << "client connected" << endl;  //output to mark success
	return client_fd; //return client socket
}

//implementation of prototype defined and described in included header "server.hh"
void serveclient(int client, movieTable *database)
{
	char buffer[BUF_LEN]; //array to store client input

	cout << "Serving Client... " << endl;
	//send message to greet client
	if (send(client, "HELLO\n\n", strlen("HELLO\n\n"), 0) < 0) {
		cerr << "ERROR writing to client" << endl;
		exit(1);
	}
	//client input parsing loop (terminates with a buffer input of 'bye')
	while ((strcasecmp(buffer, "BYE\n") != 0) && (strcasecmp(buffer, "BYE") != 0)) {
		memset(buffer, 0, BUF_LEN); //reset buffer

		//read client input
		if (recv(client, buffer, BUF_LEN, 0) < 0) {
			cerr << "ERROR reading from client" << endl;
			exit(1);
		}
		//string input(buffer);
		string cmd, arg, output; //string used to parse input and send output
		istringstream in(buffer); //stream to parse input
		ostringstream out; //stream to construct output

		in >> cmd >> arg; //take first to words from buffer (ie space separated)

		if (strcasecmp(cmd.c_str(), "GET") == 0) { //valid 'get' command
			//if no argument given set output string to every row in the database
			if (arg.empty()) {
				for (unsigned int i = 0; i < database->rows(); ++i) {
					out << database->display(i);
				}
				output = out.str(); //converts sstream to string
			} else {
				try {
					output = database->display((unsigned int)stoul(arg));
				}
				catch (...) { //catching if conversion fails, ie. arg is not a number
					output = "ERROR: invalid row index\n";
				}
			}
			//all other input other then 'bye' sets output to a error msg
		} else if ((strcasecmp(buffer, "BYE\n") != 0) && (strcasecmp(buffer, "BYE") != 0)) {
			output = "Invalid command try: GET / [arg] or BYE\n";
		}

		//after parsing and output setting, send to client
		if (send(client, output.c_str(), output.length(), 0) < 0) {
			cerr << "ERROR writing to client" << endl;
			exit(1);
		}
	}
	cout << "Closing connection with client..." << endl;
	send(client, "\n...disconnected\n", strlen("\n...disconnected\n"), 0);
	close(client);
}

//main server execution loop, defines control flow of program
int main()
{
	struct sockaddr_in s_addr = {}, c_addr = {}; //declare structs to store server and client addresses

	//populate server address struct
	s_addr.sin_family = AF_INET;
	s_addr.sin_addr.s_addr = INADDR_ANY; // any address
	s_addr.sin_port = htons(PORT);

	//setup server socket
	socket_fd = createsocket();
	bindsocket(socket_fd, s_addr);
	//instance database and load csv file
	data = new movieTable;
	data->loadCSV("scifi.csv");

	signal(SIGINT, exithandler); //console interrupt handler to catch ctrl^C and call exit procedure
	while (true) { //server operation loop
		//listen and accepting clients (storing client socket)
		listenforclient(socket_fd);
		int client_fd = accecptconnection(socket_fd, c_addr, sizeof(c_addr));

		pid_t pid = fork(); //attempt to fork process

		if (pid < 0) { //fork failure
			cerr << "ERROR forking process, serving in" << endl;
			serveclient(client_fd, data);

		} else if (pid == 0) { //child process, handles client and then end process
			serveclient(client_fd, data);
			exit(0);

		} else { //parent process, closes connection with client (assumes child is servicing them) then loops
			close(client_fd);
		}
	}
}