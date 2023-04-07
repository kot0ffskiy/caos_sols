#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

typedef struct icmp_header {
  uint8_t icmp_type;
  uint8_t icmp_code;
  uint16_t check_sum;
  uint16_t icmp_id;
  uint16_t icmp_seq;
} icmp_header_t;

const size_t ICMP_HEADER_DATA_SIZE = 1024;
const size_t ICMP_PACKET_DATA_SIZE =
    sizeof(icmp_header_t) + ICMP_HEADER_DATA_SIZE;

int socket_fd;
size_t ping_count = 0;

void HANDtime() {
  printf("%lu\n", ping_count);
  fflush(stdout);
  shutdown(socket_fd, SHUT_RDWR);
  close(socket_fd);
  exit(0);
}

uint16_t checksum(void* b, int len) {
  uint16_t* buf = b;
  uint32_t sum = 0;
  uint16_t result;

  for (sum = 0; len > 1; len -= 2) {
    sum += *buf++;
  }
  if (len == 1) {
    sum += *(uint16_t*)buf;
  }
  sum = (sum >> 16) + (sum & 0xFFFF);
  sum += (sum >> 16);
  result = ~sum;
  return result;
}

void generate(char* packet) {
  icmp_header_t* header_data = (icmp_header_t*)packet;
  *header_data = (icmp_header_t){.icmp_type = 8,
                                 .icmp_code = 0,
                                 .check_sum = 0,
                                 .icmp_id = rand(),
                                 .icmp_seq = rand()};
  for (size_t i = 0; i < ICMP_PACKET_DATA_SIZE; ++i) {
    packet[i + sizeof(icmp_header_t)] = i;
  }
  header_data->check_sum = checksum(packet, ICMP_PACKET_DATA_SIZE);
}

int check_packet(char* data) {
  icmp_header_t* recieve_header = (icmp_header_t*)(data + 20);
  if (recieve_header->icmp_type == 0 && recieve_header->icmp_code == 0) {
    return 1;
  }
  return 0;
}

int main(int argc, char* argv[]) {
  in_addr_t ip = inet_addr(argv[1]);
  uint32_t time = strtol(argv[2], NULL, 10);
  uint32_t waiting_ms = strtol(argv[3], NULL, 10);

  struct sigaction action;
  memset(&action, 0, sizeof(struct sigaction));
  action.sa_flags = SA_RESTART;
  action.sa_handler = HANDtime;

  if (sigaction(SIGALRM, &action, NULL) == -1) {
    perror("sig error");
    exit(1);
  }
  alarm(time);
  socket_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
  if (socket_fd == -1) {
    perror("socket error");
    exit(1);
  }
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = ip;
  if (connect(socket_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
    perror("connection error");
    close(socket_fd);
    exit(1);
  }

  char data[ICMP_PACKET_DATA_SIZE];

  while (1) {
    generate(data);
    int res = sendto(socket_fd, data, ICMP_PACKET_DATA_SIZE, 0,
                     (struct sockaddr*)&addr, sizeof(addr));
    if (res == -1) {
      printf("Package not sended\n");
      fflush(stdout);
    }
    socklen_t* len_socket = (socklen_t*)sizeof(addr);
    recvfrom(socket_fd, data, ICMP_PACKET_DATA_SIZE, 0, (struct sockaddr*)&addr,
             len_socket);
    if (check_packet(data) == 1) {
      ++ping_count;
    }
    usleep(waiting_ms);
  }

  return 0;
}