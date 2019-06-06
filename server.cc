//
// Created by Daniel on 30/05/2019.
//

#include "server.hh"
#include "movieTable.hh"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sstream>

using namespace std;
using namespace table;

int createsocket()
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		printf("ERROR creating socket");
		exit(1);
	}
	cout << "socket created" << endl;
	return sock;
}

void bindsocket(int sock, sockaddr_in serv)
{
	if (bind(sock, (struct sockaddr *)&serv, sizeof(serv)) < 0) {
		cerr << "ERROR binding socket" << endl;
		exit(1);
	}
	cout << "socket bound" << endl;
}

void listenforclient(int sock)
{
	if (listen(sock, SOMAXCONN) < 0) {
		cerr << "ERROR listening for connections" << endl;
		exit(1);
	}
	cout << "listening for connections..." << endl;
}

int accecptconnection(int sock, sockaddr_in client, socklen_t len)
{
	int client_fd = accept(sock, (struct sockaddr *)&client, &len);
	if (client_fd < 0) {
		cerr << "ERROR accepting connection" << endl;
		exit(1);
	}
	cout << "client connected" << endl;
	return client_fd;
}

void serveclient(int client)
{
	char buffer[100];
	movieTable *data;
	data = new movieTable;
	data->loadCSV("scifi.csv");

	cout << "Serving Client... " << endl;
	if (send(client, "HELLO\n\n", strlen("HELLO\n\n"), 0) < 0) {
		cerr << "ERROR writing to client" << endl;
		exit(1);
	}

	while ((strncasecmp(buffer, "BYE", 3) != 0)) {
		memset(buffer, 0, 100);

		if (recv(client, buffer, 100, 0) < 0) {
			cerr << "ERROR reading from client" << endl;
			exit(1);
		}
		//string input(buffer);
		string cmd, arg, output;
		istringstream in(buffer);
		ostringstream out;

		in >> cmd >> arg;

		if (strncasecmp(buffer, "GET", 3) == 0) {
			if (arg.empty()) {
				for (unsigned int i = 0; i < data->rows(); ++i) {
					output = data->display(*data->get(i));
					send(client, output.c_str(), output.length(), 0);
				}
				cout << endl;
			} else {
				try {
					movie *m = data->get((unsigned int)stoul(arg));  //converts string->integer
					if (m == nullptr) {
						throw "invalid row index";
					}
					output = data->display(*m);
					send(client, output.c_str(), output.length(), 0);
				}
				catch (...) {
					send(client, "ERROR: invalid row index\n", strlen("ERROR: invalid row index\n"), 0);
				} //catching if conversion fails, ie. argv[2] is not a number } /
			}
		} else send(client, "Invalid command try: GET\n", strlen("Invalid command try: GET\n"), 0);
	}

	cout << "Closing connection with client..." << endl;
	send(client, "\n...disconnected\n", strlen("\n...disconnected\n"), 0);
	close(client);
	free(data);
	exit(0);
}

int main()
{
	struct sockaddr_in s_addr = {}, c_addr = {};

	s_addr.sin_family = AF_INET;
	s_addr.sin_addr.s_addr = INADDR_ANY; // any address
	s_addr.sin_port = htons(PORT);

	int socket_fd = createsocket();
	bindsocket(socket_fd, s_addr);

	while (true) {
		listenforclient(socket_fd);
		int client_fd = accecptconnection(socket_fd, c_addr, sizeof(c_addr));

		pid_t pid = fork();

		if (pid < 0) { //fork failure
			serveclient(client_fd);
			break;

		} else if (pid == 0) { //child
			serveclient(client_fd);

		} else { //parent
			close(client_fd);
		}
	}
	close(socket_fd);
	exit(0);
}

