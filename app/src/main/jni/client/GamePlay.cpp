#include "GamePlay.hpp"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <stdlib.h>
#include <time.h>

#define LOCAL_ADDR "192.168.56.1" //"10.16.133.225" //"192.168.0.4"
#define LOCAL_PORT 5555

enum types
{
    COIN = 1,
    POWERUP,
};
bool update = false;
Player player1(24, 32);
int counter;
int counter2;
static int request_id = 1;
static int chat_request_id = 1;

sf::View view(sf::FloatRect(200, 200, 300, 200));

std::vector <Player> enemies;
vector<Player>::const_iterator playerIter;
bool enemyUpdate = false;
Player enem(24, 32);
Enemy enemy(0, 0, 48, 48);

vector<Projectile>::const_iterator projectileIter;
vector<Projectile> projectileArr;
Projectile projectile;

vector<Enemy>::const_iterator enemyIter;
vector<Enemy> enemyArr;

vector<Item>::const_iterator itemIter;
vector<Item> itemArr;
Item item(10, 10, 200, 150, COIN);

vector<Wall>::const_iterator wallIter;
vector<Wall> wallArr;
Wall wall(0, 32, 18, 18);

vector<IngameText>::const_iterator ingameTextIter;
vector<IngameText> ingameTextArr;

IngameText ingameText;

sf::Font maumFont;

sf::Sprite energyBallSprite;
//AnimatedGIF gif(("energyball.gif").c_str());

sf::Text hpText("Money: ", maumFont, 50);
sf::Text scoreText("Money: ", maumFont, 50);
sf::Text powerUpText("Power Level: ", maumFont, 50);


sf::Clock projectileClock;
sf::Clock playerCollisionClock;
sf::Clock aggroClock;

static net::AsyncTCPClient client;

static ChatBox *chatBox;

int scaleX = 1;
int scaleY = 1;
int wallScaleX = 3;
int wallScaleY = 3;

sf::Text poss;

sf::RectangleShape chatButton;
sf::Text chatText;

bool active = true;

GamePlay::GamePlay(std::shared_ptr<Context>& context)
    : m_context(context)
{
    srand(time(nullptr));
    chatBox = new ChatBox(sf::Vector2f(50, 100), context->m_window->getSize().x / 3, 5, 20, 15, maumFont);
    chatBox->setFillColor(sf::Color::Transparent);
    chatBox->setOutlineColor(sf::Color::White);
    chatBox->setCharColor(sf::Color::White);




    int dx = 0;
    int dy = 0;
    int width = context->m_window->getSize().x;
    int height = context->m_window->getSize().y;
    //__android_log_print(ANDROID_LOG_INFO, "OBJECT_UPDATE", "window size: %d : %d", width, height);
    if (width == 768 && height == 1280)
    {
        dy = 120;

                chatBox->setBox1Size(1.5,1.5);
                chatBox->setBox23Size(1.5,1.5);
                chatBox->setTextSize(2,2);

    }
    else if (width == 1080 && height == 1776)
    {
        dy = 120;

        chatBox->setBox1Size(2,2);
        chatBox->setBox23Size(2,2);
        chatBox->setTextSize(4,4);
        //chatBox->setTextPosition(chatBox->text.getPosition().x, chatBox->text.getPosition().y - dy);

    }



    /*boost::asio::io_context tcp_io_context;
    std::unique_ptr<std::thread> th(new std::thread([&]()
        {
            net::receiver2 r(tcp_io_context,
                LOCAL_ADDR,
                5557);
            tcp_io_context.run();
        }));*/
}

GamePlay::~GamePlay()
{
}

