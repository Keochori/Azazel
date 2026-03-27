#include "pch.h"
#include "Object.h"

Object::Object(const std::string& aName) : myName(aName)
{
}

void Object::SetMesh(const std::shared_ptr<Mesh>& aMesh)
{
	myMesh = aMesh;
}

void Object::SetMaterial(const std::shared_ptr<Material>& aMaterial)
{
	myMaterial = aMaterial;
}

const std::string& Object::GetName()
{
	return myName;
}

const std::shared_ptr<Mesh>& Object::GetMesh()
{
	return myMesh;
}

const std::shared_ptr<Material>& Object::GetMaterial()
{
	return myMaterial;
}

Transform& Object::GetTransform()
{
	return myTransform;
}
