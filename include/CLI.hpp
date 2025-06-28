#ifndef CLI_HPP
#define CLI_HPP

#include <optional>
class Journal;

class CLI {
public:
    explicit CLI(Journal& j) : m_journal(j) {}
    void run();

private:
    Journal& m_journal;

    void showMenu() const;
    void cmdAdd();
    void cmdList() const;
    void cmdSearchAuthor() const;
    void cmdSearchTitle()  const;

    static std::optional<int> readInt(const char* prompt);
};

#endif // CLI_HPP
