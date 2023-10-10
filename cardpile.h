#ifndef CARDPILE_H
#define CARDPILE_H

#include "card.h"
#include "stack.h"

class CardPile: public Stack<Card> {
    private:
    int closed_pos; // the last position of closed cards

    public:
    CardPile(): CardPile(10) {}
    CardPile(int capacity): CardPile(capacity, -1) {}
    CardPile(int capacity, int closed_pos);
    
    int get_closed_pos() const;
    void set_closed_pos(int position);

    bool is_opened(int position);

    bool top_is_opened() const;

    void pop(bool silent = false);

    const Card& view(int position) const;

    bool operator==(const CardPile& item) const;
    bool operator!=(const CardPile& item) const;
};

#endif