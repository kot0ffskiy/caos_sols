#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>

#define LOCAL_HOST "127.0.0.1"

struct epoll_event __ev_rd[1000];
char buffer[4096];
volatile sig_atomic_t __fl_ex = 1;

int __serv_fd = -1;
int __ep_dscr = -1;
struct sigaction __sigint_hand;

void func_hand(int sig) { __fl_ex = 0; }

void handle_event(int __ev_nm, struct sockaddr_in* socket_) {
  printf("in handle");
  fflush(stdout);
  for (int i = 0; i < __ev_nm; ++i) {
    if (__ev_rd[i].data.fd == __serv_fd) {
      uint32_t socksize = sizeof(*socket_);
      int __client_fd = accept(__serv_fd, (struct sockaddr*)socket_, &socksize);
      if (__client_fd == -1) {
        break;
      }
      int __rem_fd1 = fcntl(__ep_dscr, F_GETFD);
      if (__rem_fd1 == -1) {
        perror("fd1 error");
        close(__serv_fd);
        exit(1);
      }
      int __rem_fd2 = fcntl(__ep_dscr, F_SETFL, __rem_fd1 | O_NONBLOCK);
      if (__rem_fd2 == -1) {
        perror("fd2 error");
        close(__serv_fd);
        exit(1);
      }
      struct epoll_event action;
      action.events = EPOLLIN;
      action.data.fd = __client_fd;
      epoll_ctl(__ep_dscr, EPOLL_CTL_ADD, __client_fd, &action);
    } else {
      int __tmp_buff = 0;
      while ((__tmp_buff = read(__ev_rd[i].data.fd, buffer, sizeof(buffer))) >
             0) {
        for (int k = 0; k < __tmp_buff; k++) {
          buffer[k] = toupper(buffer[k]);
        }
        write(__ev_rd[i].data.fd, buffer, __tmp_buff);
      }
    }
  }
}

int main(int argc, char* argv[]) {
  __sigint_hand.sa_flags = SA_RESTART;
  __sigint_hand.sa_handler = func_hand;

  sigaction(SIGTERM, &__sigint_hand, NULL);

  struct sockaddr_in socket_;
  memset(&socket_, 0, sizeof(socket_));
  socket_.sin_port = htons(strtol(argv[1], NULL, 10));
  socket_.sin_family = AF_INET;
  inet_aton(LOCAL_HOST, &socket_.sin_addr);
  __serv_fd = socket(AF_INET, SOCK_STREAM, 0);
  size_t addrsize = sizeof(socket_);
  if (bind(__serv_fd, (struct sockaddr*)&socket_, addrsize)) {
    perror("bind");
    close(__serv_fd);
    exit(1);
  }

  if (listen(__serv_fd, SOMAXCONN)) {
    perror("listen");
    close(__serv_fd);
    exit(1);
  }

  __ep_dscr = epoll_create1(0);
  int __rem_fd1 = fcntl(__serv_fd, F_GETFD);
  if (__rem_fd1 == -1) {
    perror("fd1 error");
    close(__serv_fd);
    exit(1);
  }
  int __rem_fd2 = fcntl(__serv_fd, F_SETFL, __rem_fd1 | O_NONBLOCK);
  if (__rem_fd2 == -1) {
    perror("fd2 error");
    close(__serv_fd);
    exit(1);
  }
  struct epoll_event action;
  action.events = EPOLLIN;
  action.data.fd = __serv_fd;

  // File event registation
  epoll_ctl(__ep_dscr, EPOLL_CTL_ADD, __serv_fd, &action);

  while (__fl_ex == 1) {
    int __ev_nm = 0;
    size_t __bf_sz = sizeof(__ev_rd);
    printf("in while");
    fflush(stdout);
    __ev_nm = epoll_wait(__ep_dscr, __ev_rd, __bf_sz, -1);
    handle_event(__ev_nm, &socket_);
  }
  close(__serv_fd);
  return 0;
}