#include "Deck.h"

#include "Card.h"

#include <gtest/gtest.h>

#include <algorithm>
#include <map>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

using Suit = Card::Suit;
using Rank = Card::Rank;

namespace {

std::string cardKey(const Card& c) {
    return std::to_string(static_cast<int>(c.suit())) + ":" + std::to_string(static_cast<int>(c.rank()));
}

std::map<std::string, int> multisetFromDeck(Deck& d) {
    std::map<std::string, int> m;
    while (d.size() > 0) {
        const Card c = d.draw();
        m[cardKey(c)] += 1;
    }
    return m;
}

Deck freshDeck() { return Deck{}; }

} // namespace

// =============================================================================
// Klasse Deck — Aequivalenzklassen Konstruktor / size
// Grenzwert: 52 Karten nach Erzeugung (volles franzoesisches Deck)
// =============================================================================

TEST(DeckConstruction, InitialSizeIsFiftyTwoBoundaryFullPack) {
    Deck d;
    EXPECT_EQ(d.size(), 52u);
}

TEST(DeckConstruction, RepeatedConstructionYieldsFiftyTwo) {
    for (int i = 0; i < 5; ++i) {
        Deck d;
        EXPECT_EQ(d.size(), 52u);
    }
}

// =============================================================================
// Inhalt: genau 4x13 Karten, jede Kombination SuitxRank genau einmal
// Typischer Wert „Mitte“: Herz Sieben; Grenzen: Kreuz Zwei, Karo Ass
// =============================================================================

TEST(DeckContent, ContainsExactlyThirteenRanksPerSuit) {
    Deck d;
    std::map<Suit, int> perSuit;
    while (d.size() > 0) {
        const Card c = d.draw();
        perSuit[c.suit()] += 1;
    }
    EXPECT_EQ(perSuit[Suit::Kreuz], 13);
    EXPECT_EQ(perSuit[Suit::Pik], 13);
    EXPECT_EQ(perSuit[Suit::Herz], 13);
    EXPECT_EQ(perSuit[Suit::Karo], 13);
}

TEST(DeckContent, EachRankAppearsFourTimesAcrossSuits) {
    Deck d;
    std::map<Rank, int> perRank;
    while (d.size() > 0) {
        const Card c = d.draw();
        perRank[c.rank()] += 1;
    }
    for (const auto& kv : perRank) {
        EXPECT_EQ(kv.second, 4) << "rank count should be four suits";
    }
    EXPECT_EQ(perRank.size(), 13u);
}

TEST(DeckContent, FiftyTwoUniqueStringRepresentations) {
    Deck d;
    std::set<std::string> seen;
    while (d.size() > 0) {
        const Card c = d.draw();
        seen.insert(c.toString());
    }
    EXPECT_EQ(seen.size(), 52u);
}

// =============================================================================
// shuffle() — Invariante Groesse; Inhalt als Multimenge gleich
// =============================================================================

TEST(DeckShuffle, SizeUnchangedAfterShuffleTypical) {
    Deck d;
    d.shuffle();
    EXPECT_EQ(d.size(), 52u);
}

TEST(DeckShuffle, SizeUnchangedAfterMultipleShufflesBoundaryStress) {
    Deck d;
    for (int i = 0; i < 20; ++i) {
        d.shuffle();
        EXPECT_EQ(d.size(), 52u);
    }
}

TEST(DeckShuffle, MultisetUnchangedAfterShuffleEquivalence) {
    Deck a;
    const auto before = multisetFromDeck(a);
    Deck b;
    b.shuffle();
    const auto after = multisetFromDeck(b);
    EXPECT_EQ(before, after);
}

TEST(DeckShuffle, TwoConsecutiveShufflesLikelyProduceDifferentOrderHighEntropy) {
    Deck d1;
    d1.shuffle();
    Deck d2;
    d2.shuffle();
    std::vector<std::string> k1, k2;
    while (d1.size() > 0) k1.push_back(cardKey(d1.draw()));
    while (d2.size() > 0) k2.push_back(cardKey(d2.draw()));
    ASSERT_EQ(k1.size(), k2.size());
    // Sehr unwahrscheinlich identisch bei 52! Permutationen — wir erlauben seltenen Zufallstreffer
    int diff = 0;
    for (std::size_t i = 0; i < k1.size(); ++i) {
        if (k1[i] != k2[i]) ++diff;
    }
    EXPECT_GT(diff, 0) << "two shuffles should almost always differ in order";
}

// =============================================================================
// draw() — gueltige Klasse solange size>0; Grenzfall letzte Karte
// =============================================================================

TEST(DeckDraw, DrawDecrementsSizeByOneTypicalMiddle) {
    Deck d;
    (void)d.draw();
    EXPECT_EQ(d.size(), 51u);
}

TEST(DeckDraw, DrawFiftyTwoTimesEmptiesDeckBoundary) {
    Deck d;
    for (int i = 0; i < 52; ++i) {
        ASSERT_GT(d.size(), 0u);
        (void)d.draw();
    }
    EXPECT_EQ(d.size(), 0u);
}

TEST(DeckDraw, DrawOnEmptyDeckThrowsRuntimeErrorInvalidClass) {
    Deck d;
    while (d.size() > 0) (void)d.draw();
    EXPECT_THROW((void)d.draw(), std::runtime_error);
}

TEST(DeckDraw, LastCardLeavesZeroThenThrow) {
    Deck d;
    for (int i = 0; i < 51; ++i) (void)d.draw();
    EXPECT_EQ(d.size(), 1u);
    (void)d.draw();
    EXPECT_EQ(d.size(), 0u);
    EXPECT_THROW((void)d.draw(), std::runtime_error);
}

