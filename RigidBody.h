#pragma once
#include"Vector.h"

class RigidBodys;

struct CollisionRect
{
	CollisionRect(string name, sf::FloatRect rect) :Name(name), Rect(rect)
	{}
	string Name;
	sf::FloatRect Rect;
};

class RigidBody :public sf::Drawable
{
	friend class RigidBodys;

	string UnknowName = "UnknowName";

public:
	RigidBody() 
		: Name(UnknowName)
	{
	}

	RigidBody(string const& name)
		: Name(name)
	{
		m_ball.setRadius(50);
		m_ball.setFillColor(sf::Color::White);
		m_ball.setOrigin(m_ball.getRadius(), m_ball.getRadius());
		newX = m_ball.getPosition();
	}

	void SetFillColor(sf::Color color) { m_ball.setFillColor(color); }

	sf::Color GetFiilColor()const { return m_ball.getFillColor(); }

	sf::Vector2f GetPos()const { return m_ball.getPosition(); }
	
	float GetR()const { return m_ball.getRadius(); }

	Vector GetdX()const { return dX; }

	void SetName(string const& name) { this->Name = name; }

	void SetdX(Vector const& dx) { dX = dx; }

	void SetPos(sf::Vector2f const& position) { m_ball.setPosition(position); newX = this->GetPos(); }

	bool InitisLegal(RigidBody const& ball)
	{
		Vector vec_distance = ball.GetPos() - this->GetPos();
		return vec_distance.GetLength() > ball.GetR() + ball.GetR() + 5; //if >, Legal
	}

private:
	std::string Name;

	sf::CircleShape m_ball;

	Vector newX, dX;

	map<string, bool>CollisionMap;

	void UpdateAxis()
	{
		newX += dX;
		this->SetPos(sf::Vector2f{ newX.x,newX.y });
	}

	void UpdateAxis(Vector const& dx)
	{
		newX += dx;
		this->SetPos(sf::Vector2f{ newX.x,newX.y });
	}

	void UpdateVelocity()
	{
		dX *= 0.995;
		if (dX.x<0.1 and dX.y<0.1 and dX.x>-0.1 and dX.y>-0.1)dX = { 0,0 };
	}

	RigidBody RangeCircleCollision(RigidBody const& ball) = delete;

	RigidBody RangeRectCollision(CollisionRect const& rect)
	{
		auto CollisionCheck = CollisionMap.find(rect.Name);

		////if CircleCollision
		if (this->m_ball.getGlobalBounds().intersects(rect.Rect))
		{
			if ((rect.Rect.height > rect.Rect.width))   //  左右
			{
				if (CollisionCheck != CollisionMap.end())
				{
					if (CollisionMap[rect.Name] == true)
						return *this;
				}
				else
				{
					CollisionMap[rect.Name] = true;
				}
				dX.x = -dX.x;
				return *this;
			}
			else 
			{
				if (CollisionCheck != CollisionMap.end())
				{
					if (CollisionMap[rect.Name] == true)
						return *this;
				}
				else
				{
					CollisionMap[rect.Name] = true;
				}
				dX.y = -dX.y;
				return *this;
			}
		}
		////if Not CirCleCollision
		CollisionMap[rect.Name] = false;
		return *this;
	}

	RigidBody CircleCollision(RigidBody const& ball)
	{
		Vector d12 = ball.GetPos() - this->GetPos();
		float MinDistance = ball.GetR() + this->GetR();
		auto CollisionCheck = CollisionMap.find(ball.Name);

		////if CircleCollision
		if (d12.GetLength() < MinDistance)
		{
			if (CollisionCheck != CollisionMap.end())
			{
				if (this->CollisionMap[ball.Name] == true) ////  之前碰撞过
					return *this;
			}
			else ////    之前没碰撞过但现在碰撞了
			{
				this->CollisionMap[ball.Name] = true;
			}
			auto dX_ = dX - dX * d12 / d12.GetLengthSq() * d12 + ball.dX * d12 / d12.GetLengthSq() * d12;
			RigidBody newR;
			newR = *this;
			newR.dX = dX_;
			return newR;
		}
		////if Not CirCleCollision
		this->CollisionMap[ball.Name] = false;  ////  碰撞解除
		return *this;
	}

