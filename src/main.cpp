#include "Journal.hpp"
#include "CLI.hpp"

int main() {
    Journal journal;
    CLI     cli(journal);
    cli.run();
    return 0;
}
