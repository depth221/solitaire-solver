#include <iostream>
#include <random>
#include <string>

#include "solitaire.h"

const char* Solitaire::SYMBOL_STR[] = {"♠ ", "♥ ", "♣ ", "◆ "};

bool Solitaire::is_sussesive(const Card& top, const Card& bottom) const {
    return (top.number == bottom.number - 1) &&
            ((static_cast<int>(top.symbol) + static_cast<int>(bottom.symbol)) % 2 == 1);
}

void Solitaire::print_each(const int count,
                const Card& moved_card,
                const std::string from_position,
                const std::string to_position) const {
    std::cout << "-------------------------------------------------\n"
                << count
                << ": ";
    if (moved_card.symbol % 2 == 1) {
        std::cout << "\033[1;31m";
    }
    std::cout << SYMBOL_STR[moved_card.symbol] <<NUMBER[moved_card.number]  << "\033[0m"
                << " ["
                << from_position
                << " →  "
                << to_position
                << "] ";
    if (to_position == "output") {
        std::cout << "(score: " << score << " →  " << score + EARNING_SCORE << ")";
    }
    std::cout << "\n\n";
}

bool Solitaire::equal_to_the_last_move(const CardPile& cards, int from, int to) const {
    return last_moving.cards == cards && last_moving.from == to && last_moving.to == from;
}

bool Solitaire::equal_to_the_last_move(const Card& card, int from, int to) const {
    CardPile tmp_cardpile = CardPile();
    tmp_cardpile.push(card);
    return equal_to_the_last_move(tmp_cardpile, from, to);
}

void Solitaire::update_last_moving(const CardPile& cards, int from, int to) {
    last_moving.cards = cards;
    last_moving.from = from;
    last_moving.to = to;
}

void Solitaire::update_last_moving(const Card& card, int from, int to) {
    CardPile tmp_cardpile = CardPile();
    tmp_cardpile.push(card);
    update_last_moving(tmp_cardpile, from, to);
}

Solitaire::Solitaire(int seed) {
    std::mt19937_64 gen(seed);

    // shuffle cards
    int tmp_card_sequence[52];
    for (int i = 0; i < 52; i++) {
        tmp_card_sequence[i] = i;
    }

    std::uniform_int_distribution<int> dis(0, 51);
    for (int i = 0; i < 100; i++) {
        int first_pos = dis(gen);
        int second_pos = dis(gen);
        int tmp_num = tmp_card_sequence[first_pos];
        tmp_card_sequence[first_pos] = tmp_card_sequence[second_pos];
        tmp_card_sequence[second_pos] = tmp_num;
    }

    int count = 0;

    // insert to playing piles
    for (int i = 0; i < PLAYING_LINES; i++) {
        playing_piles[i] = CardPile(i + 1);
        
        for (int j = 0; j < i + 1; j++) {
            Card tmp_card = Card();
            tmp_card.symbol = static_cast<Card::Symbols>(tmp_card_sequence[count] / 13);
            tmp_card.number = tmp_card_sequence[count] % 13 + 1;
            playing_piles[i].push(tmp_card);
            count++;
        }

        playing_piles[i].set_closed_pos(playing_piles[i].size() - 2);
    }

    // insert to the stock
    for (int i = 0; i < 24; i++) {
        Card tmp_card = Card();
        tmp_card.symbol = static_cast<Card::Symbols>(tmp_card_sequence[count] / 13);
        tmp_card.number = tmp_card_sequence[count] % 13 + 1;
        stock.push(tmp_card);
        count++;
    }

    stock.set_closed_pos(24 - 1);
}

