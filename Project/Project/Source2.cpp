/*#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <math.h>
#include <algorithm>
#include <iostream>
#include <sstream>

#define PI   3.14159265358979323846

using namespace sf;

bool intersects(const RectangleShape& rect1, const RectangleShape& rect2)
{
    FloatRect r1 = rect1.getGlobalBounds();
    FloatRect r2 = rect2.getGlobalBounds();
    return r1.intersects(r2);
}

float clamp(const float x, const float a, const float b)
{
    return std::min(std::max(a, x), b);
}


class Brick : public RectangleShape
{
public:
    unsigned int armor;

    Brick() :RectangleShape()
    {
        armor = 2;
    }

    Brick(const Vector2f& size) :RectangleShape(size)
    {
        armor = 2;
    }


    bool collide(RectangleShape& ball, Vector2f& ballSpeed)
    {
        // ball collides brick
        if (intersects(ball, *this))
        {
            FloatRect r = getGlobalBounds();
            FloatRect b = ball.getGlobalBounds();

            //let o be the center of p
            Vector2f o = Vector2f(r.left + r.width / 2, r.top + r.height / 2);
            //om: vector from o to the center of the ball M
            Vector2f om = Vector2f(b.left + b.width / 2 - o.x, b.top + b.height / 2 - o.y);
            // let's scale om to square dimensions and act as if p is a square
            om.x /= r.width;
            om.y /= r.height;
            // reflect the ball according to the angle of om
            float angle = atan2(om.y, om.x);
            if (angle >= PI / 4 && angle <= 3 * PI / 4) //bottom
            {
                ballSpeed.y = abs(ballSpeed.y);
                float u = r.top + r.height - b.top;
                ball.move(0, 2 * u);
                return true;
            }
            if (angle <= -PI / 4 && angle >= -3 * PI / 4) //top
            {
                ballSpeed.y = -abs(ballSpeed.y);
                float u = b.top + b.height - r.top;
                ball.move(0, -2 * u);
                return true;
            }
            if (angle <= -3 * PI / 4) //left
            {
                ballSpeed.y = -abs(ballSpeed.x);
                float u = b.left + b.width - r.left;
                ball.move(-2 * u, 0);
                return true;
            }
            if (abs(angle) <= PI / 4) //right
            {
                ballSpeed.y = abs(ballSpeed.x);
                float u = r.left + r.width - b.left;
                ball.move(2 * u, 0);
                return true;
            }
        }
        return false;
    }


};


class Grid : public FloatRect
{
private:
    unsigned int rows;
    unsigned int columns;

    std::vector<Brick>* bricks;

public:
    Grid()
    {
        rows = 0;
        columns = 0;
        bricks = NULL;
    }

    ~Grid()
    {
        if (bricks != NULL)
            delete bricks;
    }

    void setDimensions(unsigned int col, unsigned int row)
    {
        rows = row;
        columns = col;
        if (bricks != NULL)
            delete bricks;

        bricks = new std::vector<Brick>(rows * columns);
        init();
    }

    Vector2f cellSize()
    {
        return Vector2f(width / columns, height / rows);
    }

    void init()
    {
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < columns; j++)
            {
                bricks->at(i + j * rows).setSize(cellSize());
                bricks->at(i + j * rows).setFillColor(Color::Blue);
                bricks->at(i + j * rows).setOutlineColor(Color::Black);
                bricks->at(i + j * rows).setOutlineThickness(3);
                bricks->at(i + j * rows).setPosition(left + j * cellSize().x, top + i * cellSize().y);
            }
    }

    void display(RenderWindow& window)
    {
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < columns; j++)
            {
                if (bricks->at(i + j * rows).armor <= 0)
                    continue;
                window.draw(bricks->at(i + j * rows));
            }
    }

    bool collide(RectangleShape& ball, Vector2f& ballSpeed, unsigned int& score)
    {
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < columns; j++)
            {
                if (bricks->at(i + j * rows).armor <= 0)
                    continue;
                if (bricks->at(i + j * rows).collide(ball, ballSpeed))
                {
                    bricks->at(i + j * rows).armor--;
                    bricks->at(i + j * rows).setFillColor(Color::Green);
                    if (bricks->at(i + j * rows).armor == 0)
                        score += 700;

                    return true;
                }
            }
        return false;
    }

    bool isGameWon()
    {
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < columns; j++)
                if (bricks->at(i + j * rows).armor > 0)
                    return false;
        return true;
    }

};*/



