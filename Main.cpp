#include <Siv3D.hpp> // OpenSiv3D v0.6.7
#include "SasaGUI/SasaGUI.hpp"

void Main()
{
	Scene::SetBackground(ColorF{ 0.6, 0.8, 0.7 });

	SasaGUI::GUIManager gui;
	size_t counter = 0;
	bool showTextBox = true;

	while (System::Update())
	{
		ClearPrint();
		gui.frameBegin();

		showTextBox ^= KeyControl.down();

		if (gui.button(U"Click here"))
		{
			counter++;
		}
		Print << counter;

		if (showTextBox)
		{
			Print << gui.simpleTextBox(U"test").text;
		}

		gui.windowBegin(U"test1");
		if (gui.button(U"Click here"))
		{
			counter++;
		}
		gui.windowEnd();

		gui.windowBegin(U"test2");
		gui.dummy({ 200, 180 });
		gui.windowEnd();

		gui.frameEnd();
	}
}
