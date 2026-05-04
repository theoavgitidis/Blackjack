#include "ConsoleUI.h"

#include "Dealer.h"
#include "Player.h"

#include "test_stream_redirect.h"

#include <gtest/gtest.h>

#include <sstream>
#include <string>

// =============================================================================
// ConsoleUI — statische Hilfsfunktionen unter BLACKJACK_UNIT_TEST oeffentlich
// Aequivalenzklassen toLowerAscii: {leer}, {nur Kleinbuchstaben}, {Gemischt},
// {Nicht-ASCII bleibt unveraendert falls vorhanden}
// =============================================================================

TEST(ConsoleUIToLowerAscii, EmptyStringBoundary) {
    EXPECT_EQ(ConsoleUI::toLowerAscii(""), "");
}

TEST(ConsoleUIToLowerAscii, AlreadyLowerTypical) {
    EXPECT_EQ(ConsoleUI::toLowerAscii("hello"), "hello");
}

TEST(ConsoleUIToLowerAscii, AllUpperBoundary) {
    EXPECT_EQ(ConsoleUI::toLowerAscii("ABCDEFG"), "abcdefg");
}

TEST(ConsoleUIToLowerAscii, MixedTypical) {
    EXPECT_EQ(ConsoleUI::toLowerAscii("JaNeIn"), "janein");
}

TEST(ConsoleUIToLowerAscii, DigitsUnchangedEquivalenceClass) {
    EXPECT_EQ(ConsoleUI::toLowerAscii("12345"), "12345");
}

TEST(ConsoleUIToLowerAscii, SpacesPreserved) {
    EXPECT_EQ(ConsoleUI::toLowerAscii(" A B "), " a b ");
}

// =============================================================================
// formatMoney — Cent -> EUR Darstellung; Grenzwerte 0, negativ, gross
// =============================================================================

TEST(ConsoleUIFormatMoney, ZeroCentsBoundary) {
    EXPECT_EQ(ConsoleUI::formatMoney(0), "0.00 EUR");
}

TEST(ConsoleUIFormatMoney, OneCentTypicalLow) {
    EXPECT_EQ(ConsoleUI::formatMoney(1), "0.01 EUR");
}

TEST(ConsoleUIFormatMoney, NinetyNineCentsBoundaryJustBelowEuro) {
    EXPECT_EQ(ConsoleUI::formatMoney(99), "0.99 EUR");
}

TEST(ConsoleUIFormatMoney, OneEuroExactly) {
    EXPECT_EQ(ConsoleUI::formatMoney(100), "1.00 EUR");
}

TEST(ConsoleUIFormatMoney, TypicalBankrollFiveHundredEuro) {
    EXPECT_EQ(ConsoleUI::formatMoney(50000), "500.00 EUR");
}

TEST(ConsoleUIFormatMoney, NegativeOneEuroBoundary) {
    EXPECT_EQ(ConsoleUI::formatMoney(-100), "-1.00 EUR");
}

TEST(ConsoleUIFormatMoney, NegativeNinetyNineCents) {
    EXPECT_EQ(ConsoleUI::formatMoney(-99), "-0.99 EUR");
}

TEST(ConsoleUIFormatMoney, LargePositiveTypical) {
    EXPECT_EQ(ConsoleUI::formatMoney(123456789), "1234567.89 EUR");
}

// =============================================================================
// readLineTrimmed — Rand: leere Zeile, nur Leerzeichen, trim links/rechts
// =============================================================================

TEST(ConsoleUIReadLineTrimmed, EmptyLineReturnsEmpty) {
    std::istringstream iss("\n");
    blackjack_test::CinStringbufGuard guard(iss);
    EXPECT_EQ(ConsoleUI::readLineTrimmed(), "");
}

TEST(ConsoleUIReadLineTrimmed, SpacesOnlyReturnsEmptyBoundary) {
    std::istringstream iss("   \t  \n");
    blackjack_test::CinStringbufGuard guard(iss);
    EXPECT_EQ(ConsoleUI::readLineTrimmed(), "");
}

TEST(ConsoleUIReadLineTrimmed, TrimsLeadingSpacesTypical) {
    std::istringstream iss("   hello\n");
    blackjack_test::CinStringbufGuard guard(iss);
    EXPECT_EQ(ConsoleUI::readLineTrimmed(), "hello");
}

TEST(ConsoleUIReadLineTrimmed, TrimsTrailingSpacesBoundary) {
    std::istringstream iss("hello   \n");
    blackjack_test::CinStringbufGuard guard(iss);
    EXPECT_EQ(ConsoleUI::readLineTrimmed(), "hello");
}

TEST(ConsoleUIReadLineTrimmed, InnerSpacesPreservedEquivalenceClass) {
    std::istringstream iss("  a b c  \n");
    blackjack_test::CinStringbufGuard guard(iss);
    EXPECT_EQ(ConsoleUI::readLineTrimmed(), "a b c");
}

// =============================================================================
// promptStartGame — Aequivalenzklassen {j,ja}, {n,nein}, {ungueltig loop}
// =============================================================================

