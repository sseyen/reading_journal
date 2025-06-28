#ifndef STORAGE_HPP
#define STORAGE_HPP

#include <string>
#include <vector>
#include "Book.hpp"

class Storage {
public:
    virtual ~Storage() = default;
    virtual void              save(const std::vector<Book>& books,
                                   const std::string& file) = 0;
    virtual std::vector<Book> load(const std::string& file) = 0;
};

#endif // STORAGE_HPP
