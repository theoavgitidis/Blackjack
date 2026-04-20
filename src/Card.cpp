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

