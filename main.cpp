#include <SFML/Graphics.hpp>
#include "Player.h"
#include "main.h"
#include "TextureHolder.h"
#include "Bullet.h"

using namespace sf;

int main()
{    // INIT Variables
    TextureHolder holder;

    enum class State 
    {
        PAUSED, LEVELING_UP, GAME_OVER, PLAYING
    };

    State state = State::GAME_OVER;

    Vector2f resolution;
    resolution.x = VideoMode::getDesktopMode().width;
    resolution.y = VideoMode::getDesktopMode().height;

    RenderWindow window(VideoMode(resolution.x, resolution.y), "Zombie Arena", Style::Fullscreen);

    // Create view main action
    View mainView(FloatRect(0, 0, resolution.x, resolution.y));

    Clock clock;
    // How long PLAYING state has been active
    Time gameTimeTotal;

    Vector2f mouseWordlPosition;

    Vector2i mouseScreenPosition;

    Player player;

    VertexArray background;
    /*Texture textureBackground;
    textureBackground.loadFromFile("graphics/background_sheet.png");*/
    Texture textureBackground = TextureHolder::GetTexture("graphics/background_sheet.png");

    int numZombies;
    int numZombiesAlive;
    Zombie* zombies = nullptr;

    // 100 bullets should do
    Bullet bullets[100];
    int currentBullet = 0;
    int bulletSpare = 24;
    int bulletsInClip = 6;
    int clipSize = 6;
    float fireRate = 1;
    // When was fire button last pressed
    Time lastPressed;

    IntRect arena;

    while (window.isOpen())
    {
        // Handle events
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type ==  Event::Closed)
            {
                window.close();
            }
            if (event.type == Event::KeyPressed)
            {
                if (event.key.code == Keyboard::Return
                    && state == State::PLAYING)
                {
                    state = State::PAUSED;
                }
                else if (event.key.code == Keyboard::Return
                    && state == State::PAUSED)
                {
                    state = State::PLAYING;
                    clock.restart();
                }
                else if (event.key.code == Keyboard::Return
                    && state == State::GAME_OVER)
                {
                    state = State::LEVELING_UP;
                }
                if (state == State::PLAYING)
                {
                    // Reload
                    if (event.key.code == Keyboard::R)
                    {
                        if (bulletSpare >= clipSize)
                        {
                            bulletsInClip = clipSize;
                            bulletSpare -= clipSize;
                        }
                        else if (bulletSpare > 0)
                        {
                            bulletsInClip = bulletSpare;
                            bulletSpare = 0;
                        }
                        else
                        {
                            // More soon
                        }
                    }
                }
            } 
        } // End event plotting

        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
        }

        // Handle player input
        if (state == State::PLAYING)
        {
            if (Keyboard::isKeyPressed(Keyboard::W))
            {
                player.moveUp();
            }
            else
            {
                player.stopUp();
            }
            if (Keyboard::isKeyPressed(Keyboard::S))
            {
                player.moveDown();
            }
            else
            {
                player.stopDown();
            }
            if (Keyboard::isKeyPressed(Keyboard::A))
            {
                player.moveLeft();
            }
            else
            {
                player.stopLeft();
            }
            if (Keyboard::isKeyPressed(Keyboard::D))
            {
                player.moveRight();
            }
            else
            {
                player.stopRight();
            }

            // Fire bullets
            if (Mouse::isButtonPressed(sf::Mouse::Left))
            {
                if (gameTimeTotal.asMilliseconds() - lastPressed.asMilliseconds() > 1000 / fireRate && bulletsInClip > 0)
                {
                    bullets[currentBullet].shoot(
                        player.getCenter(), mouseWordlPosition
                    );

                    currentBullet++;
                    if (currentBullet > 99)
                    {
                        currentBullet = 0;
                    }
                    lastPressed = gameTimeTotal;

                    bulletsInClip--;
                }
            } // End fire bullet
        } // End WASD playing

        // Handle Leveling up
        if (state == State::LEVELING_UP)
        {
            if (event.key.code == Keyboard::Num1)
            {
                state = State::PLAYING;
            }
            if (event.key.code == Keyboard::Num2)
            {
                state = State::PLAYING;
            }
            if (event.key.code == Keyboard::Num3)
            {
                state = State::PLAYING;
            }
            if (event.key.code == Keyboard::Num4)
            {
                state = State::PLAYING;
            }
            if (event.key.code == Keyboard::Num5)
            {
                state = State::PLAYING;
            }
            if (event.key.code == Keyboard::Num6)
            {
                state = State::PLAYING;
            }
            if (state == State::PLAYING)
            {
                // Prepare the level
                // tempt 2 lines
                arena.width = 500;
                arena.height = 500;
                arena.left = 0;
                arena.top = 0;

                int tileSize = createBackground(background, arena);
                
                //temp lines
                player.spawn(arena, resolution, tileSize);

                // Create horde of zobies
                numZombies = 10;

                delete[] zombies;

                zombies = createHorde(numZombies, arena);
                numZombiesAlive = numZombies;

                clock.restart();
            }
        } // End leveling up

        /*
        *********************
                UPDATE 
        **********************
        */
        if (state == State::PLAYING)
        {
            Time dt = clock.restart(); 
            gameTimeTotal += dt;

            float dtAsSeconds = dt.asSeconds();

            // Mouse pos
            mouseScreenPosition = Mouse::getPosition();
            mouseWordlPosition = window.mapPixelToCoords(
                Mouse::getPosition(), mainView);

            player.update(dtAsSeconds, Mouse::getPosition()); // mouseScreenPosition...
            
            // Note player new position
            Vector2f playerPosition(player.getCenter());

            mainView.setCenter(player.getCenter());

            for (int i = 0; i < numZombies; i++)
            {
                if (zombies[i].isAlive())
                {
                    zombies[i].update(dt.asSeconds(), playerPosition);
                }
            }
            for (int i = 0; i < 100; i++)
            {
                if (bullets[i].isInFlight())
                {
                    bullets[i].update(dtAsSeconds);
                }
            }
        } // End updating the scene

        /*
        ========================
            Drawing the scene
        ========================
        */

        if (state == State::PLAYING)
        {
            window.clear();
            // Set mainView as displayed
            window.setView(mainView);

            // Draw the player
            window.draw(background, &textureBackground);

            for (int i = 0; i < numZombies; i++)
            {
                window.draw(zombies[i].getSprite());
            }
            for (int i = 0; i < 100; i++)
            {
                if (bullets[i].isInFlight())
                {
                    window.draw(bullets[i].getShape());
                }
            }

            window.draw(player.getSprite());
        }
        if (state == State::LEVELING_UP)
        {

        }
        if (state == State::PAUSED)
        {

        }
        if (state == State::GAME_OVER)
        {

        }
        
        window.display();


    } // End game loop

    delete[] zombies;
    return 0;
}