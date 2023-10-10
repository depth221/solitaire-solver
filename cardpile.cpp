#include "cardpile.h"

CardPile::CardPile(int capacity, int closed_pos): Stack<Card>::Stack(capacity) {
    this->closed_pos = closed_pos;
}

int CardPile::get_closed_pos() const {
    return closed_pos;
}

void CardPile::set_closed_pos(int position) {
    closed_pos = position;
}

bool CardPile::is_opened(int position) {
    return (closed_pos < position);
} 

bool CardPile::top_is_opened() const {
    return (closed_pos < top_ptr);
}

void CardPile::pop(bool silent) {
    Stack::pop();

    if (!silent && closed_pos > top_ptr - 1) {
        closed_pos = top_ptr - 1;
    }
}

const Card& CardPile::view(int position) const {
    if (position > top_ptr) std::range_error("position is over");
    return array[position];
}

bool CardPile::operator==(const CardPile& item) const {
    bool equal_cards = true;
    if (size() != item.size()) return false;

    for (int i = 0; i < size(); i++) {
        if (view(i) != item.view(i)) {
            equal_cards = false;
            break;
        }
    }

    return equal_cards;
}

bool CardPile::operator!=(const CardPile& item) const {
    return !operator==(item);
}