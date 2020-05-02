#include "model.hxx"
#include <algorithm>

using namespace std;

Model::Model() = default;

void Model::generate_cards(int row, int col) {
    cards_.clear();

    int i = 0;
    int j = 0;

    int spacing = 5;
    int card_size_w_spacing = card_width_ + spacing;
    int offsetX = (window_size_.width - (card_size_w_spacing * col)) / 2;
    int offsetY = (window_size_.height - (card_size_w_spacing * row)) / 2;

    for (; i < row; i++) {
        for (j = 0; j < col; j++) {
            Card my_card{};
            my_card.x = offsetX + card_size_w_spacing * j;
            my_card.y = offsetY + card_size_w_spacing * i;
            my_card.status = Card_Status::Up;

            cards_.push_back(my_card);
        }
    }

    // Shuffle them
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle( cards_.begin(), cards_.end(), g );

    // Assign colors
    i = 0;
    int color_index = 0;
    while(i < cards_.size() - 1) {
        cards_[i].color_index = color_index;
        cards_[i + 1].color_index = color_index;
        i += 2;
        color_index++;
    }

    game_state_ = Game_States::Playing;
    current_score_ = 0;
    is_game_over = false;
    init_ = false;
    timer_ = ge211::Timer();
}

void Model::update_card_status(ge211::Position position) {
    int i = 0;
    current_score_++;
    check_state(true);

    for (auto &card: cards_) {
        if (
                position.x > card.x &&
                position.x < card.x + card_width_ &&
                position.y > card.y &&
                position.y < card.y + card_width_
                ) {
            card.status = Card_Status::Up;
        }

        if (card.status == Card_Status::Up) i++;
    }

    if (i == cards_.size()) {
        is_game_over = true;
        scores_.push_back(current_score_);
    }
}

void Model::check_state(bool override) {
    // Debounce the function to a 1s interval
    if (timer_.elapsed_time().seconds() < 0.8 && !override) {
        return;
    }

    timer_ = ge211::Timer();
    if (!init_) {
        for(auto &card: cards_) {
            card.status = Card_Status::Down;
        }

        init_ = true;
        return;
    };

    int count = 0;
    for (auto card: cards_) {
        if (card.status == Card_Status::Up) count++;
    }

    if (count % 2 == 1) return;

    int i = 0;

    while (i < cards_.size() - 1) {
        if (
                !(
                        cards_[i].status == Card_Status::Up &&
                        cards_[i + 1].status == Card_Status::Up
                ))
        {
            cards_[i].status = Card_Status::Down;
            cards_[i + 1].status = Card_Status::Down;
        }

        i += 2;
    }
};

void Model::respond_to_click(ge211::Position position) {
    if ( game_state_ == Game_States::Playing ) {
        update_card_status(position);
    }
}

void Model::respond_to_key(ge211::Key key) {
    if (key == ge211::Key::code('m')) {
        game_state_ = Game_States::Menu;
    }

    if (game_state_ != Game_States::Menu) return;

    if (key == ge211::Key::code('1')) {
        generate_cards(4, 4);
    } else if (key == ge211::Key::code('2')) {
        generate_cards(4, 5);
    } else if (key == ge211::Key::code('3')) {
        generate_cards(4, 6);
    } else if (key == ge211::Key::code('4')) {
        game_state_ = Game_States::Scores;
    } else {
    }
}