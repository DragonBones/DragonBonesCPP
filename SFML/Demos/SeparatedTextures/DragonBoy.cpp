/*
*********************************************************************
* File          : DragonBoy.cpp
* Project       : DragonBonesSFML
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
* License       : MIT License
*********************************************************************
*/

#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>

#include "dragonBones/SFMLFactory.h"
#include "dragonBones/SFMLArmatureDisplay.h"
#include "dragonBones/SFMLTextureAtlasData.h"

// prevent memory leak
std::vector<std::unique_ptr<sf::Texture>> textures;

void loadTextures(dragonBones::DragonBonesData* dragonBonesData, dragonBones::SFMLFactory* factory, const std::string& folder)
{
auto texturesData = factory->getTexturesData(dragonBonesData, folder);

for (auto& textureData : texturesData)
{
    auto tex = new sf::Texture();
    tex->loadFromFile(textureData->path);

    textureData->setTexture(tex);

    textures.push_back(std::unique_ptr<sf::Texture>(tex));
}

factory.createTextureAtlasData(texturesData, dragonBonesData);
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(1024, 768), "My window");

    dragonBones::SFMLFactory factory;

    auto dragonBonesData = factory.loadDragonBonesData("DragonBoy_ske.json");

    loadTextures(dragonBonesData, &factory, "DragonBoy_texture");

    auto armatureDisplay = factory.buildArmatureDisplay("Dragon");
    armatureDisplay->getAnimation()->play("walk");
    armatureDisplay->setPosition({512.f, 440.f});

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
        armatureDisplay->render(window, sf::RenderStates::Default);
        window.display();
    }

    

    textures.clear();
}
