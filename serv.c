#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netdb.h>

#define K	1024

/**
 * The port number for your "server" is the same as the digits in your cs473
 * account.
 */
#define PORT	47350

int writeall(int fd, const char *buf, size_t size)
{
  int w, t = 0;
  do {
    w = write(fd, buf+t, size-t);
    if (w < 0) return w;
    t += w;
  } while (t < size);

  return t;
}

/**
 * Create a function to handle a client connection.  It should take two
 * parameters, the client socket descriptor and the path to the file to send
 * to the client.  Open the file and write the data in it to the client, then
 * close the descriptors.
 */
void handle_client(int client, char *file)
{
  char buf[K];
  int r, fd = open(file, O_RDONLY);
  if (fd < 0) {
    perror("open");
    close(client);
    return;
  }
  while((r = read(fd, buf, K)) > 0) {
    if (writeall(client, buf, r) < 0) break;
  }

  close(fd);
  close(client);
  return;
}

/**
 * serv.c: 4 points
 *  Get the name of a file from the command line and setup a server socket and
 * listen for connections, accepting them in a forever loop.  Hand each
 * connection off to the client handling function defined above.  This program
 * should not exit.
 */
int main(int argc, char *argv[])
{
  struct sockaddr_in addr;
  int sock, client_sock;
  
  if (argc < 2) {
    printf("Usage: serv <filename>\n");
    exit(0);
  }

  /* Create a SOCK_STREAM internet socket: */
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    exit(2);
  }

  /**
   * Set the socket option SO_REUSEADDR using the setsockopt(2) system call.
   * The SO_REUSEADDR option is documented in the socket(7) manual page.
   */
  int opt = 1;
  if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
    perror("setsockopt");
    exit(3);
  }

  /**
   * Setup server address, bind to any address on port defined by the PORT
   * #define above:
   */
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);
  addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) < 0) {
    perror("bind");
    exit(4);
  }
  
  /* Start listening for connections using the listen(2) call: */
  if (listen(sock, 128) < 0) {
    perror("listen");
    exit(5);
  }

  for(;;) {
    /**
     * accept(2) a connection here, pass the new descriptor with the name of the
     * file to send to the client handing function.
     * Do this forever.
     */
    client_sock = accept(sock, NULL, NULL);
    if (client_sock >= 0) handle_client(client_sock, argv[1]);
  }

  // Not reached:
  return 0;
}
