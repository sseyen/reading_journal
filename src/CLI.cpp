#include "CLI.hpp"
#include "Journal.hpp"
#include <iostream>
#include <limits>
#include <sstream>
#include <filesystem>

static std::string ask(const char* prompt) {
    std::cout << prompt;
    std::string s;
    std::getline(std::cin, s);
    return s;
}

std::optional<int> CLI::readInt(const char* prompt) {
    std::string s = ask(prompt);
    if (s.empty()) return std::nullopt;
    std::istringstream iss(s);
    int v; return (iss >> v) ? std::optional<int>{v} : std::nullopt;
}

void CLI::cmdAdd() {
    std::string author = ask("Author: ");
    std::string title  = ask("Title : ");
    auto yearOpt       = readInt("Year  : ");
    if (!yearOpt) { std::cout << "Invalid year\n"; return; }
    std::string genre  = ask("Genre : ");

    m_journal.addBook(Book(author, title, *yearOpt, genre));
    std::cout << "Book added!\n";
}

void CLI::cmdList() const {
    auto list = m_journal.books();
    if (list.empty()) { std::cout << "Journal is empty\n"; return; }
    for (const auto& b : list)
        std::cout << b.author() << " — " << b.title() << " (" << b.year()
                  << ") [" << b.genre() << "]\n";
}

void CLI::cmdSearchAuthor() const {
    std::string author = ask("Search author: ");
    auto res = m_journal.findByAuthor(author);
    if (res.empty()) { std::cout << "No matches\n"; return; }
    for (const auto& b : res)
        std::cout << b.title() << " (" << b.year() << ")\n";
}

void CLI::cmdSearchTitle() const {
    std::string title = ask("Search title : ");
    auto res = m_journal.findByTitle(title);
    if (res.empty()) { std::cout << "No matches\n"; return; }
    for (const auto& b : res)
        std::cout << b.author() << " (" << b.year() << ")\n";
}

void CLI::cmdDelete() {
    std::string title = ask("Title: ");
    m_journal.deleteBook(title);
    std::cout << "Book deleted!\n";
}

void CLI::cmdCountByAuthor()
{
    std::string author = ask("Author: ");
    auto res = m_journal.findByAuthor(author);
    std::cout << "Author has " << res.size() << " books." << "\n";
}

void CLI::showMenu() const {
    std::cout << "\n==== Reading Journal ====\n"
              << "1. Add book\n"
              << "2. List all\n"
              << "3. Search by author\n"
              << "4. Search by title\n"
              << "5. Delete book\n"
              << "6. Number of Author's books\n"
              << "0. Exit\n> ";
}

void CLI::run() {
    namespace fs = std::filesystem;
    fs::create_directories("data");

    if (fs::is_empty("data")) {
        Journal tmp;
        tmp.open("data/db.sqlite");
    }

    std::string name = ask("Database name (stored in data/): ");
    if (name.empty())
        name = "db.sqlite";

    std::string path = (name.find('/') == std::string::npos)
                       ? std::string("data/") + name
                       : name;
    bool exists = fs::exists(path);
    m_journal.open(path);
    std::cout << "Database " << name << (exists ? " opened" : " created") << "\n";
    while (true) {
        showMenu();
        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        switch (choice) {
            case 1: cmdAdd(); break;
            case 2: cmdList(); break;
            case 3: cmdSearchAuthor(); break;
            case 4: cmdSearchTitle(); break;
            case 5: cmdDelete(); break;
            case 6: cmdCountByAuthor(); break;
            case 0: return;
            default: std::cout << "Unknown option\n";
        }
    }
}
