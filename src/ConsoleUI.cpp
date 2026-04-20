#include "ConsoleUI.h"

#include <algorithm>
#include <cctype>
#include <iostream>

void ConsoleUI::showWelcome() const {
    std::cout << "=====================================\n";
    std::cout << "          BLACKJACK (Konsole)         \n";
    std::cout << "=====================================\n\n";
}

bool ConsoleUI::promptStartGame() const {
    while (true) {
        std::cout << "Moechtest du eine Runde Blackjack starten? (j/n): ";
        const auto in = toLowerAscii(readLineTrimmed());
        if (in == "j" || in == "ja") return true;
        if (in == "n" || in == "nein") return false;
        std::cout << "Ungueltige Eingabe. Bitte 'j' oder 'n' eingeben.\n";
    }
}

void ConsoleUI::showHands(const Player& player, const Dealer& dealer, bool hideDealersSecondCard) const {
    std::cout << "\n--- Aktueller Stand ---\n";
    std::cout << "Dealer:  " << dealer.hand().toString(hideDealersSecondCard);
    if (!hideDealersSecondCard) {
        std::cout << "  (Punkte: " << dealer.score() << ")";
    }
    std::cout << "\n";

    std::cout << "Spieler: " << player.hand().toString(false)
              << "  (Punkte: " << player.score() << ")\n";
    std::cout << "-----------------------\n\n";
}

void ConsoleUI::showPlayerStatus(const Player& player) const {
    std::cout << "Spieler-Hand: " << player.hand().toString(false)
              << "  (Punkte: " << player.score() << ")\n";
}

void ConsoleUI::showDealerStatus(const Dealer& dealer) const {
    std::cout << "Dealer-Hand:  " << dealer.hand().toString(false)
              << "  (Punkte: " << dealer.score() << ")\n";
}

ConsoleUI::PlayerAction ConsoleUI::promptPlayerAction() const {
    // Struktur bewusst als Enum, damit später leicht um Aktionen wie "Double Down" erweiterbar.
    while (true) {
        std::cout << "Aktion waehlen: (1) Karte ziehen  (2) Passen : ";
        const auto in = readLineTrimmed();

        if (in == "1") return PlayerAction::Hit;
        if (in == "2") return PlayerAction::Stand;

        // Auch Text-Eingaben tolerieren (REQ-F-014).
        const auto lowered = toLowerAscii(in);
        if (lowered == "hit" || lowered == "ziehen" || lowered == "karte" || lowered == "karte ziehen") {
            return PlayerAction::Hit;
        }
        if (lowered == "stand" || lowered == "passen") {
            return PlayerAction::Stand;
        }

        std::cout << "Ungueltige Eingabe. Bitte 1/2 oder 'ziehen'/'passen' eingeben.\n";
    }
}

bool ConsoleUI::promptPlayAgain() const {
    while (true) {
        std::cout << "Noch eine Runde spielen? (j/n): ";
        const auto in = toLowerAscii(readLineTrimmed());
        if (in == "j" || in == "ja") return true;
        if (in == "n" || in == "nein") return false;
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

std::string ConsoleUI::toLowerAscii(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });
    return s;
}

