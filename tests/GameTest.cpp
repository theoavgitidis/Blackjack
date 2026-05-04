#include "Game.h"

#include "Card.h"
#include "test_stream_redirect.h"

#include <gtest/gtest.h>

#include <sstream>

using Suit = Card::Suit;
using Rank = Card::Rank;

namespace {

void clearDealBoth(Game& g) {
    g.player_.clearHand();
    g.dealer_.clearHand();
}

void givePlayer(Game& g, Suit s1, Rank r1, Suit s2, Rank r2) {
    g.player_.hand().clear();
    g.player_.hand().addCard(Card(s1, r1));
    g.player_.hand().addCard(Card(s2, r2));
}

void giveDealer(Game& g, Suit s1, Rank r1, Suit s2, Rank r2) {
    g.dealer_.hand().clear();
    g.dealer_.hand().addCard(Card(s1, r1));
    g.dealer_.hand().addCard(Card(s2, r2));
}

} // namespace

// =============================================================================
// Game (Test-API unter BLACKJACK_UNIT_TEST) — determineOutcome()
// Aequivalenzklassen: Dealer Bust, Player hoeher, Dealer hoeher, Gleichstand
// Grenzwerte: Punktgleichheit, exakt 21 vs 20, Dealer 22 (Bust)
// =============================================================================

TEST(GameDetermineOutcome, DealerBustPlayerAnyNonBustWinsTypical) {
    Game g;
    clearDealBoth(g);
    g.player_.hand().addCard(Card(Suit::Herz, Rank::Zehn));
    g.player_.hand().addCard(Card(Suit::Pik, Rank::Fuenf));
    g.dealer_.hand().addCard(Card(Suit::Karo, Rank::Koenig));
    g.dealer_.hand().addCard(Card(Suit::Kreuz, Rank::Koenig));
    g.dealer_.hand().addCard(Card(Suit::Herz, Rank::Drei));
    ASSERT_TRUE(g.dealer_.isBust());
    EXPECT_EQ(g.determineOutcome(), Game::RoundOutcome::PlayerWin);
}

TEST(GameDetermineOutcome, PlayerHigherTypicalSeventeenVsSixteen) {
    Game g;
    givePlayer(g, Suit::Herz, Rank::Zehn, Suit::Pik, Rank::Sieben);
    giveDealer(g, Suit::Karo, Rank::Zehn, Suit::Kreuz, Rank::Sechs);
    EXPECT_EQ(g.determineOutcome(), Game::RoundOutcome::PlayerWin);
}

TEST(GameDetermineOutcome, DealerHigherBoundaryPlayerLoses) {
    Game g;
    givePlayer(g, Suit::Herz, Rank::Zehn, Suit::Pik, Rank::Sechs);
    giveDealer(g, Suit::Karo, Rank::Zehn, Suit::Kreuz, Rank::Sieben);
    EXPECT_EQ(g.determineOutcome(), Game::RoundOutcome::DealerWin);
}

TEST(GameDetermineOutcome, PushSameTotalNineteenTypical) {
    Game g;
    givePlayer(g, Suit::Herz, Rank::Neun, Suit::Pik, Rank::Koenig);
    giveDealer(g, Suit::Karo, Rank::Neun, Suit::Kreuz, Rank::Koenig);
    EXPECT_EQ(g.player_.score(), 19);
    EXPECT_EQ(g.dealer_.score(), 19);
    EXPECT_EQ(g.determineOutcome(), Game::RoundOutcome::Push);
}

TEST(GameDetermineOutcome, PushTwentyOneBothHard) {
    Game g;
    givePlayer(g, Suit::Herz, Rank::Sieben, Suit::Pik, Rank::Sieben);
    g.player_.hand().addCard(Card(Suit::Karo, Rank::Sieben));
    giveDealer(g, Suit::Kreuz, Rank::Acht, Suit::Herz, Rank::Neun);
    g.dealer_.hand().addCard(Card(Suit::Pik, Rank::Vier));
    EXPECT_EQ(g.player_.score(), 21);
    EXPECT_EQ(g.dealer_.score(), 21);
    EXPECT_EQ(g.determineOutcome(), Game::RoundOutcome::Push);
}

// =============================================================================
// applyPayout — Aequivalenzklassen Outcome x playerBlackjack-Flag
// Grenzwerte: bet=1 Cent, grosse bet, Push vs Win vs Loss
// =============================================================================

TEST(GameApplyPayout, PushReturnsBetOnlyTypical) {
    Game g;
    const long long before = g.player_.bankrollCents();
    const long long bet = 1000;
    g.applyPayout(bet, false, Game::RoundOutcome::Push);
    EXPECT_EQ(g.player_.bankrollCents(), before + bet);
}

TEST(GameApplyPayout, DealerWinAddsNothingBoundary) {
    Game g;
    const long long before = g.player_.bankrollCents();
    const long long bet = 2000;
    g.applyPayout(bet, false, Game::RoundOutcome::DealerWin);
    EXPECT_EQ(g.player_.bankrollCents(), before);
}

TEST(GameApplyPayout, NormalPlayerWinDoublesReturnTypical) {
    Game g;
    const long long before = g.player_.bankrollCents();
    const long long bet = 500;
    g.applyPayout(bet, false, Game::RoundOutcome::PlayerWin);
    EXPECT_EQ(g.player_.bankrollCents(), before + bet * 2);
}

TEST(GameApplyPayout, BlackjackWinUsesOnePointFiveMultiplierTypical) {
    Game g;
    const long long before = g.player_.bankrollCents();
    const long long bet = 10000;
    g.applyPayout(bet, true, Game::RoundOutcome::PlayerWin);
    const long long expectedGain = bet + (bet * 150) / 100;
    EXPECT_EQ(g.player_.bankrollCents(), before + expectedGain);
}