TEST(ConsoleUIPromptStartGame, AcceptsLowerJBoundary) {
    std::istringstream iss("j\n");
    blackjack_test::CinStringbufGuard inGuard(iss);
    std::ostringstream oss;
    blackjack_test::CoutStringbufGuard outGuard(oss);
    ConsoleUI ui;
    EXPECT_TRUE(ui.promptStartGame());
}

TEST(ConsoleUIPromptStartGame, AcceptsJaTypical) {
    std::istringstream iss("Ja\n");
    blackjack_test::CinStringbufGuard inGuard(iss);
    std::ostringstream oss;
    blackjack_test::CoutStringbufGuard outGuard(oss);
    ConsoleUI ui;
    EXPECT_TRUE(ui.promptStartGame());
}

TEST(ConsoleUIPromptStartGame, AcceptsNein) {
    std::istringstream iss("nein\n");
    blackjack_test::CinStringbufGuard inGuard(iss);
    std::ostringstream oss;
    blackjack_test::CoutStringbufGuard outGuard(oss);
    ConsoleUI ui;
    EXPECT_FALSE(ui.promptStartGame());
}

TEST(ConsoleUIPromptStartGame, RetriesOnInvalidThenYes) {
    std::istringstream iss("maybe\nj\n");
    blackjack_test::CinStringbufGuard inGuard(iss);
    std::ostringstream oss;
    blackjack_test::CoutStringbufGuard outGuard(oss);
    ConsoleUI ui;
    EXPECT_TRUE(ui.promptStartGame());
}

// =============================================================================
// promptBetCents — gueltige EUR-Ganzzahl, Grenzen 1 und bankroll/100
// =============================================================================

TEST(ConsoleUIPromptBetCents, TypicalFiveEuroFromFiveHundredBankroll) {
    std::istringstream iss("5\n");
    blackjack_test::CinStringbufGuard inGuard(iss);
    std::ostringstream oss;
    blackjack_test::CoutStringbufGuard outGuard(oss);
    ConsoleUI ui;
    EXPECT_EQ(ui.promptBetCents(50000), 500);
}

TEST(ConsoleUIPromptBetCents, MinimumOneEuroBoundary) {
    std::istringstream iss("1\n");
    blackjack_test::CinStringbufGuard inGuard(iss);
    std::ostringstream oss;
    blackjack_test::CoutStringbufGuard outGuard(oss);
    ConsoleUI ui;
    EXPECT_EQ(ui.promptBetCents(10000), 100);
}

TEST(ConsoleUIPromptBetCents, MaximumWholeEurosWhenBankrollTenEuro) {
    std::istringstream iss("10\n");
    blackjack_test::CinStringbufGuard inGuard(iss);
    std::ostringstream oss;
    blackjack_test::CoutStringbufGuard outGuard(oss);
    ConsoleUI ui;
    EXPECT_EQ(ui.promptBetCents(1000), 1000);
}

TEST(ConsoleUIPromptBetCents, RejectsZeroThenAcceptsValid) {
    std::istringstream iss("0\n2\n");
    blackjack_test::CinStringbufGuard inGuard(iss);
    std::ostringstream oss;
    blackjack_test::CoutStringbufGuard outGuard(oss);
    ConsoleUI ui;
    EXPECT_EQ(ui.promptBetCents(50000), 200);
}

TEST(ConsoleUIPromptBetCents, RejectsOverBankrollThenValid) {
    std::istringstream iss("999\n3\n");
    blackjack_test::CinStringbufGuard inGuard(iss);
    std::ostringstream oss;
    blackjack_test::CoutStringbufGuard outGuard(oss);
    ConsoleUI ui;
    EXPECT_EQ(ui.promptBetCents(500), 300);
}

// =============================================================================
// promptPlayerAction — numerische und textuelle Aequivalenzklassen
// =============================================================================

TEST(ConsoleUIPromptPlayerAction, DigitOneHit) {
    std::istringstream iss("1\n");
    blackjack_test::CinStringbufGuard inGuard(iss);
    std::ostringstream oss;
    blackjack_test::CoutStringbufGuard outGuard(oss);
    ConsoleUI ui;
    EXPECT_EQ(ui.promptPlayerAction(), ConsoleUI::PlayerAction::Hit);
}

TEST(ConsoleUIPromptPlayerAction, DigitTwoStand) {
    std::istringstream iss("2\n");
    blackjack_test::CinStringbufGuard inGuard(iss);
    std::ostringstream oss;
    blackjack_test::CoutStringbufGuard outGuard(oss);
    ConsoleUI ui;
    EXPECT_EQ(ui.promptPlayerAction(), ConsoleUI::PlayerAction::Stand);
}

TEST(ConsoleUIPromptPlayerAction, DigitThreeDoubleDown) {
    std::istringstream iss("3\n");
    blackjack_test::CinStringbufGuard inGuard(iss);
    std::ostringstream oss;
    blackjack_test::CoutStringbufGuard outGuard(oss);
    ConsoleUI ui;
    EXPECT_EQ(ui.promptPlayerAction(), ConsoleUI::PlayerAction::DoubleDown);
}

