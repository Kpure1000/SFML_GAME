#pragma once
#include"AssetManager.h"
#include<list>
#include<string>

using namespace std;

class Animator
{
private:
	class Animation
	{
	public:
		string Name;
		string TextureName;
		vector<sf::IntRect>Frames;
		sf::Time Duration; //  lasting time
		bool Looping;

		Animation(string const& name, string const& texturename,
			sf::Time const& duration, bool looping) :
			Name(name),	TextureName(texturename), 
			Duration(duration), Looping(looping) {}

		void AddFrames(sf::Vector2i const& StartForm, sf::Vector2i const& FrameSize, unsigned int frames)
		{
			sf::Vector2i current = StartForm;
			for (unsigned int i = 0; i < frames; i++)
			{
				Frames.push_back(sf::IntRect(current.x, current.y, FrameSize.x, FrameSize.y));
				current.x += FrameSize.x;
			}
		}
		~Animation() 
		{ 
			//cout << "Animation: \"" << Name << "\" has been distructed." << endl; 
		}
	
	};

private:
	Animation* FindAnimation(string const& name);

	void SwitchAnimation(Animator::Animation* animation);

	sf::Sprite& sprite;
	sf::Time CurrentTime;
	list<Animation>Animations;
	Animator::Animation* CurrentAnimation;


public:
	Animator(sf::Sprite& sprite) : sprite(sprite), CurrentTime(), CurrentAnimation(nullptr) {}
	Animation& CreateAnimation(string const& name, string const& texturename, sf::Time const& duration, bool loop = false);
	
	void Update(sf::Time const& dt); //  deltatime

	bool SwitchAnimation(string const& name);
	string GetCurrentAnimation()const;
};

