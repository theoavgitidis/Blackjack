#include "Game.h"

#include <string>

Game::Game() = default;

void Game::run() {
    ui_.showWelcome();
    if (!ui_.promptStartGame()) {
        ui_.showInfo("Alles klar. Programm wird beendet.");
        return;
    }

    // Ein Deck wird für mehrere Runden verwendet: nur einmal mischen,
    // gezogene Karten bleiben aus dem Stapel entfernt.
    deck_.shuffle();

    bool again = true;
    while (again) {
        if (player_.bankrollCents() <= 0) {
            ui_.showInfo("Du hast kein Guthaben mehr. Spiel beendet.");
            break;
        }

        playRound();
        if (player_.bankrollCents() <= 0) {
            ui_.showInfo("Du hast kein Guthaben mehr. Spiel beendet.");
            break;
        }
        again = ui_.promptPlayAgain();
    }

    ui_.showInfo("Danke fuers Spielen! Auf Wiedersehen.");
}

void Game::ensureDeckReady() {
    // Sobald nur noch <= 10 Karten im Deck sind, neues Deck erzeugen und mischen.
    if (deck_.size() <= reshuffleWhenAtOrBelow_) {
        ui_.showInfo("\n--- Neues Deck wird gemischt ---");
        deck_ = Deck{};
        deck_.shuffle();
        ui_.showInfo("Karten im Deck: " + std::to_string(deck_.size()));
    }
}

void Game::playRound() {
    ui_.showInfo("");
    ensureDeckReady();
    ui_.showInfo("Karten im Deck: " + std::to_string(deck_.size()));
    long long betCents = ui_.promptBetCents(player_.bankrollCents());
    player_.adjustBankroll(-betCents); // Einsatz wird zu Rundenbeginn abgezogen
    ui_.showInfo("Einsatz gesetzt: " + std::to_string(betCents / 100) + " EUR");

    player_.clearHand();
    dealer_.clearHand();

    initialDeal(deck_);
    ui_.showHands(player_, dealer_, true);

    if (handleInitialBlackjack(betCents)) {
        ui_.showBankroll(player_.bankrollCents());
        return;
    }

    playerTurn(deck_, betCents);
    if (player_.isBust()) {
        ui_.showHands(player_, dealer_, false);
        ui_.showRoundResult("Du bist ueber 21. Bust! Der Dealer gewinnt.");
        applyPayout(betCents, false, RoundOutcome::DealerWin);
        ui_.showBankroll(player_.bankrollCents());
        return;
    }

    dealerTurn(deck_);
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

    applyPayout(betCents, false, outcome);
    ui_.showBankroll(player_.bankrollCents());
}

void Game::initialDeal(Deck& deck) {
    // Standard-Deal: abwechselnd jeweils 2 Karten.
    player_.takeCard(deck);
    dealer_.takeCard(deck);
    player_.takeCard(deck);
    dealer_.takeCard(deck);
}

bool Game::handleInitialBlackjack(long long betCents) {
    const bool playerBJ = player_.hasBlackjack();
    const bool dealerBJ = dealer_.hasBlackjack();

    if (!playerBJ && !dealerBJ) return false;

    ui_.showHands(player_, dealer_, false);

    if (playerBJ && dealerBJ) {
        ui_.showRoundResult("Beide haben Blackjack. Unentschieden (Push).");
        applyPayout(betCents, true, RoundOutcome::Push);
    } else if (playerBJ) {
        ui_.showRoundResult("Blackjack! Du gewinnst sofort.");
        applyPayout(betCents, true, RoundOutcome::PlayerWin);
    } else {
        ui_.showRoundResult("Der Dealer hat Blackjack. Du verlierst.");
        applyPayout(betCents, false, RoundOutcome::DealerWin);
    }
    return true;
}

void Game::playerTurn(Deck& deck, long long& betCents) {
    while (true) {
        const auto action = ui_.promptPlayerAction();

        if (action == ConsoleUI::PlayerAction::Stand) {
            ui_.showInfo("Du passt.");
            return;
        }

        if (action == ConsoleUI::PlayerAction::DoubleDown) {
            // Double Down: Einsatz verdoppeln, genau eine Karte ziehen, dann automatisch passen.
            if (!player_.canAfford(betCents)) {
                ui_.showInfo("Double Down nicht moeglich: nicht genug Guthaben, um den Einsatz zu verdoppeln.");
                continue;
            }

            player_.adjustBankroll(-betCents);
            betCents *= 2;
            ui_.showInfo("Double Down: Einsatz verdoppelt. Du erhaelst genau eine Karte.");
            player_.takeCard(deck);
            ui_.showHands(player_, dealer_, true);
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

void Game::applyPayout(long long betCents, bool playerBlackjack, RoundOutcome outcome) {
    // Einsatz wurde bereits abgezogen.
    // Regeln:
    // - Blackjack (Spieler) gewinnt: Gewinn = 1.5x Einsatz, plus Einsatz zurueck
    // - Normaler Sieg: Gewinn = 1x Einsatz, plus Einsatz zurueck
    // - Push: Einsatz zurueck
    // - Niederlage: nichts (Einsatz ist weg)
    if (outcome == RoundOutcome::Push) {
        player_.adjustBankroll(betCents);
        return;
    }

    if (outcome == RoundOutcome::DealerWin) {
        return;
    }

    if (playerBlackjack) {
        const long long payout = betCents + (betCents * 150) / 100; // 2.5x Einsatz als Auszahlung
        player_.adjustBankroll(payout);
        return;
    }

    const long long payout = betCents * 2; // Einsatz + 1x Gewinn
    player_.adjustBankroll(payout);
}

