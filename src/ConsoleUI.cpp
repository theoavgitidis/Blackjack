#include "ConsoleUI.h"

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <vector>
#include <sstream>

namespace {

void printHandAscii(const Hand& hand, bool hideSecondCard) {
    const auto& cards = hand.cards();
    if (cards.empty()) {
        std::cout << "(keine Karten)\n";
        return;
    }

    std::vector<std::vector<std::string>> renderedCards;
    renderedCards.reserve(cards.size());

    for (std::size_t i = 0; i < cards.size(); ++i) {
        if (hideSecondCard && i == 1) {
            renderedCards.push_back(Card::backAsciiArt());
        } else {
            renderedCards.push_back(cards[i].toAsciiArt());
        }
    }

    const std::size_t height = renderedCards.front().size();
    for (std::size_t row = 0; row < height; ++row) {
        for (std::size_t cardIndex = 0; cardIndex < renderedCards.size(); ++cardIndex) {
            if (cardIndex > 0) std::cout << " ";
            std::cout << renderedCards[cardIndex][row];
        }
        std::cout << "\n";
    }
}

} // anonymous namespace

void ConsoleUI::showWelcome() const {
    std::cout << "=====================================\n";
    std::cout << "          BLACKJACK (Konsole)         \n";
    std::cout << "=====================================\n\n";
}

bool ConsoleUI::promptStartGame() const {
    while (true) {
        std::cout << "Moechtest du eine Runde Blackjack starten? (j/n): ";
        const auto input = toLowerAscii(readLineTrimmed());
        if (input == "j" || input == "ja") return true;
        if (input == "n" || input == "nein") return false;
        std::cout << "Ungueltige Eingabe. Bitte 'j' oder 'n' eingeben.\n";
    }
}

void ConsoleUI::showHands(const Player& player, const Dealer& dealer, bool hideDealersSecondCard) const {
    std::cout << "\n--- Aktueller Stand ---\n";
    std::cout << "Dealer:\n";
    printHandAscii(dealer.hand(), hideDealersSecondCard);
    if (!hideDealersSecondCard) std::cout << "Punkte: " << dealer.score() << "\n";

    std::cout << "\nSpieler:\n";
    printHandAscii(player.hand(), false);
    std::cout << "Punkte: " << player.score() << "\n";
    std::cout << "-----------------------\n\n";
}

void ConsoleUI::showPlayerStatus(const Player& player) const {
    std::cout << "Spieler:\n";
    printHandAscii(player.hand(), false);
    std::cout << "Punkte: " << player.score() << "\n";
}

void ConsoleUI::showDealerStatus(const Dealer& dealer) const {
    std::cout << "Dealer:\n";
    printHandAscii(dealer.hand(), false);
    std::cout << "Punkte: " << dealer.score() << "\n";
}

long long ConsoleUI::promptBetCents(long long bankrollCents) const {
    // Bet is requested in whole EUR and stored internally as cents.
    // This ensures the 1.5x payout (Blackjack) can be exactly represented in cents.
    constexpr long long kCentsPerEuro = 100;
    constexpr int kBase10 = 10;

    while (true) {
        std::cout << "Dein Kontostand: " << formatMoney(bankrollCents) << "\n";
        std::cout << "Einsatz in EUR waehlen (z.B. 10). Mindest 1, maximal " << (bankrollCents / kCentsPerEuro) << ": ";

        const auto input = readLineTrimmed();
        if (input.empty()) {
            std::cout << "Ungueltige Eingabe. Bitte eine Zahl eingeben.\n";
            continue;
        }

        try {
            std::size_t pos = 0;
            const long long euros = std::stoll(input, &pos, kBase10);
            if (pos != input.size()) {
                std::cout << "Ungueltige Eingabe. Bitte nur Zahlen eingeben.\n";
                continue;
            }
            if (euros <= 0) {
                std::cout << "Der Einsatz muss mindestens 1 EUR sein.\n";
                continue;
            }
            const long long betCents = euros * kCentsPerEuro;
            if (betCents > bankrollCents) {
                std::cout << "Du hast nicht genug Guthaben fuer diesen Einsatz.\n";
                continue;
            }
            return betCents;
        } catch (...) {
            std::cout << "Ungueltige Eingabe. Bitte eine gueltige Zahl eingeben.\n";
        }
    }
}

void ConsoleUI::showBankroll(long long bankrollCents) const {
    std::cout << "Kontostand: " << formatMoney(bankrollCents) << "\n";
}

ConsoleUI::PlayerAction ConsoleUI::promptPlayerAction() const {
    // The structure is intentionally an enum so it can be easily extended with actions like "Double Down" later.
    while (true) {
        std::cout << "Aktion waehlen: (1) Karte ziehen  (2) Passen  (3) Double Down : ";
        const auto input = readLineTrimmed();

        if (input == "1") return PlayerAction::Hit;
        if (input == "2") return PlayerAction::Stand;
        if (input == "3") return PlayerAction::DoubleDown;

        // Tolerate text inputs as well (REQ-F-014).
        const auto lowerInput = toLowerAscii(input);
        if (lowerInput == "hit" || lowerInput == "ziehen" || lowerInput == "karte" || lowerInput == "karte ziehen") {
            return PlayerAction::Hit;
        }
        if (lowerInput == "stand" || lowerInput == "passen") {
            return PlayerAction::Stand;
        }
        if (lowerInput == "double" || lowerInput == "doubledown" || lowerInput == "double down") {
            return PlayerAction::DoubleDown;
        }

        std::cout << "Ungueltige Eingabe. Bitte 1/2/3 eingeben.\n";
    }
}

bool ConsoleUI::promptPlayAgain() const {
    while (true) {
        std::cout << "Noch eine Runde spielen? (j/n): ";
        const auto input = toLowerAscii(readLineTrimmed());
        if (input == "j" || input == "ja") return true;
        if (input == "n" || input == "nein") return false;
        std::cout << "Ungueltige Eingabe. Bitte 'j' oder 'n' eingeben.\n";
    }
}

void ConsoleUI::showRoundResult(const std::string& resultText) const {
    std::cout << "\n=== Ergebnis ===\n" << resultText << "\n================\n\n";
}

void ConsoleUI::showInfo(const std::string& text) const {
    std::cout << text << "\n";
}

std::string ConsoleUI::readLineTrimmed() {
    std::string line;
    std::getline(std::cin, line);

    auto isSpace = [](unsigned char ch) { return std::isspace(ch) != 0; };
    while (!line.empty() && isSpace(static_cast<unsigned char>(line.front()))) line.erase(line.begin());
    while (!line.empty() && isSpace(static_cast<unsigned char>(line.back()))) line.pop_back();
    return line;
}

std::string ConsoleUI::toLowerAscii(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });
    return str;
}

std::string ConsoleUI::formatMoney(long long cents) {
    constexpr long long kCentsPerEuro = 100;
    
    const long long absoluteCents = cents < 0 ? -cents : cents;
    const long long euros = absoluteCents / kCentsPerEuro;
    const long long remainder = absoluteCents % kCentsPerEuro;

    std::ostringstream moneyStream;
    if (cents < 0) {
        moneyStream << "-";
    }
    moneyStream << euros << "." << std::setw(2) << std::setfill('0') << remainder << " EUR";
    return moneyStream.str();
}
