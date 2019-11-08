#ifndef SERVER_H
#define SERVER_H

#include "Socket.hpp"

class Server : public virtual Socket {
 protected:
  int yes;
  struct sockaddr_storage socket_addr;
  int client_fd;
  socklen_t socket_addr_len;
  char server_buffer[1024] = {0};

 public:
  Server() {
    yes = 1;
    socket_addr_len = sizeof(socket_addr);
  }

  void ServerInit(const char * port_num) {
    hostname = NULL;
    port = port_num;
    host_info.ai_flags = AI_PASSIVE;
    Socket::init();

    status = setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    status = bind(server_fd, host_info_list->ai_addr, host_info_list->ai_addrlen);
    if (status == -1) {
      perror("cannot bind socket");
    }

    status = listen(server_fd, 100);
    if (status == -1) {
      perror("Error: cannot listen on socket");
    }

    std::cout << "Waiting for connection on port " << port << std::endl;

    client_fd = accept(server_fd, (struct sockaddr *)&socket_addr, &socket_addr_len);
    if (client_fd == -1) {
      perror("Error: cannot accept connection on socket");
    }

    std::cout << "Server: successfully accepted\n";
  }

  void receiver() {
    int valread = recv(client_fd, server_buffer, 1024, 0);
    std::cout << "Server received " << valread << " bytes: " << server_buffer
              << std::endl;
  }
  void sender(const char * msg) { send(client_fd, msg, strlen(msg), 0); }
};

#endif
