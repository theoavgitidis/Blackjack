#pragma once

#include "Dealer.h"
#include "Player.h"

#include <string>

class ConsoleUI {
public:
    enum class PlayerAction { Hit, Stand };

    void showWelcome() const;
    bool promptStartGame() const;

    void showHands(const Player& player, const Dealer& dealer, bool hideDealersSecondCard) const;
    void showPlayerStatus(const Player& player) const;
    void showDealerStatus(const Dealer& dealer) const;

    PlayerAction promptPlayerAction() const;
    bool promptPlayAgain() const;

    void showRoundResult(const std::string& resultText) const;
    void showInfo(const std::string& text) const;

private:
    static std::string readLineTrimmed();
    static std::string toLowerAscii(std::string s);
};

