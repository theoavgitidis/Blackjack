#include "ConsoleUI.h"
#include "TestIO.h"

#include <gtest/gtest.h>

TEST(ConsoleUITest, ReadLineTrimmedRemovesSurroundingWhitespace) {
    ScopedInput input("  ja  \n");

    EXPECT_EQ(ConsoleUI::readLineTrimmed(), "ja");
}

TEST(ConsoleUITest, ToLowerAsciiLowercasesInput) {
    EXPECT_EQ(ConsoleUI::toLowerAscii("HiT JA 123"), "hit ja 123");
}

TEST(ConsoleUITest, FormatMoneyRendersCentsAndNegativeValues) {
    EXPECT_EQ(ConsoleUI::formatMoney(12345), "123.45 EUR");
    EXPECT_EQ(ConsoleUI::formatMoney(-105), "-1.05 EUR");
}

TEST(ConsoleUITest, PromptStartGameAcceptsYesAndRetriesInvalidInput) {
    const ConsoleUI ui;
    ScopedInput input("maybe\nj\n");
    ScopedOutput output;

    EXPECT_TRUE(ui.promptStartGame());
    EXPECT_NE(output.str().find("Ungueltige Eingabe"), std::string::npos);
}

TEST(ConsoleUITest, PromptStartGameAcceptsNo) {
    const ConsoleUI ui;
    ScopedInput input("nein\n");
    ScopedOutput output;

    EXPECT_FALSE(ui.promptStartGame());
}

TEST(ConsoleUITest, PromptBetCentsAcceptsOnlyAffordableWholeEuroAmounts) {
    const ConsoleUI ui;
    ScopedInput input("\nabc\n0\n999\n25\n");
    ScopedOutput output;

    EXPECT_EQ(ui.promptBetCents(5000), 2500);
    EXPECT_NE(output.str().find("Ungueltige Eingabe"), std::string::npos);
    EXPECT_NE(output.str().find("nicht genug Guthaben"), std::string::npos);
}

TEST(ConsoleUITest, PromptPlayerActionAcceptsNumbersAndText) {
    const ConsoleUI ui;
    {
        ScopedInput input("1\n");
        ScopedOutput output;
        EXPECT_EQ(ui.promptPlayerAction(), ConsoleUI::PlayerAction::Hit);
    }
    {
        ScopedInput input("passen\n");
        ScopedOutput output;
        EXPECT_EQ(ui.promptPlayerAction(), ConsoleUI::PlayerAction::Stand);
    }
    {
        ScopedInput input("double down\n");
        ScopedOutput output;
        EXPECT_EQ(ui.promptPlayerAction(), ConsoleUI::PlayerAction::DoubleDown);
    }
}

TEST(ConsoleUITest, PromptPlayerActionRetriesInvalidInput) {
    const ConsoleUI ui;
    ScopedInput input("x\n2\n");
    ScopedOutput output;

    EXPECT_EQ(ui.promptPlayerAction(), ConsoleUI::PlayerAction::Stand);
    EXPECT_NE(output.str().find("Ungueltige Eingabe"), std::string::npos);
}

TEST(ConsoleUITest, PromptPlayAgainAcceptsYesNoAndRetriesInvalidInput) {
    const ConsoleUI ui;
    {
        ScopedInput input("?\nja\n");
        ScopedOutput output;
        EXPECT_TRUE(ui.promptPlayAgain());
        EXPECT_NE(output.str().find("Ungueltige Eingabe"), std::string::npos);
    }
    {
        ScopedInput input("n\n");
        ScopedOutput output;
        EXPECT_FALSE(ui.promptPlayAgain());
    }
}

TEST(ConsoleUITest, ShowFunctionsWriteExpectedStatusText) {
    const ConsoleUI ui;
    Player player;
    Dealer dealer;
    player.hand().addCard(Card(Card::Suit::Kreuz, Card::Rank::Ass));
    player.hand().addCard(Card(Card::Suit::Herz, Card::Rank::Koenig));
    dealer.hand().addCard(Card(Card::Suit::Pik, Card::Rank::Zehn));
    dealer.hand().addCard(Card(Card::Suit::Karo, Card::Rank::Acht));
    ScopedOutput output;

    ui.showWelcome();
    ui.showHands(player, dealer, true);
    ui.showPlayerStatus(player);
    ui.showDealerStatus(dealer);
    ui.showBankroll(12345);
    ui.showRoundResult("Test Ergebnis");
    ui.showInfo("Infozeile");

    const std::string text = output.str();
    EXPECT_NE(text.find("BLACKJACK"), std::string::npos);
    EXPECT_NE(text.find("Dealer"), std::string::npos);
    EXPECT_NE(text.find("Spieler"), std::string::npos);
    EXPECT_NE(text.find("Punkte: 21"), std::string::npos);
    EXPECT_NE(text.find("Kontostand: 123.45 EUR"), std::string::npos);
    EXPECT_NE(text.find("Test Ergebnis"), std::string::npos);
    EXPECT_NE(text.find("Infozeile"), std::string::npos);
}
