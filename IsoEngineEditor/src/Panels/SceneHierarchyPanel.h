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
		void ResetSelectionContext();

		void OnImGuiRender();

		Entity GetSelectionContext() const { return m_SelectionContext; }

	private:
		void DrawEntityNode(Entity entity);
		void DrawEntityComponents(Entity entity);

		Ref<Scene> m_Context;
		Entity m_SelectionContext;
		Entity m_PrevSelectionContext;
	};

}