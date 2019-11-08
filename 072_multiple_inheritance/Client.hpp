#ifndef CLIENT_H
#define CLIENT_H

#include "Socket.hpp"

class Client : public virtual Socket {
 protected:
  char client_buffer[1024] = {0};

 public:
  Client() {}

  void ClientInit(const char * host_name, const char * port_num) {
    hostname = host_name;
    port = port_num;
    Socket::init();

    std::cout << "Connecting to " << hostname << " on port " << port << "..."
              << std::endl;

    status = connect(server_fd, host_info_list->ai_addr, host_info_list->ai_addrlen);
    if (status == -1) {
      perror("Error: cannot connect to socket");
    }

    std::cout << "Client: successfully connected\n";
  }

  void sender(const char * msg) { send(server_fd, msg, strlen(msg), 0); }
  void receiver() {
    int valread = recv(server_fd, client_buffer, 1024, 0);
    std::cout << "Client received " << valread << " bytes: " << client_buffer
              << std::endl;
  }
};

#endif
