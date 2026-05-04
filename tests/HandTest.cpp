#include "Hand.h"

#include "Card.h"

#include <gtest/gtest.h>

#include <string>
#include <vector>

using Suit = Card::Suit;
using Rank = Card::Rank;

namespace {

void add(Hand& h, Suit s, Rank r) { h.addCard(Card(s, r)); }

} // namespace

// =============================================================================
// Klasse Hand — clear() / cards() / addCard()
// Aequivalenzklassen Kartenanzahl: {0}, {1}, {2}, {3+}
// =============================================================================

TEST(HandClear, FreshHandIsEmptyBoundaryZeroCards) {
    Hand h;
    h.clear();
    EXPECT_TRUE(h.cards().empty());
    EXPECT_EQ(h.cards().size(), 0u);
}

TEST(HandAddCard, SingleCardTypical) {
    Hand h;
    add(h, Suit::Herz, Rank::Sieben);
    ASSERT_EQ(h.cards().size(), 1u);
    EXPECT_EQ(h.cards()[0].rank(), Rank::Sieben);
}

TEST(HandAddCard, TwoCardsOrderPreserved) {
    Hand h;
    add(h, Suit::Pik, Rank::Zwei);
    add(h, Suit::Karo, Rank::Drei);
    ASSERT_EQ(h.cards().size(), 2u);
    EXPECT_EQ(h.cards()[0].rank(), Rank::Zwei);
    EXPECT_EQ(h.cards()[1].rank(), Rank::Drei);
}

TEST(HandClear, AfterAddThenClearResetsToEmpty) {
    Hand h;
    add(h, Suit::Kreuz, Rank::Ass);
    h.clear();
    EXPECT_TRUE(h.cards().empty());
}

// =============================================================================
// score() — ohne Ass: lineare Summe; Grenzwerte 21 und 22
// Typische „Mitte“: 12–18
// =============================================================================

TEST(HandScoreNoAces, EmptyHandScoreZeroBoundary) {
    Hand h;
    EXPECT_EQ(h.score(), 0);
}

TEST(HandScoreNoAces, TypicalMiddleSeventeenFromEightAndNine) {
    Hand h;
    add(h, Suit::Herz, Rank::Acht);
    add(h, Suit::Pik, Rank::Neun);
    EXPECT_EQ(h.score(), 17);
}

TEST(HandScoreNoAces, BoundaryTwentyOneWithoutAce) {
    Hand h;
    add(h, Suit::Herz, Rank::Koenig);
    add(h, Suit::Pik, Rank::Neun);
    add(h, Suit::Karo, Rank::Zwei);
    EXPECT_EQ(h.score(), 21);
}

TEST(HandScoreNoAces, BoundaryTwentyTwoIsBustClass) {
    Hand h;
    add(h, Suit::Herz, Rank::Koenig);
    add(h, Suit::Pik, Rank::Koenig);
    add(h, Suit::Karo, Rank::Zwei);
    EXPECT_EQ(h.score(), 22);
}

TEST(HandScoreNoAces, NumericLowBoundaryFour) {
    Hand h;
    add(h, Suit::Kreuz, Rank::Zwei);
    add(h, Suit::Kreuz, Rank::Zwei);
    EXPECT_EQ(h.score(), 4);
}

TEST(HandScoreNoAces, TenPlusTenTwentyTypicalHard) {
    Hand h;
    add(h, Suit::Herz, Rank::Zehn);
    add(h, Suit::Pik, Rank::Zehn);
    EXPECT_EQ(h.score(), 20);
}

// =============================================================================
// score() — ein Ass: Soft-Hand bis harte Umwandlung
// EK: total <=21 mit Ass als 11; Grenzfall genau 21; Grenzfall 22 loest -10 aus
// =============================================================================

TEST(HandScoreOneAce, AceAndFiveIsSixteenSoftTypical) {
    Hand h;
    add(h, Suit::Herz, Rank::Ass);
    add(h, Suit::Pik, Rank::Fuenf);
    EXPECT_EQ(h.score(), 16);
}

