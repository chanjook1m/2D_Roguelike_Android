#include "MainMenu.hpp"

#include "GamePlay.hpp"

#include "Inputbox.h"
#include "../net/client_interface.hpp"

#include "sha256.h"

#include <SFML/Window/Event.hpp>

#include <android/log.h>

#include <jni.h>
#include <android/native_activity.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

// Since we want to get the native activity from SFML, we'll have to use an
// extra header here:
#include <SFML/System/NativeActivity.hpp>


static net::AsyncTCPClient client;
int player_id = 0;

sf::Font font;

//static ChatBox chatBox(sf::Vector2f(50, 100), 300, 5, 20, 15, font);
InputBox* inputBox1;// (sf::Vector2f(500, 500), 120, 50, 40, 15, font, 0);
InputBox* inputBox2;// (sf::Vector2f(500, 600), 120, 50, 40, 15, font, 1);

sf::RectangleShape button;
bool isSelected = false;
sf::String s;
sf::Text text;
sf::Text loginResultMessage;
sf::Text pos;


#define LOCAL_ADDR "192.168.56.1" //"10.16.133.225" //"192.168.0.4"
#define LOCAL_PORT 5557


MainMenu::MainMenu(std::shared_ptr<Context>& context)
	: m_context(context), m_isPlayButtonSelected(true),
	m_isPlayButtonPressed(false), m_isExitButtonSelected(false),
	m_isExitButtonPressed(false)
{
    inputBox1 = new InputBox(sf::Vector2f(m_context->m_window->getSize().x / 2 - 50, m_context->m_window->getSize().y / 2 - 25.f),
                120.0f, 50.0f, 40, 15, font, 0);
    inputBox2 = new InputBox(sf::Vector2f(m_context->m_window->getSize().x / 2 - 50, m_context->m_window->getSize().y / 2 + 75.f),
        120.0f, 50.0f, 40, 15, font, 1);

    int width = m_context->m_window->getSize().x;
    int height = m_context->m_window->getSize().y;
    __android_log_print(ANDROID_LOG_INFO, "OBJECT_UPDATE", "window size: %d : %d", width, height);
    if (width == 768 && height == 1280)
    {
        button.setSize(sf::Vector2f(120,50));
            button.setScale(1, 1);
            button.setPosition(m_context->m_window->getSize().x / 2 - 50, m_context->m_window->getSize().y / 2 + 175.f);

        text.setFont(font);
         text.setString(L"로그인");
         text.setPosition(button.getPosition().x + 40, button.getPosition().y);
         text.setScale(1, 1);
         text.setFillColor(sf::Color::Black);

          loginResultMessage.setFont(font);
             loginResultMessage.setFillColor(sf::Color::White);
             loginResultMessage.setScale(1, 1);
             loginResultMessage.setPosition(button.getPosition().x, button.getPosition().y + 100);
    }
    else if (width == 1000 && height == 800)
    {
        inputBox1->setBoxSize(2, 1);
        inputBox2->setBoxSize(2, 1);

        inputBox1->setBoxPosition(inputBox1->getBoxPosition().x - 50,
            inputBox1->getBoxPosition().y);
        inputBox2->setBoxPosition(inputBox2->getBoxPosition().x - 50,
            inputBox2->getBoxPosition().y);

        inputBox1->setTextSize(3, 1);
        inputBox2->setTextSize(3, 1);

        inputBox1->setTextPosition(inputBox1->getTextPosition().x - 60,
            inputBox1->getTextPosition().y);
        inputBox2->setTextPosition(inputBox2->getTextPosition().x - 60,
             inputBox2->getTextPosition().y);

         button.setSize(sf::Vector2f(120,50));
         button.setScale(2, 1);
         button.setPosition(m_context->m_window->getSize().x / 2 - 100, m_context->m_window->getSize().y / 2 + 175.f);

         text.setFont(font);
         text.setString(L"로그인");
         text.setPosition(button.getPosition().x + 40, button.getPosition().y);
         text.setScale(3, 1);
         text.setFillColor(sf::Color::Black);

          loginResultMessage.setFont(font);
             loginResultMessage.setFillColor(sf::Color::White);
             loginResultMessage.setScale(3, 1);
             loginResultMessage.setPosition(button.getPosition().x, button.getPosition().y + 100);
    }



}

MainMenu::~MainMenu()
{

}

void MainMenu::Init()
{


	// add font

    if (!font.loadFromFile("godoMaum.ttf"))
    {
        std::cout << "Error" << std::endl;
    }





    pos.setFont(font);
        pos.setFillColor(sf::Color::Red);
        pos.setPosition(button.getPosition().x, button.getPosition().y + 50);
        pos.setString("A");


    // Title
    m_gameTitle.setFont(font);
    m_gameTitle.setScale(2, 2);
    m_gameTitle.setString("Roguelike Game");

    m_gameTitle.setOrigin(m_gameTitle.getLocalBounds().width / 2,
        m_gameTitle.getLocalBounds().height / 2);
    m_gameTitle.setPosition(m_context->m_window->getSize().x / 2,
        m_context->m_window->getSize().y / 2 - 150.f);


}

