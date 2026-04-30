#include "Game.h"
#include "TestIO.h"

#include <gtest/gtest.h>

namespace {

void makeHand(Participant& participant, std::initializer_list<Card::Rank> ranks) {
    participant.clearHand();
    for (const auto rank : ranks) {
        participant.hand().addCard(Card(Card::Suit::Kreuz, rank));
    }
}

} // namespace

TEST(GameTest, ConstructorCreatesInitialParticipantsAndDeck) {
    const Game game;

    EXPECT_EQ(game.player_.name(), "Spieler");
    EXPECT_EQ(game.dealer_.name(), "Dealer");
    EXPECT_EQ(game.deck_.size(), 52U);
}

TEST(GameTest, EnsureDeckReadyKeepsLargeDeckAndRefreshesLowDeck) {
    Game game;
    ScopedOutput output;

    game.ensureDeckReady();
    EXPECT_EQ(game.deck_.size(), 52U);

    while (game.deck_.size() > 10U) {
        (void)game.deck_.draw();
    }

    game.ensureDeckReady();
    EXPECT_EQ(game.deck_.size(), 52U);
}

TEST(GameTest, InitialDealGivesPlayerAndDealerTwoCards) {
    Game game;
    Deck deck;

    game.initialDeal(deck);

    EXPECT_EQ(game.player_.hand().cards().size(), 2U);
    EXPECT_EQ(game.dealer_.hand().cards().size(), 2U);
    EXPECT_EQ(deck.size(), 48U);
}

TEST(GameTest, HandleInitialBlackjackReturnsFalseWhenNoBlackjack) {
    Game game;
    makeHand(game.player_, {Card::Rank::Zehn, Card::Rank::Neun});
    makeHand(game.dealer_, {Card::Rank::Zehn, Card::Rank::Acht});
    ScopedOutput output;

    EXPECT_FALSE(game.handleInitialBlackjack(1000));
}

TEST(GameTest, HandleInitialBlackjackPaysPlayerBlackjack) {
    Game game;
    makeHand(game.player_, {Card::Rank::Ass, Card::Rank::Koenig});
    makeHand(game.dealer_, {Card::Rank::Zehn, Card::Rank::Acht});
    game.player_.adjustBankroll(-1000);
    ScopedOutput output;

    EXPECT_TRUE(game.handleInitialBlackjack(1000));
    EXPECT_EQ(game.player_.bankrollCents(), 51500);
}

TEST(GameTest, HandleInitialBlackjackPushReturnsBet) {
    Game game;
    makeHand(game.player_, {Card::Rank::Ass, Card::Rank::Koenig});
    makeHand(game.dealer_, {Card::Rank::Ass, Card::Rank::Dame});
    game.player_.adjustBankroll(-1000);
    ScopedOutput output;

    EXPECT_TRUE(game.handleInitialBlackjack(1000));
    EXPECT_EQ(game.player_.bankrollCents(), 50000);
}

TEST(GameTest, PlayerTurnStandDoesNotDraw) {
    Game game;
    Deck deck;
    long long betCents = 1000;
    ScopedInput input("2\n");
    ScopedOutput output;

    game.playerTurn(deck, betCents);

    EXPECT_TRUE(game.player_.hand().cards().empty());
    EXPECT_EQ(betCents, 1000);
    EXPECT_EQ(deck.size(), 52U);
}

TEST(GameTest, PlayerTurnHitDrawsExactlyOneCardThenCanStand) {
    Game game;
    Deck deck;
    long long betCents = 1000;
    ScopedInput input("1\n2\n");
    ScopedOutput output;

    game.playerTurn(deck, betCents);

    EXPECT_EQ(game.player_.hand().cards().size(), 1U);
    EXPECT_EQ(deck.size(), 51U);
}

TEST(GameTest, PlayerTurnDoubleDownDoublesBetDrawsOneCardAndEndsTurn) {
    Game game;
    Deck deck;
    long long betCents = 1000;
    ScopedInput input("3\n1\n");
    ScopedOutput output;

    game.playerTurn(deck, betCents);

    EXPECT_EQ(betCents, 2000);
    EXPECT_EQ(game.player_.bankrollCents(), 49000);
    EXPECT_EQ(game.player_.hand().cards().size(), 1U);
    EXPECT_EQ(deck.size(), 51U);
}

