#pragma once
#include<SFML/Graphics.hpp>
#include<iostream>
#include<map>
#include<cassert>
using namespace std;
class AssetManager
{	
private:
	std::map<std::string, sf::Texture> m_Textures;
	std::map<string, sf::Font>m_Fonts;
	static AssetManager* sInstance;
public:
	AssetManager();
	~AssetManager() {}
	static sf::Font& GetFont(string const& filename);
	static sf::Texture& GetTexture(string const& filename);
	static void SetTextureSmooth(string const& filename, bool BeSmooth);
};