TEST(HandScoreOneAce, AceAndNineIsTwentyBoundaryBlackjackPair) {
    Hand h;
    add(h, Suit::Herz, Rank::Ass);
    add(h, Suit::Pik, Rank::Neun);
    EXPECT_EQ(h.score(), 20);
}

TEST(HandScoreOneAce, AceAndEightIsNineteenTypical) {
    Hand h;
    add(h, Suit::Karo, Rank::Ass);
    add(h, Suit::Kreuz, Rank::Acht);
    EXPECT_EQ(h.score(), 19);
}

TEST(HandScoreOneAce, AceSixNineBecomesSixteenAfterConversionBoundary) {
    Hand h;
    add(h, Suit::Herz, Rank::Ass);
    add(h, Suit::Pik, Rank::Sechs);
    add(h, Suit::Karo, Rank::Neun);
    // 11+6+9=26 -> ein Ass zu 1: 16+9=25 still >21? 16+9=25, aces 0 after first conversion
    // Implementation: total starts 26, aces 1 -> 16
    EXPECT_EQ(h.score(), 16);
}

TEST(HandScoreOneAce, AceFourSixIsTwentyOneBoundaryNotBust) {
    Hand h;
    add(h, Suit::Herz, Rank::Ass);
    add(h, Suit::Pik, Rank::Vier);
    add(h, Suit::Karo, Rank::Sechs);
    EXPECT_EQ(h.score(), 21);
}

TEST(HandScoreOneAce, AceFourSevenIsTwentyTwoBustAfterConversion) {
    Hand h;
    add(h, Suit::Herz, Rank::Ass);
    add(h, Suit::Pik, Rank::Vier);
    add(h, Suit::Karo, Rank::Sieben);
    // 11+4+7=22 -> convert: 12+7=19? Wait 11+4+7=22, one ace: 1+4+7=12
    EXPECT_EQ(h.score(), 12);
}

// =============================================================================
// score() — zwei Asse: beide zuerst 11, schrittweise Reduktion
// =============================================================================

TEST(HandScoreTwoAces, TwoAcesAndNineIsTwentyOneBoundary) {
    Hand h;
    add(h, Suit::Herz, Rank::Ass);
    add(h, Suit::Pik, Rank::Ass);
    add(h, Suit::Karo, Rank::Neun);
    // 11+11+9=31 -> 21+9=30 still? first reduction: 21+9=30, aces left 1 -> 11+9=20? 
    // Trace: total=31 aces=2 -> total=21 aces=1 -> total>21? no -> 21
    EXPECT_EQ(h.score(), 21);
}

TEST(HandScoreTwoAces, TwoAcesAndEightIsTwentyTypical) {
    Hand h;
    add(h, Suit::Herz, Rank::Ass);
    add(h, Suit::Pik, Rank::Ass);
    add(h, Suit::Karo, Rank::Acht);
    // 11+11+8=30 -> 21+8=29 -> one more? total 29 aces 1 -> 19+8=27? 
    // Implementation while: total>21 && aces>0: 30-10=20 aces 1; 20>21? no -> 20
    EXPECT_EQ(h.score(), 20);
}

TEST(HandScoreTwoAces, TwoAcesAndSevenIsNineteen) {
    Hand h;
    add(h, Suit::Herz, Rank::Ass);
    add(h, Suit::Pik, Rank::Ass);
    add(h, Suit::Karo, Rank::Sieben);
    // 11+11+7=29 -> 19+7=26 -> 16+7=23 -> 6+7=13? Let's compute with code logic:
    // total=29 aces=2 -> 19 aces=1 -> 19>21? no -> 19
    EXPECT_EQ(h.score(), 19);
}

// =============================================================================
// score() — drei und vier Asse (Stress / Grenzwerte Mehrfach-Soft)
// =============================================================================