TEST(GameTest, PlayerTurnDoubleDownRetriesWhenPlayerCannotAffordExtraBet) {
    Game game;
    Deck deck;
    game.player_.adjustBankroll(-49500);
    long long betCents = 1000;
    ScopedInput input("3\n2\n");
    ScopedOutput output;

    game.playerTurn(deck, betCents);

    EXPECT_EQ(betCents, 1000);
    EXPECT_TRUE(game.player_.hand().cards().empty());
    EXPECT_NE(output.str().find("Double Down nicht moeglich"), std::string::npos);
}

TEST(GameTest, DealerTurnDrawsUntilAtLeastSeventeen) {
    Game game;
    Deck deck;
    makeHand(game.dealer_, {Card::Rank::Zwei, Card::Rank::Drei});
    ScopedOutput output;

    game.dealerTurn(deck);

    EXPECT_GE(game.dealer_.score(), 17);
    EXPECT_LT(deck.size(), 52U);
}

TEST(GameTest, DealerTurnDoesNotDrawAtSeventeen) {
    Game game;
    Deck deck;
    makeHand(game.dealer_, {Card::Rank::Zehn, Card::Rank::Sieben});
    ScopedOutput output;

    game.dealerTurn(deck);

    EXPECT_EQ(game.dealer_.hand().cards().size(), 2U);
    EXPECT_EQ(deck.size(), 52U);
}

TEST(GameTest, DetermineOutcomeHandlesDealerBustHigherScoresAndPush) {
    Game game;

    makeHand(game.player_, {Card::Rank::Zehn, Card::Rank::Acht});
    makeHand(game.dealer_, {Card::Rank::Koenig, Card::Rank::Dame, Card::Rank::Zwei});
    EXPECT_EQ(game.determineOutcome(), Game::RoundOutcome::PlayerWin);

    makeHand(game.player_, {Card::Rank::Zehn, Card::Rank::Neun});
    makeHand(game.dealer_, {Card::Rank::Koenig, Card::Rank::Acht});
    EXPECT_EQ(game.determineOutcome(), Game::RoundOutcome::PlayerWin);

    makeHand(game.player_, {Card::Rank::Zehn, Card::Rank::Sieben});
    makeHand(game.dealer_, {Card::Rank::Koenig, Card::Rank::Acht});
    EXPECT_EQ(game.determineOutcome(), Game::RoundOutcome::DealerWin);

    makeHand(game.player_, {Card::Rank::Zehn, Card::Rank::Acht});
    makeHand(game.dealer_, {Card::Rank::Koenig, Card::Rank::Acht});
    EXPECT_EQ(game.determineOutcome(), Game::RoundOutcome::Push);
}

TEST(GameTest, ApplyPayoutHandlesPushLossNormalWinAndBlackjackWin) {
    Game pushGame;
    pushGame.player_.adjustBankroll(-1000);
    pushGame.applyPayout(1000, false, Game::RoundOutcome::Push);
    EXPECT_EQ(pushGame.player_.bankrollCents(), 50000);

    Game lossGame;
    lossGame.player_.adjustBankroll(-1000);
    lossGame.applyPayout(1000, false, Game::RoundOutcome::DealerWin);
    EXPECT_EQ(lossGame.player_.bankrollCents(), 49000);

    Game winGame;
    winGame.player_.adjustBankroll(-1000);
    winGame.applyPayout(1000, false, Game::RoundOutcome::PlayerWin);
    EXPECT_EQ(winGame.player_.bankrollCents(), 51000);

    Game blackjackGame;
    blackjackGame.player_.adjustBankroll(-1000);
    blackjackGame.applyPayout(1000, true, Game::RoundOutcome::PlayerWin);
    EXPECT_EQ(blackjackGame.player_.bankrollCents(), 51500);
}
