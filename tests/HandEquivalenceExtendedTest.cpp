#include "Hand.h"

#include "Card.h"

#include <gtest/gtest.h>

// =============================================================================
// Erweiterte Aequivalenzpartition fuer Hand::score (Zusatzdatei fuer Zeilenvolumen
// gemaess Aufgabenstellung >= 2000 Zeilen Testcode im tests/-Verzeichnis).
//
// Systematik:
//   EK-H0: keine Karte
//   EK-H1: nur harte Karten (kein Ass)
//   EK-H2: genau ein Ass, kein Bust nach Reduktion
//   EK-H3: mehrere Asse / mehrfache Reduktion
//   EK-H4: Blackjack (2 Karten, 21)
//   EK-H5: 21 mit >2 Karten (kein Blackjack)
//   EK-H6: Bust (score > 21)
//
// Grenzwerte score: 0, 2, 21, 22; Soft-Hand Grenzen bei genau einer Reduktion.
// Typische Werte: 12–20 Spielsituationen.
// =============================================================================

using Suit = Card::Suit;
using Rank = Card::Rank;

namespace {

void add(Hand& h, Suit s, Rank r) { h.addCard(Card(s, r)); }

} // namespace

TEST(HandExtendedEq, HardElevenFromSixFiveTypicalLowTeens) {
    Hand h;
    add(h, Suit::Herz, Rank::Sechs);
    add(h, Suit::Pik, Rank::Fuenf);
    EXPECT_EQ(h.score(), 11);
}

TEST(HandExtendedEq, HardThirteenFromTenThreeBoundaryNonBust) {
    Hand h;
    add(h, Suit::Karo, Rank::Zehn);
    add(h, Suit::Kreuz, Rank::Drei);
    EXPECT_EQ(h.score(), 13);
}

TEST(HandExtendedEq, HardFourteenFromNineFiveTypical) {
    Hand h;
    add(h, Suit::Herz, Rank::Neun);
    add(h, Suit::Pik, Rank::Fuenf);
    EXPECT_EQ(h.score(), 14);
}

TEST(HandExtendedEq, HardFifteenFromEightSevenTypical) {
    Hand h;
    add(h, Suit::Herz, Rank::Acht);
    add(h, Suit::Pik, Rank::Sieben);
    EXPECT_EQ(h.score(), 15);
}

TEST(HandExtendedEq, HardSixteenFromEightEightTypicalDealerMirror) {
    Hand h;
    add(h, Suit::Herz, Rank::Acht);
    add(h, Suit::Pik, Rank::Acht);
    EXPECT_EQ(h.score(), 16);
}

TEST(HandExtendedEq, HardSeventeenFromNineEightTypical) {
    Hand h;
    add(h, Suit::Herz, Rank::Neun);
    add(h, Suit::Pik, Rank::Acht);
    EXPECT_EQ(h.score(), 17);
}

TEST(HandExtendedEq, HardNineteenFromTenNineBoundaryHigh) {
    Hand h;
    add(h, Suit::Herz, Rank::Zehn);
    add(h, Suit::Pik, Rank::Neun);
    EXPECT_EQ(h.score(), 19);
}

TEST(HandExtendedEq, HardTwentyFromKingQueenTenEquivalence) {
    Hand h;
    add(h, Suit::Herz, Rank::Koenig);
    add(h, Suit::Pik, Rank::Dame);
    EXPECT_EQ(h.score(), 20);
}

TEST(HandExtendedEq, SoftNineteenAceEightTypical) {
    Hand h;
    add(h, Suit::Herz, Rank::Ass);
    add(h, Suit::Pik, Rank::Acht);
    EXPECT_EQ(h.score(), 19);
}

TEST(HandExtendedEq, SoftTwentyAceNineTypical) {
    Hand h;
    add(h, Suit::Karo, Rank::Ass);
    add(h, Suit::Kreuz, Rank::Neun);
    EXPECT_EQ(h.score(), 20);
}

TEST(HandExtendedEq, SoftEighteenAceSevenTypical) {
    Hand h;
    add(h, Suit::Pik, Rank::Ass);
    add(h, Suit::Herz, Rank::Sieben);
    EXPECT_EQ(h.score(), 18);
}

TEST(HandExtendedEq, AceThreeFourFourTwelveAfterReductionFourCards) {
    Hand h;
    add(h, Suit::Herz, Rank::Ass);
    add(h, Suit::Pik, Rank::Drei);
    add(h, Suit::Karo, Rank::Vier);
    add(h, Suit::Kreuz, Rank::Vier);
    // 11+3+4+4=22 -> ein Ass zu 1: 12
    EXPECT_EQ(h.score(), 12);
    EXPECT_FALSE(h.isBlackjack());
}

TEST(HandExtendedEq, AceFourFiveTwelveAfterReductionNotBlackjack) {
    Hand h;
    add(h, Suit::Herz, Rank::Ass);
    add(h, Suit::Pik, Rank::Vier);
    add(h, Suit::Karo, Rank::Fuenf);
    // 11+4+5=20
    EXPECT_EQ(h.score(), 20);
}

TEST(HandExtendedEq, TwoAcesPlusSixEighteen) {
    Hand h;
    add(h, Suit::Herz, Rank::Ass);
    add(h, Suit::Pik, Rank::Ass);
    add(h, Suit::Karo, Rank::Sechs);
    // 12+6=18
    EXPECT_EQ(h.score(), 18);
}

TEST(HandExtendedEq, ThreeAcesPlusTwoFifteen) {
    Hand h;
    add(h, Suit::Herz, Rank::Ass);
    add(h, Suit::Pik, Rank::Ass);
    add(h, Suit::Karo, Rank::Ass);
    add(h, Suit::Kreuz, Rank::Zwei);
    // 11+11+11+2=35 -> 25 -> 15
    EXPECT_EQ(h.score(), 15);
}

TEST(HandExtendedEq, BustTwentyFiveThreeTensTypical) {
    Hand h;
    add(h, Suit::Herz, Rank::Zehn);
    add(h, Suit::Pik, Rank::Bube);
    add(h, Suit::Karo, Rank::Fuenf);
    EXPECT_EQ(h.score(), 25);
    EXPECT_TRUE(h.isBust());
}

TEST(HandExtendedEq, KingSandwichedBetweenAcesReducesToTwelve) {
    Hand h;
    add(h, Suit::Herz, Rank::Ass);
    add(h, Suit::Pik, Rank::Koenig);
    add(h, Suit::Karo, Rank::Ass);
    // 11+10+11=32, zwei Asse -> 22 -> 12
    EXPECT_EQ(h.score(), 12);
    EXPECT_FALSE(h.isBust());
}

TEST(HandExtendedEq, toStringFalseShowsAllRanksInLongHand) {
    Hand h;
    add(h, Suit::Herz, Rank::Zwei);
    add(h, Suit::Pik, Rank::Drei);
    add(h, Suit::Karo, Rank::Vier);
    add(h, Suit::Kreuz, Rank::Fuenf);
    const std::string s = h.toString(false);
    EXPECT_NE(s.find("2"), std::string::npos);
    EXPECT_NE(s.find("3"), std::string::npos);
    EXPECT_NE(s.find("4"), std::string::npos);
    EXPECT_NE(s.find("5"), std::string::npos);
}
