#ifndef CARD_H
#define CARD_H

static constexpr const char* NUMBER[] = {"0!", "A ", "2 ", "3 ", "4 ", "5 ", "6 ", "7 ", "8 ", "9 ", "10", "J ", "Q ", "K "};

class Card {
    public:
    enum Symbols {spade, heart, clover, diamond} symbol;
    int number;
    enum {Jack = 11, Queen = 12, King = 13};

    bool operator==(const Card& item) const {
        return (symbol == item.symbol && number == item.number);
    }

    bool operator!=(const Card& item) const {
        return !operator==(item);
    }
};

#endif