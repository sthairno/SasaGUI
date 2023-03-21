#include <Siv3D.hpp> // OpenSiv3D v0.6.7

namespace SasaGUI
{
	namespace detail
	{
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
	};

	class GUIManager
	{
	public:

		GUIManager();

		static GUIManager& FromAddon(StringView name);

	public:

		// Frame

		void frameBegin();

		void frameEnd();

		// Window

		void windowBegin(StringView name, WindowFlag flags = WindowFlag::None);

		void windowEnd();

		const Window& getDefaultWindow() { return *m_defaultWindow; }

		const Window& getCurrentWindow() { return *m_stack.back(); }

	private:
		
		std::unique_ptr<detail::GUIImpl> m_impl;

		std::vector<Window*> m_stack;

		Window* m_defaultWindow;

	public:

		~GUIManager();
	};

	class GUIAddon : IAddon
	{
	public:

		GUIManager gui;

	private:

		bool init() override { return true; }

		bool update() override { return true; }

		void draw() const override {}

		void postPresent() override {}
	};
}
