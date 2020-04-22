#include <Siv3D.hpp>
#include "SasaGUI.hpp"

void Main()
{
	Scene::SetBackground(Palette::White);

	SasaGUI::GUIManager gui;

	bool enabled = true;

	bool checked = false;
	int32 radioItem = 0;
	String dropDownText;
	String text1 = U"";
	String text2 = U"";
	Texture barsIcon(Icon(0xf0c9, 20));
	double spinBoxVal = 20;
	int32 sliderVal = 20;
	HSV hsvCol = Palette::Red;
	Texture windmill(U"example/windmill.png");

	while (System::Update())
	{
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
				//Tab
				gui.groupBegin(U"Tab", true);
				{
					switch (gui.tab({ U"Tab1",U"Tab2",U"Tab3" }))
					{
					case 0:
						gui.newLine();
						gui.label(U"Item1");
						break;
					case 1:
						gui.newLine();
						gui.label(U"Item2");
						break;
					case 2:
						gui.newLine();
						gui.label(U"Item3");
						break;
					}
				}
				gui.groupEnd(); gui.newLine();

				//CheckBox
				gui.groupBegin(U"CheckBox", true);
				{
					gui.checkBox(checked, U"Checked");
				}
				gui.groupEnd(); gui.newLine();

				//RadioButton
				gui.groupBegin(U"RadioButton", true);
				{
					gui.radioButton(radioItem, 0, U"Item0"); gui.newLine();
					gui.radioButton(radioItem, 1, U"Item1"); gui.newLine();
					gui.radioButton(radioItem, 2, U"Item2"); gui.newLine();
				}
				gui.groupEnd(); gui.newLine();

				//Label
				gui.groupBegin(U"Label", true);
				{
					gui.label(U"HogeHoge");
				}
				gui.groupEnd(); gui.newLine();

				//DropDownList
				gui.groupBegin(U"DropDownList", true);
				{
					gui.dropdownList(dropDownText, { U"ValueA",U"ValueB",U"ValueC" }); gui.newLine();
					gui.label(U"dropDownText:{}"_fmt(dropDownText));
				}
				gui.groupEnd(); gui.newLine();

				//TextBox
				gui.groupBegin(U"TextBox", true);
				{
					gui.textBox(text1, U"1行", SasaGUI::TextInputFlag::All); gui.newLine();
					gui.textBox(text2, U"複数行", SasaGUI::TextInputFlag::All | SasaGUI::TextInputFlag::MultiLine);
				}
				gui.groupEnd(); gui.newLine();

				//Button
				gui.groupBegin(U"Button", true);
				{
					if (gui.button(U"Button"))
					{
						gui.newLine();
						Print << U"Clicked(Text)";
					}
					gui.newLine();
					if (gui.button(barsIcon, Palette::Gray))
					{
						gui.newLine();
						Print << U"Clicked(Image)";
					}
				}
				gui.groupEnd(); gui.newLine();

				//SpinBox
				gui.groupBegin(U"SpinBox", true);
				{
					gui.spinBox(spinBoxVal, 0.0, 100.0, 0.1);
				}
				gui.groupEnd(); gui.newLine();

				//Slider
				gui.groupBegin(U"Slider", true);
				{
					gui.slider(sliderVal, 0, 100); gui.newLine();
					gui.label(U"sliderVal:{}"_fmt(sliderVal));
				}
				gui.groupEnd(); gui.newLine();

				//ProgressBar
				gui.groupBegin(U"ProgressBar", true);
				{
					gui.progressBar(Periodic::Sine0_1(30s), 0.0, 1.0);
				}
				gui.groupEnd(); gui.newLine();

				//Split
				gui.groupBegin(U"Split", true);
				{
					gui.split(); gui.newLine();
					gui.space(SizeF(200, 0));
				}
				gui.groupEnd(); gui.newLine();

				//ColorPicker
				gui.groupBegin(U"ColorPicker", true);
				{
					gui.colorPicker(hsvCol); gui.newLine();
					gui.label(U"H:{},S:{},V:{}"_fmt(hsvCol.h, hsvCol.s, hsvCol.v), ColorF(hsvCol));
				}
				gui.groupEnd(); gui.newLine();

				//Image
				gui.groupBegin(U"Image", true);
				{
					gui.image(windmill);
				}
				gui.groupEnd(); gui.newLine();
			}
			gui.groupEnd();

			gui.windowEnd();
		}
		gui.windowEnd();

		gui.showDebugWindow();

		gui.frameEnd();
	}
}