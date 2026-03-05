#pragma once

class Obstacle {
    private:
        float x, y;
        float width, height;
    public:
        Obstacle(float x, float y, float width, float height) : x(x), y(y), width(width), height(height) {}
        void draw();
        void checkCollision(Player &player);
        void onCollision();
};