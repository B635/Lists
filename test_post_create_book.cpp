#include <iostream>
#include <string>

#include <curl/curl.h>

#include "proto_generated/book_api.pb.h"

int main() {
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    lists::book::api::CreateBookRequest req;
    req.set_title("The C++ Programming Language");
    req.set_cover_image_url("https://example.com/cpp-book.jpg");
    req.set_author("Bjarne Stroustrup");

    google::protobuf::Timestamp* start = req.mutable_start_date();
    start->set_seconds(1700000000);
    start->set_nanos(0);

    google::protobuf::Timestamp* end = req.mutable_end_date();
    end->set_seconds(1700864000);
    end->set_nanos(0);

    req.set_rating(lists::book::RATING_5);
    req.set_note("Test create book via HTTP POST");

    std::string payload;
    if (!req.SerializeToString(&payload)) {
        std::cerr << "Failed to serialize CreateBookRequest" << std::endl;
        return 1;
    }

    std::cout << "Serialized CreateBookRequest size: " << payload.size() << " bytes\n";

    CURLcode global_init_res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (global_init_res != CURLE_OK) {
        std::cerr << "curl_global_init() failed: "
                  << curl_easy_strerror(global_init_res) << std::endl;
        return 1;
    }

    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "curl_easy_init() failed" << std::endl;
        curl_global_cleanup();
        return 1;
    }

    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/x-protobuf");
    headers = curl_slist_append(headers, "Accept: application/x-protobuf");

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8080/api/books");

    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.data());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, static_cast<long>(payload.size()));


    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: "
                  << curl_easy_strerror(res) << std::endl;
    } else {
        std::cout << "\nPOST /api/books success\n";
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}