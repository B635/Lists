#ifndef API_SERVER_H
#define API_SERVER_H

#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>

#include <iostream>
#include <memory>
#include <vector>

#include "model/book.h"
#include "book_api.pb.h"

class APIServer {
private:
    std::shared_ptr<Pistache::Http::Endpoint> endpoint;
    std::shared_ptr<Pistache::Rest::Router> router;
    int port;

    std::vector<Book> books_;
    int64_t next_id = 1;


public: 
    APIServer(int port = 8080);
    ~APIServer();

    void setupRoutes();
    void start();
    void shutdown();

    void handleGetBooks(const Pistache::Rest::Request& request,
        Pistache::Http::ResponseWriter response);
    
    void handleGetBook(const Pistache::Rest::Request& request,
                       Pistache::Http::ResponseWriter response);

    void handleCreateBook(const Pistache::Rest::Request& request,
                          Pistache::Http::ResponseWriter response);

    void handleUpdateBook(const Pistache::Rest::Request& request,
                          Pistache::Http::ResponseWriter response);

};

#endif