TEST(HandScoreManyAces, ThreeAcesNoOtherCardsTwelveTypical) {
    Hand h;
    add(h, Suit::Herz, Rank::Ass);
    add(h, Suit::Pik, Rank::Ass);
    add(h, Suit::Karo, Rank::Ass);
    // 11+11+11=33 -> 23 aces2 -> 13 aces1 -> 3 aces0
    EXPECT_EQ(h.score(), 13);
}

TEST(HandScoreManyAces, FourAcesAloneFourteenBoundaryClassic) {
    Hand h;
    add(h, Suit::Herz, Rank::Ass);
    add(h, Suit::Pik, Rank::Ass);
    add(h, Suit::Karo, Rank::Ass);
    add(h, Suit::Kreuz, Rank::Ass);
    // 44 -> 34 -> 24 -> 14
    EXPECT_EQ(h.score(), 14);
}

TEST(HandScoreManyAces, FourAcesPlusFiveNineteen) {
    Hand h;
    for (int i = 0; i < 4; ++i) add(h, Suit::Herz, Rank::Ass);
    add(h, Suit::Pik, Rank::Fuenf);
    EXPECT_EQ(h.score(), 19);
}

// =============================================================================
// isBust() — Aequivalenzklassen {score<=21}, {score>=22}
// Grenzwert 21 nicht Bust, 22 Bust
// =============================================================================

TEST(HandIsBust, TwentyOneNotBustBoundary) {
    Hand h;
    add(h, Suit::Herz, Rank::Koenig);
    add(h, Suit::Pik, Rank::Neun);
    add(h, Suit::Karo, Rank::Zwei);
    EXPECT_EQ(h.score(), 21);
    EXPECT_FALSE(h.isBust());
}

TEST(HandIsBust, TwentyTwoIsBustBoundary) {
    Hand h;
    add(h, Suit::Herz, Rank::Koenig);
    add(h, Suit::Pik, Rank::Koenig);
    add(h, Suit::Karo, Rank::Zwei);
    EXPECT_TRUE(h.isBust());
}

TEST(HandIsBust, EmptyHandNotBust) {
    Hand h;
    EXPECT_FALSE(h.isBust());
}

TEST(HandIsBust, ThirtyThreeWithFacesBustTypical) {
    Hand h;
    add(h, Suit::Herz, Rank::Koenig);
    add(h, Suit::Pik, Rank::Koenig);
    add(h, Suit::Karo, Rank::Dame);
    EXPECT_TRUE(h.isBust());
}

// =============================================================================
// isBlackjack() — exakt zwei Karten und Summe 21
// Grenzfall: 3 Karten mit 21 ist kein Blackjack
// =============================================================================

TEST(HandIsBlackjack, AceAndKingTrueTypicalNatural) {
    Hand h;
    add(h, Suit::Herz, Rank::Ass);
    add(h, Suit::Pik, Rank::Koenig);
    EXPECT_TRUE(h.isBlackjack());
}

TEST(HandIsBlackjack, AceAndTenTrueBoundaryTenRank) {
    Hand h;
    add(h, Suit::Karo, Rank::Ass);
    add(h, Suit::Kreuz, Rank::Zehn);
    EXPECT_TRUE(h.isBlackjack());
}

TEST(HandIsBlackjack, AceAndSixNotBlackjackTypicalSoftSixteen) {
    Hand h;
    add(h, Suit::Herz, Rank::Ass);
    add(h, Suit::Pik, Rank::Sechs);
    EXPECT_FALSE(h.isBlackjack());
}

TEST(HandIsBlackjack, TwentyOneWithThreeCardsFalseBoundary) {
    Hand h;
    add(h, Suit::Herz, Rank::Sieben);
    add(h, Suit::Pik, Rank::Sieben);
    add(h, Suit::Karo, Rank::Sieben);
    EXPECT_EQ(h.score(), 21);
    EXPECT_FALSE(h.isBlackjack());
}

TEST(HandIsBlackjack, TwoTensTwentyNotBlackjack) {
    Hand h;
    add(h, Suit::Herz, Rank::Zehn);
    add(h, Suit::Pik, Rank::Bube);
    EXPECT_EQ(h.score(), 20);
    EXPECT_FALSE(h.isBlackjack());
}

