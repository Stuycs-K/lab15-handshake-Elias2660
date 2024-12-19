#include "pipe_networking.h"

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
// UPSTREAM = to the server / from the client
// DOWNSTREAM = to the client / from the server
/*=========================
  server_setup

  creates the WKP and opens it, waiting for a  connection.
  removes the WKP once a connection has been made

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_setup() {
  mkfifo(WKP, 0666);

  printf("Created the client pipe in the server setup function\n");
  int from_client = open(
      WKP, O_RDONLY, 0666);  // server is only reading messages from the client

  return from_client;
}

/*=========================
  server_handshake
  args: int * to_client

  Performs the server side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe (Client's
  private pipe).

  returns the file descriptor for the upstream pipe (see server setup).
  =========================*/
int server_handshake(int *to_client) {
  int from_client;

  //   set up the private pipe here
  int private_pipe[2];
  pipe(private_pipe);
  *to_client = private_pipe;

  //   got from client();
  from_client = server_setup();
  //   get the file descriptor for the client private pipe
  //   then set the from client to the server's pipe
  //   how do I get the downstream pipe of the client? calling client
  //   handshake???

  return from_client;
}

/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  // gets the file descriptor for the pipe of the server
  //   also write the number the server
  // but then read back from the pipe...
  // gets the and then returns file descriptor of the upstream pipe? creates it,
  // or does it get from another function
  *to_server = open(WKP, O_WRONLY, 0666);

  int random_number = random_urandom();
  printf("Sending random number %d to server\n",random_number);
  if (write(to_server, random_number,  HANDSHAKE_BUFFER_SIZE) == -1) err();

  int from_server;

  return from_server;
}

/*=========================
  server_connect
  args: int from_client

  handles the subserver portion of the 3 way handshake

  returns the file descriptor for the downstream pipe.
  =========================*/
int server_connect(int from_client) {
  remove(WKP);
  mkfifo(WKP, 0666);
  int to_client = open(WKP, O_RDONLY, 0666);
  //   hands of the pipe to a client pipe
  return to_client;
}

int err() {
  printf("\x1b[31m errno %d\x1b[0m\n", errno);
  printf("%s\n", strerror(errno));
  exit(1);
}

int random_random() {
  int r_file = open("/dev/random", O_RDONLY, 0);
  if (r_file == -1) err();
  int bytes;
  int read_result = read(r_file, &bytes, sizeof(bytes));
  if (read_result == -1) err();
  return bytes;
}

int random_urandom() {
  int r_file = open("/dev/urandom", O_RDONLY, 0);
  if (r_file == -1) err();
  int bytes;
  int read_result = read(r_file, &bytes, sizeof(bytes));
  if (read_result == -1) err();
  return bytes;
}