	void draw(sf::RenderTarget& tar, sf::RenderStates sta)const
	{
		tar.draw(m_ball, sta);
	}
};

class RigidBodys :public sf::Drawable
{
public:
	RigidBodys()
	{
		m_tmp = m_bodys;
	}

	RigidBodys(vector<RigidBody>const& rigidbodys)
	{
		m_bodys = rigidbodys;
		m_tmp = m_bodys;
	}

	RigidBody GetWhiteBall()const { return m_bodys[0]; }

	void SetWhiteBall(RigidBody const& Newwhiteball) { m_bodys[0] = Newwhiteball; m_tmp = m_bodys; }

	bool GetStatic()const
	{
		auto it = m_bodys.begin();
		for (; it != m_bodys.end(); it++)
		{
			if (!(it->dX == Vector{ 0,0 }))
				break;
		}
		if (it != m_bodys.end())return false;
		return true;
	}

	void UpdateAxis(vector<RigidBody>RangeCircle, vector<CollisionRect>Range)
	{
		for (size_t i = 0; i < m_bodys.size(); i++)
		{
			//1.边缘尖端碰撞球（固定）
			/*for (auto it = RangeCircle.begin(); it != RangeCircle.end(); it++)
			{
				m_tmp[i] = m_bodys[i].RangeCircleCollision(*it);
			}*/
			//2.边缘平面碰撞板（固定）
			for (auto it = Range.begin(); it != Range.end(); it++)
			{
				m_tmp[i] = m_bodys[i].RangeRectCollision(*it);
			}

			//3.另外的球（活动）
			float MaxDistance = INT_MAX;
			int j = 0, flag = j;
			for (; j < m_bodys.size(); j++)
			{
				if (m_bodys[j].Name != m_bodys[j].UnknowName and m_bodys[j].Name != m_bodys[i].Name)
				{
					Vector Distance = m_bodys[j].GetPos() - m_bodys[i].GetPos();
					if (Distance.GetLength() < MaxDistance)
					{
						MaxDistance = Distance.GetLength();
						flag = j;
					}
				}
			}
			m_tmp[i] = m_bodys[i].CircleCollision(m_bodys[flag]);

			//4.Update
			m_tmp[i].UpdateAxis();
			m_tmp[i].UpdateVelocity();
		}
		m_bodys = m_tmp;
	}
private:
	vector<RigidBody>m_bodys;
	vector<RigidBody>m_tmp;

	virtual void draw(sf::RenderTarget& tar, sf::RenderStates sta)const
	{
		for (auto it = m_bodys.begin(); it != m_bodys.end(); it++)
		{
			tar.draw(it->m_ball, sta);
		}
	}
};

class Cue :public sf::Drawable
{
public:
	Cue() :isViewed(false), m_pos({ 0,0 }), m_dir({ 0,0 }), m_length(0) {}

	Cue(bool const& viewed, Vector const& pos, Vector const& dir, float const& length)
		:isViewed(false), m_pos(pos), m_dir(dir), m_length(length)
	{

	}

	bool isViewed;

	Vector m_pos;

	Vector m_dir;

	float m_length;

	void draw(sf::RenderTarget& tar, sf::RenderStates sta)const
	{
		
	}
};

/*RigidBody RigidBody::RangeCircleCollision(RigidBody const& ball)
{
	Vector d12 = ball.GetPos() - this->GetPos();
	float MinDistance = ball.GetR() + this->GetR();
	auto CollisionCheck = CollisionMap.find(ball.Name);

	////if CircleCollision
	if (d12.GetLength() < MinDistance)
	{
		if (CollisionCheck != CollisionMap.end())
		{
			if (CollisionMap[ball.Name] == true)
				return *this;
		}
		else
		{
			CollisionMap[ball.Name] = true;
		}
		auto dX_ = (dX - 2 * dX * d12 / d12.GetLengthSq() * d12);
		RigidBody newR = *this;
		newR.dX = dX_;
		//this->dX = dX_;
		return newR;
	}
	////if Not CirCleCollision
	CollisionMap[ball.Name] = false;
	return *this;
}*/