bool Solitaire::solve() {
    bool is_moved = false;
    int solve_count = 1;
    bool game_over = false;

    while (true) {
        print_board();

        // game clear
        if (score == 208) {
            break;
        }

        // (a)
        // at playing piles
        for (int i = 0; i < PLAYING_LINES; i++) {
            if (!playing_piles[i].is_empty() && playing_piles[i].top_is_opened()) {
                const Card& top_card = playing_piles[i].top();
                int top_card_number = top_card.number;
                int top_card_symbol = static_cast<int>(top_card.symbol);

                // can move
                if (top_card_number == output_piles[top_card_symbol].size() + 1 &&
                    !equal_to_the_last_move(top_card, i + 1, last_moving.Output)) {
                    output_piles[top_card_symbol].push(top_card);
                    playing_piles[i].pop();

                    // print the description for the moving
                    update_last_moving(top_card, i + 1, last_moving.Output);
                    print_each(solve_count, top_card, "playing #" + std::to_string(i + 1), "output");
                    is_moved = true;
                    solve_count++;
                    score += EARNING_SCORE;
                    break;
                }
            }
        }
        if (is_moved) {
            is_moved = false;
            continue;
        }

        // at waste pile
        if (!is_moved && !waste_pile.is_empty()) {
            const Card& top_card = waste_pile.top();
            int top_card_number = top_card.number;
            int top_card_symbol = static_cast<int>(top_card.symbol);

            // can move
            if (top_card_number == output_piles[top_card_symbol].size() + 1) {
                    
                    
                output_piles[top_card_symbol].push(top_card);
                waste_pile.pop();

                // print the description for the moving
                update_last_moving(top_card, last_moving.Waste, last_moving.Output);
                print_each(solve_count, top_card, "waste", "output");
                is_moved = true;
                solve_count++;
                score += EARNING_SCORE;
            }
        }
        if (is_moved) {
            is_moved = false;
            continue;
        }

        // (b)
        if (!stock.is_empty()) {
            // move a card from the stock to the waste
            if (waste_pile.is_empty()) {
                waste_pile.push(stock.top());

                update_last_moving(stock.top(), last_moving.Stock, last_moving.Waste);
                print_each(solve_count, stock.top(), "stock", "waste");
                stock.pop();
                stock.set_closed_pos(stock.size() - 1);
                is_moved = true;
            }
        }
        if (is_moved) {
            is_moved = false;
            continue;
        }

        // waste to any playing piles
        if (!waste_pile.is_empty()) {
            const Card& waste_card = waste_pile.top();
            const int waste_card_number = waste_card.number;
            const int waste_card_symbol = static_cast<int>(waste_card.symbol);

            for (int i = 0; i < PLAYING_LINES; i++) {
                if (playing_piles[i].is_empty()) {
                    if (waste_card_number == Card::King) {
                        playing_piles[i].push(waste_card);
                        waste_pile.pop();

                        update_last_moving(waste_card, last_moving.Waste, i + 1);
                        print_each(solve_count, waste_card, "waste", "playing #" + std::to_string(i + 1));
                        is_moved = true;
                        solve_count++;
                        break;
                    }
                } else {
                    const Card& top_card = playing_piles[i].top();
                    const int top_card_number = top_card.number;
                    const int top_card_symbol = static_cast<int>(top_card.symbol);

                    // the different color → can move
                    if (is_sussesive(waste_card, top_card)) {
                        playing_piles[i].push(waste_card);
                        waste_pile.pop();

                        update_last_moving(waste_card, last_moving.Waste, i + 1);
                        print_each(solve_count, waste_card, "waste", "playing #" + std::to_string(i + 1));
                        is_moved = true;
                        solve_count++;
                        break;
                    }
                }

                // a king → empty plie
                if (playing_piles[i].is_empty() && waste_card_number == Card::King) {
                    playing_piles[i].push(waste_card);
                    waste_pile.pop();

                    update_last_moving(waste_card, last_moving.Waste, i + 1);
                    print_each(solve_count, waste_card, "waste", "playing #" + std::to_string(i + 1));
                    is_moved = true;
                    solve_count++;
                    break;
                }
            }
        }
        if (is_moved) {
            is_moved = false;
            continue;
        }

        // (c)
        for (int i = 0; i < PLAYING_LINES; i++) {
            CardPile tmp_stack;

            if (playing_piles[i].is_empty()) {
                continue;
            }

            // find the longest card sequence
            for (int j = playing_piles[i].size() - 1; ; j--) {
                tmp_stack.push(playing_piles[i].view(j));

                const Card& tmp_stack_top = tmp_stack.top();
                const int tmp_stack_top_number = tmp_stack_top.number;
                const int tmp_stack_top_symbol = static_cast<int>(tmp_stack_top.symbol);

                // the pile is empty or the card is king
                if (playing_piles[i].size() <= 1 || tmp_stack_top_number == Card::King) {
                    break;
                }

                // if not incresing numbers or not alternating color → the longest sequence
                if (j <= 0 ||
                    !playing_piles[i].is_opened(j - 1) ||
                    !is_sussesive(tmp_stack_top, playing_piles[i].view(j - 1))) {
                    break;
                }
            }

            // compare top of the other lines
            const Card& tmp_stack_top = tmp_stack.top();
            const int tmp_stack_top_number = tmp_stack_top.number;
            const int tmp_stack_top_symbol = static_cast<int>(tmp_stack_top.symbol);

            int suitable_pile = -1;
            for (int j = 0; j < PLAYING_LINES; j++) {
                if (i == j) {
                    continue;
                }

                if (playing_piles[j].is_empty()) {
                    if (tmp_stack_top_number == Card::King) {
                        suitable_pile = j;
                        break;
                    } else {
                        continue;
                    }
                }

                const Card& playing_pile_top = playing_piles[j].top();
                const int playing_pile_top_number = playing_pile_top.number;
                const int playing_pile_top_symbol = static_cast<int>(playing_pile_top.symbol);

                // if incresing numbers and alternating color → suitable
                if (is_sussesive(tmp_stack_top, playing_pile_top)) {
                    suitable_pile = j;
                    break;
                }
            }

            // move to the other pile → move
            if (suitable_pile != -1 &&
                !equal_to_the_last_move(tmp_stack, i + 1, suitable_pile + 1)) {

                // try to move king and the other cards from empty pile to the other empty pile → unnecessary moving
                if (playing_piles[i].size() == tmp_stack.size() &&
                    tmp_stack_top.number == Card::King &&
                    playing_piles[suitable_pile].size() == 0) {
                        suitable_pile = -1;
                        continue;
                }

                // cut from the orig playing piles
                for (int j = 0; j < tmp_stack.size(); j++) {
                    playing_piles[i].pop();
                }

                update_last_moving(tmp_stack, i + 1, suitable_pile + 1);

                // paste to the dest playing piles
                while (!tmp_stack.is_empty()) {
                    playing_piles[suitable_pile].push(tmp_stack.top());
                    print_each(solve_count, tmp_stack.top(),
                                "playing #" + std::to_string(i + 1),
                                "playing #" + std::to_string(suitable_pile + 1));

                    tmp_stack.pop();
                    solve_count++;
                }

                is_moved = true;
                break;
            }
        }
        if (is_moved) {
            is_moved = false;
            continue;
        }

        // (e)
        if (!stock.is_empty()) {
            waste_pile.push(stock.top());

            update_last_moving(stock.top(), last_moving.Stock, last_moving.Waste);
            print_each(solve_count, stock.top(), "stock", "waste");

            stock.pop();
            stock.set_closed_pos(stock.size() - 1);
            solve_count++;
            is_moved = true;
        }
        if (is_moved) {
            is_moved = false;
            continue;
        }

        // fallback (no move while (a) to (e))
        game_over = true;
        break;
    }

    if (game_over) {
        std::cout << "game over!\n"
                    << "score: " << score << "\n";
    } else {
        std::cout << "game clear!\n"
                    << "score: " << score << "\n";
    }

    return !game_over;
}

