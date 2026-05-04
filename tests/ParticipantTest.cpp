#include "Dealer.h"
#include "Deck.h"
#include "Participant.h"
#include "Player.h"

#include "Card.h"

#include <gtest/gtest.h>

#include <stdexcept>
#include <string>

using Suit = Card::Suit;
using Rank = Card::Rank;

// =============================================================================
// Participant — Konstruktor, name(), hand(), clearHand(), takeCard(), score(),
// isBust(), hasBlackjack()
// Aequivalenzklassen Name: {leer?}, typischer Name, langer Name
// =============================================================================

TEST(Participant, NameAccessibleAfterConstructionTypical) {
    Participant p("Alice");
    EXPECT_EQ(p.name(), "Alice");
}

TEST(Participant, NameBoundaryEmptyString) {
    Participant p("");
    EXPECT_EQ(p.name(), "");
}

TEST(Participant, NameTypicalLongGerman) {
    const std::string longName = "TestTeilnehmerMitSehrLangemNamen";
    Participant p(longName);
    EXPECT_EQ(p.name(), longName);
}

TEST(ParticipantHand, ClearHandEmptiesScoreZero) {
    Participant p("P");
    Deck d;
    p.takeCard(d);
    p.clearHand();
    EXPECT_TRUE(p.hand().cards().empty());
    EXPECT_EQ(p.score(), 0);
}

TEST(ParticipantTakeCard, DrawsFromDeckDecrementsDeckSize) {
    Participant p("P");
    Deck d;
    const auto before = d.size();
    p.takeCard(d);
    EXPECT_EQ(d.size(), before - 1);
    EXPECT_EQ(p.hand().cards().size(), 1u);
}

TEST(ParticipantScore, DelegatesToHandTypical) {
    Participant p("P");
    Deck d;
    p.takeCard(d);
    p.takeCard(d);
    EXPECT_EQ(p.score(), p.hand().score());
}

TEST(ParticipantBust, TrueWhenHandBusts) {
    Participant p("P");
    p.hand().clear();
    p.hand().addCard(Card(Suit::Herz, Rank::Koenig));
    p.hand().addCard(Card(Suit::Pik, Rank::Dame));
    p.hand().addCard(Card(Suit::Karo, Rank::Fuenf));
    EXPECT_EQ(p.score(), 25);
    EXPECT_TRUE(p.isBust());
}

TEST(ParticipantBlackjack, HasBlackjackMatchesHand) {
    Participant p("P");
    p.hand().clear();
    p.hand().addCard(Card(Suit::Herz, Rank::Ass));
    p.hand().addCard(Card(Suit::Pik, Rank::Koenig));
    EXPECT_TRUE(p.hasBlackjack());
}

TEST(ParticipantNonVirtualApi, MutableHandReferenceAddsCard) {
    Participant p("P");
    p.hand().addCard(Card(Suit::Karo, Rank::Drei));
    EXPECT_EQ(p.hand().cards().size(), 1u);
}

// =============================================================================
// Player — bankrollCents(), canAfford(), adjustBankroll()
// Aequivalenzklassen Einsatz: {<=0 ungueltig}, {1..bankroll}, {>bankroll}
// Grenzwerte: 1 Cent vs volle Bankroll
// =============================================================================

TEST(Player, DefaultBankrollIsFiftyThousandCentsTypical) {
    Player pl;
    EXPECT_EQ(pl.bankrollCents(), 50000);
}

TEST(PlayerCanAfford, ZeroBetFalseBoundaryInvalidClass) {
    Player pl;
    EXPECT_FALSE(pl.canAfford(0));
}

TEST(PlayerCanAfford, NegativeBetFalseInvalidClass) {
    Player pl;
    EXPECT_FALSE(pl.canAfford(-100));
}

TEST(PlayerCanAfford, OneCentTrueIfBankrollPositiveBoundary) {
    Player pl;
    EXPECT_TRUE(pl.canAfford(1));
}

TEST(PlayerCanAfford, FullBankrollTrueBoundary) {
    Player pl;
    const long long b = pl.bankrollCents();
    EXPECT_TRUE(pl.canAfford(b));
}

