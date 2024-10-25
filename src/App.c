#include <stdio.h>

#include "TCPClient.h"
#include "getch.h"

int main(int argc, char const *argv[])
{
    createClientSocket();
    createClientSocketAddress();
    attemptConnection();
    clearPacket(read_buffer);
    clearPacket(write_buffer);
    
    while (1)
    {
        readServer();
        writeToServer();
        if (game_status == -1)
            break;
    }

    disconnectClient();
    return 0;
}