void Solitaire::print_board() {
    int max_length = 0;
    for (int i = 0; i < PLAYING_LINES; i++) {
        if (max_length < playing_piles[i].size()) {
            max_length = playing_piles[i].size();
        }
    }

    Card** board = new Card*[max_length + 2];
    bool** is_hidden = new bool*[max_length + 2];
    for (int i = 0; i < max_length + 2; i++) {
        board[i] = new Card[PLAYING_LINES > 7 ? PLAYING_LINES : 7];
        is_hidden[i] = new bool[PLAYING_LINES > 7 ? PLAYING_LINES : 7];

        for (int j = 0; j < (PLAYING_LINES > 7 ? PLAYING_LINES : 7); j++) {
            board[i][j].number = -1;
        }
    }

    // stock
    if (!stock.is_empty()) {
        board[0][0] = stock.top();
        is_hidden[0][0] = !stock.top_is_opened();
    }

    // waste
    if (!waste_pile.is_empty()) {
        board[0][1] = waste_pile.top();
        is_hidden[0][1] = !waste_pile.top_is_opened();
    }
    
    // output
    for (int i = 0; i < 4; i++) {
        if (output_piles[i].is_empty()) {
            continue;
        }

        board[0][i + 3] = output_piles[i].top();
        is_hidden[0][i + 3] = !output_piles[i].top_is_opened();
    }

    // playing piles
    for (int i = 0; i < PLAYING_LINES; i++) {
        if (playing_piles[i].is_empty()) {
            continue;
        }

        CardPile tmp_pile = playing_piles[i];
        
        for (int j = tmp_pile.size() - 1; j >= 0; j--) {
            board[j + 2][i] = tmp_pile.view(j);
            is_hidden[j + 2][i] = !tmp_pile.is_opened(j);
        }
    }

    for (int i = 0; i < max_length + 2; i++) {
        for (int j = 0; j < (PLAYING_LINES > 7 ? PLAYING_LINES : 7); j++) {
            if (board[i][j].number == -1) {
                std::cout << "     ";
                continue;
            }

            if (is_hidden[i][j]) {
                std::cout << "\033[1;34m";
            } else if (board[i][j].symbol % 2 == 1) {
                std::cout << "\033[1;31m";
            }

            std::cout << SYMBOL_STR[board[i][j].symbol] << NUMBER[board[i][j].number] << " ";

            std::cout << "\033[0m";
        }
        std::cout << "\n";
    }

    std::cout << "\n";

    for (int i = 0; i < max_length + 2; i++) {
        delete board[i];
        delete is_hidden[i];
    }
    
    delete board;
    delete is_hidden;
}