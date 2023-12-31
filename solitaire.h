#ifndef SOLITAIRE_H
#define SOLITAIRE_H

#include <string>
#include <fstream>
#include <ostream>

#include "cardpile.h"

class Solitaire {
    private:
    static const int PLAYING_LINES = 7;
    static const int EARNING_SCORE = 5;
    static const char* SYMBOL_STR[];

    int solve_count = 0;

    std::ofstream output_file;

    CardPile playing_piles[PLAYING_LINES];
    CardPile waste_pile;
    CardPile stock;
    CardPile output_piles[4];

    struct {
        CardPile cards;
        int from;
        int to;
        enum {Stock = -1, Waste = -2, Output = -3};
    } last_moving;

    int score = -52;

    bool is_sussesive(const Card& top, const Card& bottom) const;
    
    void print_each(const Card& moved_card,
                    const std::string from_position, const std::string to_position,
                    const bool count_increment = true);

    bool equal_to_the_last_move(const CardPile& cards, int from, int to) const;
    bool equal_to_the_last_move(const Card& card, int from, int to) const;

    void update_last_moving(const CardPile& cards, int from, int to);
    void update_last_moving(const Card& card, int from, int to);

    public:
    Solitaire(): Solitaire::Solitaire(1) {}
    Solitaire(int seed);
    Solitaire(std::string path): Solitaire::Solitaire(1, path) {}
    Solitaire(int seed, std::string path);
    ~Solitaire();

    bool solve();

    void print_board();

};

#endif