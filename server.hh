//
// Created by Daniel on 30/05/2019.
//

#ifndef _SERVER_HH_
#define _SERVER_HH_

#include "movieTable.hh"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#define PORT 12345  //socket port
#define BUF_LEN 100

//global variables defining database and server socket
//needed from exit handler to be able to free and close them
table::movieTable *data;
int socket_fd;

/* @brief creates & return socket
 *
 * attempts to create a socket, exiting on failure
 * returning socket if successful
 */
int createsocket();

/* @brief attempts to bind socket to address
 * @param sock  socket to bind
 * @param serv  server address structure
 *
 * attempts to bind given socket and address
 * exiting upon failure to do so;
 * outputting error and exit(1)
 */
void bindsocket(int sock, sockaddr_in serv);

/* @brief attempts to listen on socket for connecting
 * @param sock  socket on which to listen for clients
 *
 * attempts to listen on a given socket
 * exiting upon failure to do so;
 * outputting error and exit(1)
 */
void listenforclient(int sock);

/* @brief attempts to accept incomming connection, returns client socket
 * @param sock  socket on which to connect a client
 * @param client  address struct for connecting client
 * @param len  size of client address struct
 *
 * attempts to connect client to server and store address
 * exiting upon failure to do so; outputting error and exit(1)
 * when successful returning the client socket
 */
int accecptconnection(int sock, sockaddr_in client, socklen_t len);

/* @brief processing a connected client
 * @param client  client socket to serve/process
 *
 * this method, deals with client input and output
 * instances the data base and accepts and parses client inputs
 * outputs in accordance to parsing logic (in implantation)
 * when client requests disconnect, drop client and end
 */
void serveclient(int client, table::movieTable *database);

#endif //_SERVER_HH_
