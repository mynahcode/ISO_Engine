#include <IsoEngine.h>
#include <IsoEngine/Core/EntryPoint.h>

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "IsoEditorLayer.h"

namespace IE
{
	class IsoEngineEditor : public Application
	{
	public:
		IsoEngineEditor()
			: Application("IsoEngine Editor")
		{
			PushLayer(new IsoEditorLayer());
		}

		~IsoEngineEditor()
		{

		}
	};

	Application* CreateApplication()
	{
		return new IsoEngineEditor();
	}
}