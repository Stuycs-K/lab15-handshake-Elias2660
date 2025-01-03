#include "colors.h"
#include "pipe_networking.h"

int to_server, from_server;

void handle_sigint(int sig) {
    close(to_server);
    close(from_server);
    exit(0);
}

int main() {
  signal(SIGINT, handle_sigint);

  from_server = client_handshake(&to_server);
  printf("(" HCYN "CLIENT" reset
         "): Advancing client established by the server\n");
  while (1) {
    int randomized_number;
    if (read(from_server, &randomized_number, sizeof(randomized_number)) ==
        -1) {
      err();
    }
    if (randomized_number == -1) {
      printf("(" HCYN "CLIENT" reset
             "): Detected pipe "HRED"CLOSURE"reset" by server; closing down\n");
      close(to_server);
      close(from_server);
      exit(0);
    }
    printf("(" HCYN "CLIENT" reset "): Recieved random number " HGRN "%d" reset
           " from server\n",
           randomized_number);
  }
}
