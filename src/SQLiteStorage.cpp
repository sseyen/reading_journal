#include "SQLiteStorage.hpp"
#include <sqlite3.h>
#include <stdexcept>

namespace {
    constexpr const char* kCreateTable =
        "CREATE TABLE IF NOT EXISTS books("
        "author TEXT, title TEXT, year INTEGER, genre TEXT);";
}

SQLiteStorage::SQLiteStorage() = default;
SQLiteStorage::~SQLiteStorage() { close(); }

void SQLiteStorage::open(const std::string& path) {
    if (sqlite3_open(path.c_str(), &m_db) != SQLITE_OK)
        throw std::runtime_error("Cannot open DB: " + std::string(sqlite3_errmsg(m_db)));
    ensureSchema();
}

void SQLiteStorage::close() {
    if (m_db) {
        sqlite3_close(m_db);
        m_db = nullptr;
    }
}

void SQLiteStorage::ensureSchema() {
    char* err = nullptr;
    if (sqlite3_exec(m_db, kCreateTable, nullptr, nullptr, &err) != SQLITE_OK) {
        std::string msg(err);
        sqlite3_free(err);
        throw std::runtime_error("Schema error: " + msg);
    }
}

void SQLiteStorage::save(const std::vector<Book>& books, const std::string& path) {
    open(path);
    char* err = nullptr;
    sqlite3_exec(m_db, "BEGIN TRANSACTION;", nullptr, nullptr, &err);

    sqlite3_exec(m_db, "DELETE FROM books;", nullptr, nullptr, &err);

    const char* kInsert = "INSERT INTO books(author,title,year,genre) VALUES(?,?,?,?);";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(m_db, kInsert, -1, &stmt, nullptr);

    for (const auto& b : books) {
        sqlite3_bind_text (stmt, 1, b.author().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text (stmt, 2, b.title().c_str(),  -1, SQLITE_TRANSIENT);
        sqlite3_bind_int  (stmt, 3, b.year());
        sqlite3_bind_text (stmt, 4, b.genre().c_str(),  -1, SQLITE_TRANSIENT);
        sqlite3_step(stmt);
        sqlite3_reset(stmt);
    }
    sqlite3_finalize(stmt);
    sqlite3_exec(m_db, "COMMIT;", nullptr, nullptr, &err);
    close();
}

std::vector<Book> SQLiteStorage::load(const std::string& path) {
    open(path);
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
    close();
    return res;
}
