#include <Siv3D.hpp> // OpenSiv3D v0.6.7
#include "SasaGUI/SasaGUI.hpp"

void TopRightWindow()
{
	auto& gui = SasaGUI::Get();

	gui.windowBegin(U"TopRightWindow", SasaGUI::WindowFlag::NoTitlebar | SasaGUI::WindowFlag::AutoResize);
	gui.setWindowPos(Arg::topRight = Scene::Rect().tr() + Vec2{ -10, 10 });
	gui.label(U"TopRight");
	gui.windowEnd();
}

void Main()
{
	Scene::SetBackground(ColorF{ 0.6, 0.8, 0.7 });

	SasaGUI::RegisterAddon();
	auto& gui = SasaGUI::Get();

	Stopwatch repeatStw(StartImmediately::Yes);
	size_t counter = 0;
	Texture texture(Emoji{ U"🎍" });
	bool checked = false;
	int score = 0;
	double barValue = 0;
	HSV color = Palette::Red;
	double value = 0;
	bool toggleSwitchValue = false;

	while (System::Update())
	{
		ClearPrint();

		if (repeatStw.elapsed() > 1s)
		{
			barValue = RandomClosed(0.0, 1.0);
			repeatStw.restart();
		}

		gui.label(U"Label:"); gui.sameLine(); gui.label(U"test", HSV{ Periodic::Sawtooth0_1(3s) * 360 });
		gui.label(U"Image:"); gui.sameLine(); gui.image(texture);
		gui.label(U"CheckBox:"); gui.sameLine(); gui.checkbox(checked, U"HogeHoge");
		gui.label(U"RadioButton:");
		gui.radiobutton(score, 0, U"0"); gui.sameLine();
		gui.radiobutton(score, 1, U"1"); gui.sameLine();
		gui.radiobutton(score, 2, U"2");
		gui.label(U"Tab:");
		{
			switch (gui.tab(U"TabDemo", { U"Tab 1", U"Tab 2", U"Tab 3" }))
			{
			case 0: gui.label(U"Hoge (0)"); break;
			case 1: gui.label(U"Fuga (1)"); break;
			case 2: gui.label(U"FooBar (2)"); break;
			}
		}
		gui.label(U"ProgressBar:");
		gui.progressbar(barValue);
		gui.label(U"Button:"); gui.sameLine();
		if (gui.button(U"Click here"))
		{
			counter++;
		}
		gui.sameLine();
		gui.label(Format(counter));
		gui.label(U"SimpleTextBox:"); gui.sameLine();
		gui.simpleTextBox(U"test");
		gui.label(U"SimpleColorPicker:");
		gui.simpleColorpicker(color);
		gui.label(U"SimpleSlider:");
		gui.simpleSlider(value);
		gui.label(U"ToggleSwitch:"); gui.sameLine();
		gui.toggleSwitch(toggleSwitchValue);

		TopRightWindow();
	}
}
