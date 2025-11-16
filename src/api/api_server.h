#ifndef API_SERVER_H
#define API_SERVER_H

#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>

#include <iostream>
#include <memory>

class APIServer {
 private:
  std::shared_ptr<Pistache::Http::Endpoint> endpoint;
  std::shared_ptr<Pistache::Rest::Router> router;
  int port;

 public:
  APIServer(int port = 8080);
  ~APIServer();

  void setupRoutes();
  void start();
  void shutdown();

  void handleGetBooks(const Pistache::Rest::Request& request,
                      Pistache::Http::ResponseWriter response);
};

#endif