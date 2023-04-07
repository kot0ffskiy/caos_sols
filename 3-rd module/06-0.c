#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
  uint32_t port_number = strtol(argv[1], NULL, 10);

  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd == -1) {
    perror("socket");
    exit(1);
  }

  struct sockaddr_in addr = {.sin_family = AF_INET,
                             .sin_addr = inet_addr("127.0.0.1"),
                             .sin_port = htons(port_number)};

  int send_num, recv_num;
  while (scanf("%d", &send_num) > 0) {
    sendto(sockfd, &send_num, sizeof(send_num), 0,
           (const struct sockaddr*)&addr, sizeof(addr));
    recvfrom(sockfd, &recv_num, sizeof(recv_num), 0, NULL, NULL);
    printf("%d\n", recv_num);
  }

  shutdown(sockfd, SHUT_RDWR);
  close(sockfd);
  return 0;
}