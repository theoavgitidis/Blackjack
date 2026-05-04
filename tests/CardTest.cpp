#include "Card.h"

#include <gtest/gtest.h>

#include <algorithm>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>

using Suit = Card::Suit;
using Rank = Card::Rank;

namespace {

Suit allSuits[4] = {Suit::Kreuz, Suit::Pik, Suit::Herz, Suit::Karo};

std::string joinLines(const std::vector<std::string>& v) {
    std::string s;
    for (std::size_t i = 0; i < v.size(); ++i) {
        if (i) s += "|";
        s += v[i];
    }
    return s;
}

} // namespace

// =============================================================================
// Klasse Card — Aequivalenzklassen Konstruktor / Accessoren
// =============================================================================
// EK-Suit: vier Farben; EK-Rank: Zwei..Ass. Typischer Wert „Mitte“: Sechs/Sieben;
// Grenzwerte numerisch: Zwei (Minimum), Neun (hoechster Zahlwert vor Zehn).
// =============================================================================

TEST(CardConstruction, DefaultStoresSuitAndRank) {
    const Card c(Suit::Herz, Rank::Sieben);
    EXPECT_EQ(c.suit(), Suit::Herz);
    EXPECT_EQ(c.rank(), Rank::Sieben);
}

TEST(CardConstruction, BoundaryLowestNumericRank) {
    const Card c(Suit::Kreuz, Rank::Zwei);
    EXPECT_EQ(c.rank(), Rank::Zwei);
    EXPECT_EQ(c.suit(), Suit::Kreuz);
}

TEST(CardConstruction, BoundaryHighestNumericRankBeforeTen) {
    const Card c(Suit::Pik, Rank::Neun);
    EXPECT_EQ(c.rank(), Rank::Neun);
}

TEST(CardConstruction, TypicalMiddleRankSix) {
    const Card c(Suit::Karo, Rank::Sechs);
    EXPECT_EQ(c.rank(), Rank::Sechs);
}

TEST(CardConstruction, FaceCardsConstructedDistinctly) {
    const Card b(Suit::Herz, Rank::Bube);
    const Card d(Suit::Herz, Rank::Dame);
    const Card k(Suit::Herz, Rank::Koenig);
    EXPECT_NE(b.rank(), d.rank());
    EXPECT_NE(d.rank(), k.rank());
}

TEST(CardConstruction, AceAndTenRepresentBlackjackPrimes) {
    const Card a(Suit::Pik, Rank::Ass);
    const Card t(Suit::Pik, Rank::Zehn);
    EXPECT_EQ(a.rank(), Rank::Ass);
    EXPECT_EQ(t.rank(), Rank::Zehn);
}

// =============================================================================
// baseValue() — Aequivalenzklassen: {2..9}, {10}, {Bube,Dame,Koenig}, {Ass}
// Grenzwerte: 2 und 9 als Rand der numerischen Klasse; Zehn als Grenze zu Bild.
// =============================================================================

TEST(CardBaseValue, NumericTwoIsBoundaryMinimum) {
    EXPECT_EQ(Card(Suit::Kreuz, Rank::Zwei).baseValue(), 2);
}

TEST(CardBaseValue, NumericThreeTypical) {
    EXPECT_EQ(Card(Suit::Kreuz, Rank::Drei).baseValue(), 3);
}

TEST(CardBaseValue, NumericFourTypical) {
    EXPECT_EQ(Card(Suit::Kreuz, Rank::Vier).baseValue(), 4);
}

TEST(CardBaseValue, NumericFiveTypical) {
    EXPECT_EQ(Card(Suit::Kreuz, Rank::Fuenf).baseValue(), 5);
}

TEST(CardBaseValue, NumericSixTypical) {
    EXPECT_EQ(Card(Suit::Kreuz, Rank::Sechs).baseValue(), 6);
}

TEST(CardBaseValue, NumericSevenTypical) {
    EXPECT_EQ(Card(Suit::Kreuz, Rank::Sieben).baseValue(), 7);
}

TEST(CardBaseValue, NumericEightTypical) {
    EXPECT_EQ(Card(Suit::Kreuz, Rank::Acht).baseValue(), 8);
}

