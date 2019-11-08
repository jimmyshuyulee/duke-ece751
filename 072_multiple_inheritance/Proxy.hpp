#ifndef PROXY_H
#define PROXY_H

#include "Client.hpp"
#include "Server.hpp"

class Proxy : public Server, public Client {
 public:
  Proxy() {}
  void ProxyInit(const char * client_conn_port,
                 const char * server_host,
                 const char * server_conn_port) {
    Server::ServerInit(client_conn_port);
    std::cout << "Proxy connectd to client at port " << client_conn_port << std::endl;
    Client::ClientInit(server_host, server_conn_port);
    std::cout << "Proxy connected to server at port " << server_conn_port << std::endl;
  }

  void from_client_to_server() {
    int valread = recv(client_fd, server_buffer, 1024, 0);
    std::cout << "Proxy received " << valread << " bytes from client: " << server_buffer
              << std::endl;
    send(server_fd, server_buffer, strlen(server_buffer), 0);
  }

  void from_server_to_client() {
    int valread = recv(server_fd, client_buffer, 1024, 0);
    std::cout << "Proxy received " << valread << " bytes from server: " << client_buffer
              << std::endl;
    send(client_fd, client_buffer, strlen(client_buffer), 0);
  }
};

#endif
