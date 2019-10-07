#pragma once
#ifndef TEXTURE_HOLDER_H
#define TEXTURE_HOLDER_H

#include <SFML\Graphics.hpp>
#include <map>

using namespace sf;

class TextureHolder{
private:
	//holds related pairs of string and textures
	std::map<std::string, Texture> m_textures;
	//the one and only instance of the class
	static TextureHolder* m_s_Instance;//it can be used without an instance of the class,though the class itself
public:
	TextureHolder();
	//again can be used without instance of the class
	//takes a string as a constant reference,it returns a reference so it avoids making a copy
	static Texture& GetTexture(std::string const& filename);
};
#endif // !TEXTURE_HOLDER_H