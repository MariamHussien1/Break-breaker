#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include <random>
#include <functional>
#include <cstdlib>
#include <cmath>

int bordercollision(float& radii, sf::Vector2f& newp, sf::Vector2f& direct) {
    if (newp.x - radii < 0) { // left window edge
        direct.x *= -1;
        newp.x = 0 + radii;
    }
    else if (newp.x + radii >= 800) { // right window edge
        direct.x *= -1;
        newp.x = 800 - radii;
    }
    else if (newp.y - radii < 0) { // top of window
        direct.y *= -1;
        newp.y = 0 + radii;
    }
    return 0;
}
int main()
{
    bool move = false, score = false, fall = false, button1 = false, button2 = false, button3 = false, end1 = false;
    int score1 = 0;

    sf::RenderWindow window(sf::VideoMode(800, 600), "Brick Breaker !");

    //BALL
    float ball_radius = 16.f;
    sf::CircleShape ball(5);
    ball.setOutlineThickness(4);
    ball.setOutlineColor(sf::Color(215, 215, 0, 220));
    ball.setFillColor(sf::Color(0, 0, 0, 0));
    ball.setOrigin(ball_radius / 2, ball_radius / 2);
    ball.setPosition(400, 520);

    //Board
    sf::RectangleShape board(sf::Vector2f(60.f, 12.f));
    board.setFillColor(sf::Color(0, 0, 0, 0));
    board.setPosition(sf::Vector2f(400.f, 540.f));
    board.setOutlineThickness(4);
    board.setOutlineColor(sf::Color(0, 0, 169, 255));

    //Bricks
    sf::RectangleShape rect[16][7];
    for (int x = 0; x < 16; x++)
    {
        for (int y = 0; y < 7; y++)
        {
            rect[x][y].setSize(sf::Vector2f(45, 20));
            rect[x][y].setPosition(x * 50.5, y * 26);
            rect[x][y].setFillColor(sf::Color(0, 0, 100));
            rect[x][y].setOutlineThickness(2);
            rect[x][y].setOutlineColor(sf::Color(215, 215, 0, 200));
        }
    }

    //collide with bricks sound
    sf::SoundBuffer buffer1;
    if (!buffer1.loadFromFile("collide.ogg"))
        return -1;
    sf::Sound sound1;
    sound1.setBuffer(buffer1);
    sound1.setVolume(100);

    //collide with board sound
    sf::SoundBuffer buffer2;
    if (!buffer2.loadFromFile("collideBoard.ogg"))
        return-1;
    sf::Sound sound2;
    sound2.setBuffer(buffer2);
    sound2.setVolume(30);

    //Backgrounds
    sf::Texture bckgrnd;
    bckgrnd.loadFromFile("C:/Users/Lenovo/source/repos/Project/Project/image/photom.png");
    sf::Sprite bckg;
    bckg.setTexture(bckgrnd);
    sf::Texture bckgrnd1;
    bckgrnd1.loadFromFile("C:/Users/Lenovo/source/repos/Project/Project/image/photo7.png");
    sf::Sprite bckg1;
    bckg1.setTexture(bckgrnd1);

    //Fonts
    sf::Font font;
    sf::Font font1;
    font1.loadFromFile("C:/Users/Lenovo/source/repos/Project/Project/Fonts/orange_juice.TTF");
    font.loadFromFile("C:/Users/Lenovo/source/repos/Project/Project/Fonts/Performer.TTF");

    //Buttons
    sf::RectangleShape Button1(sf::Vector2f(150, 50));
    Button1.setPosition(100, 500);
    Button1.setFillColor(sf::Color(0,0,0,0));
    sf::Text button1Text;
    button1Text.setFont(font);
    button1Text.setString("START");
    button1Text.setPosition(115, 497);
    button1Text.setFillColor(sf::Color(0, 80, 120));
    button1Text.setCharacterSize(45);

    sf::RectangleShape Button2(sf::Vector2f(325, 70));
    Button2.setPosition(260, 460);
    Button2.setFillColor(sf::Color(0, 0, 0, 0));
    sf::Text button2Text;
    button2Text.setFont(font1);
    button2Text.setString("End Game");
    button2Text.setPosition(270, 445);
    button2Text.setFillColor(sf::Color(215, 215, 0));
    button2Text.setCharacterSize(75);

    //win&lose messages
    sf::Text lose, winwin;
    lose.setFont(font1);
    lose.setString("GAME OVER !");
    lose.setFillColor(sf::Color(215, 215, 0));
    lose.setPosition(200, 300);
    lose.setCharacterSize(80);
    winwin.setFont(font1);
    winwin.setString("PERFECT !");
    winwin.setFillColor(sf::Color(215, 215, 0));
    winwin.setPosition(115, 100);
    winwin.setCharacterSize(150);

    //Score
    sf::Text score2;
    score2.setFont(font1);
    score2.setString("SCORE : " + std::to_string(score1));
    score2.setCharacterSize(40);
    score2.setFillColor(sf::Color(215, 215, 0));
    score2.setPosition(0, 550);
    
    //Ball Direction and velocity remove random
    sf::Vector2f direction(10.f, 10.f);
    float velocity = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    sf::Clock clock;
    sf::Time elapsed = clock.restart();
    const sf::Time update_ms = sf::seconds(1.f / 30.f);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {    
             // moving ball for first time   
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            {
                move = true;
            }
            if ((event.type == sf::Event::Closed) ||
                ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))) {
                window.close();
                break;
            }

            // moving board
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                    board.move(-20.f, 0.f);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                    board.move(20.f, 0.f);
                }
                if (board.getPosition().x < 0) { //left window edge
                    board.setPosition(0,540);
                }
                if (board.getPosition().x > 740) { //right window edge
                    board.setPosition(740, 540);
                }

            // control the buttons
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)
                && sf::Mouse::getPosition(window).x >= 100
                && sf::Mouse::getPosition(window).y >= 497
                && sf::Mouse::getPosition(window).x <= 550
                && sf::Mouse::getPosition(window).y <= 547){
                button1 = true;
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left) 
                && sf::Mouse::getPosition(window).x >= 260
                && sf::Mouse::getPosition(window).y >= 460
                && sf::Mouse::getPosition(window).x <= 595
                && sf::Mouse::getPosition(window).y <= 520){
                button2 = true;
            }
        }
 
        elapsed += clock.restart();

        //ball collison
        if (elapsed >= update_ms && move)
        {
            const auto pos = ball.getPosition();
            const auto delta = update_ms.asSeconds() * velocity;
            sf::Vector2f new_pos(pos.x + direction.x * delta, pos.y + direction.y * delta);
            bordercollision(ball_radius, new_pos, direction);
            if (new_pos.y + ball_radius >= 600) { // bottom of window
                fall = true;      
                end1 = true;
            }

            else if (ball.getGlobalBounds().intersects(board.getGlobalBounds()))
            {
                sound2.play();
                direction.y = -1 * velocity;
            }
           ball.setPosition(new_pos);
           elapsed -= update_ms; 
        }

        //displaying the interface
        window.clear();
        window.draw(bckg);
        window.draw(Button1);
        window.draw(button1Text);

        if (button1)
        {
            // displaying the game
            window.draw(bckg1);
            for (int x = 0; x < 16; x++)
            {
                for (int y = 0; y < 7; y++)
                {
                    window.draw(rect[x][y]);
                    if (ball.getGlobalBounds().intersects(rect[x][y].getGlobalBounds()))
                    { 
                        sound1.play();
                        rect[x][y].move(0.f, 700.f);
                        direction.y *= -1;
                        score = true;
                        score1++;
                    }
                    if (score)
                    {
                        score2.setString("SCORE : " + std::to_string(score1));
                    }
                }
            }

            window.draw(ball);
            window.draw(board);
            window.draw(score2);

            //winning the game
            if (score1 >=112)
            {
                window.draw(winwin);
                ball.move(0.f, 900.f);
                fall = false;
                window.draw(Button2);
                window.draw(button2Text);
            }

            //losing the game
            if (fall)
            {
                window.draw(lose);
                ball.move(0.f, 900.f);
                window.draw(Button2);
                window.draw(button2Text);
            }
        }
        // Ending the game
        if (button2 && end1)
        { 
            window.close();
        }
        window.display();
    }
    return 0;
}