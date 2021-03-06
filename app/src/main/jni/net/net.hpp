#pragma once
#include <iostream>
#include <string>

#include <boost/serialization/serialization.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/base_object.hpp>
#include <stdlib.h>
#include <boost/bind.hpp>

#include <cereal/cereal.hpp>
//#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/binary.hpp>



namespace net
{

	static bool generateRandomBool()
	{
		int randomNumber = rand();
		int random = (randomNumber % 2) + 1;

		if (random == 1)
			return true;
		else
			return false;
	}

	static int generateRandom(int max)
	{
		int randomNumber = rand();
		int random = (randomNumber % max) + 1;

		return random;
	}

	template <typename T1, typename T2>
	bool detectCollision(T1 a, T2 b)
	{
		if (a.collisionRect_x< b.collisionRect_x + 3 * b.width &&
			a.collisionRect_x + a.width > b.collisionRect_x &&
			a.collisionRect_y < b.collisionRect_y + 3 * b.height &&
			a.collisionRect_y + a.height > b.collisionRect_y) {
			return true;
		}
		else
		{
			return false;
		}
	}

	class Wall
	{
		//friend class boost::serialization::access;
		friend class cereal::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar(collisionRect_x, collisionRect_y, destructible, hp, width, height, isAlive, isCollide, id);
			/*ar& collisionRect_x;
			ar& collisionRect_y;
			ar& destructible;
			ar& hp;
			ar& width;
			ar& height;
			ar& isAlive;
			ar& isCollide;
			ar& id;*/
		}


