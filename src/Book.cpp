#include "Book.hpp"

Book::Book(std::string a, std::string t, int y, std::string g)
    : m_author(std::move(a)), m_title(std::move(t)), m_year(y), m_genre(std::move(g)) {}

const std::string& Book::author() const { return m_author; }
const std::string& Book::title()  const { return m_title;  }
int                Book::year()   const { return m_year;   }
const std::string& Book::genre()  const { return m_genre;  }

void Book::setAuthor(std::string a) { m_author = std::move(a); }
void Book::setTitle (std::string t) { m_title  = std::move(t); }
void Book::setYear  (int y)         { m_year   = y; }
void Book::setGenre (std::string g) { m_genre  = std::move(g); }
