#include "api_server.h"

APIServer::APIServer(int port)
    : port(port),
      endpoint(std::make_shared<Pistache::Http::Endpoint>(
          Pistache::Address(Pistache::Ipv4::any(), port))) {
  router = std::make_shared<Pistache::Rest::Router>();
}

APIServer::~APIServer() = default;

void APIServer::handleGetBooks(const Pistache::Rest::Request& request,
                               Pistache::Http::ResponseWriter response) {
  std::cout << "📍 GET /api/books" << std::endl;
  response.send(Pistache::Http::Code::Ok, "Books: []");
}

void APIServer::setupRoutes() {
  using namespace Pistache::Rest;

  Routes::Get(*router, "/api/books",
              Routes::bind(&APIServer::handleGetBooks, this));
  
    endpoint->setHandler(router->handler());
}

void APIServer::start() {
  std::cout << "\n========================================" << std::endl;
  std::cout << "🚀 API Server Starting..." << std::endl;
  std::cout << "========================================" << std::endl;

  setupRoutes();
  auto opts =
      Pistache::Http::Endpoint::options().threads(4).maxRequestSize(4096);
  endpoint->init(opts);
  std::cout << "📌 Listening on http://0.0.0.0:" << port << std::endl;
  std::cout << "📌 Available endpoints:" << std::endl;
  std::cout << "   GET    /api/books" << std::endl;
  std::cout << "   GET    /api/books/:id" << std::endl;
  std::cout << "   POST   /api/books" << std::endl;
  std::cout << "========================================\n" << std::endl;
  endpoint->serve();
}

void APIServer::shutdown() {
  if (endpoint) {
    endpoint->shutdown();
  }
}