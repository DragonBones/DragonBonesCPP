/** @file AnimationLayer.cpp
 ** @author Piotr Krupa (piotrkrupa06@gmail.com)
 ** @license MIT License
 **/

#include <SFML/Graphics.hpp>

#include <dragonBones/SFMLFactory.h>
#include <dragonBones/SFMLArmatureDisplay.h>


int main()
{
	sf::RenderWindow window(sf::VideoMode(1024, 768), "My window");
	window.setFramerateLimit(60);

	dragonBones::SFMLFactory factory;

	sf::Texture texture;
	texture.loadFromFile("mecha_1004d_tex.png");

	factory.loadDragonBonesData("mecha_1004d_ske.json");
	factory.loadTextureAtlasData("mecha_1004d_tex.json", &texture);

	dragonBones::SFMLArmatureDisplay armatureDisplay("mecha_1004d");
	armatureDisplay.getAnimation()->play("walk");
	armatureDisplay.setPosition({ 512.f, 440.f });
	armatureDisplay.getEventDispatcher()->addDBEventListener(dragonBones::EventObject::LOOP_COMPLETE, [&] (dragonBones::EventObject* event) {

		auto attackState = armatureDisplay.getAnimation()->getState("attack_01");
		if (!attackState)
		{
			attackState = armatureDisplay.getAnimation()->fadeIn("attack_01", 0.1f, 1, 1);
			attackState->resetToPose = false;
			attackState->autoFadeOutTime = 0.1f;
			attackState->addBoneMask("chest");
			attackState->addBoneMask("effect_l");
			attackState->addBoneMask("effect_r");
		}
	});

	sf::Clock clock;

	while (window.isOpen())
	{
		float deltaTime = clock.restart().asSeconds();

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		factory.update(deltaTime);

		window.clear();
		window.draw(armatureDisplay);
		window.display();
	}

	return 0;
}
