#include <iostream>
#include "MainMenu.hpp"

MainMenu::MainMenu(sf::Texture * const texture) :
  APanelScreen(texture)
{
  if (!_font.loadFromFile("../client/assets/font.otf"))
    std::cerr << "Can't load font" << std::endl; // replace this by a throw about ressources

  Widget	*mBack = new Widget("menuback", {30, 50}, NULL);
  Widget	*menuTitleWidget = new Widget("menu-title", {30, 50}, new sf::Text("MAIN", _font, 40));
  Widget	*playWidget = new Widget("play", {50, 150}, new sf::Text("PLAY ONLINE", _font, 30));
  Widget	*optWidget = new Widget("opt", {50, 230}, new sf::Text("OPTION", _font, 30));
  Widget	*quitWidget = new Widget("quit", {50, 310}, new sf::Text("QUIT", _font, 30));
  Widget	*background = new Widget("background", {0, 0}, NULL);
  sf::Sprite	bgSprite(*texture, sf::IntRect(0, 0, 1920, 1080));
  sf::Texture	*bgMenu;

  addSpriteForWidget(menuTitleWidget, sf::Color(10, 06, 12, 255), {300, 70});
  bgMenu = addSpriteForWidget(mBack, sf::Color(125, 125, 125, 150), {300, 600});

  background->addSprite(bgSprite);
  menuTitleWidget->setTextAttr(sf::Text::Bold);
  menuTitleWidget->alignText({30,50}, {300, 70}, 25, 50);
  playWidget->alignText({50,150}, {260, 60}, 50, 50);
  playWidget->addSprite(texture, sf::IntRect(0, 1080, 260, 60));
  optWidget->alignText({50,230}, {260, 60}, 50, 50);
  optWidget->addSprite(texture, sf::IntRect(0, 1080, 260, 60));
  quitWidget->alignText({50,310}, {260, 60}, 50, 50);
  quitWidget->addSprite(texture, sf::IntRect(0, 1080, 260, 60));

  _widgets.push_back(background);
  _widgets.push_back(mBack);
  _widgets.push_back(menuTitleWidget);
  _widgets.push_back(playWidget);
  _widgets.push_back(optWidget);
  _widgets.push_back(quitWidget);
}

MainMenu::~MainMenu()
{
}

int	MainMenu::run(const sf::Event &event)
{
  for (auto &widget : _widgets)
    widget->update(event);
}
