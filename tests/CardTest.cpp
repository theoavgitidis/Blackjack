#include "Card.h"

#include <gtest/gtest.h>

TEST(CardTest, ConstructorAndAccessorsPreserveSuitAndRank) {
    const Card card(Card::Suit::Herz, Card::Rank::Dame);

    EXPECT_EQ(card.suit(), Card::Suit::Herz);
    EXPECT_EQ(card.rank(), Card::Rank::Dame);
}

TEST(CardTest, BaseValueMatchesBlackjackRules) {
    EXPECT_EQ(Card(Card::Suit::Kreuz, Card::Rank::Zwei).baseValue(), 2);
    EXPECT_EQ(Card(Card::Suit::Kreuz, Card::Rank::Zehn).baseValue(), 10);
    EXPECT_EQ(Card(Card::Suit::Kreuz, Card::Rank::Bube).baseValue(), 10);
    EXPECT_EQ(Card(Card::Suit::Kreuz, Card::Rank::Dame).baseValue(), 10);
    EXPECT_EQ(Card(Card::Suit::Kreuz, Card::Rank::Koenig).baseValue(), 10);
    EXPECT_EQ(Card(Card::Suit::Kreuz, Card::Rank::Ass).baseValue(), 11);
}

TEST(CardTest, ToStringUsesGermanRankAndSuitNames) {
    EXPECT_EQ(Card(Card::Suit::Karo, Card::Rank::Ass).toString(), "Ass Karo");
    EXPECT_EQ(Card(Card::Suit::Pik, Card::Rank::Koenig).toString(), "Koenig Pik");
}

TEST(CardTest, ToAsciiArtRendersFiveLinesWithRankAndSuit) {
    const auto lines = Card(Card::Suit::Pik, Card::Rank::Ass).toAsciiArt();

    ASSERT_EQ(lines.size(), 5U);
    EXPECT_NE(lines[1].find("A"), std::string::npos);
    EXPECT_NE(lines[2].find("\u2660"), std::string::npos);
    EXPECT_NE(lines[3].find("A"), std::string::npos);
}

TEST(CardTest, BackAsciiArtRendersHiddenCard) {
    const auto lines = Card::backAsciiArt();

    ASSERT_EQ(lines.size(), 5U);
    EXPECT_NE(lines[1].find("#######"), std::string::npos);
    EXPECT_NE(lines[2].find("#######"), std::string::npos);
    EXPECT_NE(lines[3].find("#######"), std::string::npos);
}
