#pragma once
#include <vector>
#include <SFML/Network/Packet.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Image.hpp>

inline sf::Packet& operator<<(sf::Packet& packet, const sf::Texture& texture) {
    sf::Image image = texture.copyToImage();
    sf::Vector2u size = texture.getSize();
    const uint8_t* pixels = image.getPixelsPtr();
    uint32_t dataSize = size.x * size.y * 4;

    packet << size.x << size.y;
    for (uint32_t i = 0; i < dataSize; ++i)
        packet << pixels[i];

    return packet;
}

inline sf::Packet& operator>>(sf::Packet& packet, sf::Texture& texture) {
    uint32_t width, height;
    packet >> width >> height;

    std::vector<uint8_t> pixels(width * height * 4);
    for (auto& p : pixels)
        packet >> p;

    sf::Image image(sf::Vector2u{width, height}, pixels.data());
    texture = sf::Texture(image);

    return packet;
}