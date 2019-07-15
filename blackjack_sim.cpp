#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

using namespace std; 

std::vector<int> cardDeck;
std::vector<int> burnDeck;

std::vector<int> playerCards;
std::vector<int> playerSubDeck;
std::vector<int> dealerCards;

bool doublesAllowed;
bool doublesAfterAllowed;
bool lateSurrenderAllowed;

string betType;
string playStyle;

int bank;
int numDecks;
int numPlays;
int currentBet;
int percentOrFlat;

int totalWins;
int totalLoses;
int totalTies;


// Creates and shuffles the selected number of decks
void createDeck()
{
    cardDeck.clear();
    burnDeck.clear();

    int tempDecks = numDecks;
    // We will add as many decks as necessary to the main deck
    // To make it easy  0s will depict face card values and 1s will depect Aces
    while(tempDecks > 0)
    {
        // First we add 12 0's for each face card
        cardDeck.insert(cardDeck.end(), 12, 0);

        // Now we add 4 Aces
        cardDeck.insert(cardDeck.end(), 4, 1);

        // Finally we add 4 of each card between 2-10
        for(int i = 2; i <= 10; i++)
        {
            cardDeck.insert(cardDeck.end(), 4, i);
        }
        tempDecks--;
    }


    // Now that the cards are added we shuffle the deck
    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(cardDeck.begin(), cardDeck.end(), g);
}

// Returns true if a split is suggested false if normal play is required
bool splitOutlier(int dealerUp)
{
    int otherCard = playerCards.at(0);
    // All the basic strategies for splits
    if(otherCard == 0 || 8) return true;
    if(otherCard == 10) return false;
    if((otherCard == 9) && (dealerUp == 7 || 10 || 0)) return false;
    if((otherCard == 7) && (dealerUp > 7)) return false;
    if((otherCard == 6) && (dealerUp > 6)) return false;
    if(otherCard == 5) return false;
    if((otherCard == 4) && (dealerUp < 6 || dealerUp > 6)) return false;
    if((otherCard == 3) && (dealerUp > 7)) return false;
    if((otherCard == 2) && (dealerUp > 7)) return false;

    return true;
}

int countTotal(std::vector < int > currentHand) {
  int sum = 0;
  int aces = 0;
  for (int i = 0; i < currentHand.size(); i++) {
    if (currentHand[i] > 1) {
        // cout<< "\nAdd " << currentHand[i] << "";
      sum += currentHand[i];
    } else if (currentHand[i] == 0) {
        // cout<< "\nAdd 10";
      sum += 10;
    } else {
      aces++;
    }

  }
  while (aces > 0) {
    if (sum + 11 < 21) {
        // cout<< "\nAdd 11";
      sum += 11;
    } else {
        // cout<< "\nAdd 1";
      sum++;
    }
    aces--;
  }
  cout << "\n";
  return sum;
}

void subtractBet() {
  if (betType[0] == 'P') {
    if (bank * (percentOrFlat / 100) > 10) {
      currentBet = (percentOrFlat / 100) * bank;
    } else {
      cout << "Percent bet more than minimum, going all in\n\n";
      currentBet = bank;
    }
  } 
  else 
  {
    if (percentOrFlat < bank) {
      currentBet = percentOrFlat;
    } else {
      cout << "Flat bet more than minimum, going all in\n\n";
      currentBet = bank;
    }
  }
  bank -= currentBet;
}

void endTurn(std::vector < int >  currentHand)
{
    burnDeck.insert(burnDeck.end(), currentHand.begin(), currentHand.end());
    // cout << "Burn Deck Size: " << burnDeck.size() << "\n";
    // cout << "Card Deck Size: " << cardDeck.size() << "\n\n";
}

int hitDeck()
{
    int newCard = cardDeck.front();
    cardDeck.erase(cardDeck.begin());

    // cout << "Hit: " << newCard<< "\n   New size: " << cardDeck.size() << "\n\n";
    return newCard;
}

int doubleDown()
{
    subtractBet();
    // cout << "Deducted Bank: " << bank;
    return hitDeck();
}

