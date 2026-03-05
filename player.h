//
// Created by yoshi on 06.03.2026.
//

#ifndef CPP_PLAYER_H
#define CPP_PLAYER_H

class Player {
private:
    float x, y;
    float width, height;
    float velocityX, velocityY;
    float moveSpeed;
    float jumpForce;
    float gravity;
    float maxFallSpeed;
    bool isGrounded;
    bool isFacingRight;

public:
    Player(float startX, float startY, float playerWidth, float playerHeight);

    void update(float deltaTime);
    void handleInput();
    void draw(sf::RenderWindow &window);

    float getX() const { return x; }
    float getY() const { return y; }
    float getWidth() const { return width; }
    float getHeight() const { return height; }
    bool getGrounded() const { return isGrounded; }

    void setGrounded(bool grounded) { isGrounded = grounded; }
    void setPosition(float newX, float newY) { x = newX; y = newY; }
    void resetVelocity() { velocityX = 0; velocityY = 0; }
};

#endif //CPP_PLAYER_H