TEST(ConsoleUIPromptPlayerAction, TextHitSynonymZiehen) {
    std::istringstream iss("ziehen\n");
    blackjack_test::CinStringbufGuard inGuard(iss);
    std::ostringstream oss;
    blackjack_test::CoutStringbufGuard outGuard(oss);
    ConsoleUI ui;
    EXPECT_EQ(ui.promptPlayerAction(), ConsoleUI::PlayerAction::Hit);
}

TEST(ConsoleUIPromptPlayerAction, TextStandPassen) {
    std::istringstream iss("passen\n");
    blackjack_test::CinStringbufGuard inGuard(iss);
    std::ostringstream oss;
    blackjack_test::CoutStringbufGuard outGuard(oss);
    ConsoleUI ui;
    EXPECT_EQ(ui.promptPlayerAction(), ConsoleUI::PlayerAction::Stand);
}

TEST(ConsoleUIPromptPlayerAction, InvalidThenStand) {
    std::istringstream iss("x\n2\n");
    blackjack_test::CinStringbufGuard inGuard(iss);
    std::ostringstream oss;
    blackjack_test::CoutStringbufGuard outGuard(oss);
    ConsoleUI ui;
    EXPECT_EQ(ui.promptPlayerAction(), ConsoleUI::PlayerAction::Stand);
}

// =============================================================================
// promptPlayAgain — gleiche j/n Klassen wie promptStartGame
// =============================================================================

TEST(ConsoleUIPromptPlayAgain, NeinFalse) {
    std::istringstream iss("n\n");
    blackjack_test::CinStringbufGuard inGuard(iss);
    std::ostringstream oss;
    blackjack_test::CoutStringbufGuard outGuard(oss);
    ConsoleUI ui;
    EXPECT_FALSE(ui.promptPlayAgain());
}

TEST(ConsoleUIPromptPlayAgain, JaTrue) {
    std::istringstream iss("ja\n");
    blackjack_test::CinStringbufGuard inGuard(iss);
    std::ostringstream oss;
    blackjack_test::CoutStringbufGuard outGuard(oss);
    ConsoleUI ui;
    EXPECT_TRUE(ui.promptPlayAgain());
}

// =============================================================================
// showRoundResult / showInfo — Rauchtest mit Cout-Umleitung (kein Crash)
// =============================================================================

TEST(ConsoleUIShowMessages, ShowRoundResultWritesText) {
    std::ostringstream oss;
    blackjack_test::CoutStringbufGuard outGuard(oss);
    ConsoleUI ui;
    ui.showRoundResult("Testergebnis");
    EXPECT_NE(oss.str().find("Testergebnis"), std::string::npos);
}

TEST(ConsoleUIShowMessages, ShowInfoWritesLine) {
    std::ostringstream oss;
    blackjack_test::CoutStringbufGuard outGuard(oss);
    ConsoleUI ui;
    ui.showInfo("Infozeile");
    EXPECT_NE(oss.str().find("Infozeile"), std::string::npos);
}

TEST(ConsoleUIShowMessages, ShowWelcomeContainsBlackjack) {
    std::ostringstream oss;
    blackjack_test::CoutStringbufGuard outGuard(oss);
    ConsoleUI ui;
    ui.showWelcome();
    EXPECT_NE(oss.str().find("BLACKJACK"), std::string::npos);
}

// =============================================================================
// showHands / showPlayerStatus / showDealerStatus — ASCII-Render Smoke
// =============================================================================

TEST(ConsoleUIShowHands, RendersPlayerAndDealerWithoutThrow) {
    std::ostringstream oss;
    blackjack_test::CoutStringbufGuard outGuard(oss);
    Player pl;
    Dealer dl;
    pl.hand().addCard(Card(Card::Suit::Herz, Card::Rank::Drei));
    dl.hand().addCard(Card(Card::Suit::Pik, Card::Rank::Vier));
    ConsoleUI ui;
    EXPECT_NO_THROW(ui.showHands(pl, dl, false));
    EXPECT_FALSE(oss.str().empty());
}

TEST(ConsoleUIShowStatus, ShowPlayerStatusSmoke) {
    std::ostringstream oss;
    blackjack_test::CoutStringbufGuard outGuard(oss);
    Player pl;
    pl.hand().addCard(Card(Card::Suit::Herz, Card::Rank::Ass));
    ConsoleUI ui;
    EXPECT_NO_THROW(ui.showPlayerStatus(pl));
}

TEST(ConsoleUIShowStatus, ShowDealerStatusSmoke) {
    std::ostringstream oss;
    blackjack_test::CoutStringbufGuard outGuard(oss);
    Dealer dl;
    dl.hand().addCard(Card(Card::Suit::Karo, Card::Rank::Zehn));
    ConsoleUI ui;
    EXPECT_NO_THROW(ui.showDealerStatus(dl));
}

TEST(ConsoleUIShowBankroll, UsesFormatMoneyTypical) {
    std::ostringstream oss;
    blackjack_test::CoutStringbufGuard outGuard(oss);
    ConsoleUI ui;
    ui.showBankroll(12345);
    EXPECT_NE(oss.str().find("123.45"), std::string::npos);
}