void GamePlay::Init()
{


    bool done = false;





    maumFont.loadFromFile("godoMaum.ttf");

    poss.setFont(maumFont);
                poss.setFillColor(sf::Color::Red);

                poss.setString("A");


    playerTexture.loadFromFile("rpg_sprite_walk.png");



    player1.text.setFont(maumFont);
    player1.text.setPosition(295, 370);
    player1.isMainPlayer = true;
    player1.sprite.setTexture(playerTexture);

    // client
    static int request_id = 1;




    //client.WriteOperation(5, LOCAL_ADDR, 5555, net::handler, player1.id, "", 0);

    //std::this_thread::sleep_for(std::chrono::seconds(5));
    std::cout << "[INFO] connection status: " << (net::connected ? "connected" : "not connected") << std::endl;
    if (net::connected)
    {
        player1.id = net::id;
    }


    std::this_thread::sleep_for(std::chrono::milliseconds(5));



    //client.WriteOperation(5, LOCAL_ADDR, 5555, net::handler, player1.id, 1);
    //





    view.setSize(sf::Vector2f(m_context->m_window->getSize().x, m_context->m_window->getSize().y));
    view.setCenter(sf::Vector2f(view.getSize().x / 2, view.getSize().y / 2));
    m_context->m_window->setView(view);

    //if (STRESS_TEST == true)
        //m_context->m_window->setVisible(false);


    // virtual controller
        left.setSize(sf::Vector2f(50, 50));
        left.setPosition(player1.collisionRect.getPosition().x - 270, player1.collisionRect.getPosition().y + 450);
        left.setFillColor(sf::Color(255, 0, 0));
        left.setOutlineThickness(10);
        left.setOutlineColor(sf::Color(153, 0, 0));

        right.setSize(sf::Vector2f(50, 50));
        right.setPosition(player1.collisionRect.getPosition().x - 130, player1.collisionRect.getPosition().y + 450);
        right.setFillColor(sf::Color(255, 0, 0));
        right.setOutlineThickness(10);
        right.setOutlineColor(sf::Color(153, 0, 0));

        up.setSize(sf::Vector2f(50, 50));
        up.setPosition(player1.collisionRect.getPosition().x - 200, player1.collisionRect.getPosition().y + 380);
        up.setFillColor(sf::Color(255, 0, 0));
        up.setOutlineThickness(10);
        up.setOutlineColor(sf::Color(153, 0, 0));

        down.setSize(sf::Vector2f(50, 50));
        down.setPosition(player1.collisionRect.getPosition().x - 200, player1.collisionRect.getPosition().y + 520);
        down.setFillColor(sf::Color(255, 0, 0));
        down.setOutlineThickness(10);
        down.setOutlineColor(sf::Color(153, 0, 0));

        button1.setRadius(50);
        button1.setPosition(player1.collisionRect.getPosition().x + 200, player1.collisionRect.getPosition().y + 430);
        button1.setFillColor(sf::Color(0, 0, 255));
        button1.setOutlineThickness(10);
        button1.setOutlineColor(sf::Color(0, 0, 153));

        //


    enemyTexture.loadFromFile("goblin.png");


    coinTexture.loadFromFile("coin.png");



    powerUpTexture.loadFromFile("powerup.png");



    wallTexture.loadFromFile("wall.png");



    sf::Sprite energyBallSprite;
    energyBallSprite.setScale(sf::Vector2f(0.1, 0.1));





    // objects and objects array



    projectileTexture.loadFromFile("projectile.png");

    projectile.sprite.setTexture(projectileTexture);




    enemy.sprite.setTexture(enemyTexture);
    enemy.text.setFont(maumFont);
    enemy.text.setFillColor(sf::Color::Red);




    item.sprite.setTexture(coinTexture);
    item.sprite.setScale(sf::Vector2f(0.2, 0.2));



    wall.sprite.setTextureRect(sf::IntRect(0, 0, 18, 18));
    wall.sprite.setTexture(wallTexture);
    wall.sprite.setScale(3.0, 3.0);

    //// create room
    //int roomSize = 10;
    //int bossRoomSize = 15;
    //int verticalDoorAt = 2;
    //int horizontalDoorAt = 2;
    //int initialRoomX = 200;
    //int initialRoomY = 300;

    //// generate shop item
    //item = Item(0, 0, 100, 100, POWERUP);

    Enemy boss(48 * 6, 0, 48, 48);
    boss.sprite.setTexture(enemyTexture);
    boss.text.setFont(maumFont);
    boss.text.setFillColor(sf::Color::Red);
    /*boss.maxHp = 100;
    boss.hp = 100;
    boss.attackDamage = 10;
    boss.collisionRect.setPosition(50 * bossRoomSize/2 + initialRoomX + (bossRoomSize * 50 * 2) + 100, (bossRoomSize/2 * 50) + 50 + initialRoomY);*/

    //enemyArr.push_back(boss);


    ingameText.text.setPosition(200, 200);
    ingameText.text.setFont(maumFont);
    ingameText.text.setScale(scaleX, scaleY);
    ingameTextArr.push_back(ingameText);


    hpText.setPosition(50, 50);
    hpText.setScale(scaleX, scaleY);

    scoreText.setPosition(50, 50);


    powerUpText.setPosition(50, 100);



    projectileClockElapsed = projectileClock.getElapsedTime();
    playerCollisionClockElapsed = playerCollisionClock.getElapsedTime();
    aggroClockElapsed = aggroClock.getElapsedTime();




    enem.text.setFont(maumFont);

    int width = m_context->m_window->getSize().x;
        int height = m_context->m_window->getSize().y;
        __android_log_print(ANDROID_LOG_INFO, "OBJECT_UPDATE", "window size: %d : %d", width, height);
        if (width == 768 && height == 1280)
        {
        }
        else if (width == 1080 && height == 1776)
        {
            scaleX = 2;
            scaleY = 2;
            wallScaleX = 2;
            wallScaleY = 2;
            player1.text.setScale(scaleX,scaleY);
            player1.text.setPosition(player1.text.getPosition().x - 20, player1.text.getPosition().y - 20);
            //player1.sprite.setScale(scaleX,scaleY);
            //projectile.sprite.setScale(scaleX,scaleY);
            enemy.sprite.setScale(scaleX,scaleY);
            //enemy.text.setScale(scaleX,scaleY);

            boss.sprite.setScale(scaleX,scaleY);
            ingameText.text.setScale(scaleX,scaleY);

            //enem.text.setScale(2,2);
        }
        else
        {
            __android_log_print(ANDROID_LOG_INFO, "OBJECT_UPDATE", "wwindow size: %d : %d", width, height);
        }

        chatButton.setSize(sf::Vector2f(120,50));
            chatButton.setScale(1, 1);
            chatButton.setFillColor(sf::Color::White);

            chatText.setFont(maumFont);
             chatText.setString(L"채팅");

             chatText.setScale(scaleX, scaleY);
             chatText.setFillColor(sf::Color::Black);


    //sf::Keyboard::setVirtualKeyboardVisible(true);
}

