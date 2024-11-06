#pragma once

#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <stdio.h>
#include <stdlib.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define IP "127.0.0.1"
#define PORT 6378
#define BUFFER_MAX 1024
#define MAX_CONNECTION_ATTEMPT_COUNT 10
#define CONNECTION_ATTEMPT_DELAY_S 1

typedef char packet_t[BUFFER_MAX];

extern void clearPacket(packet_t p);
extern void loadPacket(packet_t p, const char *msg);
extern int comparePacket(packet_t p, const char *msg);
extern int containsPacket(packet_t p, const char *msg);


extern void sendPacket(int sd, packet_t p);
extern int receivePacket(int sd, packet_t p);

extern packet_t read_buffer, write_buffer;
extern int client_socket;
extern struct sockaddr_in server_addr;
extern socklen_t server_addr_len;
extern int game_status;

extern const char *terminate_msg;
extern const char *start_msg;
extern const char *turn_msg;
extern const char *turn_wait_msg;
extern const char *mid_disconnection_msg;
extern const char *winner_msg;
extern const char *loser_msg;
extern const char *tie_msg;

extern void createClientSocket();
extern void createClientSocketAddress();
extern int connectClientToServer();
extern void attemptConnection();
// extern void handleClient();
extern void disconnectClient();

extern void readServer();
extern void writeToServer();

#endif // TCP_CLIENT_H