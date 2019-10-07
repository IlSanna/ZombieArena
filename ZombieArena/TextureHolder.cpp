#include "TextureHolder.h"
// Include the "assert feature"
#include <assert.h>

TextureHolder* TextureHolder::m_s_Instance = nullptr;

TextureHolder::TextureHolder(){//building a singleton
	//ensures that m_s_Instance equals nullptr.If it doesn't, the game will exit execution
	assert(m_s_Instance == nullptr);
	/*we now have a pointer to a TextureHolder that points to the
	one and only instance of itself.*/
	m_s_Instance = this;//assigns the pointer to this instance
}

Texture& TextureHolder::GetTexture(std::string const& filename) {
	//get a reference to the full map m_textures
	std::map<std::string, sf::Texture>& m = m_s_Instance->m_textures;
	//Create an iterator to hold a key-value-pair (kvp) and search 
	//for the required kvp using the passed in filename
	std::map<std::string, sf::Texture>::iterator keyValuePair = m.find(filename);
	if (keyValuePair != m.end()) {//if is == end it means we didnt find it
		return keyValuePair->second;//the second part of the map, the texture in this case
	}
	else {
		//not found, create a new key
		//this si like map["john"], return the texture with the associated [filename]
		std::map<std::string, sf::Texture>::mapped_type& texture = m[filename];//m[filename] è sf::Texture, quindi texture è una reference ad un sf::texture
		texture.loadFromFile(filename);
		return texture;
	}
}
