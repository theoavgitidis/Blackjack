#include "ConsoleUI.h"

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <sstream>

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

long long ConsoleUI::promptBetCents(long long bankrollCents) const {
    // Einsatz wird als ganze EUR abgefragt und intern als Cent gespeichert.
    // So bleibt die Auszahlung 1.5x (Blackjack) exakt in Cent darstellbar.
    while (true) {
        std::cout << "Dein Kontostand: " << formatMoney(bankrollCents) << "\n";
        std::cout << "Einsatz in EUR waehlen (z.B. 10). Mindest 1, maximal " << (bankrollCents / 100) << ": ";

        const auto in = readLineTrimmed();
        if (in.empty()) {
            std::cout << "Ungueltige Eingabe. Bitte eine Zahl eingeben.\n";
            continue;
        }

        try {
            std::size_t pos = 0;
            const long long eur = std::stoll(in, &pos, 10);
            if (pos != in.size()) {
                std::cout << "Ungueltige Eingabe. Bitte nur Zahlen eingeben.\n";
                continue;
            }
            if (eur <= 0) {
                std::cout << "Der Einsatz muss mindestens 1 EUR sein.\n";
                continue;
            }
            const long long betCents = eur * 100;
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
    // Struktur bewusst als Enum, damit später leicht um Aktionen wie "Double Down" erweiterbar.
    while (true) {
        std::cout << "Aktion waehlen: (1) Karte ziehen  (2) Passen  (3) Double Down : ";
        const auto in = readLineTrimmed();

        if (in == "1") return PlayerAction::Hit;
        if (in == "2") return PlayerAction::Stand;
        if (in == "3") return PlayerAction::DoubleDown;

        // Auch Text-Eingaben tolerieren (REQ-F-014).
        const auto lowered = toLowerAscii(in);
        if (lowered == "hit" || lowered == "ziehen" || lowered == "karte" || lowered == "karte ziehen") {
            return PlayerAction::Hit;
        }
        if (lowered == "stand" || lowered == "passen") {
            return PlayerAction::Stand;
        }
        if (lowered == "double" || lowered == "doubledown" || lowered == "double down") {
            return PlayerAction::DoubleDown;
        }

        std::cout << "Ungueltige Eingabe. Bitte 1/2/3 eingeben.\n";
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

std::string ConsoleUI::formatMoney(long long cents) {
    const long long absCents = cents < 0 ? -cents : cents;
    const long long eur = absCents / 100;
    const long long rem = absCents % 100;

    std::ostringstream oss;
    if (cents < 0) oss << "-";
    oss << eur << "." << std::setw(2) << std::setfill('0') << rem << " EUR";
    return oss.str();
}

