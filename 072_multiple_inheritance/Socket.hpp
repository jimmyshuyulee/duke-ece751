#ifndef SOCKET_H
#define SOCKET_H

#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

class Socket {
 protected:
  int status;
  int server_fd;
  struct addrinfo host_info;
  struct addrinfo * host_info_list;
  const char * hostname;
  const char * port;

 public:
  Socket() {
    memset(&host_info, 0, sizeof(host_info));
    hostname = NULL;
    port = NULL;
  }
  void init() {
    host_info.ai_family = AF_UNSPEC;
    host_info.ai_socktype = SOCK_STREAM;
    host_info.ai_flags = AI_PASSIVE;

    status = getaddrinfo(hostname, port, &host_info, &host_info_list);
    if (status != 0) {
      perror("Cannot get address info for host");
    }

    server_fd = socket(host_info_list->ai_family,
                       host_info_list->ai_socktype,
                       host_info_list->ai_protocol);
    if (server_fd == -1) {
      perror("Error: cannot create socket");
    }
  }

  void sender(const char * msg);
  void receiver();
  ~Socket() {
    freeaddrinfo(host_info_list);
    close(server_fd);
  }
};

#endif
