#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netdb.h>

/**
 * The port number for your "server" is the same as the digits in your cs473
 * account.
 */
#define K	1024
#define PORT	47350

/*
 * get.c: 3 points
 *   A program to connect to a server (hostname provided as the first command
 * line parameter) and receive data into a file (name provided as the second
 * command line parameter.)  Print a usage and exit if there are not enough
 * parameters.
 * 
 * Usage: get <hostname> <filename>
 */
int main(int argc, char *argv[])
{
  int sock, fd, r;
  struct hostent *he;
  struct sockaddr_in addr;
  char buf[K];

  if (argc < 3) {
    printf("Usage: get <hostname> <filename>\n");
    exit(0);
  }

  /**
   * Use open to open the file specified by argv[2] for writing, create the
   * file if it doesn't exist and truncate the file if it does.
   * Print an error message and exit if the open fails.
   */
  if ((fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0) {
    perror("open");
    exit(1);
  }

  /* Create a SOCK_STREAM internet socket: */
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    exit(2);
  }

  /* Setup the severs address, use gethostbyname(3) to get the address: */
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);
  if ((he = gethostbyname(argv[1])) == NULL) {
    perror("gethostbyname");
    exit(3);
  }
  memcpy(&addr.sin_addr.s_addr, he->h_addr_list[0], he->h_length);

  /* Use connect(2) to connect to the server: */
  if (connect(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) < 0) {
    perror("connect");
    exit(4);
  }

  /**
   * In a loop, read K sized buffers from the socket and write the data into
   * the file until the socket connection is closed.  Make sure to only write
   * as much data into the file as was actually read from the socket.
   */
  while ((r = read(sock, buf, K)) > 0) {
    if (write(fd, buf, r) < 0) break;
  }
  close(fd);
  close(sock);

  return 0;
}
