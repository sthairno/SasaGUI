#include <Siv3D.hpp> // OpenSiv3D v0.6.7
#include "SasaGUI/SasaGUI.hpp"

void Main()
{
	Scene::SetBackground(ColorF{ 0.6, 0.8, 0.7 });

	SasaGUI::GUIManager gui;
	
	while (System::Update())
	{
		gui.frameBegin();

		gui.windowBegin(U"test");
		gui.windowEnd();

		gui.windowBegin(U"test2");
		gui.windowEnd();

		gui.frameEnd();
	}
}
