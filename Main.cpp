#include <Siv3D.hpp> // OpenSiv3D v0.6.7
#include "SasaGUI/SasaGUI.hpp"

void Main()
{
	Scene::SetBackground(ColorF{ 0.6, 0.8, 0.7 });

	SasaGUI::GUIManager gui;
	
	while (System::Update())
	{
		gui.frameBegin();

		if (gui.button(U"Click here"))
		{
			Print << U"Clicked!";
		}

		gui.windowBegin(U"test1");
		gui.dummy({ 300, 200 });
		gui.dummy({ 100, 50 });
		gui.windowEnd();

		if (Periodic::Square0_1(1s) == 0.0)
		{
			gui.windowBegin(U"test2");
			gui.windowEnd();
		}

		gui.frameEnd();
	}
}
