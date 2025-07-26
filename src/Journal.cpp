#include "Journal.hpp"
#include <stdexcept>

Journal::~Journal() {
    if (m_db)
        sqlite3_close(m_db);
}

void Journal::open(const std::string& path) {
    if (sqlite3_open(path.c_str(), &m_db) != SQLITE_OK)
        throw std::runtime_error("Cannot open DB: " + std::string(sqlite3_errmsg(m_db)));
    ensureSchema();
}

void Journal::ensureSchema() {
    const char* kCreate = "CREATE TABLE IF NOT EXISTS books(" \
                         "author TEXT, title TEXT, year INTEGER, genre TEXT);";
    char* err = nullptr;
    if (sqlite3_exec(m_db, kCreate, nullptr, nullptr, &err) != SQLITE_OK) {
        std::string msg(err);
        sqlite3_free(err);
        throw std::runtime_error("Schema error: " + msg);
    }
}

void Journal::addBook(const Book& b) {
    const char* kInsert = "INSERT INTO books(author,title,year,genre) VALUES(?,?,?,?);";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(m_db, kInsert, -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, b.author().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, b.title().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, b.year());
    sqlite3_bind_text(stmt, 4, b.genre().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

std::vector<Book> Journal::books() const {
    const char* kSelect = "SELECT author,title,year,genre FROM books;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(m_db, kSelect, -1, &stmt, nullptr);
    std::vector<Book> res;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string author(reinterpret_cast<const char*>(sqlite3_column_text(stmt,0)));
        std::string title (reinterpret_cast<const char*>(sqlite3_column_text(stmt,1)));
        int         year  = sqlite3_column_int(stmt,2);
        std::string genre (reinterpret_cast<const char*>(sqlite3_column_text(stmt,3)));
        res.emplace_back(author, title, year, genre);
    }
    sqlite3_finalize(stmt);
    return res;
}

std::vector<Book> Journal::findByAuthor(const std::string& a) const {
    const char* kSelect = "SELECT author,title,year,genre FROM books WHERE author=?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(m_db, kSelect, -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, a.c_str(), -1, SQLITE_TRANSIENT);
    std::vector<Book> res;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string author(reinterpret_cast<const char*>(sqlite3_column_text(stmt,0)));
        std::string title (reinterpret_cast<const char*>(sqlite3_column_text(stmt,1)));
        int         year  = sqlite3_column_int(stmt,2);
        std::string genre (reinterpret_cast<const char*>(sqlite3_column_text(stmt,3)));
        res.emplace_back(author, title, year, genre);
    }
    sqlite3_finalize(stmt);
    return res;
}

std::vector<Book> Journal::findByTitle(const std::string& t) const {
    const char* kSelect = "SELECT author,title,year,genre FROM books WHERE title=?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(m_db, kSelect, -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, t.c_str(), -1, SQLITE_TRANSIENT);
    std::vector<Book> res;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string author(reinterpret_cast<const char*>(sqlite3_column_text(stmt,0)));
        std::string title (reinterpret_cast<const char*>(sqlite3_column_text(stmt,1)));
        int         year  = sqlite3_column_int(stmt,2);
        std::string genre (reinterpret_cast<const char*>(sqlite3_column_text(stmt,3)));
        res.emplace_back(author, title, year, genre);
    }
    sqlite3_finalize(stmt);
    return res;
}

void Journal::deleteBook(const std::string& title) {
    const char* kDelete = "DELETE FROM books WHERE title=?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(m_db, kDelete, -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, title.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

// int Journal::countByAuthor(const std::vector<Book>& b) const {
//     int counter = 0;

//     for (Book book : b)
//         counter++;
//     return counter;
// }