	public:
		int id = 0;
		int collisionRect_x;
		int collisionRect_y;
		bool destructible = false;
		int hp = 3;
		int width = 18;
		int height = 18;
		bool isAlive = false;
		bool isCollide = false;
	};

	class Item
	{
		//friend class boost::serialization::access;
		friend class cereal::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar(animateSpriteNumber, delayCounter, animateDelay, type, scaleValue, inShop, cost,
				collisionRect_x, collisionRect_y, isAlive, isCollide, id);
			/*ar& animateSpriteNumber;
			ar& delayCounter;
			ar& animateDelay;
			ar& type;
			ar& scaleValue;
			ar& inShop;
			ar& cost;
			ar& collisionRect_x;
			ar& collisionRect_y;
			ar& isAlive;
			ar& isCollide;
			ar& id;*/
		}

		enum types
		{
			COIN = 1,
			POWERUP,
		};
	public:
		int id;
		bool isCollide = false;
		int animateSpriteNumber = 0;
		int delayCounter = 0;
		int animateDelay = 5;
		int type = 0;
		float scaleValue = 0.f;
		bool inShop = false;
		int cost = 0;
		int collisionRect_x;
		int collisionRect_y;
		bool isAlive = false;
		int width = 10;
		int height = 10;

		void update()
		{
			animate();
		}

		void animate()
		{
			delayCounter++;
			if (delayCounter == animateDelay)
			{
				delayCounter = 0;
				animateSpriteNumber++;
				if (animateSpriteNumber == 5) {
					animateSpriteNumber = 0;
				}
			}
		}
	};

	class Enemy
	{
		//friend class boost::serialization::access;
		friend class cereal::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar(isAlive, velocity, attackDamage, walkSpriteNumber, direction, delayCounter,
				movementDelay, hp, maxHp, canMoveUp, canMoveDown, canMoveLeft, canMoveRight,
				aggroedBy, collisionRect_x, collisionRect_y, isCollide, id, isBoss);
			/*ar& isAlive;
			ar& velocity;
			ar& attackDamage;
			ar& walkSpriteNumber;
			ar& direction;
			ar& delayCounter;
			ar& movementDelay;
			ar& hp;
			ar& maxHp;

			ar& canMoveUp;
			ar& canMoveDown;
			ar& canMoveLeft;
			ar& canMoveRight;

			ar& aggroedBy;

			ar& collisionRect_x;
			ar& collisionRect_y;
			ar& isCollide;
			ar& id;
			ar& isBoss;*/
		}

	public:
		bool isBoss = false;
		int id = 0;
		bool isCollide = false;
		bool isAlive = false;
		int velocity = 1;
		float attackDamage = 1.0f;
		int walkSpriteNumber = 0;
		int direction = 0;
		int delayCounter = 0;
		int movementDelay = 40;
		int hp = 3;
		int maxHp = 3;
		int width = 48;
		int height = 48;

		bool canMoveUp = true;
		bool canMoveDown = true;
		bool canMoveLeft = true;
		bool canMoveRight = true;

		int aggroedBy = 0;

		int collisionRect_x = 300;
		int collisionRect_y = 400;

		int temp_x = collisionRect_x;
		int temp_y = collisionRect_y;
		void move()
		{
			if (direction == 1 && canMoveUp)
			{
				//collisionRect.move(0.f, -velocity);
				//sprite.setTextureRect(sf::IntRect(walkSpriteNumber * spriteWidth + x, spriteHeight * 3 + y, spriteWidth, spriteHeight));
				collisionRect_x = temp_x;
				collisionRect_y = temp_y - velocity;
				temp_y = collisionRect_y;

				canMoveUp = true;
				canMoveDown = true;
				canMoveLeft = true;
				canMoveRight = true;
			}
			else if (direction == 2 && canMoveDown)
			{
				//collisionRect.move(0.f, velocity);
				//sprite.setTextureRect(sf::IntRect(walkSpriteNumber * spriteWidth + x, 0 + y, spriteWidth, spriteHeight));
				collisionRect_x = temp_x;
				collisionRect_y = temp_y + velocity;
				temp_y = collisionRect_y;

				canMoveUp = true;
				canMoveDown = true;
				canMoveLeft = true;
				canMoveRight = true;
			}
			else if (direction == 3 && canMoveLeft)
			{
				//collisionRect.move(-velocity, 0.f);
				//sprite.setTextureRect(sf::IntRect(walkSpriteNumber * spriteWidth + x, spriteHeight * 1 + y, spriteWidth, spriteHeight));
				collisionRect_x = temp_x - velocity;
				temp_x = collisionRect_x;
				collisionRect_y = temp_y;

				canMoveUp = true;
				canMoveDown = true;
				canMoveLeft = true;
				canMoveRight = true;
			}
			else if (direction == 4 && canMoveRight)
			{
				//collisionRect.move(velocity, 0.f);
				//sprite.setTextureRect(sf::IntRect(walkSpriteNumber * spriteWidth + x, spriteHeight * 2 + y, spriteWidth, spriteHeight));
				collisionRect_x = temp_x + velocity;
				temp_x = collisionRect_x;
				collisionRect_y = temp_y;

				canMoveUp = true;
				canMoveDown = true;
				canMoveLeft = true;
				canMoveRight = true;
			}
			else
			{
				canMoveUp = true;
				canMoveDown = true;
				canMoveLeft = true;
				canMoveRight = true;
			}




			walkSpriteNumber++;
			if (walkSpriteNumber == 3) {
				walkSpriteNumber = 0;
			}

			delayCounter++;
			if (delayCounter == movementDelay)
			{
				int randomNumber = rand();
				int random = (randomNumber % 4) + 1;
				delayCounter = 0;
				direction = random;
			}
		}
		void update()
		{
			move();
		}
	};

	class Projectile
	{
		//friend class boost::serialization::access;
		friend class cereal::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar(id, velocity, attackDamage, direction, lifetime, lifetimeCounter,
				collisionRect_x, collisionRect_y, isAlive, isCollide);
			/*ar& id;
			ar& velocity;
			ar& attackDamage;
			ar& direction;
			ar& lifetime;
			ar& lifetimeCounter;
			ar& collisionRect_x;
			ar& collisionRect_y;
			ar& isAlive;
			ar& isCollide;*/
		}
	public:
		bool isCollide = false;
		int velocity = 2;
		float attackDamage = 1.0f;
		int direction = 0; // 1 = up, 2 = down, 3 = left, 4 = right
		int lifetime = 100;
		int lifetimeCounter = 0;
		int id = 0;
		int collisionRect_x;
		int collisionRect_y;
		bool isAlive = false;
		int width = 13;
		int height = 13;

		int temp_projectile_x = collisionRect_x;
		int temp_projectile_y = collisionRect_y;

		void update()
		{
			if (direction == 1)
			{
				collisionRect_x = temp_projectile_x;
				collisionRect_y = temp_projectile_y - velocity;
				temp_projectile_y = collisionRect_y;
			}

			if (direction == 2 || direction == 0)
			{
				collisionRect_x = temp_projectile_x;
				collisionRect_y = temp_projectile_y + velocity;
				temp_projectile_y = collisionRect_y;
			}

			if (direction == 3)
			{
				collisionRect_y = temp_projectile_y;
				collisionRect_x = temp_projectile_x - velocity;
				temp_projectile_x = collisionRect_x;

			}

			if (direction == 4)
			{
				collisionRect_y = temp_projectile_y;
				collisionRect_x = temp_projectile_x + velocity;
				temp_projectile_x = collisionRect_x;
			}

			lifetimeCounter++;

			if (lifetimeCounter >= lifetime)
			{
				lifetimeCounter = 0;
				temp_projectile_x = collisionRect_x;
				temp_projectile_y = collisionRect_y;
				isAlive = false;
			}
		}
	};

	class Player
	{
		//friend class boost::serialization::access;
		friend class cereal::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar(id, collisionRect_x, collisionRect_y, velocity, powerUpLevel,
				direction, hp, maxHp, score, canMoveUp, canMoveDown, canMoveLeft, canMoveRight,
				isAlive, isCollide);
			/*ar& id;
			ar& collisionRect_x;
			ar& collisionRect_y;
			ar& velocity;
			ar& powerUpLevel;
			ar& direction;
			ar& hp;
			ar& maxHp;
			ar& score;
			ar& canMoveUp;
			ar& canMoveDown;
			ar& canMoveLeft;
			ar& canMoveRight;
			ar& isAlive;
			ar& isCollide;*/
		}
	public:
		unsigned short port;
		int connectDelay = 200;
		int connectCounter = 0;
		bool isConnected = false;
		bool isCollide = false;
		bool isAlive = false;
		int id = 1;
		int x = 0;
		int y = 0;
		int velocity = 2;
		float attackDamage = 1.0f;
		int powerUpLevel = 1;
		int maxPowerUpLevel = 4;
		int walkSpriteNumber = 0;
		int direction = 0; // 1 = up, 2 = down, 3 = left, 4 = right
		int hp = 100;
		int maxHp = 100;
		int score = 0;
		int width = 24;
		int height = 32;

		bool canMoveUp = true;
		bool canMoveDown = true;
		bool canMoveLeft = true;
		bool canMoveRight = true;

		int collisionRect_x = 300;
		int collisionRect_y = 400;

		int temp_x = collisionRect_x;
		int temp_y = collisionRect_y;


		void update()
		{
			move();
			//sprite.setPosition(collisionRect.getPosition());
		}

		void move()
		{
			if (direction == 1 && canMoveUp)
			{
				//collisionRect.move(0.f, -velocity);
				//collisionRect_x = collisionRect.getPosition().x;
				//collisionRect_y = collisionRect.getPosition().y;
				collisionRect_x = temp_x;
				collisionRect_y = temp_y - velocity;
				temp_y = collisionRect_y;
				canMoveUp = true;
				canMoveDown = true;
				canMoveLeft = true;
				canMoveRight = true;


			}
			else if (direction == 2 && canMoveDown)
			{
				/*collisionRect.move(0.f, velocity);
				collisionRect_x = collisionRect.getPosition().x;
				collisionRect_y = collisionRect.getPosition().y;*/
				collisionRect_x = temp_x;
				collisionRect_y = temp_y + velocity;
				temp_y = collisionRect_y;
				canMoveUp = true;
				canMoveDown = true;
				canMoveLeft = true;
				canMoveRight = true;


			}
			else if (direction == 3 && canMoveLeft)
			{
				collisionRect_x = temp_x - velocity;
				temp_x = collisionRect_x;
				collisionRect_y = temp_y;
				canMoveUp = true;
				canMoveDown = true;
				canMoveLeft = true;
				canMoveRight = true;

			}
			else if (direction == 4 && canMoveRight)
			{
				collisionRect_x = temp_x + velocity;
				temp_x = collisionRect_x;
				collisionRect_y = temp_y;
				canMoveUp = true;
				canMoveDown = true;
				canMoveLeft = true;
				canMoveRight = true;

			}
			else
			{
				canMoveUp = true;
				canMoveDown = true;
				canMoveLeft = true;
				canMoveRight = true;
				collisionRect_x = temp_x;
				collisionRect_y = temp_y;

			}
			//direction = 0;
		}
	};

	// ------------------------------

	struct LoginPacket
	{

		int type = 0;
		int id = 0;



		//friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, std::uint32_t const version)
		{
			ar(type, id);
			//ar& type;
			//ar& id;

		}




		void save(std::ostream& oss)
		{
			//boost::archive::binary_oarchive oa(oss);

			//oa&* (this);
			cereal::PortableBinaryOutputArchive oa(oss);
			oa(*this);

		}
		void load(std::string str_data)
		{
			std::istringstream iss(str_data);
			cereal::PortableBinaryInputArchive ia(iss);
			ia(*this);
			//boost::archive::binary_iarchive ia(iss);
			//ia&* (this);
		}
	};



	struct ServerPacket
	{
		int type = 0;
		int id = 0;
		std::wstring msg;
		std::vector<Player> players;
		std::vector<Projectile> projectiles;
		std::vector<Enemy> enemies;
		std::vector<Item> items;
		std::vector<Wall> walls;



		//friend class boost::serialization::access;
		//friend class cereal::access;
		template<class Archive>
		void serialize(Archive& ar, std::uint32_t const version)
		{
			ar(type, id, msg, players, projectiles, enemies, walls, items);
			//	ar& type;
			//	ar& id;
			//	ar& msg;
			//	ar& players;
			//	ar& projectiles;
			//	ar& enemies;
			//	ar& walls;
			//	ar& items;
		}






		ServerPacket()
		{

		}

		ServerPacket(std::wstring msg) : msg(msg)
		{

		}



		void save(std::ostream& oss)
		{
			//boost::archive::binary_oarchive oa(oss);

			//oa&* (this);
			cereal::PortableBinaryOutputArchive oa(oss);
			//oa(*this);
			oa&* (this);

		}
		void load(std::string str_data)
		{
			std::istringstream iss(str_data);
			cereal::PortableBinaryInputArchive ia(iss);
			//ia(*this);
			ia&* (this);
			//boost::archive::binary_iarchive ia(iss);
			//ia&* (this);
		}
	};

	struct ClientPacket
	{

		int key; // 1 = up, ... , 5 = space
		int player_id;



		//friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, std::uint32_t const version)
		{
			ar(key, player_id);
			//ar& key;
			//ar& player_id;
		}


		ClientPacket()
		{

		}
		ClientPacket(int key, int player_id) : key(key), player_id(player_id)
		{

		}

		void save(std::ostream& oss)
		{
			//boost::archive::binary_oarchive oa(oss);

			//oa&* (this);
			cereal::PortableBinaryOutputArchive oa(oss);
			//oa(*this);
			oa&* (this);

		}
		void load(std::string str_data)
		{
			std::istringstream iss(str_data);
			cereal::PortableBinaryInputArchive ia(iss);
			//ia(*this);
			//boost::archive::binary_iarchive ia(iss);
			ia&* (this);
		}
	};

	struct ChatPacket
    	{


    		int player_id;
    		std::wstring msg;



    		//friend class boost::serialization::access;
    		template<class Archive>
    		void serialize(Archive& ar, std::uint32_t const version)
    		{
    			ar(player_id, msg);
    			//ar& key;
    			//ar& player_id;
    		}


    		ChatPacket()
    		{

    		}
    		ChatPacket(int player_id, std::wstring msg) : player_id(player_id), msg(msg)
    		{

    		}

    		void save(std::ostream& oss)
    		{
    			//boost::archive::binary_oarchive oa(oss);

    			//oa&* (this);
    			cereal::PortableBinaryOutputArchive oa(oss);
    			//oa(*this);
    			oa&* (this);

    		}
    		void load(std::string str_data)
    		{
    			std::istringstream iss(str_data);
    			cereal::PortableBinaryInputArchive ia(iss);
    			//ia(*this);
    			//boost::archive::binary_iarchive ia(iss);
    			ia&* (this);
    		}
    	};

	//// udp
	const short multicast_port = 30001;
	const int max_message_count = 10;

	class sender
	{
	public:
		sender(boost::asio::io_context& io_context,
			const boost::asio::ip::address& multicast_address)
			: endpoint_(multicast_address, multicast_port),
			socket_(io_context, endpoint_.protocol()),
			timer_(io_context),
			message_count_(0)
		{


			//
			/*std::ostringstream os;
			os << "Message " << str;
			message_ = os.str();*/

			socket_.async_send_to(
				boost::asio::buffer(message_), endpoint_,
				boost::bind(&sender::handle_send_to, this,
					boost::asio::placeholders::error));
		}

		void handle_send_to(const boost::system::error_code& error)
		{
			if (!error && message_count_ < max_message_count)
			{
				timer_.expires_from_now(boost::posix_time::seconds(1));
				timer_.async_wait(
					boost::bind(&sender::handle_timeout, this,
						boost::asio::placeholders::error));
			}
		}

		void handle_timeout(const boost::system::error_code& error)
		{
			if (!error)
			{
				std::ostringstream os;
				os << "Message " << "hello world";
				message_ = os.str();

				//
				boost::asio::streambuf buf;
				net::ServerPacket pack;
				net::Player newPlayer;
				newPlayer.id = 100;
				pack.players.push_back(newPlayer);

				std::ostream oss(&buf);

				//pack.save(oss);
				boost::asio::streambuf::const_buffers_type bufs = buf.data();
				std::string str(boost::asio::buffers_begin(bufs),
					boost::asio::buffers_begin(bufs) + bufs.size());

				message_ = str;
				//std::cout << message_ << std::endl;
				socket_.async_send_to(
					boost::asio::buffer(message_), endpoint_,
					boost::bind(&sender::handle_send_to, this,
						boost::asio::placeholders::error));
			}
		}

	private:
		boost::asio::ip::udp::endpoint endpoint_;
		boost::asio::ip::udp::socket socket_;
		boost::asio::deadline_timer timer_;
		int message_count_;
		std::string message_;
	};

	inline std::vector<Player> players;
	inline std::vector<Projectile> projectiles;
	inline std::vector<Enemy> enemies;
	inline std::vector<Item> items;
	inline std::vector<Wall> walls;
	inline std::wstring chat;

	class receiver
	{
	public:
		receiver(boost::asio::io_context& io_context,
			const boost::asio::ip::address& listen_address,
			const boost::asio::ip::address& multicast_address)
			: socket_(io_context)
		{
			// Create the socket so that multiple may be bound to the same address.
			boost::asio::ip::udp::endpoint listen_endpoint(
				listen_address, multicast_port);
			socket_.open(listen_endpoint.protocol());
			socket_.set_option(boost::asio::ip::udp::socket::reuse_address(true));
			socket_.bind(listen_endpoint);

			// Join the multicast group.
			socket_.set_option(
				boost::asio::ip::multicast::join_group(multicast_address));

			socket_.async_receive_from(
				boost::asio::buffer(data_, max_length), sender_endpoint_,
				boost::bind(&receiver::handle_receive_from, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));
		}

		std::string convertToString(char* a, int size)
		{
			int i;
			std::string s = "";
			for (i = 0; i < size; i++) {
				s = s + a[i];
			}
			return s;
		}
		void handle_receive_from(const boost::system::error_code& error,
			size_t bytes_recvd)
		{
			if (!error)
			{
				//std::cout.write(data_, bytes_recvd);
				//std::stringstream buffer;
				//buffer << data_ << std::endl;
				std::string str = convertToString(data_, bytes_recvd);

				net::ServerPacket pack;

				//pack.load(str);
				players = pack.players;
				projectiles = pack.projectiles;
				enemies = pack.enemies;
				items = pack.items;
				walls = pack.walls;

				socket_.async_receive_from(
					boost::asio::buffer(data_, max_length), sender_endpoint_,
					boost::bind(&receiver::handle_receive_from, this,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred));
			}
		}

	private:
		boost::asio::ip::udp::socket socket_;
		boost::asio::ip::udp::endpoint sender_endpoint_;
		enum { max_length = 1000000 };
		char data_[max_length];
	};

	class receiver2
	{
	public:
		receiver2(boost::asio::io_context& io_context,
			const std::string& raw_ip_address,
			const int port_num)
			: socket_(io_context)
		{
			// Create the socket so that multiple may be bound to the same address.

			boost::asio::ip::tcp::endpoint m_ep(boost::asio::ip::address::from_string(raw_ip_address),
				port_num);


			if (stopped)
			{
				std::cout << "[chat connect]" << std::endl;
				socket_.open(m_ep.protocol());
				//socket_.set_option(boost::asio::ip::tcp::socket::reuse_address(true));

				socket_.async_connect(m_ep,
					[&](const boost::system::error_code& ec)
					{
						if (ec)
						{
							std::cout << "[ERROR] chat request failed" << std::endl;
							return;
						}
						std::cout << "[SUCCESS] connect request successful" << std::endl;
						//__android_log_print(ANDROID_LOG_INFO, "CONNECTION_TEST", "recv connect successful");
						//std::cout << "read connect reqeust response start" << std::endl;

						stopped = false;

						start_read();

					});
			}

		}

		void start_read()
		{
			boost::asio::async_read_until(socket_,
				m_chat_response_buf,
				//boost::asio::transfer_at_least(packetSize),
				"\r\n", boost::bind(&receiver2::handle_receive_from, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));
		}

		void handle_receive_from(const boost::system::error_code& error,
			size_t bytes_recvd)
		{
			if (stopped)
				return;
			if (!error)
			{
				std::cout << "[SUCCESS] read chat successful" << std::endl;
				//__android_log_print(ANDROID_LOG_INFO, "CONNECTION_TEST", "successful");
				boost::asio::streambuf::const_buffers_type bufs = m_chat_response_buf.data();
				std::string str(boost::asio::buffers_begin(bufs),
					boost::asio::buffers_begin(bufs) + bufs.size());

				ChatPacket pack;

				pack.load(str);

				chat = pack.msg;

				m_chat_response_buf.consume(m_chat_response_buf.size() + 1);

				start_read();

			}
			else
			{
				stop();
			}
		}

		void stop()
		{
			stopped = true;
		}

		void read()
		{
			boost::asio::async_read_until(socket_,
				m_chat_response_buf,
				//boost::asio::transfer_at_least(packetSize),
				"\r\n",
				[this](const boost::system::error_code& ec,
					std::size_t bytes_transferred)
				{
					if (ec)
					{
						std::cout << "[ERROR] read chat failed" << std::endl;
					}
					else
					{
						std::cout << "[SUCCESS] read chat successful" << std::endl;
						//__android_log_print(ANDROID_LOG_INFO, "CONNECTION_TEST", "successful");
						boost::asio::streambuf::const_buffers_type bufs = m_chat_response_buf.data();
						std::string str(boost::asio::buffers_begin(bufs),
							boost::asio::buffers_begin(bufs) + bufs.size());

						ServerPacket pack;

						pack.load(str);

						chat = pack.msg;


						//__android_log_print(ANDROID_LOG_INFO, "CONNECTION_TEST", "response : %d ", str.size());


						std::wcout << "--->[CHAT RESPONSE] : " << chat << std::endl;
						//__android_log_print(ANDROID_LOG_INFO, "CONNECTION_TEST", "request : %d", pack.id);

					}

					//onRequestComplete(session);
				});
		}


	private:
		bool stopped = true;
		boost::asio::ip::tcp::endpoint m_ep;
		boost::asio::ip::tcp::socket socket_;
		boost::asio::ip::tcp::endpoint sender_endpoint_;
		enum { max_length = 1000000 };
		char data_[max_length];
		boost::asio::streambuf m_chat_response_buf;
	};
}
