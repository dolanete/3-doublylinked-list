#include <cmath>
#include <fstream>
#include <sstream>
#include <istream>
#include "Card.h"
#include <iostream>
#include <stdexcept>

#include <string>
using namespace std;

int rand221(void) {
    return rand();
}

// You may want to write the following three functions, but they are not required

// std::istream& operator>>(std::istream& in, Suit& suit) {
//    
// }

Rank to_rank(const std::string& string) {
   if (string == "ace") {
    return 1;
   }
   if (string == "jack") {
    return 11;
   }
   if (string == "queen") {
    return 12;
   }
   if (string == "king") {
    return 13;
   }
   return stoi(string);
}

std::istream& operator>>(std::istream& in, Card& card) {
    string temp;
    getline(in, temp, ' ');
    //cout << temp << endl;
    if (temp == "spades") {
        card.suit = Suit::SPADES;
    }
    if (temp == "diamonds") {
        card.suit = Suit::DIAMONDS;
    }
    if (temp == "clubs") {
        card.suit = Suit::CLUBS;
    }
    if (temp == "hearts") {
        card.suit = Suit::HEARTS;
    }
    getline(in, temp);
    //cout << temp << endl;
    card.rank = to_rank(temp);
    
    //cout << card.rank << endl;
    return in;
}

List<Card> buildDeck(std::istream& file) {
    istringstream ss;
    List<Card> deck;
    string str;
    while(getline(file, str)) {
        istringstream ss(str);
        Card li;
        ss >> li;
        deck.push_back(li);
    }

    return deck;
}

List<Card> shuffle(const List<Card>& deck) {
    List<Card> shuffledDeck;
    List<Card> originalDeck = deck;
    int ht;
    while(originalDeck.cbegin() != originalDeck.cend()) {
        
        ht = rand221();
        if (ht % 2 == 0) {
            shuffledDeck.push_front(originalDeck.front());
        } else {
            shuffledDeck.push_back(originalDeck.front());
        }
        cout << "size of new: " << shuffledDeck.size() << endl;
        originalDeck.erase(originalDeck.cbegin());
        cout << "size of original: " << originalDeck.size() << endl;
    }
}