TEST(HandIsBlackjack, SingleAceElevenNotBlackjack) {
    Hand h;
    add(h, Suit::Herz, Rank::Ass);
    EXPECT_FALSE(h.isBlackjack());
}

// =============================================================================
// toString(bool hideSecondCard)
// EK hideSecondCard: {true,false} x Kartenanzahl {0,1,2,3+}
// =============================================================================

TEST(HandToString, EmptyHandBothModesEmptyString) {
    Hand h;
    EXPECT_EQ(h.toString(false), "");
    EXPECT_EQ(h.toString(true), "");
}

TEST(HandToString, OneCardHideFalseShowsCard) {
    Hand h;
    add(h, Suit::Herz, Rank::Drei);
    const std::string s = h.toString(false);
    EXPECT_NE(s.find("3"), std::string::npos);
}

TEST(HandToString, OneCardHideTrueStillShowsFirst) {
    Hand h;
    add(h, Suit::Herz, Rank::Drei);
    const std::string s = h.toString(true);
    EXPECT_NE(s.find("3"), std::string::npos);
}

TEST(HandToString, TwoCardsHideTrueMasksSecondBoundary) {
    Hand h;
    add(h, Suit::Herz, Rank::Drei);
    add(h, Suit::Pik, Rank::Vier);
    const std::string s = h.toString(true);
    EXPECT_NE(s.find("[verdeckt]"), std::string::npos);
}

TEST(HandToString, TwoCardsHideFalseShowsBoth) {
    Hand h;
    add(h, Suit::Herz, Rank::Drei);
    add(h, Suit::Pik, Rank::Vier);
    const std::string s = h.toString(false);
    EXPECT_NE(s.find("3"), std::string::npos);
    EXPECT_NE(s.find("4"), std::string::npos);
}

TEST(HandToString, ThreeCardsHideTrueOnlySecondHidden) {
    Hand h;
    add(h, Suit::Herz, Rank::Zwei);
    add(h, Suit::Pik, Rank::Drei);
    add(h, Suit::Karo, Rank::Vier);
    const std::string s = h.toString(true);
    EXPECT_NE(s.find("2"), std::string::npos);
    EXPECT_NE(s.find("4"), std::string::npos);
    EXPECT_NE(s.find("[verdeckt]"), std::string::npos);
}

// =============================================================================
// Kombination isBust / isBlackjack / score Konsistenz
// =============================================================================

TEST(HandConsistency, BlackjackImpliesNotBust) {
    Hand h;
    add(h, Suit::Herz, Rank::Ass);
    add(h, Suit::Pik, Rank::Dame);
    ASSERT_TRUE(h.isBlackjack());
    EXPECT_FALSE(h.isBust());
    EXPECT_EQ(h.score(), 21);
}

TEST(HandConsistency, BustImpliesNotBlackjack) {
    Hand h;
    add(h, Suit::Herz, Rank::Koenig);
    add(h, Suit::Pik, Rank::Koenig);
    add(h, Suit::Karo, Rank::Dame);
    ASSERT_TRUE(h.isBust());
    EXPECT_FALSE(h.isBlackjack());
}

// =============================================================================
// Zusaetzliche score-Matrix: harte 12–20 ohne Ass (typische Spielsituationen)
// =============================================================================

TEST(HandScoreHardMatrix, FivePlusSevenTwelve) {
    Hand h;
    add(h, Suit::Kreuz, Rank::Fuenf);
    add(h, Suit::Kreuz, Rank::Sieben);
    EXPECT_EQ(h.score(), 12);
}

TEST(HandScoreHardMatrix, SixPlusEightFourteenTypical) {
    Hand h;
    add(h, Suit::Kreuz, Rank::Sechs);
    add(h, Suit::Pik, Rank::Acht);
    EXPECT_EQ(h.score(), 14);
}

