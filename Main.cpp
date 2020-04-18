#include <Siv3D.hpp>
#include "SasaGUI.hpp"

void Main()
{
	Scene::SetBackground(Palette::White);

	SasaGUI::GUIManager gui;

	String text;
	String text2;
	HSV color = Palette::Red;
	double valDouble = 0;
	int valInt = 0;
	bool enabled = true;
	bool checked;

	Texture windmill(U"example/windmill.png");

	while (System::Update())
	{
		ClearPrint();
		gui.frameBegin();

		if (gui.button(U"Close"))
		{
			System::Exit();
		}

		gui.windowBegin(U"Preview");
		{
			gui.checkBox(enabled, U"Enabled");
			gui.newLine();
			gui.groupBegin(U"", true, enabled);
			{
				gui.tab({ U"Tab1",U"Tab2",U"Tab3" }); gui.newLine();
				gui.checkBox(checked, U"Checked"); gui.newLine();
				gui.radioButton(checked, false, U"!Checked"); gui.newLine();
				gui.label(U"abc"); gui.newLine();
				gui.dropdownList(text, { U"ValueA",U"ValueB",U"ValueC" }); gui.newLine();
				gui.textBox(text); gui.newLine();
				gui.textBox(text2, U"", SasaGUI::All | SasaGUI::MultiLine); gui.newLine();
				if (gui.button(U"Button"))
				{
					Print << U"Click";
				}
				gui.newLine();
				gui.spinBox(valDouble, 0.0, 100.0, 0.1); gui.newLine();
			}
			gui.groupEnd();
			gui.groupBegin(U"", true, enabled);
			{
				gui.slider(valInt, 0, 100); gui.label(Format(valInt)); gui.newLine();
				gui.progressBar(valDouble, 0.0, 100.0); gui.newLine();
				gui.split(); gui.newLine();
				gui.colorPicker(color);
			}
			gui.groupEnd(); gui.newLine();
			gui.image(windmill);
			gui.windowEnd();
		}
		gui.windowEnd();

		gui.showDebugWindow();

		gui.frameEnd();
	}
}