#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
using namespace sf;
using namespace std;
// Constants for game dimensions
const int width = 700;
const int height = 600;
const int gridSize = 20;
void w2(Font& font, int& score) {
	RenderWindow w(VideoMode(300.f, 320.f), "Snake Game");
	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(24);
	scoreText.setFillColor(Color::White);
	scoreText.setPosition(90, 90);
	Text m("Keep It Up!", font, 25);
	m.setFillColor(Color::Red);
	m.setPosition(Vector2f(80.f, 150.f));
	m.setOutlineThickness(5.f);
	m.setOutlineColor(Color::Black);
	while (w.isOpen()) {
		Event event;
		while (w.pollEvent(event)) {
			if (event.type == Event::Closed)
				w.close();
			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
				w.close();
		}
		w.clear(Color::Blue);
		scoreText.setString("Score: " + to_string(score));
		w.draw(scoreText);
		w.draw(m);
		w.display();
	}
}
// Function for the gameplay window
void gameWindow(Font& font) {
    RenderWindow game(VideoMode(width, height), "Snake Game");
    // Snake setup
    vector<RectangleShape> snake(1);
    snake[0].setSize(Vector2f(gridSize, gridSize));
    snake[0].setFillColor(Color::Green);
    snake[0].setPosition(width / 2, height / 2);
    // Fruit setup
    RectangleShape fruit(Vector2f(gridSize, gridSize));
    fruit.setFillColor(Color::Red);
    fruit.setPosition(gridSize * (rand() % (width / gridSize)), gridSize * (rand() % (height / gridSize)));
    // Game variables
    Vector2f direction(0, 0);
    int score = 0;
    Clock clock;
    float speed = 0.1f;
    // Score display
    Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(Color::White);
    scoreText.setPosition(10, 10);
    while (game.isOpen()) {
        // Event handling
        Event event;
        while (game.pollEvent(event)) {
            if (event.type == Event::Closed)
                game.close();
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::W && direction.y == 0) direction = Vector2f(0, -gridSize);
                if (event.key.code == Keyboard::S && direction.y == 0) direction = Vector2f(0, gridSize);
                if (event.key.code == Keyboard::A && direction.x == 0) direction = Vector2f(-gridSize, 0);
                if (event.key.code == Keyboard::D && direction.x == 0) direction = Vector2f(gridSize, 0);
            }
        }
        // Update game state
        if (clock.getElapsedTime().asSeconds() > speed) {
            clock.restart();
            // Move snake
            for (int i = snake.size() - 1; i > 0; --i)
                snake[i].setPosition(snake[i - 1].getPosition());
            snake[0].move(direction);
            // Check collisions with fruit
            if (snake[0].getGlobalBounds().intersects(fruit.getGlobalBounds())) {
                score += 10;
                snake.push_back(RectangleShape(Vector2f(gridSize, gridSize)));
                snake.back().setFillColor(Color::Green);
                fruit.setPosition(gridSize * (rand() % (width / gridSize)), gridSize * (rand() % (height / gridSize)));
            }
            // Check collisions with walls
			if (snake[0].getPosition().x < 0 || snake[0].getPosition().x >= width ||
				snake[0].getPosition().y < 0 || snake[0].getPosition().y >= height) {
				game.close();
				w2(font, score);
			}
            // Check collisions with itself
			for (int i = 1; i < snake.size(); ++i) {
				if (snake[0].getPosition() == snake[i].getPosition()) {
					game.close();
					w2(font, score);
				}
            }
        }
        // Render
        game.clear(Color::Black);
        // Draw fruit and snake
        game.draw(fruit);
        for (const auto& segment : snake)
            game.draw(segment);
        // Display score
        scoreText.setString("Score: " + to_string(score));
        game.draw(scoreText);
        game.display();
    }
}
int main() {
	RenderWindow w1(VideoMode(700.f, 600.f), "Snake Game");
	Font font;
	if (!font.loadFromFile("arial.ttf")) {
		cout << "Failed to load font!" << endl;
		return -1;
	}
	Text m("Snake Game", font, 45);
	m.setFillColor(Color::Magenta);
	m.setPosition(Vector2f(200.f, 150.f));
	m.setOutlineThickness(5.f);
	m.setOutlineColor(Color::Black);
	RectangleShape box(Vector2f(100.f, 50.f));
	box.setFillColor(Color::Cyan);
	box.setPosition(Vector2f(300.f, 251.f));
	box.setOutlineThickness(5.f);
	box.setOutlineColor(Color::Black);
	Text t("Play", font, 45);
	t.setFillColor(Color::Blue);
	t.setPosition(Vector2f(305.f, 245.f));
	t.setOutlineThickness(5.f);
	t.setOutlineColor(Color::Black);
	while (w1.isOpen()) {
		Event event;
		while (w1.pollEvent(event)) {
			if (event.type == Event::Closed)
				w1.close();
			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
				w1.close();
			if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
				Vector2f mousePosition = w1.mapPixelToCoords(Mouse::getPosition(w1));
				if (box.getGlobalBounds().contains(mousePosition)) {
					gameWindow(font);
				}
			}
		}
		w1.clear(Color::Yellow);
		w1.draw(m);
		w1.draw(box);
		w1.draw(t);
		w1.display();
	}
	return 0;
}