void GamePlay::ProcessInput()
{
    sf::Event event;
    projectileClockElapsed = projectileClock.getElapsedTime();
    playerCollisionClockElapsed = playerCollisionClock.getElapsedTime();
    aggroClockElapsed = aggroClock.getElapsedTime();
    while (active ? m_context->m_window->pollEvent(event) : m_context->m_window->waitEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            m_context->m_window->close();
            break;
        case sf::Event::KeyPressed:
            if (event.key.code == sf::Keyboard::Escape)
                m_context->m_window->close();
            break;
        case sf::Event::Resized:
            view.setSize(event.size.width, event.size.height);
            view.setCenter(event.size.width / 2, event.size.height / 2);
            m_context->m_window->setView(view);
            break;
        case sf::Event::LostFocus:
            //background = sf::Color::Black;
            break;
        case sf::Event::GainedFocus:
            //background = sf::Color::White;

            break;

            // On Android MouseLeft/MouseEntered are (for now) triggered,
            // whenever the app loses or gains focus.
        case sf::Event::MouseLeft:
            active = false;
            break;
        case sf::Event::MouseEntered:
            active = true;
            break;
        case sf::Event::TouchBegan:
        //case sf::Event::TouchMoved:
            if (event.touch.finger == 0)
            {
                int width = m_context->m_window->getSize().x;
                int height = m_context->m_window->getSize().y;
                int posX;
                int posY;
                int dx;
                int dy;


                if (width == 768 && height == 1280)
                {

                    posX = event.touch.x - 100;
                    posY = event.touch.y - 260;
                }
                else if (width == 1080 && height == 1776)
                {
                    dy = 60;

                    posX = event.touch.x - 180;
                    posY = event.touch.y - 470;

                }
                poss.setPosition(posX, posY);

                //image.setPosition(posX, posY);


                if (posX >= chatButton.getPosition().x
                                                && posX <= chatButton.getPosition().x + chatButton.getSize().x
                                                && posY >= chatButton.getPosition().y
                                                && posY <= chatButton.getPosition().y + chatButton.getSize().y)
                                                // The box has been selected
                                                // Toggle the boolean
                {
                    chatBox->isVisible = !chatBox->isVisible;
                }


                if (left.getGlobalBounds().contains(sf::Vector2f(posX, posY)))
                {
                    player1.virtualKeyPressed = 3;
                    //__android_log_print(ANDROID_LOG_INFO, "CONNECTION_TEST", "clicked");
                    player1.sprite.setPosition(posX, posY);

                }
                else if (right.getGlobalBounds().contains(sf::Vector2f(posX, posY)))
                {
                    player1.virtualKeyPressed = 4;
                    //__android_log_print(ANDROID_LOG_INFO, "CONNECTION_TEST", "clicked");
                }
                else if (up.getGlobalBounds().contains(sf::Vector2f(posX, posY)))
                {
                    player1.virtualKeyPressed = 1;
                    //__android_log_print(ANDROID_LOG_INFO, "CONNECTION_TEST", "clicked");
                }
                else if (down.getGlobalBounds().contains(sf::Vector2f(posX, posY)))
                {
                    player1.virtualKeyPressed = 2;
                    //__android_log_print(ANDROID_LOG_INFO, "CONNECTION_TEST", "clicked");
                }
                else
                {
                    //player1.virtualKeyPressed = 0;
                }
                player1.direction = player1.virtualKeyPressed;
                update = true;

                if (button1.getGlobalBounds().contains(sf::Vector2f(posX, posY)))
                {
                    //player1.sprite.setPosition(posX, posY);
                    //__android_log_print(ANDROID_LOG_INFO, "CONNECTION_TEST", "clicked");
                    //update = false;
                    //player1.virtualKeyPressed = 0;
                    counter = 0;
                    if (projectileClockElapsed.asSeconds() >= 1.0)
                    {
                        projectileClock.restart();

                        //client.WriteOperation(5, LOCAL_ADDR, LOCAL_PORT, net::handler, player1.id, 2);
                        std::string newStr = "";
                        client.WriteOperation(player1.direction, LOCAL_ADDR, 5555, net::handler, player1.id, newStr, 4);


                    }
                }
            }
            break;
        }

        chatBox->handleEvent(event, *(m_context->m_window), player1.id);
    }
}

