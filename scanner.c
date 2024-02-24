#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define USAGE(x) printf("Usage:\n%s <target_ip>\n", x)
#define MAX_PORT_ATTEMPTS 65535
#define TIMEOUT_SECONDS 5

void return_time(char* timebuff);
int connect_to(const char *host, int port);

void set_socket_timeout(int client_socket);
void banner_grab(int client_socket, bool verbosity);

int main(int argc, char* argv[])
{

	int code = 0;
	bool verbose = false;

	// Get target ip param
	if (argc < 2)
	{
		USAGE(argv[0]);

		code = -1;

		return code;
	}

	// Get verbosity
	if (argc == 3)
	{
		if (strcmp(argv[2], "-v") == 0)
		{
			verbose = true;
			printf("[ ] Verbosity set - Scan will attempt to grab banners\n\n");
		}
	}

	// Time stuff done
	const char* target = argv[1];
	char* timebuff;

	timebuff = (char*)malloc(32);

	return_time(timebuff);

	printf("[+ %s +] Beginning port scan for target => %s\n\n", timebuff, target);
	memset(timebuff,0,strlen(timebuff));

	// Start Socket Creation
	// DOMAIN, TYPE, PROTO
	int client_socket;
	unsigned short current_port = 1;

	bool no_open_ports = true;

	while (current_port < MAX_PORT_ATTEMPTS) 
	{
        client_socket = connect_to(target, current_port);

        if (client_socket != -1) 
        {
            // Connection successful
            return_time(timebuff);

        	printf("[%s] Discovered open port %d", timebuff, current_port);
        	
        	printf("%s", (verbose ? " | " : "\n"));

        	banner_grab(client_socket, verbose);
        		
        	no_open_ports = false;
            
            memset(timebuff, '0', sizeof(timebuff));
        } 

        current_port++;
    }

    if (no_open_ports)
    {
    	printf("\nNo open ports discovered...\n");
    }

	return_time(timebuff);

    printf("\n\n[%s] SCAN FINISHED for host - %s\n", timebuff, target);
    
    if (client_socket != -1)
    	close(client_socket);

	free(timebuff);

	return code;
}


void return_time(char* timebuff)
{
	timebuff = (char*)realloc(timebuff, 32);

	time_t t = time(NULL);
	struct tm* timeinfo = localtime(&t);

	strftime(timebuff, 32, "%H:%M:%S", timeinfo);
}

int connect_to(const char *host, int port)
{
    int client_socket;
    struct sockaddr_in server_address;

    // Create socket
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
    {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

   	// Configure server address
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    if (inet_pton(AF_INET, host, &server_address.sin_addr) < 0) 
    {
        printf("Invalid address or address not supported\n");
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) 
    {
        close(client_socket); // Close the socket

        // Print an error message if the connection was refused
        if (errno == ECONNREFUSED) 
        {
            return -1; // Connection refused
        } 
        else 
        {
            perror("Error connecting to server");
            exit(EXIT_FAILURE);
        }
    }

    //printf("Connected to %s on port %d\n", host, port);
    return client_socket;
}

void set_socket_timeout(int client_socket)
{
	struct timeval timeout;
    timeout.tv_sec = TIMEOUT_SECONDS;
    timeout.tv_usec = 0;

    if (setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0) 
    {
        perror("Error setting socket timeout");
        exit(EXIT_FAILURE);
    }
}

void banner_grab(int client_socket, bool verbosity) 
{

	if (!verbosity)
		return;

    char buffer[32];
    int total_received = 0;
    ssize_t bytes_received;

    set_socket_timeout(client_socket);

    // Send a request to the server, extremely rudimentary "banner grabber" LMAO
    const char *request = "GET / HTTP/1.0\r\n\r\n";
    if (send(client_socket, request, strlen(request), 0) == -1) 
    {
        perror("Error sending request");
        return;
    }

    while ((bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0)) > 0) 
    {
        buffer[bytes_received] = '\0';
        printf("%s\n", buffer);

        total_received++;
        if (total_received == 3)
        	break; // 3 lines of size 32 each...
    }

    if (bytes_received == -1) 
    {
        printf("(?)\n");
        return;
    }
}
