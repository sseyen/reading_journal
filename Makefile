CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -Iinclude
LDFLAGS  = -lsqlite3
TARGET   = reading_journal

SRC  := $(wildcard src/*.cpp)
OBJ  := $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(TARGET)

re: fclean all

.PHONY: all clean fclean re