TEST(PlayerCanAfford, OneOverBankrollFalseBoundary) {
    Player pl;
    const long long b = pl.bankrollCents();
    EXPECT_FALSE(pl.canAfford(b + 1));
}

TEST(PlayerAdjustBankroll, AddIncreasesTypical) {
    Player pl;
    const long long before = pl.bankrollCents();
    pl.adjustBankroll(250);
    EXPECT_EQ(pl.bankrollCents(), before + 250);
}

TEST(PlayerAdjustBankroll, SubtractDecreasesTypical) {
    Player pl;
    const long long before = pl.bankrollCents();
    pl.adjustBankroll(-1000);
    EXPECT_EQ(pl.bankrollCents(), before - 1000);
}

TEST(PlayerAdjustBankroll, LargeDebitFloorsAtZeroBoundary) {
    Player pl;
    pl.adjustBankroll(-(pl.bankrollCents() + 999999));
    EXPECT_EQ(pl.bankrollCents(), 0);
}

TEST(PlayerInheritance, NameDefaultSpieler) {
    Player pl;
    EXPECT_EQ(pl.name(), "Spieler");
}

// =============================================================================
// Dealer — shouldHit() Regel: ziehen bei score < 17, stehen ab 17
// Grenzwerte: 16 trifft noch zu, 17 nicht
// =============================================================================

TEST(DealerShouldHit, SixteenTrueBoundaryBelowStandThreshold) {
    Dealer dl;
    dl.hand().clear();
    dl.hand().addCard(Card(Suit::Herz, Rank::Koenig));
    dl.hand().addCard(Card(Suit::Pik, Rank::Sechs));
    EXPECT_EQ(dl.score(), 16);
    EXPECT_TRUE(dl.shouldHit());
}

TEST(DealerShouldHit, SeventeenFalseBoundaryStandThreshold) {
    Dealer dl;
    dl.hand().clear();
    dl.hand().addCard(Card(Suit::Herz, Rank::Zehn));
    dl.hand().addCard(Card(Suit::Pik, Rank::Sieben));
    EXPECT_EQ(dl.score(), 17);
    EXPECT_FALSE(dl.shouldHit());
}

TEST(DealerShouldHit, SoftSeventeenStillSeventeenDealerStandsHere) {
    Dealer dl;
    dl.hand().clear();
    dl.hand().addCard(Card(Suit::Herz, Rank::Ass));
    dl.hand().addCard(Card(Suit::Pik, Rank::Sechs));
    EXPECT_EQ(dl.score(), 17);
    EXPECT_FALSE(dl.shouldHit());
}

TEST(DealerShouldHit, TwentyOneFalseTypicalStandingTotal) {
    Dealer dl;
    dl.hand().clear();
    dl.hand().addCard(Card(Suit::Herz, Rank::Koenig));
    dl.hand().addCard(Card(Suit::Pik, Rank::Ass));
    EXPECT_EQ(dl.score(), 21);
    EXPECT_FALSE(dl.shouldHit());
}

TEST(DealerShouldHit, FifteenTrueTypicalHitRange) {
    Dealer dl;
    dl.hand().clear();
    dl.hand().addCard(Card(Suit::Herz, Rank::Acht));
    dl.hand().addCard(Card(Suit::Pik, Rank::Sieben));
    EXPECT_EQ(dl.score(), 15);
    EXPECT_TRUE(dl.shouldHit());
}

TEST(DealerShouldHit, EmptyHandScoreZeroHits) {
    Dealer dl;
    dl.hand().clear();
    EXPECT_EQ(dl.score(), 0);
    EXPECT_TRUE(dl.shouldHit());
}

// =============================================================================
// Integration Participant + Deck: takeCard bis leer wirft
// =============================================================================

TEST(ParticipantDeckIntegration, TakeUntilDeckEmptyThenThrows) {
    Participant p("P");
    Deck d;
    EXPECT_NO_THROW({
        while (d.size() > 0) {
            p.takeCard(d);
        }
    });
    EXPECT_THROW(p.takeCard(d), std::runtime_error);
}
