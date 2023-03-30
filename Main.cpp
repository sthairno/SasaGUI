#include <Siv3D.hpp> // OpenSiv3D v0.6.7
#include "SasaGUI/SasaGUI.hpp"

void Main()
{
	Scene::SetBackground(ColorF{ 0.6, 0.8, 0.7 });

	SasaGUI::GUIManager gui;
	size_t counter = 0;
	bool showTextBox = true;

	Texture texture(Emoji{ U"🎍" });
	bool checked = false;
	int score = 0;

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
		gui.label(U"Label:"); gui.sameLine(); gui.label(U"test", HSV{ Periodic::Sawtooth0_1(3s) * 360 });
		gui.label(U"Image:"); gui.sameLine(); gui.image(texture);
		gui.label(U"CheckBox:"); gui.sameLine(); gui.checkbox(checked, U"HogeHoge");
		gui.label(U"RadioButton:");
		gui.radiobutton(score, 0, U"0"); gui.sameLine();
		gui.radiobutton(score, 1, U"1"); gui.sameLine();
		gui.radiobutton(score, 2, U"2");
		gui.windowEnd();

		gui.windowBegin(U"test2");
		gui.dummy({ 60, 80 }); gui.sameLine(); gui.dummy({ 100, 100 });
		gui.dummy({ 100, 100 });
		gui.windowEnd();

		gui.frameEnd();
	}
}
