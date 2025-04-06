#include <SFML/Graphics.hpp>

#include <iostream>
#include <vector>
#include <cmath>
#include <random>

class Particle {
public:
    sf::CircleShape shape;
    sf::Vector2f    velocity;
    float           lifetime = 0.f;
};

class Rocket {
public:
    sf::RectangleShape shape;
    sf::Vector2f       velocity;
    sf::Color          particleColor;
};

int main() {
    constexpr unsigned int windowWidth  = 1920u;
    constexpr unsigned int windowHeight = 1080u;

    auto window = sf::RenderWindow(sf::VideoMode({ windowWidth, windowHeight }), "Fireworks");
    window.setFramerateLimit(60);

    std::random_device rd;
    std::mt19937       engine(rd());

    std::vector<Particle> particles;
    std::vector<Rocket>   rockets;
    sf::Clock             clock;
    sf::Clock             launchClock;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        const float dt = clock.restart().asSeconds();
        if (launchClock.getElapsedTime().asSeconds() >= 2.f) {
            launchClock.restart();
            Rocket r;
            r.shape = sf::RectangleShape({ 3.f, 10.f });
            r.shape.setFillColor(sf::Color::White);
            r.particleColor = sf::Color(
                std::uniform_int_distribution<int>(150, 255)(engine),
                std::uniform_int_distribution<int>(150, 255)(engine),
                std::uniform_int_distribution<int>(150, 255)(engine)
            );

            const unsigned int randomX = std::uniform_int_distribution<
                int>(windowWidth / 4, windowWidth * 3 / 4)(engine);
            r.shape.setPosition({ static_cast<float>(randomX), windowHeight });
            float vx   = std::uniform_real_distribution<float>(-20, 20)(engine);
            float vy   = std::uniform_real_distribution<float>(-500, -700)(engine);
            r.velocity = sf::Vector2f({ vx, vy });
            rockets.push_back(r);
        }

        for (auto r = rockets.begin(); r != rockets.end();) {
            r->shape.move(r->velocity * dt);
            r->velocity.y += 9.8f * dt * 30.f;
            if (r->velocity.y > 0) {
                const sf::Vector2f pos = r->shape.getPosition();
                for (int i = 0; i < 500; ++i) {
                    Particle p;
                    p.shape = sf::CircleShape(2.5f);
                    p.shape.setFillColor(r->particleColor);
                    p.shape.setPosition(pos);
                    const float angle = std::uniform_real_distribution<float>(-180.f, 180.f)(engine) * 3.14f / 180.f;
                    const float speed = std::uniform_real_distribution<float>(0.f, 250.f)(engine);

                    p.velocity = sf::Vector2f({ std::cos(angle) * speed, std::sin(angle) * speed });
                    p.lifetime = std::uniform_real_distribution<float>(1.f, 2.5f)(engine);
                    particles.push_back(p);
                }
                r = rockets.erase(r);
            } else {
                ++r;
            }
        }

        for (auto p = particles.begin(); p != particles.end();) {
            p->lifetime -= dt;
            // int   r    = p->shape.getFillColor().r;
            // int   g    = p->shape.getFillColor().g;
            // int   b    = p->shape.getFillColor().b;
            // float life = p->lifetime / 4.f + .5f;
            // p->shape.setFillColor({
            //     static_cast<std::uint8_t>(r * life),
            //     static_cast<std::uint8_t>(g * life),
            //     static_cast<std::uint8_t>(b * life)
            // });
            p->velocity = sf::Vector2f({ p->velocity.x * .99f, p->velocity.y * .99f });
            if (p->lifetime <= 0.f) {
                p = particles.erase(p);
            } else {
                p->shape.move(p->velocity * dt);
                p->velocity.y += 9.8f * dt * 10.f;
                ++p;
            }
        }

        window.clear(sf::Color::Black);
        for (const auto& r : rockets) {
            window.draw(r.shape);
        }
        for (const auto& p : particles) {
            window.draw(p.shape);
        }
        window.display();
    }

    return 0;
}
