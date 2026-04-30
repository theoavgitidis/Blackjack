#include "Deck.h"

#include <gtest/gtest.h>

#include <set>
#include <stdexcept>
#include <string>

TEST(DeckTest, ConstructorCreatesStandardFiftyTwoCardDeck) {
    Deck deck;

    EXPECT_EQ(deck.size(), 52U);
}

TEST(DeckTest, DrawRemovesExactlyOneCardUntilDeckIsEmpty) {
    Deck deck;

    const Card drawn = deck.draw();

    EXPECT_EQ(deck.size(), 51U);
    EXPECT_FALSE(drawn.toString().empty());
}

TEST(DeckTest, StandardDeckContainsEveryCardOnce) {
    Deck deck;
    std::set<std::string> cards;

    while (deck.size() > 0) {
        cards.insert(deck.draw().toString());
    }

    EXPECT_EQ(cards.size(), 52U);
    EXPECT_EQ(deck.size(), 0U);
}

TEST(DeckTest, DrawFromEmptyDeckThrows) {
    Deck deck;
    while (deck.size() > 0) {
        (void)deck.draw();
    }

    EXPECT_THROW((void)deck.draw(), std::runtime_error);
}

TEST(DeckTest, ShuffleKeepsCardCountAndUniqueCards) {
    Deck deck;

    deck.shuffle();

    std::set<std::string> cards;
    while (deck.size() > 0) {
        cards.insert(deck.draw().toString());
    }

    EXPECT_EQ(cards.size(), 52U);
}
