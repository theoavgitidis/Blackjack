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

static std::string suitToGerman(Card::Suit s) {
    switch (s) {
        case Card::Suit::Kreuz: return "Kreuz";
        case Card::Suit::Pik:   return "Pik";
        case Card::Suit::Herz:  return "Herz";
        case Card::Suit::Karo:  return "Karo";
    }
    throw std::logic_error("Unbekannte Kartenfarbe");
}

static std::string rankToGerman(Card::Rank r) {
    switch (r) {
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
    throw std::logic_error("Unbekannter Kartenrang");
}

std::string Card::toString() const {
    return rankToGerman(rank_) + " " + suitToGerman(suit_);
}

static std::string rankToShort(Card::Rank r) {
    switch (r) {
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
    throw std::logic_error("Unbekannter Kartenrang");
}

static std::string suitToSymbol(Card::Suit s) {
    // UTF-8 Suit-Symbole (setzt voraus, dass die Konsole auf UTF-8 steht).
    switch (s) {
        case Card::Suit::Pik:   return "\u2660"; // ♠
        case Card::Suit::Herz:  return "\u2665"; // ♥
        case Card::Suit::Karo:  return "\u2666"; // ♦
        case Card::Suit::Kreuz: return "\u2663"; // ♣
    }
    throw std::logic_error("Unbekannte Kartenfarbe");
}

static std::string padRight(const std::string& s, std::size_t width) {
    if (s.size() >= width) return s;
    return s + std::string(width - s.size(), ' ');
}

static std::string padLeft(const std::string& s, std::size_t width) {
    if (s.size() >= width) return s;
    return std::string(width - s.size(), ' ') + s;
}

static std::string repeatUtf8(const char* token, std::size_t count) {
    std::string out;
    for (std::size_t i = 0; i < count; ++i) out += token;
    return out;
}

std::vector<std::string> Card::toAsciiArt() const {
    // Format (Breite 9, Höhe 5):
    // ┌───────┐
    // │A      │
    // │   ♠   │
    // │      A│
    // └───────┘
    const std::string r = rankToShort(rank_);
    const std::string s = suitToSymbol(suit_);

    // Innenbreite ist 7.
    const std::string left = padRight(r, 2);   // "A " oder "10"
    const std::string right = padLeft(r, 2);   // " A" oder "10"

    std::vector<std::string> lines;
    lines.reserve(5);
    const std::string top    = std::string("\u250C") + repeatUtf8("\u2500", 7) + "\u2510"; // ┌───────┐
    const std::string bottom = std::string("\u2514") + repeatUtf8("\u2500", 7) + "\u2518"; // └───────┘

    lines.push_back(top);
    lines.push_back(std::string("\u2502") + padRight(left, 7) + "\u2502"); // │
    lines.push_back(std::string("\u2502") + std::string(3, ' ') + s + std::string(3, ' ') + "\u2502");
    lines.push_back(std::string("\u2502") + padLeft(right, 7) + "\u2502");
    lines.push_back(bottom);
    return lines;
}

std::vector<std::string> Card::backAsciiArt() {
    return {
        std::string("\u250C") + repeatUtf8("\u2500", 7) + "\u2510",
        std::string("\u2502") + "#######" + "\u2502",
        std::string("\u2502") + "#######" + "\u2502",
        std::string("\u2502") + "#######" + "\u2502",
        std::string("\u2514") + repeatUtf8("\u2500", 7) + "\u2518",
    };
}

