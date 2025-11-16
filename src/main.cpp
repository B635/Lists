#include <iostream>
#include "api/api_server.h"


APIServer* g_server = nullptr;


void signal_handler(int signum) {
  if (g_server) {
    g_server->shutdown();
  }
  exit(signum);
}


int main() {
  APIServer server(8080);
  g_server = &server;
  signal(SIGINT, signal_handler);
  signal(SIGTERM, signal_handler);
  
  
  server.start();
  std::cout << "PosterDB started!" << std::endl;
  return 0;
}