#include <Nazel.h>
#include <Nazel\Core\EntryPoint.h>

#include "EditorLayer.h"

namespace Nazel{
class NazelEditor : public Application
{
public:
	NazelEditor()
		: Application("Nazel Editor")
	{
		PushLayer(new EditorLayer());
	}
	~NazelEditor() = default;
};

Application* CreateApplication()
{
	return new NazelEditor();
}
}

