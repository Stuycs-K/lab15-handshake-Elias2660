#include "colors.h"
#include "pipe_networking.h"
#include <signal.h>
int to_client, from_client;

pid_t a = -1;

void handle_sigpipe(int sig) {
  printf("(" HRED "SERVER" reset "): Caught SIGPIPE, client disconnected\n");
}

void handle_sigint(int sig) {
  if (a != 0) {
    printf("(" HRED "SERVER" reset "): Closing down server due to "HRED"SIGINT"reset"\n");
    remove(WKP);
    close(from_client);
    exit(0);
  } else {
    printf("(" HRED "SERVER" reset "): Closing child processes\n");
    int close_num = -1;
    if (write(to_client, &close_num, sizeof(close_num)) == -1) err();
    close(to_client);
    exit(0);
  }
}

int main() {
  signal(SIGPIPE, handle_sigpipe);  // Set up signal handler for SIGPIPE
  while (1) {
    if (a != 0) {
      from_client = server_handshake(&to_client);  // initial handshake
      printf("(" HMAG "SERVER" reset
             "): Connection established on the server side\n");
      a = fork();
    }

    if (a == 0) {
      signal(SIGINT, handle_sigint);
      printf("(" HYEL "CHILD SERVER" reset "): Handed off to child\n");
      while (1) {
        int random_int = abs(random_urandom() % 100);
        if (write(to_client, &random_int, sizeof(random_int)) == -1) {
          printf("(" HYEL "CHILD SERVER" reset
                 "): Client "HRED"DISCONNECT"reset" or other error\n");
          close(to_client);
          exit(0);
        }
        sleep(1);
      }
    }
  }
}