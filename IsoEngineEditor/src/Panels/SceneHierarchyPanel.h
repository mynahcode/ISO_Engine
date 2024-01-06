#pragma once

#include "IsoEngine/Core/IsoMacros.h"
#include "IsoEngine/Scene/Scene.h"
#include "IsoEngine/Scene/Entity.h"

namespace IE
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& context);

		void SetContext(const Ref<Scene>& context);
		void SetSelectedEntity(Entity entity);

		void OnImGuiRender();

	private:
		void DrawEntityNode(Entity entity);
		void DrawEntityComponents(Entity entity);

		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};

}