// Returns the best play to make for current hand
// 
std::vector < int > makePerfectPlay(int dealerUp, std::vector < int > currentHand) {
  int firstCard = currentHand.at(0);
  int secondCard = currentHand.at(1);

  // To see if our turn is over
  // If we want to double down the turn is considered over
  bool endHand = false;

  // Once single ace play is over we go to normal game mode
  bool acePlay = false;

  if (firstCard == 1 && secondCard == 0) {
    cout << "BLACKJACK!\n\n";
    acePlay = true;
    endHand = true;
  }

  // Check if first card is an Ace
  while (firstCard == 1 && !acePlay) {
    // cout << "Making Ace Play\n\n";

    if (secondCard < 8) {
      switch (secondCard) {
      case 7:
        if (dealerUp > 2 && dealerUp < 7) {
          doubleDown();
          endHand = true;
          break;
        } else if (dealerUp > 8) {
          hitDeck();
        } else {
          // Literally do nothing, stand
        }
      case 6:
        if (dealerUp > 2 && dealerUp < 7) {
          doubleDown();
          endHand = true;
          break;
        } else {
          hitDeck();
        }
      case 5:
        if (dealerUp > 3 && dealerUp < 7) {
          doubleDown();
          endHand = true;
          break;
        } else {
          hitDeck();
        }
      case 4:
        if (dealerUp > 3 && dealerUp < 7) {
          doubleDown();
          endHand = true;
          break;
        } else {
          hitDeck();
        }
      case 3:
        if (dealerUp > 4 && dealerUp < 7) {
          doubleDown();
          endHand = true;
          break;
        } else {
          hitDeck();
        }
      case 2:
        if (dealerUp > 4 && dealerUp < 7) {
          doubleDown();
          endHand = true;
          break;
        } else {
          hitDeck();
        }
      }
    }
    acePlay = true;
  }

//   cout << "Making Normal Play\n\n";
  // We end if doubledDown == true
  // else we continue until we stand
  while (!endHand) {
    // cout << "Continuing Hand\n\n";
    // The deck is in play mode therefore we will play according to the card

    // Reminder: Ace(1), Face(0), Others(#)

    // First we check for any hits that can be made
    // Then we check for doubling down (if allowed)
    // Then we know that the rest are stands
    int cardTotal = countTotal(currentHand);
    if (cardTotal < 17) {
      int newCard;
      bool toHit = false;
      if ((dealerUp > 6 && cardTotal > 11) || ((dealerUp == 2 || 3) && cardTotal == 12)) {
        toHit = true;
      } else if (cardTotal > 4 && cardTotal < 9) {
        toHit = true;
      } else if (cardTotal == 9 && (dealerUp < 3 || dealerUp > 6)) {
        toHit = true;
      } else if (cardTotal == 10 && (dealerUp == 10 || dealerUp < 2)) {
        toHit = true;
      } else if (cardTotal == 11 && dealerUp == 1) {
        toHit = true;
      }

      if (toHit) {
        cout << "Making Hit\n\n";
        newCard = hitDeck();
        currentHand.push_back(newCard);
      }

      // We would place a test for doubles here but lets imagine they're allowed
      bool toDouble = false;
      if (!toHit) {
        if (cardTotal == 11 && (dealerUp != 1)) {
          toDouble = true;
        } else if (cardTotal == 10 && (dealerUp != 1 || 0 || 10)) {
          toDouble = true;
        } else if (cardTotal == 9 && (dealerUp < 6 && dealerUp > 2)) {
          toDouble = true;
        }
        if (toDouble) {
          cout << "Making Double\n\n";
          newCard = doubleDown();
          currentHand.push_back(newCard);
        }
      }

      // If they are equal then we must stand then
      if (toDouble == toHit) {
        cout << "Stand at current cards!\n";
        endHand = true;
      } else {
        cout << "Hand Size: " << currentHand.size() << "\n";
        cout << "Total of: " << cardTotal << "\n";
        cardTotal = countTotal(currentHand);

        cout << "Doubles: " << (toDouble == 1) << "\n";
        cout << "Hit: " << (toHit == 1) << "\n\n";

        // Check for bust
        if (cardTotal > 21) {
          cout << "\nSTAND (higher than 21): ";
          cout << countTotal(currentHand) << " (Bust)\n";
          endHand = true;
        }
      }

    } else {
      cout << "\nSTAND (higher than 17): ";
      cout << countTotal(currentHand) << "\n";
      endHand = true;
    }
  }
  return currentHand;
}