void MainMenu::ProcessInput()
{
    sf::Event event;
    while (m_context->m_window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            m_context->m_window->close();
        }
        else if (event.type == sf::Event::KeyPressed)
        {
            switch (event.key.code)
            {
            case sf::Keyboard::Up:
            {
                if (!m_isPlayButtonSelected)
                {
                    m_isPlayButtonSelected = true;
                    m_isExitButtonSelected = false;
                }
                break;
            }
            case sf::Keyboard::Down:
            {
                if (!m_isExitButtonSelected)
                {
                    m_isPlayButtonSelected = false;
                    m_isExitButtonSelected = true;
                }
                break;
            }
            case sf::Keyboard::Return:
            {
                m_isPlayButtonPressed = false;
                m_isExitButtonPressed = false;

                if (m_isPlayButtonSelected)
                {
                    m_isPlayButtonPressed = true;
                }
                else
                {
                    m_isExitButtonPressed = true;
                }

                break;
            }
            default:
            {


                break;
            }
            }
        }
        else if (event.type == sf::Event::MouseButtonReleased)
        {
            if (event.key.code == sf::Mouse::Left)
            {
                if (sf::Mouse::getPosition(*(m_context->m_window)).x >= button.getPosition().x
                    && sf::Mouse::getPosition(*(m_context->m_window)).x <= button.getPosition().x + button.getSize().x
                    && sf::Mouse::getPosition(*(m_context->m_window)).y >= button.getPosition().y
                    && sf::Mouse::getPosition(*(m_context->m_window)).y <= button.getPosition().y + button.getSize().y)
                    // The box has been selected
                    // Toggle the boolean
                {
                    isSelected = !isSelected;

                    std::string newStr1 = inputBox1->s.toAnsiString();
                    std::string newStr2 = inputBox2->s.toAnsiString();
                    std::string newStr = newStr1 + ";" + sha256(newStr2);

                    std::cout << newStr << std::endl;

                    m_context->m_window->clear(sf::Color::Black);
                    loginResultMessage.setString(L"로그인 중...");
                    m_context->m_window->draw(loginResultMessage);
                    m_context->m_window->display();

                    client.WriteOperation(0, LOCAL_ADDR, 5556, net::handler, player_id, newStr, 2);

                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    std::cout << net::id << std::endl;

                    if (net::connected)
                    {
                        client.WriteOperation(5, LOCAL_ADDR, 5555, net::handler, player_id, newStr, 0);

                        std::this_thread::sleep_for(std::chrono::seconds(3));
                    }



                    loginResultMessage.setString(net::connected ? L"로그인 성공" : L"로그인 실패");
                }
            }
        }
        else if (event.type == sf::Event::TouchBegan)
        	{
        	    if (event.touch.finger == 0)
                {
                    int width = m_context->m_window->getSize().x;
                    int height = m_context->m_window->getSize().y;
                    int posX;
                    int posY;
                    int dx = 0;
                    int dy = 0;


                    if (width == 768 && height == 1280)
                    {

                        posX = event.touch.x + 100;
                        posY = event.touch.y - 350;
                    }
                    else if (width == 1080 && height == 1776)
                    {
                        dy = 60;

                        posX = event.touch.x - 50;
                        posY = event.touch.y - 750;

                    }

                   //image.setPosition(posX - dx, posY + dy);
                   pos.setPosition(posX, posY);
                   isSelected = false;
                   if (posX >= button.getPosition().x
                                && posX <= button.getPosition().x + button.getSize().x
                                && posY >= button.getPosition().y
                                && posY <= button.getPosition().y + button.getSize().y)
                                // The box has been selected
                                // Toggle the boolean
                            {
                                isSelected = !isSelected;
                                std::string newStr1 = inputBox1->s.toAnsiString();
                                std::string newStr2 = inputBox2->s.toAnsiString();
                                std::string newStr = newStr1 + ";" + sha256(newStr2);

                                std::cout << newStr << std::endl;

                                 m_context->m_window->clear(sf::Color::Black);
                                 loginResultMessage.setString(L"로그인 중...");
                                 m_context->m_window->draw(loginResultMessage);
                                 m_context->m_window->display();

                                client.WriteOperation(0, LOCAL_ADDR, 5556, net::handler, player_id, newStr, 2);

                                std::this_thread::sleep_for(std::chrono::seconds(1));
                                std::cout << net::id << std::endl;

                                if (net::connected)
                                {
                                    client.WriteOperation(5, LOCAL_ADDR, 5555, net::handler, player_id, newStr, 0);

                                    std::this_thread::sleep_for(std::chrono::seconds(3));
                                }



                                loginResultMessage.setString(net::connected ? L"로그인 성공" : L"로그인 실패");


                                if (loginResultMessage.getString() == L"로그인")
                                {
                                 // First we'll need the native activity handle
                                    ANativeActivity* activity = sf::getNativeActivity();

                                    // Retrieve the JVM and JNI environment
                                    JavaVM* vm = activity->vm;
                                    JNIEnv* env = activity->env;

                                vm->AttachCurrentThread(&env, nullptr);
                                //jclass cl = env->FindClass("Lcom/google/android/gms/auth/api/signin/GoogleSignInOptions");
                                jstring url_string = env->NewStringUTF("https://accounts.google.com/o/oauth2/v2/auth/oauthchooseaccount?scope=https%3A%2F%2Fwww.googleapis.com%2Fauth%2Fdrive.metadata.readonly&state=state_parameter_passthrough_value&redirect_uri=https%3A%2F%2Foauth2.example.com%2Fcode&access_type=offline&response_type=code&client_id=583306224539-atbcaa8ne8g85e8kc006o6vmq99qiid0.apps.googleusercontent.com&flowName=GeneralOAuthFlow");
                                jclass uri_class = env->FindClass("android/net/Uri");
                                jmethodID uri_parse = env->GetStaticMethodID(uri_class, "parse", "(Ljava/lang/String;)Landroid/net/Uri;");
                                jobject uri = env->CallStaticObjectMethod(uri_class, uri_parse, url_string);
                                jclass intent_class = env->FindClass("android/content/Intent");
                                jfieldID action_view_id = env->GetStaticFieldID(intent_class, "ACTION_VIEW", "Ljava/lang/String;");
                                jobject action_view = env->GetStaticObjectField(intent_class, action_view_id);
                                jmethodID new_intent = env->GetMethodID(intent_class, "<init>", "(Ljava/lang/String;Landroid/net/Uri;)V");
                                jobject intent = env->AllocObject(intent_class);
                                env->CallVoidMethod(intent, new_intent, action_view, uri);
                                jclass activity_class = env->FindClass("android/app/Activity");
                                jmethodID start_activity = env->GetMethodID(activity_class, "startActivity", "(Landroid/content/Intent;)V");
                                env->CallVoidMethod(activity->clazz, start_activity, intent);
                                vm->DetachCurrentThread();
                                }
                            }

                }
            }
        //else if (event.type == sf::Event::TextEntered)
        //{
        //    char code = static_cast<char>(event.text.unicode);

        //    if (code != '\b')
        //        s += event.text.unicode;//buffer.push_back(code);
        //    else if (code == '\b')
        //    {
        //        /*if (buffer.size() > 0)
        //            buffer.pop_back();*/
        //        if (s.getSize() > 0) {
        //            //std::cout << "���ڿ����� : " << s.getSize() << std::endl;
        //            s.erase(s.getSize() - 1); //���������ڸ� �����.
        //        }
        //    }
        //}

        //std::cout << isSelected << std::endl;
        /*chatBox.handleEvent(event);
        std::string toBePushed;*/
        inputBox1->handleEvent(event, *m_context->m_window);
        inputBox2->handleEvent(event, *m_context->m_window);
    }
}