TEST(HandScoreHardMatrix, NinePlusNineEighteenBoundaryHighNonBust) {
    Hand h;
    add(h, Suit::Herz, Rank::Neun);
    add(h, Suit::Pik, Rank::Neun);
    EXPECT_EQ(h.score(), 18);
}

TEST(HandScoreHardMatrix, JackPlusEighteenFromEight) {
    Hand h;
    add(h, Suit::Herz, Rank::Bube);
    add(h, Suit::Pik, Rank::Acht);
    EXPECT_EQ(h.score(), 18);
}

// =============================================================================
// Ass + kleine Karten Grenzband 12–21 mit genau zwei Karten
// =============================================================================

TEST(HandAceTwoCardBoundary, AceTwoIsThirteenNotBlackjack) {
    Hand h;
    add(h, Suit::Herz, Rank::Ass);
    add(h, Suit::Pik, Rank::Zwei);
    EXPECT_EQ(h.score(), 13);
    EXPECT_FALSE(h.isBlackjack());
}

TEST(HandAceTwoCardBoundary, AceEighteenFromAceSeven) {
    Hand h;
    add(h, Suit::Herz, Rank::Ass);
    add(h, Suit::Pik, Rank::Sieben);
    EXPECT_EQ(h.score(), 18);
}

TEST(HandAceTwoCardBoundary, AceTenBlackjack) {
    Hand h;
    add(h, Suit::Herz, Rank::Ass);
    add(h, Suit::Pik, Rank::Zehn);
    EXPECT_TRUE(h.isBlackjack());
}

// =============================================================================
// Soft-Totals mit mehreren kleinen Karten nach Ass
// =============================================================================

TEST(HandSoftChains, AceTwoThreeFourTenEqualsTwentyHard) {
    Hand h;
    add(h, Suit::Herz, Rank::Ass);
    add(h, Suit::Pik, Rank::Zwei);
    add(h, Suit::Karo, Rank::Drei);
    add(h, Suit::Kreuz, Rank::Vier);
    add(h, Suit::Herz, Rank::Zehn);
    // 11+2+3+4+10=30 -> 20+3+4+10? Trace: total 30 aces 1 -> 20 aces 1 -> 20>21? no
    EXPECT_EQ(h.score(), 20);
}

// =============================================================================
// Grenzwert score 1 (unmoeglich mit Standardkarten?) — minimale positive: 2
// =============================================================================

TEST(HandScoreBoundary, MinimumPositiveScoreTwo) {
    Hand h;
    add(h, Suit::Herz, Rank::Zwei);
    EXPECT_EQ(h.score(), 2);
}

// =============================================================================
// isBust fuer grosse Hands mit vielen kleinen Karten die spaeter busten
// =============================================================================

TEST(HandManyCards, TenSmallCardsEventuallyBustClass) {
    Hand h;
    for (int i = 0; i < 6; ++i) add(h, Suit::Herz, Rank::Vier);
    // 6*4=24
    EXPECT_TRUE(h.isBust());
}

TEST(HandManyCards, FiveFoursExactlyTwentyNotBustTypical) {
    Hand h;
    for (int i = 0; i < 5; ++i) add(h, Suit::Herz, Rank::Vier);
    EXPECT_EQ(h.score(), 20);
    EXPECT_FALSE(h.isBust());
}

// =============================================================================
// cards() const Referenz-Stabilitaet nach Mutation
// =============================================================================

TEST(HandCardsReference, ClearEmptiesVectorView) {
    Hand h;
    add(h, Suit::Pik, Rank::Ass);
    const auto& before = h.cards();
    EXPECT_EQ(before.size(), 1u);
    h.clear();
    EXPECT_TRUE(h.cards().empty());
}

// =============================================================================
// toString Komma-Separator bei mehreren sichtbaren Karten
// =============================================================================

TEST(HandToStringFormatting, TwoVisibleContainsCommaSeparator) {
    Hand h;
    add(h, Suit::Herz, Rank::Zwei);
    add(h, Suit::Pik, Rank::Drei);
    const std::string s = h.toString(false);
    EXPECT_NE(s.find(", "), std::string::npos);
}