/**
 * Basic Game Structure
 * use init and exec in main*
 
class Game
{
private:
    static const int FRAMES_PER_SECOND = 60;
    static const int MAX_FRAMESKIP = 10;
    static const int width = 640;
    static const int height = 480;
    static const int borderSize = 12;
    static const int margin = 50;
    static const int moveDistance = 40;


    RenderWindow window;
    Font font;

    RectangleShape top;
    RectangleShape left;
    RectangleShape right;
    RectangleShape bottom;

    RectangleShape ball;

    Vector2f ballSpeed;

    RectangleShape player;

    Text title;
    Text start;
    Text won;
    Text lost;
    Text score;
    Text lives;
    Text fps;


    Grid grid;

    Time time;
    Int32 updateTime;

    unsigned int playerScore;
    unsigned int playerLives;

    enum states { INTRO, PLAYING, GAME_WON, GAME_LOST };

    int gameState;

public:
    Game()
    {
    }

    bool init()
    {
        VideoMode videoMode(width, height);
        window.create(videoMode, "Break Out");
        window.setVerticalSyncEnabled(true);
        window.setFramerateLimit(FRAMES_PER_SECOND);
        setup();
        return true;
    }

    int exec()
    {
        Clock renderClock, updateClock;
        while (window.isOpen())
        {
            time = renderClock.getElapsedTime();
            float fFps = 1000000 / time.asMicroseconds();
            std::stringstream s;
            s << "fps: " << fFps;
            fps.setString(s.str());
            renderClock.restart();

            const Int64 frameTime = 1000000 / FRAMES_PER_SECOND;
            Clock c;
            Time t = c.getElapsedTime();
            Int64 nextFrameTime = t.asMicroseconds() + frameTime;

            int loops = 0;
            while (t.asMicroseconds() < nextFrameTime && loops < MAX_FRAMESKIP)
            {
                processEvents();
                updateTime = updateClock.restart().asMilliseconds();
                update();
                t = c.getElapsedTime();
                loops++;
            }

            display();
        }

        return EXIT_SUCCESS;
    }

private:

    void processEvents()
    {
        Event event;
        while (window.pollEvent(event))
        {
            if ((event.type == Event::Closed) ||
                ((event.type == Event::KeyPressed) && (event.key.code == Keyboard::Escape)))
                window.close();
            else
                if ((event.type == Event::KeyPressed) && (gameState == INTRO))
                    gameState = PLAYING;
        }
    }

    void update()
    {
        if (gameState != PLAYING)
            return;

        updatePlayer();
        checkCollisions();
        updateBall();

        // detect if game is over
        if (playerLives < 0)
            gameState = GAME_LOST;
    }

    void setup()
    {
        top.setPosition(0, 0);
        top.setSize(Vector2f(width, borderSize));

        left.setPosition(0, borderSize);
        left.setSize(Vector2f(borderSize, height + borderSize));

        right.setPosition(width - borderSize, borderSize);
        right.setSize(Vector2f(borderSize, height + borderSize));

        bottom.setPosition(0, height + 2 * borderSize);
        bottom.setSize(Vector2f(width, borderSize));

        top.setFillColor(Color(100, 100, 100));
        top.setOutlineColor(Color::Black);
        top.setOutlineThickness(2);

        left.setFillColor(Color(100, 100, 100));
        left.setOutlineColor(Color::Black);
        left.setOutlineThickness(2);

        right.setFillColor(Color(100, 100, 100));
        right.setOutlineColor(Color::Black);
        right.setOutlineThickness(2);

        bottom.setFillColor(Color(100, 100, 100));
        bottom.setOutlineColor(Color::Black);
        bottom.setOutlineThickness(2);

        ball.setPosition(width / 2, height - margin);
        ball.setSize(Vector2f(20, 20));
        ball.setFillColor(Color::Green);
        ball.setOutlineColor(Color::Red);
        ball.setOutlineThickness(2);

        player.setSize(Vector2f(90, borderSize));
        player.setPosition(width / 2 - 45, height - margin - borderSize);
        player.setFillColor(Color(0, 122, 245));
        player.setOutlineColor(Color::Green);
        player.setOutlineThickness(3);

        title.setString("Break Out!");
        title.setFont(font);
        title.setCharacterSize(50);
        title.setPosition(width / 2 - title.getGlobalBounds().width / 2, 100);
        title.setFillColor(sf::Color::Blue);

        start.setString("Press any key to start");
        start.setFont(font);
        start.setCharacterSize(30);
        start.setPosition(width / 2 - start.getGlobalBounds().width / 2, 400);
        start.setFillColor(sf::Color::Red);

        won.setString("You have won this game.\n\n Congratulations !");
        won.setFont(font);
        won.setCharacterSize(20);
        won.setPosition(width / 2 - won.getGlobalBounds().width / 2, height / 2 - won.getGlobalBounds().height / 2);
        won.setFillColor(sf::Color::Green);

        lost.setString("You have lost this game, \n better luck next time!");
        lost.setFont(font);
        lost.setCharacterSize(20);
        lost.setPosition(width / 2 - lost.getGlobalBounds().width / 2, height / 2 - lost.getGlobalBounds().height / 2);
        lost.setFillColor(sf::Color::Red);

        score.setString("0");
        score.setFont(font);
        score.setCharacterSize(30);
        score.setPosition(width / 2 + score.getGlobalBounds().width / 2, height - 60);
        score.setFillColor(sf::Color(0, 0, 100, 50));

        lives.setString("5");
        lives.setFont(font);
        lives.setCharacterSize(50);
        lives.setPosition(width / 2 - lives.getGlobalBounds().width / 2, height - 60);
        lives.setFillColor(sf::Color(0, 0, 100, 50));

        fps.setString("0");
        fps.setFont(font);
        fps.setCharacterSize(30);
        fps.setPosition(fps.getGlobalBounds().width / 2, height - 40);
        fps.setFillColor(sf::Color(52, 0, 100, 50));


        resetGame();
        gameState = INTRO;

        grid.setDimensions(7, 5);
        grid.left = borderSize + 5;
        grid.top = borderSize + 5;
        grid.width = width - 2 * borderSize - 10;
        grid.height = 250;
        grid.init();
    }

    void display()
    {
        window.clear(Color::White);
        switch (gameState)
        {
        case INTRO:
            window.draw(title);
            window.draw(start);
            break;
        case PLAYING:
            window.draw(left);
            window.draw(right);
            grid.display(window);
            window.draw(player);
            window.draw(ball);
            window.draw(score);
            window.draw(lives);
            window.draw(top);
            window.draw(bottom);
            window.draw(fps);
            break;
        case GAME_WON:
            window.draw(won);
            break;
        case GAME_LOST:
            window.draw(lost);
            break;
        }
        window.display();
    }

    void updatePlayer()
    {
        // move player 1 pad
        if (Keyboard::isKeyPressed(Keyboard::Left))
        {
            player.move(-moveDistance * updateTime / 50.0, 0);
        }
        else
            if (Keyboard::isKeyPressed(Keyboard::Right))
            {
                player.move(moveDistance * updateTime / 50.0, 0);
            }
    }

    void updateBall()
    {
        ball.move(ballSpeed.x * updateTime, ballSpeed.y * updateTime);
    }

    void checkCollisions()
    {

        // block players pad inside the play area
        if (intersects(player, left) || intersects(player, right))
        {
            FloatRect l = left.getGlobalBounds();
            FloatRect r = right.getGlobalBounds();
            Vector2f p = player.getPosition();
            p.x = clamp(p.x, l.left + l.width + 5, r.left - player.getGlobalBounds().width - 5);
            player.setPosition(p.x, p.y);
  
        }
        // ball collides with top, left and right
        if (intersects(ball, top))
        {
            FloatRect t = top.getGlobalBounds();
            FloatRect b = ball.getGlobalBounds();
            ballSpeed.y = abs(ballSpeed.y);
            int u = t.top + t.height - b.top;
            ball.move(0, 2 * u);

        }
        if (intersects(ball, left))
        {
            FloatRect l = left.getGlobalBounds();
            FloatRect b = ball.getGlobalBounds();
            ballSpeed.x = abs(ballSpeed.x);
            int u = l.left + l.width - b.left;
            b.left = l.left + l.width + u;
            ball.setPosition(b.left, b.top);

        }

        if (intersects(ball, right))
        {
            FloatRect r = right.getGlobalBounds();
            FloatRect b = ball.getGlobalBounds();
            ballSpeed.x = -abs(ballSpeed.x);
            int u = b.left + b.width - r.left;
            b.left = r.left - b.width - u;
            ball.setPosition(b.left, b.top);

        }

        if (intersects(ball, bottom))
        {

            playerLives--;
            std::stringstream str;
            str << playerLives;
            lives.setString(str.str());
            lives.setPosition(width / 2 - lives.getGlobalBounds().width / 2, height - 60);
            resetBall();
        }
        // ball collides with player
        if (intersects(ball, player))
        {
            FloatRect p = player.getGlobalBounds();
            FloatRect b = ball.getGlobalBounds();

            //let o be the center of p
            Vector2f o = Vector2f(p.left + p.width / 2, p.top + p.height / 2);
            //om: vector from o to the center of the ball M
            Vector2f om = Vector2f(b.left + b.width / 2 - o.x, b.top + b.height / 2 - o.y);
            // let's scale om to square dimensions and act as if p is a square
            om.x /= p.width;
            om.y /= p.height;
            // reflect the ball according to the angle of om
            float angle = atan2(om.y, om.x);
            if (angle < -PI / 4 && angle > -3 * PI / 4) //top
            {
                ballSpeed.y = -abs(ballSpeed.y);//in case of double contact
                ballSpeed.x = (b.left + b.width / 2 - p.left - p.width / 2) / 200;
                int u = b.top + b.height - p.top;
                b.top = p.top - b.height - u;
                ball.setPosition(b.left, b.top);
                //increase ball speed by 2%
                ballSpeed.x *= 1.02f;
                ballSpeed.y *= 1.02f;

            }
        }
        //ball collides with grid
        if (grid.collide(ball, ballSpeed, playerScore))
        {
  
            playerScore += 200;
            std::stringstream str;
            str << playerScore << " pts  ";
            score.setString(str.str());
            score.setPosition(width / 2 + score.getGlobalBounds().width / 2 - margin, height - 60);
        }
        // check for end of game
        if (grid.isGameWon())
            gameState = GAME_WON;
        if (playerLives <= 0)
            gameState = GAME_LOST;
    }

    void resetGame()
    {
        playerScore = 0;
        playerLives = 5;
        resetBall();
    }
    void resetBall()
    {
        FloatRect p = player.getGlobalBounds();
        FloatRect b = ball.getGlobalBounds();
        ball.setPosition(p.left + p.width / 2 + 5, p.top);
        ballSpeed.x = 0.1f;
        ballSpeed.y = -0.3f;
    }
};


int main()
{
    Game game;
    if (!game.init())
        return EXIT_FAILURE;
    return game.exec();
}*/