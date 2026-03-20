#pragma once
#include "Assets/Mesh.h"
#include "Transform.h"
#include <memory>

class Object
{
public:
	Object(std::shared_ptr<Mesh> aMesh);
	~Object();

	std::shared_ptr<Mesh> GetMesh() const;
	Transform& GetTransform();

private:
	std::shared_ptr<Mesh> myMesh;
	Transform myTransform;
};

