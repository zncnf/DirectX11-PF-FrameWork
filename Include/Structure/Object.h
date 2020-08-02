#pragma once
class Object
{
public:
	Object() = default;
	~Object() = default;

public:
	virtual void Init() = 0;
	virtual void Update() = 0;

public:
	string name;
};

