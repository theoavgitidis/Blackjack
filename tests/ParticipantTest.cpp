#include "Dealer.h"
#include "Player.h"

#include <gtest/gtest.h>

TEST(ParticipantTest, PlayerAndDealerConstructWithExpectedNames) {
    const Player player;
    const Dealer dealer;

    EXPECT_EQ(player.name(), "Spieler");
    EXPECT_EQ(dealer.name(), "Dealer");
}

TEST(ParticipantTest, HandAccessorsExposeMutableAndConstHands) {
    Player player;

    player.hand().addCard(Card(Card::Suit::Kreuz, Card::Rank::Ass));
    const Player& constPlayer = player;

    EXPECT_EQ(constPlayer.hand().cards().size(), 1U);
    EXPECT_EQ(player.score(), 11);
}

TEST(ParticipantTest, ClearHandRemovesCards) {
    Player player;
    player.hand().addCard(Card(Card::Suit::Kreuz, Card::Rank::Ass));

    player.clearHand();

    EXPECT_TRUE(player.hand().cards().empty());
}

TEST(ParticipantTest, TakeCardDrawsFromDeckIntoHand) {
    Player player;
    Deck deck;

    player.takeCard(deck);

    EXPECT_EQ(player.hand().cards().size(), 1U);
    EXPECT_EQ(deck.size(), 51U);
}

TEST(ParticipantTest, DelegatesScoreBustAndBlackjackToHand) {
    Player player;
    player.hand().addCard(Card(Card::Suit::Kreuz, Card::Rank::Ass));
    player.hand().addCard(Card(Card::Suit::Herz, Card::Rank::Koenig));

    EXPECT_EQ(player.score(), 21);
    EXPECT_FALSE(player.isBust());
    EXPECT_TRUE(player.hasBlackjack());
}

TEST(PlayerTest, StartsWithBankrollAndChecksAffordablePositiveBets) {
    const Player player;

    EXPECT_EQ(player.bankrollCents(), 50000);
    EXPECT_TRUE(player.canAfford(100));
    EXPECT_TRUE(player.canAfford(50000));
    EXPECT_FALSE(player.canAfford(0));
    EXPECT_FALSE(player.canAfford(50100));
}

TEST(PlayerTest, AdjustBankrollAppliesDeltasAndNeverGoesBelowZero) {
    Player player;

    player.adjustBankroll(2500);
    EXPECT_EQ(player.bankrollCents(), 52500);

    player.adjustBankroll(-999999);
    EXPECT_EQ(player.bankrollCents(), 0);
}

TEST(DealerTest, ShouldHitBelowSeventeenAndStandAtSeventeenOrMore) {
    Dealer dealer;
    dealer.hand().addCard(Card(Card::Suit::Kreuz, Card::Rank::Zehn));
    dealer.hand().addCard(Card(Card::Suit::Herz, Card::Rank::Sechs));
    EXPECT_TRUE(dealer.shouldHit());

    dealer.hand().addCard(Card(Card::Suit::Pik, Card::Rank::Ass));
    EXPECT_FALSE(dealer.shouldHit());
}
