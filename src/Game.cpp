#include "Game.h"

#include <string>

namespace {
constexpr long long kCentsPerEuro = 100;
constexpr long long kBlackjackPayoutNumerator = 150;
constexpr long long kBlackjackPayoutDenominator = 100;
constexpr long long kStandardWinMultiplier = 2;
} // anonymous namespace

Game::Game() = default;

void Game::run() {
    ui_.showWelcome();
    if (!ui_.promptStartGame()) {
        ui_.showInfo("Alles klar. Programm wird beendet.");
        return;
    }

    // A deck is used for multiple rounds: shuffle only once,
    // drawn cards remain removed from the deck.
    deck_.shuffle();

    bool playAnotherRound = true;
    while (playAnotherRound) {
        if (player_.bankrollCents() <= 0) {
            ui_.showInfo("Du hast kein Guthaben mehr. Spiel beendet.");
            break;
        }

        playRound();
        if (player_.bankrollCents() <= 0) {
            ui_.showInfo("Du hast kein Guthaben mehr. Spiel beendet.");
            break;
        }
        playAnotherRound = ui_.promptPlayAgain();
    }

    ui_.showInfo("Danke fuers Spielen! Auf Wiedersehen.");
}

void Game::ensureDeckReady() {
    // Create and shuffle a new deck as soon as there are <= reshuffleWhenAtOrBelow_ cards left.
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
    player_.adjustBankroll(-betCents); // Deduct the bet at the beginning of the round
    ui_.showInfo("Einsatz gesetzt: " + std::to_string(betCents / kCentsPerEuro) + " EUR");

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
    // Standard deal: alternating 2 cards each.
    player_.takeCard(deck);
    dealer_.takeCard(deck);
    player_.takeCard(deck);
    dealer_.takeCard(deck);
}

bool Game::handleInitialBlackjack(long long betCents) {
    const bool hasPlayerBlackjack = player_.hasBlackjack();
    const bool hasDealerBlackjack = dealer_.hasBlackjack();

    if (!hasPlayerBlackjack && !hasDealerBlackjack) return false;

    ui_.showHands(player_, dealer_, false);

    if (hasPlayerBlackjack && hasDealerBlackjack) {
        ui_.showRoundResult("Beide haben Blackjack. Unentschieden (Push).");
        applyPayout(betCents, true, RoundOutcome::Push);
    } else if (hasPlayerBlackjack) {
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
            // Double Down: Double the bet, draw exactly one card, then stand automatically.
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
            return; // Bust is handled in the round sequence
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

    const int playerScore = player_.score();
    const int dealerScore = dealer_.score();

    if (playerScore > dealerScore) return RoundOutcome::PlayerWin;
    if (dealerScore > playerScore) return RoundOutcome::DealerWin;
    return RoundOutcome::Push;
}

void Game::applyPayout(long long betCents, bool playerBlackjack, RoundOutcome outcome) {
    // The bet was already deducted.
    // Rules:
    // - Blackjack (Player) wins: Payout = 1.5x bet, plus the original bet back
    // - Normal win: Payout = 1x bet, plus the original bet back
    // - Push: Original bet back
    // - Loss: Nothing (bet is gone)
    if (outcome == RoundOutcome::Push) {
        player_.adjustBankroll(betCents);
        return;
    }

    if (outcome == RoundOutcome::DealerWin) {
        return;
    }

    if (playerBlackjack) {
        const long long payout = betCents + (betCents * kBlackjackPayoutNumerator) / kBlackjackPayoutDenominator; // 2.5x bet as payout
        player_.adjustBankroll(payout);
        return;
    }

    const long long payout = betCents * kStandardWinMultiplier; // Bet + 1x win
    player_.adjustBankroll(payout);
}
