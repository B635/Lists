#pragma once

#include <string>
#include <optional>
#include <chrono>
#include "book.pb.h"

struct Book {
    int64_t id;
    std::string title;
    std::optional<std::string> cover_image_url;
    std::optional<std::string> author;
    std::optional<std::chrono::system_clock::time_point> start_date;
    std::optional<std::chrono::system_clock::time_point> end_date;
    std::optional<int> rating;
    std::optional<std::string> note;
};

Book fromProto(const lists::book::Book& pb);
lists::book::Book toProto(const Book& b);