void playBlackjack() {
  // First we create and shuffle the deck
  createDeck();

  bool playParameter = true;

  // With a shuffled deck we can begin to play
  while (bank > 0 && playParameter) {
      if(burnDeck.size() > 2*cardDeck.size())
      {
          cout << "SHUFFLING DECK\n";
          createDeck();
      }
    // Check if ending parameter have been met
   if (numPlays == 0) {
       playParameter = false;
      }
      else
      {
          numPlays--;
      }

    if(playParameter)
    {
    cout << "\n\nPLAYING NEW ROUND\n\n";
    // Remove bet value from out bank
    subtractBet();

    // Deal cards to player and dealer
    int playerFirst = hitDeck();

    int dealerDown = hitDeck();

    int playerSecond = hitDeck();

    int dealerUp = hitDeck();

    playerCards.push_back(playerFirst);
    playerCards.push_back(playerSecond);
    dealerCards.push_back(dealerDown);
    dealerCards.push_back(dealerUp);

    cout << "Player: " << playerFirst << " & " << playerSecond << "\n";
    cout << "Dealer Up: " << dealerUp << "\n\n";

    // A turn is considered done if we choose to stand or surrender
    // Our makePerfectPlay function will tell us what to do with our cards
    if (playerFirst == playerSecond) {
      if (splitOutlier(dealerUp)) {
          cout << "Playing double Split\n\n";
        // Now we place the bet again and draw two more cards
        // Now index (0,2) & (1,3) are the pairs
        int primarySecond = cardDeck.front();
        cardDeck.erase(cardDeck.begin());

        int secondarySecond = cardDeck.front();
        cardDeck.erase(cardDeck.begin());

        playerSubDeck.clear();
        playerSubDeck.push_back(playerCards[1]);
        playerSubDeck.push_back(secondarySecond);

        playerCards.erase(playerCards.begin() + 1);
        playerCards.push_back(primarySecond);

        // two decks with two cards

        // If they equal each other a second time we will just hit once and stand (low prob.)

        playerCards = makePerfectPlay(dealerUp, playerCards);
        playerSubDeck = makePerfectPlay(dealerUp, playerSubDeck);
        cout << "Perfect Play Made3\n\n";
      } else {
        playerCards= makePerfectPlay(dealerUp, playerCards);
      }
    } else {
      playerCards = makePerfectPlay(dealerUp, playerCards);
    }

    // Now the dealer plays
    int dealerCount = countTotal(dealerCards);
    while(dealerCount < 16)
    {
        cout << "Dealer Hits\n";
        cout << "Dealer Cards: " << dealerCards.size() << "\n";
        cout << "Dealer Total: " << dealerCount << "\n\n";
        int dealerNewCard = hitDeck();
        dealerCards.push_back(dealerNewCard);
        dealerCount = countTotal(dealerCards);
    }
    // Now we calculate who wins

    int playerCount = countTotal(playerCards);

    bool win = (playerCount == 21) || (playerCount > dealerCount);
    bool tie = (playerCount == dealerCount);

    cout << "Player Count: " << playerCount << "\n";
    cout << "Dealer Count: " << dealerCount << "\n";


    if(tie)
    {
        totalTies++;
        cout << "You Tie\n\n";
        bank += currentBet;
    }
    else if (win)
    {
        totalWins++;
        cout << "You Win\n\n";
         bank += 2*currentBet;
    }
    else
    {
        totalLoses++;
        cout << "You Lose\n\n";
        // The bet has been lost and isn't returned
    }
    cout << "Bank Standing: " << bank << "\n";
    cout << "Rounds left: " << numPlays << "\n\n";
    endTurn(dealerCards);
    endTurn(playerCards);
    endTurn(playerSubDeck);
    playerCards.clear();
    dealerCards.clear();
    playerSubDeck.clear();

    }

  }
  cout << "\n\n\nGame is done!\n";
  cout << "Total Earnings: " << bank <<"\n";
  cout << "Total Plays Left : " << numPlays << "\n";
  cout << "(Wins/Loses/Ties)" << "(" << totalWins<<"/"<< totalLoses << "/"<<totalTies <<")\n";
}


string changeInput(string input)
{
    input = (input[0]>=97 && input[0]<=122)?input[0]-=32: input[0];
    // cout << "-" << input << "-\n";
    return input;
}


