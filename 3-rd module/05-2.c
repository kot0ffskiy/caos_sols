#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

enum HTTP { OK = 200, FORBIDDEN = 403, NOT_FOUND = 404 };

volatile sig_atomic_t exit_trigger = 0;

int socket_file;
int client;

void Handler() {
  shutdown(socket_file, SHUT_RDWR);
  close(socket_file);
  shutdown(client, SHUT_RDWR);
  close(client);
  //   printf("END!!!!\n");
  //   fflush(stdout);
  exit(0);
}

void answer_from_server(int socket, const char* path_to_file, char* request) {
  const char* file_name = strtok(request + strlen("GET "), " ");
  char path[4096];
  memset(path, 0, sizeof(path));
  snprintf(path, sizeof(path), "%s/%s", path_to_file, file_name);
  enum HTTP http_status;
  int executable = 0;
  if (access(path, F_OK) == -1) {
    http_status = NOT_FOUND;
  } else {
    if (access(path, R_OK) == 0) {
      http_status = OK;
    } else {
      http_status = FORBIDDEN;
    }
    if (access(path, X_OK) == 0) {
      executable = 1;
    }
  }
  http_status = access(path, F_OK) == -1  ? NOT_FOUND
                : access(path, R_OK) == 0 ? OK
                                          : FORBIDDEN;
  char buffer_for_result[4096];
  if (http_status == OK) {
    // printf("status OK\n");
    // fflush(stdout);
    struct stat st;
    lstat(path, &st);
    int file_size = st.st_size;
    memset(buffer_for_result, 0, 4096);
    strcpy(buffer_for_result, "HTTP/1.1 200 OK\r\n");
    write(socket, buffer_for_result, strlen(buffer_for_result));
    memset(buffer_for_result, 0, 4096);
    if (executable == 0) {
      snprintf(buffer_for_result, 4096, "Content-Length: %d\r\n", file_size);
      write(socket, buffer_for_result, strlen(buffer_for_result));
      memset(buffer_for_result, 0, 4096);
      write(socket, "\r\n", 2);
      int fd = open(path, O_RDONLY);
      char buff[4096];
      size_t size;
      while ((size = read(fd, buff, sizeof(buff))) != 0) {
        write(socket, buff, size);
      }
      close(fd);
      write(socket, "\r\n", 2);
    } else {
      pid_t pid = fork();
      if (pid == 0) {
        dup2(socket, 1);
        close(socket);
        execlp(path, path, NULL);
        perror("exec error");
        exit(1);
      }
      waitpid(pid, NULL, 0);
    }
  }

  if (http_status == FORBIDDEN) {
    struct stat st;
    lstat(path, &st);
    int file_size = st.st_size;
    strcpy(buffer_for_result, "HTTP/1.1 403 Forbidden\r\n");
    write(socket, buffer_for_result, strlen(buffer_for_result));
    memset(buffer_for_result, 0, 4096);
    snprintf(buffer_for_result, 4096, "Content-Length: %d\r\n", file_size);
    write(socket, buffer_for_result, strlen(buffer_for_result));
    memset(buffer_for_result, 0, 4096);
    write(socket, "\r\n", 2);
  }

  if (http_status == NOT_FOUND) {
    strcpy(buffer_for_result, "HTTP/1.1 404 Not Found\r\n");
    write(socket, buffer_for_result, strlen(buffer_for_result));
    memset(buffer_for_result, 0, 4096);
    snprintf(buffer_for_result, 4096, "Content-Length: %d\r\n", 0);
    write(socket, buffer_for_result, strlen(buffer_for_result));
    memset(buffer_for_result, 0, 4096);
    write(socket, "\r\n", 2);
  }
}

int main(int argc, char* argv[]) {
  uint32_t port = htons(atoi(argv[1]));
  char* path = argv[2];
  uint32_t address = inet_addr("127.0.0.1");

  //   printf("%d\n", getpid());
  //   fflush(stdout);

  struct sigaction sigint_handler;
  memset(&sigint_handler, 0, sizeof(sigint_handler));
  sigint_handler.sa_handler = Handler;
  sigint_handler.sa_flags = SA_SIGINFO;
  sigaction(SIGTERM, &sigint_handler, NULL);
  sigaction(SIGINT, &sigint_handler, NULL);

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = port;
  addr.sin_addr.s_addr = address;

  socket_file = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_file == -1) {
    close(socket_file);
    perror("socket error");
    exit(1);
  }

  if (bind(socket_file, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)) ==
      -1) {
    close(socket_file);
    perror("bind error");
    exit(1);
  }

  if (listen(socket_file, SOMAXCONN) == -1) {
    close(socket_file);
    perror("listen error");
    exit(1);
  }
  //   printf("I'm after listen\n");

  char buff[4096];
  char request_body[8192];

  while (1) {
    // printf("I'm in exit_trigger\n");
    client = accept(socket_file, NULL, NULL);
    if (client == -1) {
      close(socket_file);
      close(client);
      perror("client connection error");
      exit(1);
    }
    // printf("I've got client\n");

    memset(request_body, 0, 8192);
    size_t len_read = 0;
    while (1) {
      memset(buff, 0, sizeof(buff));
      read(client, buff, sizeof(buff));
      strcpy(request_body + len_read, buff);
      len_read += strlen(buff);
      //   printf("I'm ready to answer\n");
      if (strcmp(request_body + strlen(request_body) - 4, "\r\n\r\n") == 0) {
        // printf("I'm in if\n");
        answer_from_server(client, path, request_body);
        break;
      }
    }

    // printf("client closed");
    close(client);
  }
  return 0;
}