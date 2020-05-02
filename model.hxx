#pragma once

#include <ge211.hxx>

enum Game_States {
    Playing,
    Menu,
    Scores
};

enum Card_Status {
    Up,
    Down,
};

struct Card {
    int x;
    int y;
    int color_index;
    Card_Status  status;
};

struct Model {
    explicit Model();

    // General Variables
    ge211::Dimensions window_size_ = ge211::Dimensions {1000, 600};
    Game_States game_state_ = Game_States::Menu;
    int card_width_ = 100;

    // Playing State
    bool init_;
    bool is_game_over;
    int current_score_;
    std::vector<Card> cards_;
    ge211::Timer timer_;

    // Score state
    std::vector<int> scores_;

    // Game functions
    void generate_cards(int row, int col);
    void update_card_status(ge211::Position);
    void check_state(bool override);

    void respond_to_click(ge211::Position);
    void respond_to_key(ge211::Key);
};