TEST(GameApplyPayout, BlackjackFlagIgnoredOnPush) {
    Game g;
    const long long before = g.player_.bankrollCents();
    const long long bet = 750;
    g.applyPayout(bet, true, Game::RoundOutcome::Push);
    EXPECT_EQ(g.player_.bankrollCents(), before + bet);
}

TEST(GameApplyPayout, SmallBetOneCentPlayerWinBoundary) {
    Game g;
    const long long before = g.player_.bankrollCents();
    const long long bet = 1;
    g.applyPayout(bet, false, Game::RoundOutcome::PlayerWin);
    EXPECT_EQ(g.player_.bankrollCents(), before + 2);
}

// =============================================================================
// ensureDeckReady — Grenzwert reshuffle bei <=10 Karten (Konstante im Game)
// =============================================================================

TEST(GameEnsureDeckReady, ReshufflesWhenTenOrFewerCardsBoundary) {
    Game g;
    while (g.deck_.size() > 10) {
        (void)g.deck_.draw();
    }
    ASSERT_EQ(g.deck_.size(), 10u);
    g.ensureDeckReady();
    EXPECT_EQ(g.deck_.size(), 52u);
}

TEST(GameEnsureDeckReady, DoesNotReshuffleWhenElevenCardsTypical) {
    Game g;
    while (g.deck_.size() > 11) {
        (void)g.deck_.draw();
    }
    ASSERT_EQ(g.deck_.size(), 11u);
    g.ensureDeckReady();
    EXPECT_EQ(g.deck_.size(), 11u);
}

// =============================================================================
// handleInitialBlackjack — rein logisch ueber Manipulation + Aufruf
// =============================================================================

TEST(GameHandleInitialBlackjack, ReturnsFalseWhenNoBlackjack) {
    Game g;
    givePlayer(g, Suit::Herz, Rank::Zehn, Suit::Pik, Rank::Fuenf);
    giveDealer(g, Suit::Karo, Rank::Neun, Suit::Kreuz, Rank::Acht);
    EXPECT_FALSE(g.handleInitialBlackjack(1000));
}

TEST(GameHandleInitialBlackjack, BothBlackjackPushPayout) {
    Game g;
    const long long before = g.player_.bankrollCents();
    givePlayer(g, Suit::Herz, Rank::Ass, Suit::Pik, Rank::Koenig);
    giveDealer(g, Suit::Karo, Rank::Ass, Suit::Kreuz, Rank::Dame);
    const long long bet = 2000;
    EXPECT_TRUE(g.handleInitialBlackjack(bet));
    EXPECT_EQ(g.player_.bankrollCents(), before + bet);
}

TEST(GameHandleInitialBlackjack, PlayerBlackjackPaysBlackjackRate) {
    Game g;
    const long long before = g.player_.bankrollCents();
    givePlayer(g, Suit::Herz, Rank::Ass, Suit::Pik, Rank::Zehn);
    giveDealer(g, Suit::Karo, Rank::Neun, Suit::Kreuz, Rank::Acht);
    const long long bet = 1000;
    EXPECT_TRUE(g.handleInitialBlackjack(bet));
    const long long gain = bet + (bet * 150) / 100;
    EXPECT_EQ(g.player_.bankrollCents(), before + gain);
}

TEST(GameHandleInitialBlackjack, DealerBlackjackPlayerLosesNoRefund) {
    Game g;
    const long long before = g.player_.bankrollCents();
    givePlayer(g, Suit::Herz, Rank::Neun, Suit::Pik, Rank::Acht);
    giveDealer(g, Suit::Karo, Rank::Ass, Suit::Kreuz, Rank::Bube);
    const long long bet = 3000;
    EXPECT_TRUE(g.handleInitialBlackjack(bet));
    EXPECT_EQ(g.player_.bankrollCents(), before);
}

// =============================================================================
// initialDeal — zieht vier Karten, zwei pro Teilnehmer
// =============================================================================

TEST(GameInitialDeal, DealsTwoCardsEachTypical) {
    Game g;
    clearDealBoth(g);
    Deck d;
    const auto before = d.size();
    g.initialDeal(d);
    EXPECT_EQ(g.player_.hand().cards().size(), 2u);
    EXPECT_EQ(g.dealer_.hand().cards().size(), 2u);
    EXPECT_EQ(d.size(), before - 4);
}

// =============================================================================
// dealerTurn / playerTurn — Smoke (nur Ablauf ohne Assertions auf exakte IO)
// =============================================================================

TEST(GameDealerTurn, StandsOnSeventeenWithoutExtraDraws) {
    Game g;
    std::ostringstream oss;
    blackjack_test::CoutStringbufGuard outGuard(oss);
    giveDealer(g, Suit::Herz, Rank::Zehn, Suit::Pik, Rank::Sieben);
    g.player_.hand().clear();
    g.player_.hand().addCard(Card(Suit::Karo, Rank::Neun));
    Deck d;
    g.dealerTurn(d);
    EXPECT_EQ(g.dealer_.score(), 17);
}

TEST(GamePlayerTurn, StandImmediatelyEndsTurn) {
    Game g;
    std::istringstream iss("2\n");
    blackjack_test::CinStringbufGuard inGuard(iss);
    std::ostringstream oss;
    blackjack_test::CoutStringbufGuard outGuard(oss);
    givePlayer(g, Suit::Herz, Rank::Zehn, Suit::Pik, Rank::Fuenf);
    long long bet = 100;
    Deck d;
    g.playerTurn(d, bet);
    EXPECT_EQ(g.player_.hand().cards().size(), 2u);
    EXPECT_EQ(bet, 100);
}
