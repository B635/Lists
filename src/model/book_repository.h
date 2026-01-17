#pragma once

#include <optional>
#include <string>
#include <pqxx/pqxx>

#include "book.h"

class BookRepository {
public:
    explicit BookRepository(const std::string& conn_str);

    Book createBook(const Book& b);
    std::optional<Book> getBookById
};