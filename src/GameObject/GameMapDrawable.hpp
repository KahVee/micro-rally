#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "MapTile.hpp"
#include "../constants.hpp"

class GameMapDrawable : public sf::Drawable, public sf::Transformable
{
public:
    bool load(const std::string& tileset, float tileSize, std::vector<MapTile*> map, int width, int height)
    {
        // load the tileset texture
        if (!m_tileset.loadFromFile(tileset))
            return false;

        // resize the vertex array to fit the level size
        m_vertices.setPrimitiveType(sf::Quads);
        m_vertices.resize(width * height * 4);

        // populate the vertex array, with one quad per tile
        for (unsigned int i = 0; i < width; ++i)
            for (unsigned int j = 0; j < height; ++j)
            {
                // get the current tile
                MapTile *tile = map[i + j * width];

                // get a pointer to the current tile's quad
                sf::Vertex* quad = &m_vertices[(i + j * width) * 4];

                // define its 4 corners
                quad[0].position = sf::Vector2f(i * tileSize, j * tileSize);
                quad[1].position = sf::Vector2f((i + 1) * tileSize, j * tileSize);
                quad[2].position = sf::Vector2f((i + 1) * tileSize, (j + 1) * tileSize);
                quad[3].position = sf::Vector2f(i * tileSize, (j + 1) * tileSize);

                // define its 4 texture coordinates
                quad[0].texCoords = sf::Vector2f(tile->textureLocationX_, tile->textureLocationY_);
                quad[1].texCoords = sf::Vector2f(tile->textureLocationX_ + MAP_TEXTURE_TILE_SIZE, tile->textureLocationY_);
                quad[2].texCoords = sf::Vector2f(tile->textureLocationX_ + MAP_TEXTURE_TILE_SIZE, tile->textureLocationY_ + MAP_TEXTURE_TILE_SIZE);
                quad[3].texCoords = sf::Vector2f(tile->textureLocationX_, tile->textureLocationY_ + MAP_TEXTURE_TILE_SIZE);
            }

        return true;
    }

private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // apply the transform
        states.transform *= getTransform();

        // apply the tileset texture
        states.texture = &m_tileset;

        // draw the vertex array
        target.draw(m_vertices, states);
    }

    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
};