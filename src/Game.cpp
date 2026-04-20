#include "Game.h"

#include <string>

Game::Game() = default;

void Game::run() {
    ui_.showWelcome();
    if (!ui_.promptStartGame()) {
        ui_.showInfo("Alles klar. Programm wird beendet.");
        return;
    }

    bool again = true;
    while (again) {
        playRound();
        again = ui_.promptPlayAgain();
    }

    ui_.showInfo("Danke fuers Spielen! Auf Wiedersehen.");
}

void Game::playRound() {
    // V1 Default: 1 Deck, vor jeder Runde frisch gemischt.
    Deck deck;
    deck.shuffle();

    player_.clearHand();
    dealer_.clearHand();

    initialDeal(deck);
    ui_.showHands(player_, dealer_, true);

    if (handleInitialBlackjack()) {
        return;
    }

    playerTurn(deck);
    if (player_.isBust()) {
        ui_.showHands(player_, dealer_, false);
        ui_.showRoundResult("Du bist ueber 21. Bust! Der Dealer gewinnt.");
        return;
    }

    dealerTurn(deck);
    ui_.showHands(player_, dealer_, false);

    const auto outcome = determineOutcome();
    switch (outcome) {
        case RoundOutcome::PlayerWin:
            ui_.showRoundResult("Du gewinnst!");
            break;
        case RoundOutcome::DealerWin:
            ui_.showRoundResult("Der Dealer gewinnt.");
            break;
        case RoundOutcome::Push:
            ui_.showRoundResult("Unentschieden (Push).");
            break;
    }
}

void Game::initialDeal(Deck& deck) {
    // Standard-Deal: abwechselnd jeweils 2 Karten.
    player_.takeCard(deck);
    dealer_.takeCard(deck);
    player_.takeCard(deck);
    dealer_.takeCard(deck);
}

bool Game::handleInitialBlackjack() {
    const bool playerBJ = player_.hasBlackjack();
    const bool dealerBJ = dealer_.hasBlackjack();

    if (!playerBJ && !dealerBJ) return false;

    ui_.showHands(player_, dealer_, false);

    if (playerBJ && dealerBJ) {
        ui_.showRoundResult("Beide haben Blackjack. Unentschieden (Push).");
    } else if (playerBJ) {
        ui_.showRoundResult("Blackjack! Du gewinnst sofort.");
    } else {
        ui_.showRoundResult("Der Dealer hat Blackjack. Du verlierst.");
    }
    return true;
}

void Game::playerTurn(Deck& deck) {
    while (true) {
        ui_.showPlayerStatus(player_);
        const auto action = ui_.promptPlayerAction();

        if (action == ConsoleUI::PlayerAction::Stand) {
            ui_.showInfo("Du passt.");
            return;
        }

        player_.takeCard(deck);
        ui_.showInfo("Du ziehst eine Karte.");
        ui_.showHands(player_, dealer_, true);

        if (player_.isBust()) {
            return; // Bust wird im Rundenablauf behandelt
        }
    }
}

void Game::dealerTurn(Deck& deck) {
    ui_.showInfo("\nDealer ist am Zug...");
    ui_.showHands(player_, dealer_, false);

    while (dealer_.shouldHit()) {
        dealer_.takeCard(deck);
        ui_.showInfo("Dealer zieht eine Karte.");
        ui_.showDealerStatus(dealer_);
        if (dealer_.isBust()) break;
    }

    if (!dealer_.isBust()) {
        ui_.showInfo("Dealer passt.");
    }
}

Game::RoundOutcome Game::determineOutcome() const {
    if (dealer_.isBust()) return RoundOutcome::PlayerWin;

    const int p = player_.score();
    const int d = dealer_.score();

    if (p > d) return RoundOutcome::PlayerWin;
    if (d > p) return RoundOutcome::DealerWin;
    return RoundOutcome::Push;
}