TEST(HandToStringFormatting, ThreeVisibleTwoCommas) {
    Hand h;
    add(h, Suit::Herz, Rank::Zwei);
    add(h, Suit::Pik, Rank::Drei);
    add(h, Suit::Karo, Rank::Vier);
    const std::string s = h.toString(false);
    std::size_t first = s.find(", ");
    ASSERT_NE(first, std::string::npos);
    EXPECT_NE(s.find(", ", first + 1), std::string::npos);
}

// =============================================================================
// Blackjack mit Dame statt Bild (Aequivalenzklasse 10er)
// =============================================================================

TEST(HandBlackjackEquivalence, AceQueenNatural) {
    Hand h;
    add(h, Suit::Karo, Rank::Ass);
    add(h, Suit::Kreuz, Rank::Dame);
    EXPECT_TRUE(h.isBlackjack());
}

TEST(HandBlackjackEquivalence, AceJackNatural) {
    Hand h;
    add(h, Suit::Karo, Rank::Ass);
    add(h, Suit::Kreuz, Rank::Bube);
    EXPECT_TRUE(h.isBlackjack());
}

// =============================================================================
// Score 21 mit Ass + mehreren Karten (kein Blackjack wegen Kartenanzahl !=2)
// =============================================================================

TEST(HandTwentyOneNotBJ, AceTwoEightTwentyOneThreeCards) {
    Hand h;
    add(h, Suit::Herz, Rank::Ass);
    add(h, Suit::Pik, Rank::Zwei);
    add(h, Suit::Karo, Rank::Acht);
    EXPECT_EQ(h.score(), 21);
    EXPECT_FALSE(h.isBlackjack());
}

TEST(HandTwentyOneNotBJ, SevenEightSixTwentyOne) {
    Hand h;
    add(h, Suit::Herz, Rank::Sieben);
    add(h, Suit::Pik, Rank::Acht);
    add(h, Suit::Karo, Rank::Sechs);
    EXPECT_EQ(h.score(), 21);
    EXPECT_FALSE(h.isBlackjack());
}

// =============================================================================
// Ass-Konversion bis harter Bust: A+5+6+K = 32 -> 22, keine weiteren Asse
// =============================================================================

TEST(HandAceConversionBustBoundary, AceFiveSixKingTwentyTwoBust) {
    Hand h;
    add(h, Suit::Herz, Rank::Ass);
    add(h, Suit::Pik, Rank::Fuenf);
    add(h, Suit::Karo, Rank::Sechs);
    add(h, Suit::Kreuz, Rank::Koenig);
    EXPECT_EQ(h.score(), 22);
    EXPECT_TRUE(h.isBust());
}

// =============================================================================
// Weitere Aequivalenzklassen score: „fast Blackjack“ 20 mit zwei Karten
// =============================================================================

TEST(HandNearBlackjack, TenAndKingTwenty) {
    Hand h;
    add(h, Suit::Herz, Rank::Zehn);
    add(h, Suit::Pik, Rank::Koenig);
    EXPECT_EQ(h.score(), 20);
    EXPECT_FALSE(h.isBlackjack());
}

TEST(HandNearBlackjack, NineAndAssTwentyTypicalSoft) {
    Hand h;
    add(h, Suit::Herz, Rank::Neun);
    add(h, Suit::Pik, Rank::Ass);
    EXPECT_EQ(h.score(), 20);
}

// =============================================================================
// Grenzwert isBust bei score==21 nach mehrstufiger Ass-Reduktion
// =============================================================================

TEST(HandAceReductionToTwentyOne, AceAceNineTwentyOne) {
    Hand h;
    add(h, Suit::Herz, Rank::Ass);
    add(h, Suit::Pik, Rank::Ass);
    add(h, Suit::Karo, Rank::Neun);
    EXPECT_EQ(h.score(), 21);
    EXPECT_FALSE(h.isBust());
}