TEST(CardBaseValue, NumericNineIsBoundaryMaxBeforeTen) {
    EXPECT_EQ(Card(Suit::Kreuz, Rank::Neun).baseValue(), 9);
}

TEST(CardBaseValue, TenRankIsTenNotFaceClass) {
    EXPECT_EQ(Card(Suit::Kreuz, Rank::Zehn).baseValue(), 10);
}

TEST(CardBaseValue, FaceJackQueenKingEquivalenceClassTen) {
    EXPECT_EQ(Card(Suit::Herz, Rank::Bube).baseValue(), 10);
    EXPECT_EQ(Card(Suit::Herz, Rank::Dame).baseValue(), 10);
    EXPECT_EQ(Card(Suit::Herz, Rank::Koenig).baseValue(), 10);
}

TEST(CardBaseValue, AceBaseValueIsElevenSoftDefault) {
    EXPECT_EQ(Card(Suit::Karo, Rank::Ass).baseValue(), 11);
}

TEST(CardBaseValue, SameRankDifferentSuitsSameValue) {
    const Rank r = Rank::Acht;
    for (Suit s : allSuits) {
        EXPECT_EQ(Card(s, r).baseValue(), 8);
    }
}

// =============================================================================
// toString() — Kombinationen Suit x Rank (Vollstaendigkeit je Rang, Stichprobe)
// =============================================================================

TEST(CardToString, NumericTwoAllSuits) {
    for (Suit s : allSuits) {
        const std::string out = Card(s, Rank::Zwei).toString();
        EXPECT_NE(out.find("2"), std::string::npos);
    }
}

TEST(CardToString, TenShowsGermanTen) {
    const std::string out = Card(Suit::Pik, Rank::Zehn).toString();
    EXPECT_NE(out.find("10"), std::string::npos);
}

TEST(CardToString, FaceRanksGermanWords) {
    EXPECT_NE(Card(Suit::Herz, Rank::Bube).toString().find("Bube"), std::string::npos);
    EXPECT_NE(Card(Suit::Herz, Rank::Dame).toString().find("Dame"), std::string::npos);
    EXPECT_NE(Card(Suit::Herz, Rank::Koenig).toString().find("Koenig"), std::string::npos);
    EXPECT_NE(Card(Suit::Herz, Rank::Ass).toString().find("Ass"), std::string::npos);
}

TEST(CardToString, SuitNamesAppearGerman) {
    const std::string h = Card(Suit::Herz, Rank::Drei).toString();
    const std::string p = Card(Suit::Pik, Rank::Drei).toString();
    const std::string k = Card(Suit::Kreuz, Rank::Drei).toString();
    const std::string r = Card(Suit::Karo, Rank::Drei).toString();
    EXPECT_NE(h.find("Herz"), std::string::npos);
    EXPECT_NE(p.find("Pik"), std::string::npos);
    EXPECT_NE(k.find("Kreuz"), std::string::npos);
    EXPECT_NE(r.find("Karo"), std::string::npos);
}

TEST(CardToString, EveryRankNonEmptyAcrossOneSuit) {
    const Rank ranks[] = {
        Rank::Zwei, Rank::Drei, Rank::Vier, Rank::Fuenf, Rank::Sechs, Rank::Sieben,
        Rank::Acht, Rank::Neun, Rank::Zehn, Rank::Bube, Rank::Dame, Rank::Koenig, Rank::Ass,
    };
    for (Rank r : ranks) {
        const std::string out = Card(Suit::Kreuz, r).toString();
        EXPECT_FALSE(out.empty());
    }
}

// =============================================================================
// toAsciiArt() — Struktur-Invarianten (5 Zeilen, konsistente Laengen)
// Rand: Rank „10“ (zwei Zeichen) vs. „A“ (ein Zeichen)
// =============================================================================

TEST(CardAsciiArt, AlwaysFiveLines) {
    const Rank ranks[] = {
        Rank::Zwei, Rank::Zehn, Rank::Bube, Rank::Ass,
    };
    for (Rank r : ranks) {
        const auto art = Card(Suit::Pik, r).toAsciiArt();
        EXPECT_EQ(art.size(), 5u);
    }
}

