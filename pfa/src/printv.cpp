#include <SFML/Graphics.hpp>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/array.hpp>

#include "printv.hpp"

std::ostream& printv(std::ostream& f, const std::string& s)
{
  size_t prevPos;;
  size_t pos;

  pos = 0;
  prevPos = 0;
  while ((pos = s.find('%', prevPos)) != std::string::npos)
    {
      f << s.substr(prevPos, pos - prevPos);
      if (s[pos + 1] != '%')
        throw std::runtime_error("invalid format string: missing arguments");
      else
        {
          ++pos;
          f << '%';
        }
      prevPos = pos + 1;
    }
  f << s.substr(prevPos);
  return f;
}

#define BOOST_SYSTEM_NO_DEPRECATED
using boost::asio::ip::tcp;

void testBoost()
{
	// Création du service principal et du résolveur.
	boost::asio::io_service ios;

	// On veut se connecter sur la machine locale, port 7171
	tcp::endpoint endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 7171);

	// On crée une socket // (1)
	tcp::socket socket(ios);

	// Tentative de connexion, bloquante // (2)
	socket.connect(endpoint);

	// Création du buffer de réception // (3)
	boost::array<char, 128> buf;
	while (1)
	{
		boost::system::error_code error;
		// Réception des données, len = nombre d'octets reçus // (4)
		int len = socket.read_some(boost::asio::buffer(buf), error);

		if (error == boost::asio::error::eof) // (5)
		{
			std::cout << "\nTerminé !" << std::endl;
			break;
		}

		// On affiche (6)
		std::cout.write(buf.data(), len);
	}
}

void testSfml()
{
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            window.clear();
            window.draw(shape);
            window.display();
        }
}