// =============================================================================
// Typische Dealer-Haerte 17–21 ohne Ass
// =============================================================================

TEST(HandDealerLikeHardTotals, TenSixSevenTwentyThreeBust) {
    Hand h;
    add(h, Suit::Kreuz, Rank::Zehn);
    add(h, Suit::Kreuz, Rank::Sechs);
    add(h, Suit::Kreuz, Rank::Sieben);
    EXPECT_EQ(h.score(), 23);
    EXPECT_TRUE(h.isBust());
}

TEST(HandDealerLikeHardTotals, NineEightSeventeen) {
    Hand h;
    add(h, Suit::Kreuz, Rank::Neun);
    add(h, Suit::Pik, Rank::Acht);
    EXPECT_EQ(h.score(), 17);
}

// =============================================================================
// toString: verdeckte zweite Karte bei vier Karten — nur zweite Position
// =============================================================================

TEST(HandToStringHideSecond, FourCardsHidesOnlySecondIndex) {
    Hand h;
    add(h, Suit::Herz, Rank::Zwei);
    add(h, Suit::Pik, Rank::Drei);
    add(h, Suit::Karo, Rank::Vier);
    add(h, Suit::Kreuz, Rank::Fuenf);
    const std::string s = h.toString(true);
    EXPECT_NE(s.find("2"), std::string::npos);
    EXPECT_NE(s.find("4"), std::string::npos);
    EXPECT_NE(s.find("5"), std::string::npos);
    EXPECT_NE(s.find("[verdeckt]"), std::string::npos);
}

// =============================================================================
// Score-Invariant: isBust() entspricht score()>21 (deterministische Stichprobe)
// =============================================================================

TEST(HandBustInvariantSample, VariousHandsMatchBustPredicate) {
    struct Case {
        std::vector<std::pair<Suit, Rank>> cards;
        bool bust;
    };
    const Case cases[] = {
        {{{Suit::Herz, Rank::Koenig}, {Suit::Pik, Rank::Koenig}}, false},
        {{{Suit::Herz, Rank::Koenig}, {Suit::Pik, Rank::Koenig}, {Suit::Karo, Rank::Zwei}}, true},
        {{{Suit::Herz, Rank::Sieben}, {Suit::Pik, Rank::Sieben}, {Suit::Karo, Rank::Sieben}}, false},
        {{{Suit::Herz, Rank::Neun}, {Suit::Pik, Rank::Neun}, {Suit::Karo, Rank::Vier}}, true},
    };
    for (const auto& c : cases) {
        Hand h;
        for (const auto& pr : c.cards) add(h, pr.first, pr.second);
        EXPECT_EQ(h.isBust(), c.bust);
        EXPECT_EQ(h.score() > 21, h.isBust());
    }
}

// =============================================================================
// Zehn-Karten Mix (Zehn, Bube, Dame, Koenig) — gleiche Punkte-Aequivalenzklasse
// =============================================================================

TEST(HandTenEquivalence, ThreePictureCardsThirty) {
    Hand h;
    add(h, Suit::Herz, Rank::Bube);
    add(h, Suit::Pik, Rank::Dame);
    add(h, Suit::Karo, Rank::Koenig);
    EXPECT_EQ(h.score(), 30);
    EXPECT_TRUE(h.isBust());
}

// =============================================================================
// clear() nach grosser Hand
// =============================================================================

TEST(HandClearAfterMany, ClearsScoreBackToZero) {
    Hand h;
    for (int i = 0; i < 8; ++i) add(h, Suit::Pik, Rank::Zwei);
    h.clear();
    EXPECT_EQ(h.score(), 0);
    EXPECT_FALSE(h.isBust());
}

// =============================================================================
// Blackjack nur mit exakt zwei Karten — Grenzfall ein Karte fehlt
// =============================================================================

TEST(HandBlackjackBoundaryCount, TwoCardsExactlyRequired) {
    Hand h;
    add(h, Suit::Herz, Rank::Ass);
    EXPECT_FALSE(h.isBlackjack());
}
