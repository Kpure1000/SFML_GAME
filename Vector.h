#pragma once
#include"total.h"
class Vector
{
public:
	Vector() :x(0), y(0) {}
	Vector(float const& X, float const& Y) :x(X), y(Y) {}
	template<class T>
	Vector(sf::Vector2<T> vec) : x(static_cast<float>(vec.x)), y(static_cast<float>(vec.y)) {}

	template<class T>
	sf::Vector2<T> operator=(Vector const& vec)const { return sf::Vector2<T>{vec.x, vec.y}; }

	bool operator==(Vector const& vec)const { return this->x == vec.x and this->y == vec.y; }

	Vector operator+(Vector const& vec)const { return Vector(this->x + vec.x, this->y + vec.y); }
	void operator+=(Vector const& vec) { this->x += vec.x, this->y += vec.y; }
	Vector operator-(Vector const& vec)const { return Vector(this->x - vec.x, this->y - vec.y); }
	void operator-=(Vector const& vec) { this->x -= vec.x, this->y -= vec.y; }

	Vector operator*(float const& val)const { return Vector(this->x * val, this->y * val); }
	void operator*=(float const& val) { this->x *= val, this->y *= val; }
	float operator*(Vector const& vec)const { return this->x * vec.x + this->y * vec.y; }
	Vector operator/(float const& val)const { return Vector{ this->x / val , this->y / val }; }
	friend Vector operator*(float const& val, Vector const& vec);

	float GetLength()const { return sqrt(this->x * this->x + this->y * this->y); }
	float GetLengthSq()const { return this->x * this->x + this->y * this->y; }

	float x, y;
};

Vector operator*(float const& val, Vector const& vec) { return  vec * val; }

