#ifndef BOOK_HPP
#define BOOK_HPP

#include <string>

class Book {
public:
    Book(std::string author,
         std::string title,
         int          year,
         std::string genre);

    // getters
    const std::string& author() const;
    const std::string& title()  const;
    int                year()   const;
    const std::string& genre()  const;

    // setters
    void setAuthor(std::string a);
    void setTitle (std::string t);
    void setYear  (int y);
    void setGenre (std::string g);

private:
    std::string m_author;
    std::string m_title;
    int         m_year;
    std::string m_genre;
};

#endif // BOOK_HPP
