#include "pch.h"
#include "Object.h"

Object::Object(std::shared_ptr<Mesh> aMesh) : myMesh(aMesh)
{
}

Object::~Object()
{
}

std::shared_ptr<Mesh> Object::GetMesh() const
{
	return myMesh;
}

Transform& Object::GetTransform()
{
	return myTransform;
}
