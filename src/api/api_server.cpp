#include "api_server.h"
#include <google/protobuf/timestamp.pb.h>

static Book* findBookById(std::vector<Book>& books, int64_t id) {
    for (auto& b : books) {
        if (b.id == id) return &b;
    }
    return nullptr;
}

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

void APIServer::handleGetBook(const Pistache::Rest::Request& request,
                               Pistache::Http::ResponseWriter response) {
  std::cout << "📍 GET /api/books/:id" << std::endl;
  auto id = request.param(":id").as<int64_t>();
  Book* b = findBookById(books_, id);
  if (!b) {
    response.send(Pistache::Http::Code::Not_Found, "Book not found");
    return;
  }
  lists::book::api::GetBookResponse pb_resp;
  *pb_resp.mutable_book() = toProto(*b);
  
  std::string out;
  pb_resp.SerializeToString(&out);
  auto mime = Pistache::Http::Mime::MediaType::fromString("application/x-protobuf");
  response.send(Pistache::Http::Code::Created, out, mime);
}

void APIServer::handleCreateBook(const Pistache::Rest::Request& request,
                               Pistache::Http::ResponseWriter response) {
  std::cout << "📍 Post /api/books" << std::endl;
  lists::book::api::CreateBookRequest pb_req;
  const std::string& body = request.body();
  if (!pb_req.ParseFromString(body)) {
    response.send(Pistache::Http::Code::Bad_Request, "Invalid protobuf");
    return;
  }

  Book b = makeBookFromCreateRequest(pb_req, next_id++);
  books_.push_back(b);

  lists::book::api::CreateBookResponse pb_resp;
  *pb_resp.mutable_book() = toProto(b);

  std::string out;
  pb_resp.SerializeToString(&out);
  auto mime = Pistache::Http::Mime::MediaType::fromString("application/x-protobuf");
  response.send(Pistache::Http::Code::Created, out, mime);
}

void APIServer::handleUpdateBook(const Pistache::Rest::Request& request,
                               Pistache::Http::ResponseWriter response) {
  std::cout << "📍 Put /api/books/:id" << std::endl;
  response.send(Pistache::Http::Code::Ok, "Update");
}

void APIServer::setupRoutes() {
  using namespace Pistache::Rest;

  Routes::Get(*router, "/api/books",
  Routes::bind(&APIServer::handleGetBooks, this));
  
  Routes::Get(*router, "/api/books/:id",
  Routes::bind(&APIServer::handleGetBook, this));

  Routes::Post(*router, "/api/books",
  Routes::bind(&APIServer::handleCreateBook, this));

  Routes::Put(*router, "/api/books/:id",
  Routes::bind(&APIServer::handleUpdateBook, this));

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