#include "TCPClient.h"

packet_t read_buffer, write_buffer;
int client_socket;
struct sockaddr_in server_addr = {};
socklen_t server_addr_len = sizeof(server_addr);
int game_status = 0;

int i = 0, select_status = 0, connection_attempts = 0, connection_status = 0, n = 0, valread = 0;

const char *terminate_msg = "terminate";
const char *start_msg = "All players are here. Game is starting!\nYou make a move by entering '(x,y)'\n";
const char *turn_msg = "It is your turn!\n";
const char *turn_wait_msg = "Waiting for other players move...\n";
const char *mid_disconnection_msg = "The other player has disconnected! Pausing game...\n";
const char *winner_msg = "You win!\n";
const char *loser_msg = "You lose!\n";
const char *tie_msg = "It's a tie!\n";

void clearPacket(packet_t p)
{
    memset(p, 0, sizeof(packet_t));
}
void loadPacket(packet_t p, const char *msg)
{
    size_t msg_len = strlen(msg);
    msg_len = msg_len > BUFFER_MAX ? BUFFER_MAX : msg_len;
    memcpy(p, msg, msg_len);
}
int comparePacket(packet_t p, const char *msg)
{
    return strncmp(p, msg, strlen(msg));
}
int containsPacket(packet_t p, const char *msg)
{
    if (strstr(p, msg) != NULL)
        return 0;
    return -1;
}
void sendPacket(int sd, packet_t p)
{
    if (send(sd, p, BUFFER_MAX, 0) < 0)
        printf("Packet send error!\n");
}
int receivePacket(int sd, packet_t p)
{
    if ((valread = read(sd, p, BUFFER_MAX)) < 0)
        printf("Packet receive error!\n");
    return valread;
}

void createClientSocket()
{
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1)
    {
        printf("Cannot create socket...\n");
        abort();
    }
    printf("Socket created!\n");
}
void createClientSocketAddress()
{
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(IP);
    server_addr.sin_port = htons(PORT); // unsigned short *hostshort* from host byte order to network byte order
}
int connectClientToServer()
{
    if (connect(client_socket, (struct sockaddr *)&server_addr, server_addr_len) != 0)
        return -1;
    printf("Connected to server! socket fd : %d, ip : %s, port : %d\n",
               client_socket,
               inet_ntoa(server_addr.sin_addr),
               ntohs(server_addr.sin_port));
    return 0;
}
void attemptConnection()
{
    connection_attempts = 0;
    connection_status = 0;
    printf("Attempting to connect to server...\n");
    while ((connection_status = connectClientToServer()) == -1 && connection_attempts++ < MAX_CONNECTION_ATTEMPT_COUNT)
        sleep(CONNECTION_ATTEMPT_DELAY_S);
    if (connection_status == 0)
        return;
    printf("Connection attempt timed out!\n");
    exit(0);
}
// void handleClient()
// {
//     while (1)
//     {
//         // memset(buffer, 0, BUFFER_MAX);
//         printf(" : ");
//         n = 0;
//         while ((buffer[n++] = getchar()) != '\n')
//             ;
//         write(client_socket, buffer, sizeof(buffer));
//         // readMessage();
//         // printBuffer();
//         if (strncmp(buffer, terminate_msg, 9) == 0)
//             break;
//     }
// }
void disconnectClient()
{
    printf("\nConnection terminated...\n");
    close(client_socket);
    game_status = -1;
}

void readServer()
{
    if (game_status != 0)
        return;
    clearPacket(read_buffer);
    if ((valread = receivePacket(client_socket, read_buffer)) == 0 || comparePacket(read_buffer, terminate_msg) == 0)
        disconnectClient();
    if (containsPacket(read_buffer, turn_msg) == 0 || containsPacket(read_buffer, winner_msg) == 0 || containsPacket(read_buffer, loser_msg) == 0 || containsPacket(read_buffer, tie_msg) == 0)
    {
        game_status = 1;
    }
    else if (containsPacket(read_buffer, turn_wait_msg) == 0 || containsPacket(read_buffer, mid_disconnection_msg) == 0)
    {
        game_status = 0;
    }
    printf("%s", read_buffer);
}

void writeToServer()
{
    if (game_status < 1)
        return;
    clearPacket(write_buffer);
    n = 0;
    // printf("Write!\n");
    while ((write_buffer[n++] = getchar()) != '\n')
        ;
    sendPacket(client_socket, write_buffer);
    // printf("Packet sent!\n");
    game_status = 0;
}
