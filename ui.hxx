#pragma once

#include "model.hxx"
#include <ge211.hxx>

struct Ui : ge211::Abstract_game {
    explicit Ui(Model&);

    Model& model_;

    ge211::Dimensions initial_window_dimensions() const override;
    void draw(ge211::Sprite_set&) override;
    void on_mouse_up(ge211::Mouse_button, ge211::Position) override;
    void on_key(ge211::Key) override;


    void update_score_sprites();

    ///
    /// Game Sprites
    ///
    ge211::Font sans {"sans.ttf", 30};
    ge211::Text_sprite score_list;
    std::vector<ge211::Text_sprite> texts_;
    std::vector<ge211::Rectangle_sprite> card_sprites_;
    ge211::Rectangle_sprite const card_turned_over_ {
            { model_.card_width_, model_.card_width_ },
            ge211::Color(100, 100, 100) };
};