#include "Card.h"

#include <stdexcept>

Card::Card(Suit suit, Rank rank) : suit_(suit), rank_(rank) {}

Card::Suit Card::suit() const noexcept { return suit_; }
Card::Rank Card::rank() const noexcept { return rank_; }

int Card::baseValue() const noexcept {
    switch (rank_) {
        case Rank::Bube:
        case Rank::Dame:
        case Rank::Koenig:
            return 10;
        case Rank::Ass:
            return 11;
        default:
            return static_cast<int>(rank_);
    }
}

namespace {

std::string getSuitName(Card::Suit suit) {
    switch (suit) {
        case Card::Suit::Kreuz: return "Kreuz";
        case Card::Suit::Pik:   return "Pik";
        case Card::Suit::Herz:  return "Herz";
        case Card::Suit::Karo:  return "Karo";
    }
    throw std::logic_error("Unknown card suit");
}

std::string getRankName(Card::Rank rank) {
    switch (rank) {
        case Card::Rank::Zwei:   return "2";
        case Card::Rank::Drei:   return "3";
        case Card::Rank::Vier:   return "4";
        case Card::Rank::Fuenf:  return "5";
        case Card::Rank::Sechs:  return "6";
        case Card::Rank::Sieben: return "7";
        case Card::Rank::Acht:   return "8";
        case Card::Rank::Neun:   return "9";
        case Card::Rank::Zehn:   return "10";
        case Card::Rank::Bube:   return "Bube";
        case Card::Rank::Dame:   return "Dame";
        case Card::Rank::Koenig: return "Koenig";
        case Card::Rank::Ass:    return "Ass";
    }
    throw std::logic_error("Unknown card rank");
}

std::string getRankShortName(Card::Rank rank) {
    switch (rank) {
        case Card::Rank::Zwei:   return "2";
        case Card::Rank::Drei:   return "3";
        case Card::Rank::Vier:   return "4";
        case Card::Rank::Fuenf:  return "5";
        case Card::Rank::Sechs:  return "6";
        case Card::Rank::Sieben: return "7";
        case Card::Rank::Acht:   return "8";
        case Card::Rank::Neun:   return "9";
        case Card::Rank::Zehn:   return "10";
        case Card::Rank::Bube:   return "J";
        case Card::Rank::Dame:   return "Q";
        case Card::Rank::Koenig: return "K";
        case Card::Rank::Ass:    return "A";
    }
    throw std::logic_error("Unknown card rank");
}

std::string getSuitSymbol(Card::Suit suit) {
    // UTF-8 suit symbols (assumes the console is set to UTF-8).
    switch (suit) {
        case Card::Suit::Pik:   return "\u2660"; // ♠
        case Card::Suit::Herz:  return "\u2665"; // ♥
        case Card::Suit::Karo:  return "\u2666"; // ♦
        case Card::Suit::Kreuz: return "\u2663"; // ♣
    }
    throw std::logic_error("Unknown card suit");
}

std::string padRight(const std::string& str, std::size_t width) {
    if (str.size() >= width) return str;
    return str + std::string(width - str.size(), ' ');
}

std::string padLeft(const std::string& str, std::size_t width) {
    if (str.size() >= width) return str;
    return std::string(width - str.size(), ' ') + str;
}

std::string repeatUtf8(const char* token, std::size_t count) {
    std::string out;
    for (std::size_t i = 0; i < count; ++i) {
        out += token;
    }
    return out;
}

} // anonymous namespace

std::string Card::toString() const {
    return getRankName(rank_) + " " + getSuitName(suit_);
}

std::vector<std::string> Card::toAsciiArt() const {
    // Format (width 9, height 5):
    // ┌───────┐
    // │A      │
    // │   ♠   │
    // │      A│
    // └───────┘
    const std::string shortRank = getRankShortName(rank_);
    const std::string symbol = getSuitSymbol(suit_);

    constexpr std::size_t kInnerWidth = 7;
    constexpr std::size_t kPadding = 3;

    const std::string leftAligned = padRight(shortRank, 2);   // "A " or "10"
    const std::string rightAligned = padLeft(shortRank, 2);   // " A" or "10"

    std::vector<std::string> lines;
    lines.reserve(5);
    
    const std::string topBorder = std::string("\u250C") + repeatUtf8("\u2500", kInnerWidth) + "\u2510";
    const std::string bottomBorder = std::string("\u2514") + repeatUtf8("\u2500", kInnerWidth) + "\u2518";

    lines.push_back(topBorder);
    lines.push_back(std::string("\u2502") + padRight(leftAligned, kInnerWidth) + "\u2502");
    lines.push_back(std::string("\u2502") + std::string(kPadding, ' ') + symbol + std::string(kPadding, ' ') + "\u2502");
    lines.push_back(std::string("\u2502") + padLeft(rightAligned, kInnerWidth) + "\u2502");
    lines.push_back(bottomBorder);
    
    return lines;
}

std::vector<std::string> Card::backAsciiArt() {
    constexpr std::size_t kInnerWidth = 7;
    const std::string topBorder = std::string("\u250C") + repeatUtf8("\u2500", kInnerWidth) + "\u2510";
    const std::string bottomBorder = std::string("\u2514") + repeatUtf8("\u2500", kInnerWidth) + "\u2518";
    const std::string patternRow = std::string("\u2502") + "#######" + "\u2502";

    return {
        topBorder,
        patternRow,
        patternRow,
        patternRow,
        bottomBorder
    };
}
