#include "ui.hxx"

int colors[][3] = {
        {25, 25, 25},
        {25, 140, 255},
        {0, 255, 0},
        {54, 0, 232},
        {211, 26, 23},
        {77, 77, 0},
        {102, 0, 0},
        {230, 230, 0},
        {255, 179, 217},
        {127, 0, 153},
        {255, 85, 0},
        {0, 230, 153},
        {178, 132, 190},
        {59, 122, 87},
        {26, 0, 4},
        {255, 255, 102},
        {0, 68, 102},
        {175, 0, 42}
};

Ui::Ui(Model& model) : model_(model)
{
    for (auto color : colors) {
        ge211::Rectangle_sprite my_sprite(
                { model_.card_width_, model_.card_width_ },
                ge211::Color( color[0], color[1], color[2] ) );

        card_sprites_.push_back(my_sprite);
    }

    char btn_texts[][150] = {
            "Memory Game",
            "Press 1 for Easy",
            "Press 2 for Medium",
            "Press 3 for Hard",
            "Press 4 for score",
            "Done! Press m to go to the menu",
            "Scores"
    };

    for (auto btn_text : btn_texts) {
        ge211::Text_sprite btn_text_sprite = ge211::Text_sprite::Builder(sans)
                .message(btn_text)
                .color( ge211::Color::medium_red() )
                .build();

        texts_.push_back(btn_text_sprite);
    }

    background_color = ge211::Color(255, 255, 255);
};

ge211::Dimensions Ui::initial_window_dimensions() const
{
    return model_.window_size_;
}

void Ui::draw(ge211::Sprite_set& sprites) {

    if (model_.game_state_ == Game_States::Playing) {
        if (model_.is_game_over) {
            sprites.add_sprite( texts_[5], { 100, 100 } );
            return;
        }

        model_.check_state(false);

        for ( auto card : model_.cards_ ) {
            sprites.add_sprite(
                    card.status == Card_Status::Up ? card_sprites_[card.color_index] : card_turned_over_,
                    { card.x, card.y }
            );
        }
    }

    if (model_.game_state_ == Game_States::Menu) {
        for (int i = 0; i < 5; i++) {
            sprites.add_sprite( texts_[i], { 200, 90 * (i + 1) } );
        }
    }

    if (model_.game_state_ == Game_States::Scores) {
        update_score_sprites();
        sprites.add_sprite(texts_[6], {200, 100});
        sprites.add_sprite(score_list, {200, 200});
    }
}

void Ui::on_mouse_up(ge211::Mouse_button, ge211::Position position) {
    model_.respond_to_click(position);
}

void Ui::on_key(ge211::Key key) {
    model_.respond_to_key(key);
}

void Ui::update_score_sprites() {
    ge211::Text_sprite::Builder builder {sans};

    builder.word_wrap(500);

    const char* delim = "";
    for (auto score : model_.scores_) {
        builder << delim << score;
        delim = ",  ";
    }

    builder.color( ge211::Color::black() );

    score_list.reconfigure(builder);
}