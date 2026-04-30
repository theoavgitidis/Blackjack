#include "Hand.h"

#include <gtest/gtest.h>

TEST(HandTest, AddCardStoresCardsAndClearRemovesThem) {
    Hand hand;

    hand.addCard(Card(Card::Suit::Kreuz, Card::Rank::Fuenf));
    hand.addCard(Card(Card::Suit::Pik, Card::Rank::Sechs));

    ASSERT_EQ(hand.cards().size(), 2U);
    hand.clear();
    EXPECT_TRUE(hand.cards().empty());
}

TEST(HandTest, ScoreAddsNumberAndFaceCards) {
    Hand hand;
    hand.addCard(Card(Card::Suit::Kreuz, Card::Rank::Neun));
    hand.addCard(Card(Card::Suit::Herz, Card::Rank::Koenig));

    EXPECT_EQ(hand.score(), 19);
    EXPECT_FALSE(hand.isBust());
}

TEST(HandTest, ScoreCountsAcesAsOneWhenNeeded) {
    Hand hand;
    hand.addCard(Card(Card::Suit::Kreuz, Card::Rank::Ass));
    hand.addCard(Card(Card::Suit::Herz, Card::Rank::Ass));
    hand.addCard(Card(Card::Suit::Pik, Card::Rank::Neun));

    EXPECT_EQ(hand.score(), 21);
    EXPECT_FALSE(hand.isBust());
}

TEST(HandTest, IsBustOnlyWhenScoreExceedsTwentyOne) {
    Hand hand;
    hand.addCard(Card(Card::Suit::Kreuz, Card::Rank::Koenig));
    hand.addCard(Card(Card::Suit::Herz, Card::Rank::Dame));
    EXPECT_FALSE(hand.isBust());

    hand.addCard(Card(Card::Suit::Pik, Card::Rank::Zwei));
    EXPECT_TRUE(hand.isBust());
}

TEST(HandTest, BlackjackRequiresExactlyTwoCardsAndTwentyOnePoints) {
    Hand blackjack;
    blackjack.addCard(Card(Card::Suit::Kreuz, Card::Rank::Ass));
    blackjack.addCard(Card(Card::Suit::Herz, Card::Rank::Koenig));
    EXPECT_TRUE(blackjack.isBlackjack());

    blackjack.addCard(Card(Card::Suit::Pik, Card::Rank::Zwei));
    EXPECT_FALSE(blackjack.isBlackjack());
}

TEST(HandTest, ToStringCanHideOnlySecondCard) {
    Hand hand;
    hand.addCard(Card(Card::Suit::Kreuz, Card::Rank::Ass));
    hand.addCard(Card(Card::Suit::Herz, Card::Rank::Koenig));
    hand.addCard(Card(Card::Suit::Pik, Card::Rank::Zwei));

    EXPECT_EQ(hand.toString(false), "Ass Kreuz, Koenig Herz, 2 Pik");
    EXPECT_EQ(hand.toString(true), "Ass Kreuz, [verdeckt], 2 Pik");
}
