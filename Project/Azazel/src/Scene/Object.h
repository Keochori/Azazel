#pragma once
#include "Assets/Mesh.h"
#include "Assets/Material.h"
#include "Transform.h"
#include <memory>

class Object
{
public:
	Object(const std::string& aName);
	~Object() = default;

	void SetMesh(const std::shared_ptr<Mesh>& aMesh);
	void SetMaterial(const std::shared_ptr<Material>& aMaterial);

	const std::string& GetName();
	const std::shared_ptr<Mesh>& GetMesh();
	const std::shared_ptr<Material>& GetMaterial();
	Transform& GetTransform();

private:
	std::string myName;
	std::shared_ptr<Mesh> myMesh;
	std::shared_ptr<Material> myMaterial;
	Transform myTransform;
};

