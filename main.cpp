#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>
#include <time.h>

// i am using the code by sfml webside to make this more fun.
class TileMap : public sf::Drawable, public sf::Transformable
{
public:

    bool load(const std::string& tileset, sf::Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height)
    {
        // load the tileset texture
        if (!m_tileset.loadFromFile(tileset))
            return false;

        // resize the vertex array to fit the level size
        m_vertices.setPrimitiveType(sf::Quads);
        m_vertices.resize(width * height * 4);

        // populate the vertex array, with one quad per tile
        for (unsigned int i = 0; i < width; ++i)
            for (unsigned int j = 0; j < height; ++j)
            {
                // get the current tile number
                int tileNumber = tiles[i + j * width];

                // find its position in the tileset texture
                int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
                int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

                // get a pointer to the current tile's quad
                sf::Vertex* quad = &m_vertices[(i + j * width) * 4];

                // define its 4 corners
                quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
                quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
                quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
                quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

                // define its 4 texture coordinates
                quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
                quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
                quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
                quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
            }

        return true;
    }

private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // apply the transform
        states.transform *= getTransform();

        // apply the tileset texture
        states.texture = &m_tileset;

        // draw the vertex array
        target.draw(m_vertices, states);
    }

    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
};

class CustomRectangleShape : public sf::RectangleShape
{
public:
    CustomRectangleShape(sf::Vector2f size, sf::Vector2f position) : sf::RectangleShape(size)
    {
        setPosition(position);
    }
    void setSpeed(double x, double y, double r)
    {
        speed_x_ = x;
        speed_y_ = y;
        speed_r_ = r;
    }
    void set_bounds(double t, double b, double l, double r)
    {
        top_ = t;
        bottom_ = b;
        left_ = l;
        right_ = r;
    }
    void moveInDirection(const sf::Time &elapsed, const sf::Keyboard::Key &key)
    {
        sf::FloatRect bounds = getGlobalBounds();
        if(key == sf::Keyboard::Up && !(bounds.top <= 0.0))
        {
            move(0, -speed_y_ * elapsed.asSeconds());
        }
        else if(key == sf::Keyboard::Right && !(bounds.left >= right_ - bounds.width))
        {
            move(speed_x_ * elapsed.asSeconds(), 0);
        }
        else if(key == sf::Keyboard::Down && !(bounds.top >= bottom_ - bounds.height))
        {
            move(0, speed_y_ * elapsed.asSeconds());
        }
        else if(key == sf::Keyboard::Left && !(bounds.left <= 0.0))
        {
            move(-speed_x_ * elapsed.asSeconds(), 0);
        }
    }
    bool isClicked(sf::Vector2i &mouse_position)
    {
        sf::FloatRect bounds = getGlobalBounds();
        if(mouse_position.x >= bounds.left && mouse_position.x <= bounds.left + bounds.width
                && mouse_position.y >= bounds.top && mouse_position.y <= bounds.top + bounds.height)
        {
            return true;
        }
        return false;
    }
private:
    double speed_x_=0, speed_y_=0, speed_r_=0;
    double top_=0, bottom_=0, left_=0, right_=0;
};
void change_color(sf::RectangleShape &rectangle)
{
    rectangle.setFillColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));
}
int main()
{




    // this code used form website sfml.com
    const int level[] =
          {
              0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
              0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2, 1, 0, 0, 0,
              1, 1, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 1, 3, 3, 3,
              0, 1, 0, 0, 2, 0, 3, 3, 3, 0, 1, 1, 1, 0, 0, 0,
              0, 1, 1, 0, 3, 3, 3, 0, 0, 0, 1, 1, 1, 2, 0, 0,
              0, 0, 1, 0, 3, 0, 2, 2, 0, 0, 1, 1, 1, 1, 2, 0,
              2, 0, 1, 0, 3, 0, 2, 2, 2, 0, 1, 1, 1, 1, 1, 1,
              0, 0, 1, 0, 3, 2, 2, 2, 0, 0, 0, 0, 1, 1, 1, 1,
          };

          // create the tilemap from the level definition
          TileMap map;
          if (!map.load("tileset.png", sf::Vector2u(32, 32), level, 16, 8))
              return -1;
    // i am adding texture
        sf::Texture image1;
        if (image1.loadFromFile("image1.png") == -1)
        {
            std::cout << "FAILED!!!" << "\n";
            return 1;
        }
    // create the window
    sf::RenderWindow window(sf::VideoMode(512, 256), "");
    CustomRectangleShape rectangle(sf::Vector2f(32.0, 32.0), sf::Vector2f(100.0, 200.0));
    rectangle.setSpeed(50, 150, 10);
    rectangle.set_bounds(0, window.getSize().y, 0, window.getSize().x);
    rectangle.setTexture(&image1);
    sf::Clock clock;
    srand(time(NULL));
    while (window.isOpen())
    {
        sf::Time elapsed = clock.restart();
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            rectangle.moveInDirection(elapsed, sf::Keyboard::Up);
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            rectangle.moveInDirection(elapsed, sf::Keyboard::Down);
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            rectangle.moveInDirection(elapsed, sf::Keyboard::Left);
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            rectangle.moveInDirection(elapsed, sf::Keyboard::Right);
        }
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if(event.type == sf::Event::MouseButtonPressed)
            {
                if(event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
                    std::cout << "Mouse clicked: " << mouse_pos.x << ", " << mouse_pos.y << std::endl;
                    if(rectangle.isClicked(mouse_pos))
                    {
                        change_color(rectangle);
                    }
                }
            }
        }
        // clear the window with black color
        window.clear(sf::Color::White);
        // for tilemap
         window.draw(map);
        // draw everything here...
        window.draw(rectangle);
        // end the current frame
        window.display();
    }
    return 0;
}