void GamePlay::Update(sf::Time deltaTime)
{
    client.ReadOperation(10, LOCAL_ADDR, 5555, net::handler, request_id++);
    sf::String ss;

    std::string newStr = "";
    client.WriteOperation(5, LOCAL_ADDR, 5555, net::handler, player1.id, newStr, 1);

    //if (generateRandom(10) == 1)
        //client.ReadChatOperation(10, LOCAL_ADDR, 5557, net::handler, chat_request_id++);

    std::this_thread::sleep_for(std::chrono::milliseconds(33));


    if (net::chat.length())
    {
        chatBox->push(net::chat);
        net::chat.clear();
    }

    std::cout << "[INFO] PLAYER ID : " << player1.id << std::endl;

    // update players
    for (int i = 0; i < net::players.size(); i++)
    {
        //std::cout << "ID2 : " << net::players[i].id << std::endl;
        if (net::players[i].id == player1.id)
        {
            player1.isAlive = net::players[i].isAlive;
            player1.hp = net::players[i].hp;
            player1.x = net::players[i].x;
            player1.y = net::players[i].y;
            player1.powerUpLevel = net::players[i].powerUpLevel;
            player1.direction = net::players[i].direction;
            player1.score = net::players[i].score;
            player1.canMoveUp = net::players[i].canMoveUp;
            player1.canMoveDown = net::players[i].canMoveDown;
            player1.canMoveLeft = net::players[i].canMoveLeft;
            player1.canMoveRight = net::players[i].canMoveRight;


            player1.collisionRect.setPosition(net::players[i].collisionRect_x, net::players[i].collisionRect_y);
            player1.sprite.setPosition(player1.collisionRect.getPosition());
            player1.sprite.setScale(scaleX, scaleY);
        }
        else
        {
            for (size_t j = 0; j < enemies.size(); j++)
            {
                //std::cout << "enem player id : " << enemies[j].id << std::endl;
                //std::cout << "[INFO] enem size : " << enemies.size() << std::endl;
                if (enemies[j].id == net::players[i].id)
                {
                    //std::cout << "enemy update - " << net::players[i].direction << std::endl;
                    enemies[j].isAlive = net::players[i].isAlive;
                    enemies[j].id = net::players[i].id;
                    enemies[j].hp = net::players[i].hp;
                    enemies[j].x = net::players[i].x;
                    enemies[j].y = net::players[i].y;
                    enemies[j].powerUpLevel = net::players[i].powerUpLevel;
                    enemies[j].direction = net::players[i].direction;
                    enemies[j].score = net::players[i].score;
                    enemies[j].canMoveUp = net::players[i].canMoveUp;
                    enemies[j].canMoveDown = net::players[i].canMoveDown;
                    enemies[j].canMoveLeft = net::players[i].canMoveLeft;
                    enemies[j].canMoveRight = net::players[i].canMoveRight;


                    enemies[j].collisionRect.setPosition(net::players[i].collisionRect_x, net::players[i].collisionRect_y);
                    enemyUpdate = true;
                }
            }


            if (enemyUpdate == false)
            {
                //std::cout << "enemyUpdate " << player.id << std::endl;
                enem.isAlive = net::players[i].isAlive;
                enem.id = net::players[i].id;
                enem.hp = net::players[i].hp;
                enem.x = net::players[i].x;
                enem.y = net::players[i].y;
                enem.powerUpLevel = net::players[i].powerUpLevel;
                enem.direction = net::players[i].direction;
                enem.score = net::players[i].score;
                enem.canMoveUp = net::players[i].canMoveUp;
                enem.canMoveDown = net::players[i].canMoveDown;
                enem.canMoveLeft = net::players[i].canMoveLeft;
                enem.canMoveRight = net::players[i].canMoveRight;


                enem.collisionRect.setPosition(net::players[i].collisionRect_x, net::players[i].collisionRect_y);
                enem.sprite.setTexture(playerTexture);
                enem.sprite.setScale(scaleX, scaleY);
                enemies.push_back(enem);

            }
            enemyUpdate = false;
        }
    }
    //

    // update projectiles

    int netProjectileSize = net::projectiles.size();
    int clientProjectileSize = projectileArr.size();
    int diff = netProjectileSize - clientProjectileSize;
    std::cout << "[INFO] projectile update: " << netProjectileSize << " : " << clientProjectileSize << std::endl;


    if (clientProjectileSize < netProjectileSize)
    {
        counter2 = netProjectileSize - diff;
        while (counter2 < netProjectileSize)
        {
            netProjectileSize = net::projectiles.size();
            if (counter2 >= netProjectileSize)
                break;
            projectile.id = net::projectiles[counter2].id;
            projectile.isAlive = net::projectiles[counter2].isAlive;
            projectile.collisionRect.setPosition(net::projectiles[counter2].collisionRect_x,
                net::projectiles[counter2].collisionRect_y);
            projectile.collisionRect.setSize(sf::Vector2f(net::projectiles[counter2].width, net::projectiles[counter2].height));
            projectile.direction = net::projectiles[counter2].direction;
            projectile.isCollide = net::projectiles[counter2].isCollide;
            projectile.sprite.setScale(scaleX, scaleY);
            projectileArr.push_back(projectile);
            counter2++;
        }
    }
    else
    {

        for (size_t i = 0; i < net::projectiles.size(); i++)
        {
            if (net::projectiles.size() > 0)
            {
                try {
                    projectileArr.at(i);
                    if (net::projectiles.size() > projectileArr.size())
                        break;
                    if (projectileArr[i].id == net::projectiles[i].id)
                    {
                        projectileArr[i].isAlive = net::projectiles[i].isAlive;
                        projectileArr[i].isCollide = net::projectiles[i].isCollide;
                        projectileArr[i].collisionRect.setPosition(net::projectiles[i].collisionRect_x,
                            net::projectiles[i].collisionRect_y);
                    }
                }
                catch (int e)
                {
                    std::cout << "[ERROR] projectile index error" << std::endl;
                }
            }
        }
    }


    //

    // update enemies
    int netEnemySize = net::enemies.size();
    int clientEnemySize = enemyArr.size();
    diff = netEnemySize - clientEnemySize;
    std::cout << "[INFO] enemy update: " << netEnemySize << " : " << clientEnemySize << std::endl;
    __android_log_print(ANDROID_LOG_INFO, "OBJECT_UPDATE", "projectile update: %d : %d", netEnemySize, clientEnemySize);
    if (clientEnemySize < netEnemySize)
    {
        counter2 = netEnemySize - diff;
        while (counter2 < netEnemySize)
        {

            enemy.id = net::enemies[counter2].id;
            enemy.isAlive = net::enemies[counter2].isAlive;
            enemy.collisionRect.setPosition(net::enemies[counter2].collisionRect_x,
                net::enemies[counter2].collisionRect_y);
            enemy.direction = net::enemies[counter2].direction;
            enemy.hp = net::enemies[counter2].hp;
            enemy.maxHp = net::enemies[counter2].maxHp;
            enemy.isCollide = net::enemies[counter2].isCollide;
            enemy.sprite.setScale(scaleX, scaleY);

            net::enemies[counter2].isBoss ? enemy.sprite.setTextureRect(sf::IntRect(48 * 6, 0, 48, 48))
                : enemy.sprite.setTextureRect(sf::IntRect(0, 0, 48, 48));
            enemyArr.push_back(enemy);
            counter2++;
        }
    }
    else //if (clientEnemySize == netEnemySize)
    {

        for (size_t i = 0; i < net::enemies.size(); i++)
        {
            if (net::enemies.size() > enemyArr.size())
                break;
            if (enemyArr[i].id == net::enemies[i].id)
            {
                enemyArr[i].hp = net::enemies[i].hp;
                enemyArr[i].isAlive = net::enemies[i].isAlive;
                enemyArr[i].isCollide = net::enemies[i].isCollide;
                enemyArr[i].collisionRect.setPosition(net::enemies[i].collisionRect_x,
                    net::enemies[i].collisionRect_y);
            }

        }

    }
    //
    // update items
    int netItemSize = net::items.size();
    int clientItemSize = itemArr.size();
    diff = netItemSize - clientItemSize;
    std::cout << "[INFO] item update: " << netItemSize << " : " << clientItemSize << std::endl;
    if (clientItemSize < netItemSize)
    {
        counter2 = netItemSize - diff;
        while (counter2 < netItemSize)
        {

            item.id = net::items[counter2].id;
            item.isAlive = net::items[counter2].isAlive;
            item.collisionRect.setPosition(net::items[counter2].collisionRect_x,
                net::items[counter2].collisionRect_y);
            item.scaleValue = net::items[counter2].scaleValue;
            item.inShop = net::items[counter2].inShop;
            item.cost = net::items[counter2].cost;
            item.type = net::items[counter2].type;
            item.isCollide = net::items[counter2].isCollide;
            item.animateSpriteNumber = net::items[counter2].animateSpriteNumber;

            item.spriteWidth = item.type == COIN ? 200 : 100;
            item.spriteHeight = item.type == COIN ? 150 : 100;
            item.y = item.type == COIN ? 10 : 0;
            item.sprite.setScale(sf::Vector2f(item.scaleValue * scaleX, item.scaleValue * scaleY));
            item.sprite.setTextureRect(item.type == COIN ?
                sf::IntRect(10, 10, 200, 150) : sf::IntRect(0, 0, 100, 100));

            item.sprite.setTexture(item.type == COIN ? coinTexture : powerUpTexture);
            if (item.inShop == true)
            {
                item.text.setFont(maumFont);
                item.text.setFillColor(sf::Color::Green);
                item.text.setString("Cost: " + to_string(item.cost));
            }
            itemArr.push_back(item);
            counter2++;
        }
    }
    else //if (clientItemSize == netItemSize)
    {
        for (size_t i = 0; i < net::items.size(); i++)
        {
            if (net::items.size() > itemArr.size())
                break;
            if (itemArr[i].id == net::items[i].id)
            {
                itemArr[i].isAlive = net::items[i].isAlive;
                itemArr[i].isCollide = net::items[i].isCollide;
                itemArr[i].collisionRect.setPosition(net::items[i].collisionRect_x,
                    net::items[i].collisionRect_y);
            }

        }
    }
    // update walls
    int netWallSize = net::walls.size();
    int clientWallSize = wallArr.size();
    diff = netWallSize - clientWallSize;
    std::cout << "[INFO] wall update: " << netWallSize << " : " << clientWallSize << std::endl;
    if (clientWallSize < netWallSize)
    {
        counter2 = netWallSize - diff;
        while (counter2 < netWallSize)
        {

            //enemy.id = net::projectiles[counter2].id;
            wall.id = net::walls[counter2].id;
            wall.isAlive = net::walls[counter2].isAlive;
            wall.hp = net::walls[counter2].hp;
            wall.collisionRect.setPosition(net::walls[counter2].collisionRect_x,
                net::walls[counter2].collisionRect_y);
            wall.destructible = net::walls[counter2].destructible;

            wall.sprite.setTextureRect(wall.destructible ? sf::IntRect(0, 0, 18, 18) : sf::IntRect(0, 32, 18, 18));
            wall.sprite.setScale(wallScaleX, wallScaleY);

            wall.isCollide = net::walls[counter2].isCollide;
            wallArr.push_back(wall);
            counter2++;
        }
    }
    else //if (clientWallSize == netWallSize)
    {
        for (size_t i = 0; i < net::walls.size(); i++)
        {
            if (net::walls.size() > wallArr.size())
                break;
            if (wallArr[i].id == net::walls[i].id)
            {
                wallArr[i].hp = net::walls[i].hp;
                wallArr[i].isAlive = net::walls[i].isAlive;
                wallArr[i].isCollide = net::walls[i].isCollide;
                wallArr[i].collisionRect.setPosition(net::walls[i].collisionRect_x,
                    net::walls[i].collisionRect_y);
            }

        }
    }

    if (STRESS_TEST == true)
    {
        player1.update();
        player1.direction = generateRandom(5);

        player1.virtualKeyPressed = player1.direction;
        std::string newStr = "";
        client.WriteOperation(player1.direction, LOCAL_ADDR, 5555, net::handler, player1.id, newStr, 4);
    }

    //
    sf::Time projectileClockElapsed = projectileClock.getElapsedTime();
    sf::Time playerCollisionClockElapsed = playerCollisionClock.getElapsedTime();
    sf::Time aggroClockElapsed = aggroClock.getElapsedTime();



    // delete not alive enemy
    counter = 0;
    for (enemyIter = enemyArr.begin(); enemyIter != enemyArr.end(); enemyIter++)
    {
        bool found = false;
        for (auto i : net::enemies)
        {
            if (enemyArr[counter].id == i.id)
            {
                found = true;
                break;
            }
        }
        if (found == false)
        {
            enemyArr.erase(enemyIter);
            break;
        }
        counter++;
    }

    // delete not alive projectile
    counter = 0;
    for (projectileIter = projectileArr.begin(); projectileIter != projectileArr.end(); projectileIter++)
    {
        bool found = false;
        for (auto i : net::projectiles)
        {
            if (projectileArr[counter].id == i.id)
            {
                found = true;
                break;
            }
        }
        if (found == false)
        {
            //std::cout << ">>>>>>>>>>> deleted" << std::endl;
            projectileArr.erase(projectileIter);
            break;
        }
        counter++;
    }

    // delete not alive ingameText
    counter = 0;
    for (ingameTextIter = ingameTextArr.begin(); ingameTextIter != ingameTextArr.end(); ingameTextIter++)
    {
        if (ingameTextArr[counter].isAlive == false)
        {
            ingameTextArr.erase(ingameTextIter);
            break;
        }
        counter++;
    }

    // delete not alive item
    counter = 0;
    for (itemIter = itemArr.begin(); itemIter != itemArr.end(); itemIter++)
    {
        bool found = false;
        for (auto i : net::items)
        {
            if (itemArr[counter].id == i.id)
            {
                found = true;
                break;
            }
        }
        if (found == false)
        {
            itemArr.erase(itemIter);
            break;
        }
        counter++;
    }

    // delete not alive wall
    counter = 0;
    for (wallIter = wallArr.begin(); wallIter != wallArr.end(); wallIter++)
    {
        bool found = false;
        for (auto i : net::walls)
        {
            if (wallArr[counter].id == i.id)
            {
                found = true;
                break;
            }
        }
        if (found == false)
        {
            wallArr.erase(wallIter);
            break;
        }
        counter++;
    }

    // delete not alive players
    counter = 0;
    for (playerIter = enemies.begin(); playerIter != enemies.end(); playerIter++)
    {
        bool found = false;
        for (auto i : net::players)
        {
            if (enemies[counter].id == i.id)
            {
                found = true;
                break;
            }
        }
        if (found == false)
        {
            enemies.erase(playerIter);
            break;
        }
        counter++;
    }


    // create projectile (space-Key)
    if (update)
    {
        counter = 0;
        if (projectileClockElapsed.asSeconds() >= 1.0 && sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            //shotSound.play();
            projectileClock.restart();
            //std::cout << "shooooooooooooooooooooooooooooooooooooooooooooooo" <<std::endl;
            std::string newStr = "";
            client.WriteOperation(5, LOCAL_ADDR, 5555, net::handler, player1.id, newStr, 4);
        }
    }

    // view player
    m_context->m_window->setView(view);
    view.setCenter(player1.collisionRect.getPosition());

}

