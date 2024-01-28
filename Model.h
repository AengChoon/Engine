#pragma once
#include <memory>
#include "DrawableBase.h"

struct aiMesh;
struct aiNode;
class Graphics;
class ModelWindow;

class Mesh : public DrawableBase<Mesh>
{
public:
	Mesh(const Graphics& InGraphics, std::vector<std::unique_ptr<Bindable>>&& InBindables);
	void Draw(const Graphics& InGraphics, const DirectX::XMMATRIX& InAccumulatedTransform) const;
	[[nodiscard]] DirectX::XMMATRIX GetTransformMatrix() const noexcept override;

private:
	mutable DirectX::XMFLOAT4X4 TransformMatrix;
};

class Node
{
	friend class Model;
	friend class ModelWindow;

public:
	Node(std::string_view InName, std::vector<Mesh*>&& InMeshes, const DirectX::XMMATRIX& InTransformMatrix);
	void Draw(const Graphics& InGraphics, const DirectX::XMMATRIX& InAccumulatedTransformMatrix) const;
	void SetAppliedTransform(DirectX::FXMMATRIX InTransform);

private:
	void AddChild(std::unique_ptr<Node>&& InNode);
	void RenderTree(int* InCurrentNodeIndexAddress, int* InSelectedNodeIndexAddress, const Node** InSelectedNode) const;

private:
	std::string Name;
	std::vector<std::unique_ptr<Node>> Children;
	std::vector<Mesh*> Meshes;
	DirectX::XMFLOAT4X4 BaseTransform;
	DirectX::XMFLOAT4X4 AppliedTransform;
};

class Model
{
public:
	Model(const Graphics& InGraphics, std::string_view InFileName);
	~Model();

	static std::unique_ptr<Mesh> ParseMesh(const Graphics& InGraphics, const aiMesh& InMesh);
	std::unique_ptr<Node> ParseNode(const aiNode& InNode);
	void Draw(const Graphics& InGraphics) const;
	void ShowWindow(std::string_view InWindowName = {}) const;

private:
	std::unique_ptr<Node> Root;
	std::vector<std::unique_ptr<Mesh>> Meshes;
	std::unique_ptr<ModelWindow> Window;
};
