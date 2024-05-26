#pragma once

class Button {
public:
    Button(float x, float y, float width, float height, const String& text) {
        shape.setPosition(x, y);
        shape.setSize(Vector2f(width, height));
        shape.setFillColor(Color::White);

        if (!font.loadFromFile("arialmt.ttf")) {
            // handle error
        }

        buttonText.setFont(font);
        buttonText.setString(text);
        buttonText.setCharacterSize(36);
        buttonText.setFillColor(Color::Black);
        buttonText.setPosition(
            x + (width - buttonText.getLocalBounds().width) / 2,
            y + (height - buttonText.getLocalBounds().height) / 2 - buttonText.getLocalBounds().height / 2
        );
    }

    void draw(RenderWindow& window) {
        window.draw(shape);
        window.draw(buttonText);
    }

    bool isClicked(Vector2i mousePos) {
        return shape.getGlobalBounds().contains(static_cast<Vector2f>(mousePos));
    }

private:
    RectangleShape shape;
    Text buttonText;
    Font font;
};