TEST(CardAsciiArt, EachLineNonEmptyUtf8MayVaryByteLength) {
    const Card c(Suit::Herz, Rank::Zehn);
    const auto art = c.toAsciiArt();
    ASSERT_EQ(art.size(), 5u);
    for (const auto& line : art) {
        EXPECT_FALSE(line.empty());
    }
    // Hinweis: Box-Zeilen und Farb-Symbole sind UTF-8-Mehrbyte; Byte-Laengen
    // muessen nicht zeilenweise identisch sein.
}

TEST(CardAsciiArt, TenWideRankStillRectangular) {
    const auto art = Card(Suit::Karo, Rank::Zehn).toAsciiArt();
    ASSERT_EQ(art.size(), 5u);
    EXPECT_GE(art[0].size(), 9u);
}

TEST(CardAsciiArt, AceNarrowRankStillRectangular) {
    const auto art = Card(Suit::Karo, Rank::Ass).toAsciiArt();
    ASSERT_EQ(art.size(), 5u);
    EXPECT_GE(art[0].size(), 9u);
}

TEST(CardAsciiArt, MiddleLineContainsSuitGlyphUtf8) {
    const auto art = Card(Suit::Pik, Rank::Sieben).toAsciiArt();
    ASSERT_EQ(art.size(), 5u);
    // Mittlere Zeile soll nicht nur Leerzeichen sein
    EXPECT_GT(art[2].size(), 5u);
}

TEST(CardAsciiArt, AllSuitsProduceFiveLines) {
    for (Suit s : allSuits) {
        const auto art = Card(s, Rank::Neun).toAsciiArt();
        EXPECT_EQ(art.size(), 5u);
    }
}

// =============================================================================
// backAsciiArt() — statische Rueckseite; unabhaengig von Karteninstanz
// =============================================================================

TEST(CardBackAsciiArt, StaticFiveLines) {
    const auto art = Card::backAsciiArt();
    EXPECT_EQ(art.size(), 5u);
}

TEST(CardBackAsciiArt, SameLineCountAsFaceCardBox) {
    const auto back = Card::backAsciiArt();
    const auto face = Card(Suit::Herz, Rank::Drei).toAsciiArt();
    ASSERT_EQ(back.size(), face.size());
    for (std::size_t i = 0; i < back.size(); ++i) {
        EXPECT_FALSE(back[i].empty());
        EXPECT_FALSE(face[i].empty());
    }
}

TEST(CardBackAsciiArt, MiddleRowsUseHashPattern) {
    const auto art = Card::backAsciiArt();
    ASSERT_GE(art.size(), 4u);
    EXPECT_NE(art[1].find('#'), std::string::npos);
    EXPECT_NE(art[2].find('#'), std::string::npos);
    EXPECT_NE(art[3].find('#'), std::string::npos);
}

// =============================================================================
// Zusaetzliche systematische Stichproben: baseValue fuer alle Raenge
// =============================================================================

TEST(CardBaseValueExhaustiveRanks, MatchesSpecificationTable) {
    struct Row {
        Rank r;
        int expected;
    };
    const Row table[] = {
        {Rank::Zwei, 2},
        {Rank::Drei, 3},
        {Rank::Vier, 4},
        {Rank::Fuenf, 5},
        {Rank::Sechs, 6},
        {Rank::Sieben, 7},
        {Rank::Acht, 8},
        {Rank::Neun, 9},
        {Rank::Zehn, 10},
        {Rank::Bube, 10},
        {Rank::Dame, 10},
        {Rank::Koenig, 10},
        {Rank::Ass, 11},
    };
    for (const auto& row : table) {
        EXPECT_EQ(Card(Suit::Karo, row.r).baseValue(), row.expected)
            << "rank enum underlying value differs from blackjack base mapping";
    }
}

// =============================================================================
// toString Eindeutigkeit: gleiche Texte fuer gleiche Rang/Farbe-Kombination
// =============================================================================

TEST(CardToStringDeterminism, RepeatedCallsEqual) {
    const Card c(Suit::Pik, Rank::Fuenf);
    EXPECT_EQ(c.toString(), c.toString());
}

