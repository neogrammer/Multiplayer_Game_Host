#pragma once
#include "../GameState.h"

class TitleState : public GameState {
public:
    TitleState();
    ~TitleState() override {}
    void Input() override;
    void Update(sf::Time deltaTime, Player* host_, Player* guest_) override;
    void Render(sf::RenderWindow& wnd_) override;
};