int main()
{
    cout << "  ____  _            _        _            _       _____ _                 _       _             \n";
    cout << " |  _ \\| |          | |      | |          | |     / ____(_)               | |     | |            \n";
    cout << " | |_) | | __ _  ___| | __   | | __ _  ___| | __ | (___  _ _ __ ___  _   _| | __ _| |_ ___  _ __ \n";
    cout << " |  _ <| |/ _` |/ __| |/ /   | |/ _` |/ __| |/ /  \\___ \\| | '_ ` _ \\| | | | |/ _` | __/ _ \\| '__|\n";
    cout << " | |_) | | (_| | (__|   < |__| | (_| | (__|   <   ____) | | | | | | | |_| | | (_| | || (_) | |   \n";
    cout << " |____/|_|\\__,_|\\___|_|\\_\\____/ \\__,_|\\___|_|\\_\\ |_____/|_|_| |_| |_|\\__,_|_|\\__,_|\\__\\___/|_|   \n";
    cout << "\n";

    // Greeting user main page
    cout << "Hello! Welcome to Blackjack Simulator\n";
    cout << "This simulator will help you see the benefits of \"Perfect Basic Strategy\"\n";
    cout << "\nWe're going to start you off with a 10,000 chip bank\n\n";

    bank = 10000;
    totalWins = 0;
    totalLoses = 0;
    totalTies = 0;

    // Get rule variables squared away
    string doubles = "!";
    string doublesAfter = "!";
    string lateSurrender = "!";
    
    cout << "Rules for the table\n\n";

    cout << "Doubles (Y / N): ";
    // cin >> doubles;
    doubles = 'Y';
    doubles = changeInput(doubles);

    while(!(doubles[0] != 'Y' || 'N'))
    {
        cout << "\nPlease use selection parameters!\n\n";
        cout << "Doubles (Y / N): ";
        cin >> doubles;
        doubles = changeInput(doubles);
    }

    if(doubles[0] == 'Y')
    {
        cout << "Doubles After Split (Y / N): ";
        // cin >> doublesAfter;
        doublesAfter = 'Y';
        doublesAfter = changeInput(doublesAfter);

        while(!(doubles[0] != 'Y' || 'N'))
        {
            cout << "\nPlease use selection parameters!\n\n";
            cout << "Doubles After Split (Y / N): ";
            cin >> doublesAfter;
            doublesAfter = changeInput(doublesAfter);
        }
    }
    

    cout << "Late Surrendering (Y / N): ";
    // cin >> lateSurrender;
    lateSurrender = 'Y';
    lateSurrender = changeInput(lateSurrender);

    while(!(lateSurrender[0] != 'Y' || 'N'))
    {
        cout << "Please use selection parameters!\n\n";
        cout << "Late Surrendering (Y / N): ";
        cin >> lateSurrender;
        lateSurrender = changeInput(lateSurrender);
    }

    doublesAllowed = (doubles[0] == 'Y');
    doublesAfterAllowed = (doublesAfter[0] == 'Y');
    lateSurrenderAllowed = (lateSurrender[0] == 'Y');

    // Now find what play style the user would like
    betType = '!';

    cout << "\nWhat style of betting would you like to play?\n\n";
    cout << "Flat number bet (F) or Percent bank bet (P): ";
    // cin >> betType;
    betType = 'F';
    betType = changeInput(betType);

    while(!(betType[0] != 'F' || 'P'))
    {
        cout << "\nPlease use selection parameters!\n";
        cout << "Flat number bet (F) or Percent bank bet (P): ";
        cin >> betType;
        betType = changeInput(betType);
    }

    percentOrFlat = 0;

    if(betType[0] == 'P')
    {
        while(percentOrFlat == 0)
        {
            cout << "\nPlease enter percent continous bank wager: ";
            // cin >> percentOrFlat;
            percentOrFlat = 10;
        }
    }
    else
    {
        while(percentOrFlat == 0)
        {
            cout << "\nPlease enter flat continous bank wager: ";
            // cin >> percentOrFlat;
            percentOrFlat = 100;
        }
    }

    // How many decks does the player want the dealer to use (1-6)
    cout << "\nHow many decks would you like to play with (1-6): ";
    // cin  >> numDecks;
    numDecks = 1;
    
    // Change for invalid numbers
    numDecks = (numDecks < 0)? 1:numDecks%7;

    cout << "Using " << numDecks << " Decks...\n";
    
    
    // Check if the player wants to play to an amount or a number of times
    // This will of course cancel out regardless if they go broke
    numPlays = 0;
    cout << "\nPlay to a limit of moves: ";
        while(numPlays == 0)
        {
            cout << "\nPlease enter moves limit: ";
            // cin >> v;
            numPlays = 100;
        }

    // Great now the game can start
    playBlackjack();
}