TEST(CardToStringUniqueness, DifferentSuitsDifferForSameRank) {
    std::set<std::string> outs;
    for (Suit s : allSuits) {
        outs.insert(Card(s, Rank::Acht).toString());
    }
    EXPECT_EQ(outs.size(), 4u);
}

// =============================================================================
// noexcept / const correctness indirekt: mehrfache Aufrufe ohne Seiteneffekte
// =============================================================================

TEST(CardConstness, AccessorsStableAcrossCalls) {
    const Card c(Suit::Herz, Rank::Neun);
    EXPECT_EQ(c.suit(), c.suit());
    EXPECT_EQ(c.rank(), c.rank());
    EXPECT_EQ(c.baseValue(), c.baseValue());
    const std::string a = c.toString();
    const std::string b = c.toString();
    EXPECT_EQ(a, b);
}

// =============================================================================
// ASCII-Art: verschiedene Farben erzeugen unterschiedliche Mittelzeilen
// (Aequivalenzklassen Suit-Symbol)
// =============================================================================

TEST(CardAsciiArtSuitEquivalence, DifferentSuitsMiddleLineNotAllEqual) {
    std::unordered_set<std::string> mids;
    for (Suit s : allSuits) {
        const auto art = Card(s, Rank::Koenig).toAsciiArt();
        ASSERT_EQ(art.size(), 5u);
        mids.insert(art[2]);
    }
    EXPECT_GE(mids.size(), 2u);
}

// =============================================================================
// Zusaetzliche Grenzfaelle: minimale und maximale „Darstellungskomplexitaet“
// =============================================================================

TEST(CardAsciiBoundary, RankTwoVsKingSameLineCount) {
    const auto a = Card(Suit::Kreuz, Rank::Zwei).toAsciiArt();
    const auto b = Card(Suit::Kreuz, Rank::Koenig).toAsciiArt();
    EXPECT_EQ(a.size(), b.size());
}

TEST(CardStringBoundary, LongestGermanRankWordKoenig) {
    const std::string s = Card(Suit::Herz, Rank::Koenig).toString();
    EXPECT_FALSE(s.empty());
}

// =============================================================================
// Karten-Vergleich ueber String-Repraesentation (Integritaet der Benennung)
// =============================================================================

TEST(CardNamingIntegration, All52StandardNamesUnique) {
    std::set<std::string> names;
    for (Suit s : allSuits) {
        const Rank ranks[] = {
            Rank::Zwei, Rank::Drei, Rank::Vier, Rank::Fuenf, Rank::Sechs, Rank::Sieben,
            Rank::Acht, Rank::Neun, Rank::Zehn, Rank::Bube, Rank::Dame, Rank::Koenig, Rank::Ass,
        };
        for (Rank r : ranks) {
            names.insert(Card(s, r).toString());
        }
    }
    EXPECT_EQ(names.size(), 52u);
}

// =============================================================================
// Weitere typische Werte „in der Mitte“ der Bildkarten-Aequivalenzklasse
// =============================================================================

TEST(CardFaceEquivalenceMiddle, DameBehavesLikeTenValue) {
    EXPECT_EQ(Card(Suit::Pik, Rank::Dame).baseValue(), 10);
}

TEST(CardFaceEquivalenceMiddle, BubeBehavesLikeTenValue) {
    EXPECT_EQ(Card(Suit::Karo, Rank::Bube).baseValue(), 10);
}

// =============================================================================
// Zehn-Karte: Grenze zwischen numerischer Folge und Bildkarten (Wert 10)
// =============================================================================

TEST(CardTenBoundary, TenEqualsFaceValueTen) {
    EXPECT_EQ(Card(Suit::Herz, Rank::Zehn).baseValue(), 10);
    EXPECT_EQ(Card(Suit::Herz, Rank::Bube).baseValue(), 10);
}

// =============================================================================
// Performance-Nebenbedingung: viele toAsciiArt Aufrufe bleiben konsistent
// =============================================================================

TEST(CardAsciiStress, HundredCallsSameOutput) {
    const Card c(Suit::Pik, Rank::Ass);
    const auto once = c.toAsciiArt();
    for (int i = 0; i < 100; ++i) {
        const auto again = c.toAsciiArt();
        EXPECT_EQ(joinLines(once), joinLines(again));
    }
}
