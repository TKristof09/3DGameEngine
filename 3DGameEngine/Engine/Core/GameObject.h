#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <map>
#include <utility>
#include <vector>
#include <typeindex>
#include "Transform.h"

class RenderingEngine;
class PhysicsEngine;
class AudioEngine;
class Shader;
class GameComponent;
class DebugUIWindow;
class TreeNode;

class GameObject
{
public:
    GameObject(std::string name = "GameObject") :
        name(std::move(name)),
        m_renderingEngine(nullptr),
        m_physicsEngine(nullptr),
        m_audioEngine(nullptr),
        m_firstSelect(true),
        m_hierarchySelected(false),
        m_hierarchyWindow(nullptr) {}

    ~GameObject() = default;
    void Start() const;
	void Update() const;
	void Render(const Shader* shader, RenderingEngine* renderingEngine) const;
	void Input() const;
	GameObject* AddChild(GameObject* child);
	void AddComponent(GameComponent* component);
	void SetRenderingEngine(RenderingEngine* renderingEngine);
	void SetPhysicsEngine(PhysicsEngine* physicsEngine);
    void SetAudioEngine(AudioEngine* audioEngine);
    void AddToHierarchyUI(DebugUIWindow* window, TreeNode* node, bool isRoot = false);

	Transform* GetTransform()
	{
	    return &m_transform;
	}

    template <typename T>
	T* GetComponent()
	{
		auto it = m_components.find(std::type_index(typeid(T)));
		if (it != m_components.end())
		{
			return dynamic_cast<T*>(it->second);
		}
		std::cout << name << " doesn't have component " << typeid(T).name() << std::endl;
		return nullptr;
	}

	GameObject* GetChild(unsigned int index) const
	{
		if(index >= m_children.size())
			return nullptr;
		return m_children[index];
	}

    const std::vector<GameObject*>& GetChildren() const
    {
        return m_children;
    }

	unsigned int GetNumChildren() const
	{
		return m_children.size();
	}

	std::string name;
private:
    Transform m_transform;
	std::vector<GameObject*> m_children;
	std::map<std::type_index, GameComponent*> m_components;
	RenderingEngine* m_renderingEngine;
	PhysicsEngine* m_physicsEngine;
    AudioEngine* m_audioEngine;

    //Temporary
    mutable bool m_firstSelect;
    bool m_hierarchySelected;
    DebugUIWindow* m_hierarchyWindow;
};

#endif // !GAMEOBJECT_H
