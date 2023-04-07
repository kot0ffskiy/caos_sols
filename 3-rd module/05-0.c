#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char** argv) {
  uint32_t address = inet_addr(argv[1]);
  uint16_t port = htons(atoi(argv[2]));

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = port;
  addr.sin_addr.s_addr = address;

  int socket_file = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_file == -1) {
    perror("error");
    exit(1);
  }

  if (connect(socket_file, (struct sockaddr*)&addr,
              sizeof(struct sockaddr_in)) == -1) {
    close(socket_file);
    perror("error");
    exit(1);
  }

  int my, get;
  while (scanf("%d", &my) != EOF) {
    if (write(socket_file, &my, sizeof(int)) < 1 ||
        read(socket_file, &get, sizeof(int)) < 1) {
      break;
    }
    printf("%d\n", get);
  }

  close(socket_file);
  shutdown(socket_file, SHUT_RDWR);
  return 0;
}