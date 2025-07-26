#ifndef JOURNAL_HPP
#define JOURNAL_HPP

#include <vector>
#include <string>
#include <sqlite3.h>
#include "Book.hpp"

class Journal {
public:
    Journal()  = default;
    ~Journal();

    void open(const std::string& path);

    void addBook(const Book& b);
    std::vector<Book> findByAuthor(const std::string& author) const;
    std::vector<Book> findByTitle (const std::string& title)  const;
    std::vector<Book> books() const;
    void deleteBook(const std::string& title);
    // int countByAuthor(const std::vector<Book>& b) const;

private:
    void ensureSchema();

    sqlite3* m_db { nullptr };
};

#endif // JOURNAL_HPP
