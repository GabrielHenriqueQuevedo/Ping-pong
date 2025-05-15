#include "SFML\Graphics.hpp"
#include <random>
#include <iostream>

void playerMove(sf::RectangleShape& player, sf::Keyboard::Key up, sf::Keyboard::Key down);
void ballMove(sf::CircleShape& bola, sf::Vector2f& velBola, const sf::RectangleShape& player1, const sf::RectangleShape& player2);
sf::RectangleShape wallBuilder(const sf::Vector2f& size, const sf::Vector2f& position);

int main() {

    srand(time(0));

    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> distribution(0, 1);
    int randX = distribution(generator) == 0 ? -1 : 1;
    int randY = distribution(generator) == 0 ? -1 : 1;

    const float width = 1960;
    const float height = 1080;

    int player1Score = 0;
    int player2Score = 0;
    sf::Vector2f velBola((randX * 6), randY);
    bool P = false;

    //janela
    sf::RenderWindow window(sf::VideoMode({ (int)width, (int)height }), "Dark SoulsII", sf::Style::Default);
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(false);

    //textura
    sf::Texture ballTexture;
    if (!ballTexture.loadFromFile("CPP_FRIEREN.png")) {
        std::cerr << "Falha ao carregar textura!\a" << std::endl;
    }
    ballTexture.setSmooth(true);

    //texto
    sf::Font fonte;
    if (!fonte.openFromFile("arial.ttf")) {
        std::cerr << "Erro ao carregar fonte!\a" << std::endl;
    }
    sf::Text scoreP1(fonte);
    scoreP1.setPosition(sf::Vector2f((0 + 30), (0 + 30)));
    scoreP1.setFillColor(sf::Color(255, 255, 255));

    sf::Text scoreP2(fonte);
    scoreP2.setPosition(sf::Vector2f((width - 255), (0 + 30)));
    scoreP2.setFillColor(sf::Color(255, 255, 255));

    //bola
    sf::CircleShape bola(50.f);
    bola.setOrigin(bola.getGeometricCenter());
    bola.setPosition(sf::Vector2((width / 2), (height / 2)));
    bola.setTexture(&ballTexture);
    bola.setScale({ 1.5f, 1.5f });

    //players
    sf::RectangleShape player1(sf::Vector2f(15.f, 200.f));
    player1.setOrigin(player1.getGeometricCenter());
    player1.setPosition(sf::Vector2f((0 + 40), (height / 2)));
    player1.setScale({ 2.f, 2.f });

    sf::RectangleShape player2(sf::Vector2f(15.f, 200.f));
    player2.setOrigin(player2.getGeometricCenter());
    player2.setPosition(sf::Vector2f((width - 40), (height / 2)));
    player2.setScale({ 2.f, 2.f });

    //paredes
    sf::RectangleShape paredeCima = wallBuilder({ width, 15.f }, { (width / 2),   0.f });
    sf::RectangleShape paredeBaixo = wallBuilder({ width, 15.f }, { (width / 2),   height });
    sf::RectangleShape paredeEsquerda = wallBuilder({ 15.f, height }, { (0 - 30),     (height / 2) });
    sf::RectangleShape paredeDireita = wallBuilder({ 15.f, height }, { (width + 30), (height / 2) });

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
                window.close();

        }

        //update

        //movesets
        playerMove(player1, sf::Keyboard::Key::W, sf::Keyboard::Key::S);
        playerMove(player2, sf::Keyboard::Key::Up, sf::Keyboard::Key::Down);
        ballMove(bola, velBola, player1, player2);

        //colisões
        if (bola.getGlobalBounds().findIntersection(paredeCima.getGlobalBounds()) || bola.getGlobalBounds().findIntersection(paredeBaixo.getGlobalBounds())) {
            velBola.y *= -1.f;
        }

        //pontuações

        if (bola.getGlobalBounds().findIntersection(paredeEsquerda.getGlobalBounds())) {
            player2Score++;
            P = true;
        }

        if (bola.getGlobalBounds().findIntersection(paredeDireita.getGlobalBounds())) {
            player1Score++;
            P = true;
        }

        if (P == true) {
            bola.setPosition(sf::Vector2f((width / 2), (height / 2)));
            player1.setPosition(sf::Vector2f((0 + 40), (height / 2)));
            player2.setPosition(sf::Vector2f((width - 40), (height / 2)));
            randX = distribution(generator) == 0 ? -1 : 1;
            randY = distribution(generator) == 0 ? -1 : 1;
            velBola = sf::Vector2f((randX * 6), randY);
            P = false;
        }

        bola.move(velBola);

        //texto
        std::string player1ScoreString = std::to_string(player1Score);
        scoreP1.setString("Player 1 score: " + player1ScoreString);

        std::string player2ScoreString = std::to_string(player2Score);
        scoreP2.setString("Player 2 score: " + player2ScoreString);

        //draw 
        window.clear();

        //draw everything
        if (P == false) {
            window.draw(bola);
        }
        window.draw(player1);
        window.draw(player2);
        window.draw(paredeCima);
        window.draw(paredeBaixo);
        window.draw(paredeEsquerda);
        window.draw(paredeDireita);
        window.draw(scoreP1);
        window.draw(scoreP2);

        window.display();
    }

    return 0;
}

void playerMove(sf::RectangleShape& player, sf::Keyboard::Key up, sf::Keyboard::Key down) {

    if (sf::Keyboard::isKeyPressed(up)) {
        player.move(sf::Vector2f(0.f, -4.f));
    }

    if (sf::Keyboard::isKeyPressed(down)) {
        player.move(sf::Vector2f(0.f, 4.f));
    }

}

void ballMove(sf::CircleShape& bola, sf::Vector2f& velBola, const sf::RectangleShape& player1, const sf::RectangleShape& player2) {

    float randF = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    const float maxSpeed = 100.f;

    if (bola.getGlobalBounds().findIntersection(player1.getGlobalBounds())) {
        velBola.x = std::abs(velBola.x);
        velBola.y += randF - 0.5f;

        if (std::abs(velBola.x) < maxSpeed) {
            velBola.x *= 1.05f;
            velBola.y *= 1.025f;
        }
    }

    if (bola.getGlobalBounds().findIntersection(player2.getGlobalBounds())) {
        velBola.x = -std::abs(velBola.x);
        velBola.y += randF - 0.5f;

        if (std::abs(velBola.x) < maxSpeed) {
            velBola.x *= 1.05f;
            velBola.y *= 1.025f;
        }
    }
}

sf::RectangleShape wallBuilder(const sf::Vector2f& size, const sf::Vector2f& position) {

    sf::RectangleShape wall(sf::Vector2f(size.x, size.y));
    wall.setOrigin(wall.getGeometricCenter());
    wall.setPosition(sf::Vector2f(position.x, position.y));
    wall.setFillColor(sf::Color(0, 0, 0, 0));

    return wall;
}
