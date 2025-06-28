#ifndef SQLITE_STORAGE_HPP
#define SQLITE_STORAGE_HPP

#include "Storage.hpp"

struct sqlite3;

class SQLiteStorage final : public Storage {
public:
    SQLiteStorage();
    ~SQLiteStorage() override;

    void              save(const std::vector<Book>&, const std::string&) override;
    std::vector<Book> load(const std::string&) override;

private:
    void open(const std::string& path);
    void close();
    void ensureSchema();

    sqlite3* m_db { nullptr };
};

#endif // SQLITE_STORAGE_HPP
