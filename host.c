#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>


/* host.c: (3pts)
 *   A program to print the IP address of a host specified on the command
 *   line.  If no argument is provided, print a usage message and exit.
 * Example input/output:
 * > ./host www.indstate.edu
 * Host: www-prod-cms-lb.indstate.edu
 * Aliases:
 *   www.indstate.edu
 * Host type: IPv4
 * Addresses:
 *   139.102.15.65
 */


int main(int argc, char *argv[])
{
  struct hostent *he;
  char buf[INET_ADDRSTRLEN];

  if (argc < 2) {
    printf("Usage: host <hostname>\n");
    exit(0);
  }
  /**
   * Use the gethostbyname(3) function to look-up the host provided on the
   * first command line argument (argv[1]), assign the result to host.  If
   * host is NULL, print "Host not found: " followed by name of the host and
   * exit with a value of 1:
   */
  if ((he = gethostbyname(argv[1])) == NULL) {
    perror("gethostbyname");
    exit(1);
  }

  /* Print the name of the host: */
  printf("%s\n", he->h_name);

  /**
   * If h_aliases[0] is not null, then print a list of aliases, otherwise don't:
   * (there may be more than one, ex: cs.indstate.edu while logged into CS)
   */
  if (he->h_aliases[0] != NULL) printf("Aliases:\n");
  for(int i=0; he->h_aliases[i] != NULL; i++) {
    printf("  %s\n", he->h_aliases[i]);
  }
  
  /* Print the host type (IPv4 or IPv6). */
  printf("Host type: %s\n", he->h_addrtype == AF_INET ? "IPv4" : "IPv6");

  /**
   * Print the addresses, there may be more than one: (ex: amazon.com)
   * Use the inet_ntop(3) function to write the IP-address into a buffer, then
   * print the string found in the buffer.
   */
  printf("Addresses:\n");
  for(int i=0; he->h_addr_list[i] != NULL; i++) {
    if (inet_ntop(he->h_addrtype, he->h_addr_list[i], buf, INET_ADDRSTRLEN) != NULL)
      printf("  %s\n", buf);
  }

  return 0;
}
/*
 struct hostent {
     char  *h_name;            // official name of host
     char **h_aliases;         // alias list
     int    h_addrtype;        // host address type
     int    h_length;          // length of address
     char **h_addr_list;       // list of addresses
 }
 #define h_addr h_addr_list[0] // for backward compatibility
*/