void GamePlay::Draw()
{

    m_context->m_window->clear();

    int dx = 0;
    int dy = 0;
    int width = m_context->m_window->getSize().x;
    int height = m_context->m_window->getSize().y;
    __android_log_print(ANDROID_LOG_INFO, "OBJECT_UPDATE", "window size: %d : %d", width, height);
    if (width == 768 && height == 1280)
    {
        dy = 20;
    }
    else if (width == 1080 && height == 1776)
    {
        dx = 290;
        dy = 30;
    }


    // draw item
    counter = 0;
    for (itemIter = itemArr.begin(); itemIter != itemArr.end(); itemIter++)
    {
        if (itemArr[counter].inShop == true)
        {
            m_context->m_window->draw(itemArr[counter].text);
        }
        itemArr[counter].update();
        m_context->m_window->draw(itemArr[counter].sprite);
        counter++;
    }

    // draw projectile
    counter = 0;
    for (projectileIter = projectileArr.begin(); projectileIter != projectileArr.end(); projectileIter++)
    {
        projectileArr[counter].update();
        // draw eneryball
        //gif.update(projectileArr[counter].sprite);
        m_context->m_window->draw(projectileArr[counter].sprite);
        //window.draw(projectileArr[counter].collisionRect);
        counter++;
    }

    // draw enemy Monster
    counter = 0;
    for (enemyIter = enemyArr.begin(); enemyIter != enemyArr.end(); enemyIter++)
    {
        enemyArr[counter].text.setString("HP " + to_string(enemyArr[counter].hp) + "/" + to_string(enemyArr[counter].maxHp));
        enemyArr[counter].text.setPosition(enemyArr[counter].text.getPosition().x, enemyArr[counter].text.getPosition().y - 50);
        enemyArr[counter].text.setScale(scaleX, scaleY);
        m_context->m_window->draw(enemyArr[counter].text);
        m_context->m_window->draw(enemyArr[counter].sprite);
        enemyArr[counter].update();
        counter++;
    }

    // draw wall
    counter = 0;
    for (wallIter = wallArr.begin(); wallIter != wallArr.end(); wallIter++)
    {
        if (wallArr[counter].isAlive == true)
        {
            wallArr[counter].update();
            m_context->m_window->draw(wallArr[counter].sprite);
            //window.draw(wallArr[counter].collisionRect);
        }
        counter++;
    }

    // draw player , scoreText
    player1.text.setString("id : " + to_string(player1.id));
    m_context->m_window->draw(player1.text);
    m_context->m_window->draw(player1.sprite);

    for (size_t i = 0; i < enemies.size(); i++)
    {
        enemies[i].text.setString("id : " + to_string(enemies[i].id));
        enemies[i].update();
        m_context->m_window->draw(enemies[i].text);
        m_context->m_window->draw(enemies[i].sprite);
    }
    if (update)
    {
        player1.update();
        if (player1.updated == true)
        {
            std::string newStr = "";
            client.WriteOperation(player1.direction, LOCAL_ADDR, 5555, net::handler, player1.id, newStr, 4);
            std::cout << "0000000000 updated " << std::endl;

        }
    }


    scoreText.setString("Money: " + to_string(player1.score));
    scoreText.setScale(scaleX, scaleY);
    m_context->m_window->draw(scoreText);
    scoreText.setPosition(player1.collisionRect.getPosition().x - m_context->m_window->getSize().x / 2,
        player1.collisionRect.getPosition().y - m_context->m_window->getSize().y / 2 + 10 * scaleY);

    powerUpText.setString("Power Level: " + to_string(player1.powerUpLevel));
    powerUpText.setScale(scaleX, scaleY);
    m_context->m_window->draw(powerUpText);
    powerUpText.setPosition(player1.collisionRect.getPosition().x - m_context->m_window->getSize().x / 2,
        player1.collisionRect.getPosition().y - m_context->m_window->getSize().y / 2 + 50 * scaleY);

    hpText.setString("HP: " + to_string(player1.hp) + "/" + to_string(player1.maxHp));
    hpText.setScale(scaleX, scaleY);
    m_context->m_window->draw(hpText);
    hpText.setPosition(player1.collisionRect.getPosition().x - m_context->m_window->getSize().x / 2,
        player1.collisionRect.getPosition().y - m_context->m_window->getSize().y / 2 + 100 * scaleY);

    chatButton.setPosition(player1.collisionRect.getPosition().x - m_context->m_window->getSize().x / 2,
                  player1.collisionRect.getPosition().y - m_context->m_window->getSize().y / 2 + 170 * scaleY);

    chatText.setPosition(chatButton.getPosition().x + 40, chatButton.getPosition().y - 20 + (scaleY == 2 ? 0 : 40));

    chatBox->box1.setPosition(player1.collisionRect.getPosition().x - m_context->m_window->getSize().x / 2 + 40,
        player1.collisionRect.getPosition().y - m_context->m_window->getSize().y / 4);
    float x2 = chatBox->box1.getGlobalBounds().left + 5;
    float y2 = chatBox->box1.getGlobalBounds().top + chatBox->box1.getGlobalBounds().height + 3 * chatBox->thickness;
    chatBox->box2.setPosition(x2, y2);
    //chatBox->box3.setPosition(x2 + chatBox->box1.getSize().x - chatBox->box3.getSize().x, y2);
    chatBox->box3.setPosition(x2 + chatBox->box1.getSize().x - chatBox->box3.getSize().x + dx, y2);

    chatBox->box1.setOutlineThickness(chatBox->thickness);
    chatBox->box2.setOutlineThickness(chatBox->thickness);
    chatBox->box3.setOutlineThickness(chatBox->thickness);

    chatBox->buttonText.setPosition(x2 + +chatBox->box1.getSize().x - chatBox->box3.getSize().x + 10 + dx, y2 + chatBox->thickness - dy);
    chatBox->text.setPosition(x2 + chatBox->thickness, y2 + chatBox->thickness - dy);


    // draw ingameText
    counter = 0;
    for (ingameTextIter = ingameTextArr.begin(); ingameTextIter != ingameTextArr.end(); ingameTextIter++)
    {
        ingameTextArr[counter].update();
        m_context->m_window->draw(ingameTextArr[counter].text);
        counter++;
    }
    //

    // draw chatBox
    chatBox->draw(*(m_context->m_window));
    chatBox->update();


    m_context->m_window->draw(left);
                m_context->m_window->draw(right);
                m_context->m_window->draw(up);
                m_context->m_window->draw(down);
                m_context->m_window->draw(button1);

    //m_context->m_window->draw(poss);
    m_context->m_window->draw(chatButton);
    m_context->m_window->draw(chatText);


    m_context->m_window->display();
}
