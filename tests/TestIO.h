#pragma once

#include <ios>
#include <iostream>
#include <sstream>
#include <string>

class ScopedInput {
public:
    explicit ScopedInput(std::string input) : input_(std::move(input)), old_(std::cin.rdbuf(input_.rdbuf())) {}
    ~ScopedInput() { std::cin.rdbuf(old_); }

    ScopedInput(const ScopedInput&) = delete;
    ScopedInput& operator=(const ScopedInput&) = delete;

private:
    std::istringstream input_;
    std::streambuf* old_;
};

class ScopedOutput {
public:
    ScopedOutput() : old_(std::cout.rdbuf(output_.rdbuf())) {}
    ~ScopedOutput() { std::cout.rdbuf(old_); }

    ScopedOutput(const ScopedOutput&) = delete;
    ScopedOutput& operator=(const ScopedOutput&) = delete;

    std::string str() const { return output_.str(); }

private:
    std::ostringstream output_;
    std::streambuf* old_;
};
