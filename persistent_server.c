#include "colors.h"
#include "pipe_networking.h"

int to_client, from_client;

void handle_sigpipe(int sig) {
  printf("(" HRED "SERVER" reset "): Caught SIGPIPE, a client disconnected\n");
}

void handle_sigint(int sig) {
  printf("(" HRED "SERVER" reset "): Closing\n");
  int close_num = -1;
  if (write(to_client, &close_num, sizeof(close_num)) != -1) {
    // in case clients still connected
    close(to_client);
  };

  close(from_client);
  remove(WKP);
  exit(0);
}

int main() {
  signal(SIGPIPE, handle_sigpipe);  // Set up signal handler for SIGPIPE
  signal(SIGINT, handle_sigint);
  while (1) {
    from_client = server_handshake(&to_client);  // initial handshake
    printf("(" HMAG "SERVER" reset
           "): Connection established on the server side\n");

    while (1) {
      int random_int = abs(random_urandom() % 100);
      if (write(to_client, &random_int, sizeof(random_int)) == -1) {
        printf("(" HMAG "SERVER" reset "): Client disconnect or other error\n");
        close(to_client);
        break;
      }
      sleep(1);
    }
  }
}
