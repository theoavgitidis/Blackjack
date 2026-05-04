#pragma once

#include <iostream>
#include <sstream>
#include <streambuf>

namespace blackjack_test {

// Richtet std::cin auf einen Stringstream um (nur sequentielle Tests, kein Parallelismus).
class CinStringbufGuard {
public:
    explicit CinStringbufGuard(std::istream& replacement)
        : old_(std::cin.rdbuf(replacement.rdbuf())) {}

    ~CinStringbufGuard() { std::cin.rdbuf(old_); }

    CinStringbufGuard(const CinStringbufGuard&) = delete;
    CinStringbufGuard& operator=(const CinStringbufGuard&) = delete;

private:
    std::streambuf* old_;
};

class CoutStringbufGuard {
public:
    explicit CoutStringbufGuard(std::ostream& replacement)
        : old_(std::cout.rdbuf(replacement.rdbuf())) {}

    ~CoutStringbufGuard() { std::cout.rdbuf(old_); }

    CoutStringbufGuard(const CoutStringbufGuard&) = delete;
    CoutStringbufGuard& operator=(const CoutStringbufGuard&) = delete;

private:
    std::streambuf* old_;
};

} // namespace blackjack_test