// =============================================================================
// draw() Reihenfolge ohne shuffle: deterministisch aus Konstruktor-Layout
// (Implementierung: pop_back — letzte eingefuegte Karte zuerst)
// =============================================================================

TEST(DeckDrawOrder, WithoutShuffleDeterministicTailOrder) {
    Deck d;
    const Card first = d.draw();
    // Letzte im vector nach Build: Karo Ass (siehe verschachtelte Schleifen Kreuz..Karo, Zwei..Ass)
    EXPECT_EQ(first.suit(), Suit::Karo);
    EXPECT_EQ(first.rank(), Rank::Ass);
}

// =============================================================================
// shuffle + draw Integration: keine Duplikate bis leer
// =============================================================================

TEST(DeckIntegration, ShuffleThenDrawAllUniqueKeys) {
    Deck d;
    d.shuffle();
    std::set<std::string> seen;
    while (d.size() > 0) {
        const Card c = d.draw();
        const std::string k = cardKey(c);
        EXPECT_TRUE(seen.insert(k).second) << "duplicate card drawn";
    }
}

// =============================================================================
// size() — typische Werte 52, Mitte 26 nach Haelfte, Grenze 0
// =============================================================================

TEST(DeckSizeProgression, HalvingDeckTypicalTwentySix) {
    Deck d;
    for (int i = 0; i < 26; ++i) (void)d.draw();
    EXPECT_EQ(d.size(), 26u);
}

TEST(DeckSizeProgression, ZeroAfterFullDrainBoundary) {
    Deck d;
    while (d.size()) (void)d.draw();
    EXPECT_EQ(d.size(), 0u);
}

// =============================================================================
// Zusaetzliche Aequivalenzpartition: „Deck nach shuffle hat keine feste erste
// Karte“ (nicht identisch zum ungeschuffelten ersten Zug in grosser Stichprobe)
// =============================================================================

TEST(DeckShuffleEffect, FirstCardAfterShuffleUsuallyDiffersFromUnshuffledFirst) {
    const Card unshuffledFirst = [] {
        Deck d;
        return d.draw();
    }();
    int different = 0;
    for (int trial = 0; trial < 30; ++trial) {
        Deck d;
        d.shuffle();
        const Card c = d.draw();
        if (c.suit() != unshuffledFirst.suit() || c.rank() != unshuffledFirst.rank()) {
            ++different;
        }
    }
    EXPECT_GT(different, 0);
}

// =============================================================================
// Fehlerklasse draw: mehrfaches throw nach leer
// =============================================================================

TEST(DeckDrawErrorClass, ThrowsRepeatedlyOnEmpty) {
    Deck d;
    while (d.size()) (void)d.draw();
    EXPECT_THROW((void)d.draw(), std::runtime_error);
    EXPECT_THROW((void)d.draw(), std::runtime_error);
}

// =============================================================================
// Konstruktor vs. Zuweisung: neues Deck wieder 52
// =============================================================================

TEST(DeckAssignment, ReassignFreshRestoresFiftyTwo) {
    Deck d;
    while (d.size() > 10) (void)d.draw();
    d = Deck{};
    EXPECT_EQ(d.size(), 52u);
}

// =============================================================================
// shuffle auf kleinem Restdeck (nach vielen Zuegen): Invariante Restgroesse
// =============================================================================

TEST(DeckShufflePartial, ShuffleWhenTenCardsLeftKeepsTen) {
    Deck d;
    while (d.size() > 10) (void)d.draw();
    ASSERT_EQ(d.size(), 10u);
    d.shuffle();
    EXPECT_EQ(d.size(), 10u);
}

TEST(DeckShufflePartial, ShuffleNineLeftPreservesSizeThenAllDrawable) {
    Deck d;
    while (d.size() > 9) (void)d.draw();
    ASSERT_EQ(d.size(), 9u);
    d.shuffle();
    int drawn = 0;
    while (d.size() > 0) {
        (void)d.draw();
        ++drawn;
    }
    EXPECT_EQ(drawn, 9);
}

// =============================================================================
// Zusaetzliche Grenzwerte size: 1 Karte verbleibend
// =============================================================================

TEST(DeckDrawBoundary, OneCardLeftThenDrawEmpties) {
    Deck d;
    while (d.size() > 1) (void)d.draw();
    EXPECT_EQ(d.size(), 1u);
    (void)d.draw();
    EXPECT_EQ(d.size(), 0u);
}

// =============================================================================
// Typischer Mittelwert: 26 Karten nach Haelfte — erneut shuffle erlaubt
// =============================================================================

TEST(DeckShuffleMid, ShuffleAtTwentySixCardsPreservesCount) {
    Deck d;
    for (int i = 0; i < 26; ++i) (void)d.draw();
    ASSERT_EQ(d.size(), 26u);
    d.shuffle();
    EXPECT_EQ(d.size(), 26u);
}

// =============================================================================
// draw liefert gueltige Karte (52-mal hintereinander)
// =============================================================================

TEST(DeckDrawValidity, EachDrawnCardHasValidRankAndSuit) {
    Deck d;
    d.shuffle();
    while (d.size() > 0) {
        const Card c = d.draw();
        const int rv = static_cast<int>(c.rank());
        EXPECT_GE(rv, 2);
        EXPECT_LE(rv, 14);
    }
}

// =============================================================================
// Stress: mehrere Decks nacheinander unabhaengig
// =============================================================================

TEST(DeckIndependence, TwoSeparateDecksBothFiftyTwo) {
    Deck a;
    Deck b;
    EXPECT_EQ(a.size(), 52u);
    EXPECT_EQ(b.size(), 52u);
    (void)a.draw();
    EXPECT_EQ(a.size(), 51u);
    EXPECT_EQ(b.size(), 52u);
}
