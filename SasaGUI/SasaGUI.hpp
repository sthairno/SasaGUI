#include <Siv3D.hpp> // OpenSiv3D v0.6.7

namespace SasaGUI
{
	namespace detail
	{
		static constexpr StringView DefaultAddonName = U"SasaGUI";

		class WindowImpl;

		class GUIImpl;
	}

	enum class WindowFlag : int32
	{
		/// <summary>
		/// 通常のウィンドウ
		/// </summary>
		None = 0,
		/// <summary>
		/// タイトルバーを表示しない
		/// </summary>
		NoTitlebar = 1 << 0,
		/// <summary>
		/// サイズ変更をしない
		/// </summary>
		NoResize = 1 << 1,
		/// <summary>
		/// 移動をしない
		/// </summary>
		NoMove = 1 << 2,
		/// <summary>
		/// 背景を表示しない
		/// </summary>
		NoBackground = 1 << 3,
		/// <summary>
		/// スクロールバーを表示しない
		/// </summary>
		NoScrollbar = 1 << 4,
		/// <summary>
		/// 自動リサイズ
		/// </summary>
		AutoResize = 1 << 5,
		/// <summary>
		/// 常に手前に表示
		/// </summary>
		AlwaysForeground = 1 << 6,
		/// <summary>
		/// フォーカスしない
		/// </summary>
		NoFocus = 1 << 7,
		/// <summary>
		/// ウィンドウを隠す
		/// </summary>
		Hide = 1 << 8,
		/// <summary>
		/// 無効
		/// </summary>
		Disable = 1 << 9,

		Debug = 1 << 31
	};
	DEFINE_BITMASK_OPERATORS(WindowFlag);

	struct Window
	{
		String displayName;

		Rect rect;

		WindowFlag flags;

		Size maxSize{ std::numeric_limits<Size::value_type>::max(), std::numeric_limits<Size::value_type>::max() };

		int32 space = 5;

		int32 padding = 10;

		bool defined = false;

		Rect lineRect{ padding, padding, 0, 0 };

		bool requestMoveToFront = false;

		Font font = SimpleGUI::GetFont();

		bool sameLine = false;
	};

	class IControl
	{
	public:

		virtual Size computeSize() const = 0;

		virtual void update(Rect rect, Optional<Vec2> cursorPos) = 0;

		virtual void draw() const = 0;

		virtual ~IControl() { };
	};

	class GUIManager
	{
	public:

		GUIManager();

	public:

		// Frame

		void frameBegin();

		void frameEnd();

		// Window

		void windowBegin(StringView name, WindowFlag flags = WindowFlag::None);

		void windowEnd();

		void setWindowSize(Size size);

		inline void setWindowPos(Point pos) { setWindowPos(pos, { 0.0, 0.0 }); }

		inline void setWindowPos(Arg::topLeft_<Vec2> topLeft) { setWindowPos(*topLeft, { 0.0, 0.0 }); }

		inline void setWindowPos(Arg::topCenter_<Vec2> topCenter) { setWindowPos(*topCenter, { 0.5, 0.0 }); }

		inline void setWindowPos(Arg::topRight_<Vec2> topRight) { setWindowPos(*topRight, { 1.0, 0.0 }); }

		inline void setWindowPos(Arg::leftCenter_<Vec2> leftCenter) { setWindowPos(*leftCenter, { 0.0, 0.5 }); }

		inline void setWindowPos(Arg::center_<Vec2> center) { setWindowPos(*center, { 0.5, 0.5 }); }

		inline void setWindowPos(Arg::rightCenter_<Vec2> rightCenter) { setWindowPos(*rightCenter, { 1.0, 0.5 }); }

		inline void setWindowPos(Arg::bottomLeft_<Vec2> bottomLeft) { setWindowPos(*bottomLeft, { 1.0, 1.0 }); }

		inline void setWindowPos(Arg::bottomCenter_<Vec2> bottomCenter) { setWindowPos(*bottomCenter, { 0.5, 1.0 }); }

		inline void setWindowPos(Arg::bottomRight_<Vec2> bottomRight) { setWindowPos(*bottomRight, { 0.0, 1.0 }); }

		const Window& getDefaultWindow() const;

		const Window& getCurrentWindow() const;

		// Controls

		void sameLine();

		void dummy(Size size);

		bool button(StringView label);

		TextEditState& simpleTextBox(StringView id, double width = 200, const Optional<size_t>& maxChars = unspecified);

		void label(StringView text, ColorF color = Palette::Black);

		void image(Texture texture, ColorF diffuse = Palette::White);

		void image(TextureRegion texture, ColorF diffuse = Palette::White);

		bool checkbox(bool& checked, StringView label = U"");

		bool radiobutton(bool selected, StringView label = U"");

		template<class T, class U = T>
		bool radiobutton(T& target, const U& value, StringView label = U"")
		{
			bool clicked = radiobutton(target == value, label);
			if (clicked)
			{
				target = value;
			}
			return clicked;
		}

		size_t& tab(StringView id, Array<String> tabNames);

		// void dropdown();

		bool simpleColorpicker(HSV& value);

		bool simpleSlider(double& value, double width = 120);

		// template<class T>
		// void spinbox(T& target, T step = static_cast<T>(1), T min = std::numeric_limits<T>::min(), T max = std::numeric_limits<T>::max());

		// void split();

		void progressbar(double value, int32 width = 200);

		bool toggleSwitch(bool& value);

		void custom(std::shared_ptr<IControl> control);

	private:
		
		std::unique_ptr<detail::GUIImpl> m_impl;

		std::vector<detail::WindowImpl*> m_stack;

		detail::WindowImpl* m_defaultWindow;

		detail::WindowImpl& getDefaultWindowImpl() { return *m_defaultWindow; }

		detail::WindowImpl& getCurrentWindowImpl() { return *m_stack.back(); }

		void setWindowPos(Vec2 pos, Vec2 offset);

	public:

		~GUIManager();
	};

	bool RegisterAddon(const StringView name, int32 priority = 0);

	bool RegisterAddon(const StringView name, int32 updatePriority, int32 drawPriority);

	inline bool RegisterAddon(int32 priority = 0)
	{
		return RegisterAddon(detail::DefaultAddonName, priority);
	}

	inline bool RegisterAddon(int32 updatePriority, int32 drawPriority)
	{
		return RegisterAddon(detail::DefaultAddonName, updatePriority, drawPriority);
	}

	GUIManager& Get(const StringView name = detail::DefaultAddonName);
}