void MainMenu::Update(sf::Time deltaTime)
{
    if (m_isPlayButtonSelected)
    {
        m_playButton.setFillColor(sf::Color::Black);
        m_exitButton.setFillColor(sf::Color::White);
    }
    else
    {
        m_exitButton.setFillColor(sf::Color::Black);
        m_playButton.setFillColor(sf::Color::White);
    }

    if (net::connected)
    {
        //std::cout << "connected" << std::endl;
        m_context->m_states->Add(std::make_unique<GamePlay>(m_context), true);
    }
    else
    {
        //std::cout << "not connected" << std::endl;
    }
    /*else if (m_isExitButtonPressed)
    {
        m_context->m_window->close();
    }*/
}

void MainMenu::Draw()
{

    /*chatBox.setFillColor(sf::Color::Cyan);
    chatBox.setOutlineColor(sf::Color::Black);
    chatBox.setCharColor(sf::Color::Black);*/

    //m_context->m_window->clear(sf::Color::Red);
    //chatBox.update();
    //chatBox.draw(*(m_context->m_window));
    m_context->m_window->clear(sf::Color::Black);

    inputBox1->update();
    inputBox1->draw(*(m_context->m_window));

    inputBox2->update();
    inputBox2->draw(*(m_context->m_window));



    m_context->m_window->draw(m_gameTitle);
    m_context->m_window->draw(m_playButton);
    m_context->m_window->draw(m_exitButton);

    m_context->m_window->draw(button);
    m_context->m_window->draw(text);
    m_context->m_window->draw(loginResultMessage);
    //m_context->m_window->draw(pos);
    //m_context->m_window->draw(image);


    m_context->m_window->display();
}
