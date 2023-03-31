#include "SasaGUI.hpp"

namespace SasaGUI
{
	namespace Config
	{
		struct CheckBox
		{
			constexpr static double BoxScale = 0.8;
			constexpr static std::array<Vec2, 3> CheckLine{
				Vec2{-0.5,-0.14},
				Vec2{-0.21,0.44},
				Vec2{0.64,-0.64}
			};
			constexpr static double CheckLineScale = 0.6;
			constexpr static double CheckLineTickness = 0.28;
		};

		struct RadioButton
		{
			constexpr static double CircleScale = 0.8;
			constexpr static double InnerCircleScale = 0.6;
		};

		struct Tab
		{
			constexpr static int32 FrameThickness = 1;
			constexpr static int32 TabR = 5;
			constexpr static int32 TabSpace = 1;
			constexpr static ColorF FrameColor = Palette::Black;
			constexpr static ColorF TabColor = ColorF{ 0.8 };
			constexpr static ColorF SelectedTabColor = ColorF::Zero();
		};
	}

	using WindowImpl = detail::WindowImpl;
	using GUIImpl = detail::GUIImpl;

	class Layer;
	class InputContext;

	enum class WindowLayer : int32
	{
		Background,
		Normal,
		Foreground
	};

	class InputContext
	{
	public:

		size_t hoveredItemId() const { return m_hoveredItemId; }

		bool hover(WindowImpl& window);

		bool capture(WindowImpl& window, bool capture = true);

		Optional<Vec2> getCursorPos();

		Optional<Vec2> getCursorPos(WindowImpl& window);

	private:

		friend Layer;
		friend GUIImpl;

		size_t m_hoveredItemId = 0;

		bool m_captured = false;

		Vec2 m_cursorPos{ 0, 0 };

		void reset();

		void frameBegin();
	};

	enum class WindowState
	{
		Default, Moving, Resizing
	};

	namespace detail
	{
		class WindowImpl : Uncopyable
		{
		public:

			using ControlGenerator = std::function<std::shared_ptr<IControl>()>;

			WindowImpl(InputContext& input, StringView id, StringView name);

		public:

			Window window;

			const String& id() const { return m_id; }

			size_t randomId() const { return m_randomId; }

			const Rect& rect() const { return window.rect; }

			int32 titlebarHeight() const { return window.font.height(); }

			Size minSize() const
			{
				if (window.flags & WindowFlag::NoBackground)
				{
					return { 0, 0 };
				}
				else if (window.flags & WindowFlag::NoTitlebar)
				{
					return { 20, 20 };
				}
				else
				{
					return { 20,  Max(20, titlebarHeight()) };
				}
			}

			void frameBegin(InputContext& input);

			void frameEnd();

			template<std::derived_from<IControl> ControlType>
			inline ControlType& nextStatelessControl(std::shared_ptr<ControlType> control)
			{
				return reinterpret_cast<ControlType&>(nextControlImpl(control));
			}

			template<std::derived_from<IControl> ControlType>
			inline ControlType& nextStatefulControl(size_t id)
			{
				s3d::detail::HashCombine(id, typeid(ControlType).hash_code());
				return reinterpret_cast<ControlType&>(nextStatefulControlImpl(
					id,
					[]() ->std::shared_ptr<IControl> { return std::make_shared<ControlType>(); }
				));
			}

			template<std::derived_from<IControl> ControlType>
			inline ControlType& nextStatefulControl(size_t id, ControlGenerator generator)
			{
				s3d::detail::HashCombine(id, typeid(ControlType).hash_code());
				return reinterpret_cast<ControlType&>(nextStatefulControlImpl(id, generator));
			}

			void draw() const;

		private:

			String m_id;

			size_t m_randomId;

			InputContext* m_input;

			WindowState m_state = WindowState::Default;

			bool m_mouseOver = false;

			bool m_firstFrame = true;

			Optional<Rect> m_titlebarRect;

			Optional<Rect> m_contentRect;

			Size m_contentSize{ 0, 0 };

			Array<std::shared_ptr<IControl>> m_controls;

			std::map<size_t, std::shared_ptr<IControl>> m_savedControls;

			IControl& nextControlImpl(std::shared_ptr<IControl> control);

			IControl& nextStatefulControlImpl(size_t id, ControlGenerator generator);

			void updateRect();

			void updateRect(Rect rect);
		};
	}

	class Layer
	{
	public:

		using container_type = HashTable<String, std::unique_ptr<WindowImpl>>;

		Layer(WindowLayer type)
			: type(type)
		{ }

	public:

		const WindowLayer type;

		const container_type& container() const { return m_container; }

		void frameBegin(InputContext& input);

		void frameEnd();

		WindowImpl& defineWindow(StringView id);

	private:

		container_type m_container;

		std::list<String> m_windowOrder;

		InputContext* m_input;

		container_type::iterator createWindow(StringView id, StringView name);

		void deleteUnusedWindow();
	};

	namespace detail
	{
		class GUIImpl
		{
		public:

			std::array<Layer, 3>& layers() { return m_layers; }

			void frameBegin();

			void frameEnd();

			Layer& getLayer(WindowLayer layer);

		private:

			std::array<Layer, 3> m_layers{
				Layer{ WindowLayer::Background },
				Layer{ WindowLayer::Normal },
				Layer{ WindowLayer::Foreground }
			};

			InputContext m_input;
		};
	}

	static StringView ToString(WindowLayer layer)
	{
		return std::array{ U"Background", U"Normal", U"Foreground" } [static_cast<int32>(layer)] ;
	}

	// InputContext

	bool InputContext::hover(WindowImpl& window)
	{
		if (m_hoveredItemId == 0)
		{
			m_hoveredItemId = window.randomId();
			return true;
		}

		if (m_hoveredItemId == window.randomId())
		{
			return true;
		}

		return false;
	}

	bool InputContext::capture(WindowImpl& window, bool capture)
	{
		if (m_hoveredItemId == 0 ||
			m_hoveredItemId == window.randomId())
		{
			m_captured = capture;
			if (capture)
			{
				m_hoveredItemId = window.randomId();
			}
			else
			{
				m_hoveredItemId = 0;
			}
			return true;
		}

		return false;
	}

	Optional<Vec2> InputContext::getCursorPos()
	{
		if (m_hoveredItemId == 0)
		{
			return m_cursorPos;
		}

		return none;
	}

	Optional<Vec2> InputContext::getCursorPos(WindowImpl& window)
	{
		if (m_hoveredItemId == 0 ||
			m_hoveredItemId == window.randomId())
		{
			return m_cursorPos;
		}

		return none;
	}

	void InputContext::reset()
	{
		m_hoveredItemId = 0;
		m_captured = false;
	}

	void InputContext::frameBegin()
	{
		m_cursorPos = Cursor::PosF();
		if (not m_captured)
		{
			m_hoveredItemId = 0;
		}
	}

	// WindowImpl

	WindowImpl::WindowImpl(InputContext& input, StringView id, StringView name)
		: window({
			.displayName = String{ name },
			.rect = { 0, 0, 100, 100 }
		})
		, m_controls()
		, m_input(&input)
		, m_id(id)
		, m_randomId(RandomUint64())
	{
		updateRect();
	}

	void WindowImpl::frameBegin(InputContext& input)
	{
		m_input = &input;

		Rect newRect = rect();
		if (auto cursor = input.getCursorPos(*this))
		{
			switch (m_state)
			{
			case SasaGUI::WindowState::Default:
				if (m_titlebarRect->contains(*cursor))
				{
					input.hover(*this);
					if (MouseL.down())
					{
						m_state = WindowState::Moving;
						window.requestMoveToFront = true;
						input.capture(*this);
					}
				}
				if (m_contentRect->contains(*cursor))
				{
					input.hover(*this);
					if (MouseL.down())
					{
						window.requestMoveToFront = true;
					}
				}
				break;
			case SasaGUI::WindowState::Moving:
				if (MouseL.pressed())
				{
					newRect.pos += Cursor::Delta();
				}
				else
				{
					m_state = WindowState::Default;
					input.capture(*this, false);
				}
				break;
			case SasaGUI::WindowState::Resizing:
				break;
			}
		}
		updateRect(newRect);

		window.defined = false;
		window.lineRect = { window.padding, window.padding, 0, 0 };
		m_contentSize = { 0, 0 };
		m_controls.clear();
		window.sameLine = false;
	}

	void WindowImpl::frameEnd()
	{
		Rect newRect = rect();
		if (not m_controls.empty())
		{
			m_contentSize += Size{ window.padding, window.padding };
		}
		if (window.flags & WindowFlag::AutoResize ||
			m_firstFrame)
		{
			newRect.size = { 0, 0 };
			if (not (window.flags & WindowFlag::NoBackground) &&
				not (window.flags & WindowFlag::NoTitlebar))
			{
				newRect.h += titlebarHeight();
			}
			newRect.size += m_contentSize;

			Size min = minSize();
			newRect.w = Max(newRect.w, min.x);
			newRect.h = Max(newRect.h, min.y);
		}
		updateRect(newRect);
		draw();

		m_firstFrame = false;
	}

	void WindowImpl::draw() const
	{
		if (not (window.flags & WindowFlag::NoBackground))
		{
			RoundRect{ window.rect, 10 }
				.drawFrame(0, 1, Palette::Lightgray)
				.draw(ColorF{ 0.9 });

			if (not (window.flags & WindowFlag::NoTitlebar))
			{
				m_titlebarRect
					->rounded(10, 10, 0, 0)
					.draw(Palette::Lightgray);
				window.font(window.displayName)
					.draw(Arg::topCenter = window.rect.topCenter(), Palette::Black);
			}
		}

		{
			ScopedViewport2D _{ *m_contentRect };
			for (auto& control : m_controls)
			{
				control->draw();
			}
		}
	}

	IControl& WindowImpl::nextControlImpl(std::shared_ptr<IControl> control)
	{
		m_controls.emplace_back(control);

		Rect localRect{ 0, 0, control->computeSize() };
		if (window.sameLine)
		{
			localRect.pos = window.lineRect.tr();
			localRect.x += window.space;
		}
		else
		{
			Point newLinePos = window.lineRect.bl();
			if (window.lineRect.area() > 0.0)
			{
				newLinePos.y += window.space;
			}

			localRect.pos = newLinePos;

			window.lineRect = { newLinePos, 0, 0 };
		}
		window.sameLine = false;

		{
			Point br = localRect.br();
			m_contentSize.x = Max(m_contentSize.x, br.x);
			m_contentSize.y = Max(m_contentSize.y, br.y);
			window.lineRect.w = Max(window.lineRect.w, br.x - window.lineRect.x);
			window.lineRect.h = Max(window.lineRect.h, br.y - window.lineRect.y);
		}

		Optional<Vec2> localCursorPos;
		if (auto globalCursorPos = m_input->getCursorPos(*this))
		{
			if (m_state == WindowState::Default &&
				m_contentRect.has_value() &&
				m_contentRect->contains(*globalCursorPos))
			{
				localCursorPos = globalCursorPos->movedBy(-m_contentRect->pos);
			}
		}
		control->update(localRect, localCursorPos);

		return *control;
	}

	IControl& WindowImpl::nextStatefulControlImpl(size_t id, ControlGenerator generator)
	{
		s3d::detail::HashCombine(id, m_randomId);

		auto itr = m_savedControls.find(id);
		if (itr == m_savedControls.end())
		{
			auto [tmp, _] = m_savedControls.emplace(
				id,
				generator()
			);
			itr = tmp;
		}

		return nextControlImpl(itr->second);
	}

	void WindowImpl::updateRect()
	{
		if (window.flags & WindowFlag::NoBackground ||
			window.flags & WindowFlag::NoTitlebar)
		{
			m_titlebarRect = { 0, 0, 0, 0 };
			m_contentRect = window.rect;
		}
		else
		{
			m_titlebarRect = {
				window.rect.pos,
				window.rect.w,
				titlebarHeight()
			};
			m_contentRect = {
				window.rect.x,
				window.rect.y + titlebarHeight(),
				window.rect.w,
				window.rect.h - titlebarHeight()
			};
		}
	}

	void WindowImpl::updateRect(Rect rect)
	{
		window.rect = rect;
		updateRect();
	}

	// Layer

	void Layer::frameBegin(InputContext& input)
	{
		m_input = &input;

		std::list<String> front;
		for (auto itr = m_windowOrder.rbegin(); itr != m_windowOrder.rend();)
		{
			auto& impl = *m_container.at(*itr);

			impl.frameBegin(*m_input);

			if (impl.window.requestMoveToFront)
			{
				front.emplace_back(std::move(*itr));
				m_windowOrder.erase(--(itr.base()));
				impl.window.requestMoveToFront = false;
			}
			else
			{
				itr++;
			}
		}
		m_windowOrder.splice(m_windowOrder.end(), std::move(front));
	}

	void Layer::frameEnd()
	{
		for (auto& id : m_windowOrder)
		{
			auto& window = *m_container.at(id);
			window.frameEnd();
		}
	}

	WindowImpl& Layer::defineWindow(StringView id)
	{
		auto itr = m_container.find(id);

		if (itr == m_container.end())
		{
			itr = createWindow(id, id);
		}

		auto& impl = *itr->second;
		impl.window.defined = true;
		return impl;
	}

	Layer::container_type::iterator Layer::createWindow(StringView id, StringView name)
	{
		auto [itr, _] = m_container.emplace(
				id,
				std::make_unique<WindowImpl>(*m_input, id, name)
		);
		m_windowOrder.emplace_back(String{ id });

		Console << U"[" << ToString(type) << U"][+] " << itr->first;

		return itr;
	}

	void Layer::deleteUnusedWindow()
	{
		m_windowOrder.remove_if([&, this](const String& id) {
			auto itr = m_container.find(id);

			if (itr->second->window.defined)
			{
				return false;
			}

			if (m_input->hoveredItemId() == itr->second->randomId())
			{
				m_input->reset();
			}

			m_container.erase(itr);

			Console << U"[" << ToString(type) << U"][-] " << id;
			return true;
		});
	}

	// GUIImpl

	void GUIImpl::frameBegin()
	{
		m_input.frameBegin();
		for (auto layerItr = m_layers.rbegin(); layerItr != m_layers.rend(); layerItr++)
		{
			layerItr->frameBegin(m_input);
		}
	}

	void GUIImpl::frameEnd()
	{
		for (auto& layer : m_layers)
		{
			layer.frameEnd();
		}
		Print << m_input.m_hoveredItemId << (m_input.m_captured ? U"🔒" : U"");
	}

	Layer& GUIImpl::getLayer(WindowLayer layer)
	{
		return m_layers[static_cast<int32>(layer)];
	}

	// GUIManager

	GUIManager::GUIManager()
		: m_impl(std::make_unique<GUIImpl>())
	{
		m_defaultWindow = &m_impl
			->getLayer(WindowLayer::Background)
			.defineWindow(U"DefaultWindow");
		m_defaultWindow->window.flags =
			WindowFlag::NoTitlebar
			| WindowFlag::NoResize
			| WindowFlag::NoMove
			| WindowFlag::NoBackground;
	}

	GUIManager& GUIManager::FromAddon(StringView name)
	{
		if (auto addon = Addon::GetAddon<GUIAddon>(name))
		{
			return addon->gui;
		}
		throw Error(U"GUIAddon (name: " + name + U") is not registered");
	}

	void GUIManager::frameBegin()
	{
		m_stack.clear();
		m_stack.push_back(m_defaultWindow);

		m_impl->frameBegin();

		m_defaultWindow = &m_impl
			->getLayer(WindowLayer::Background)
			.defineWindow(U"DefaultWindow");
		m_defaultWindow->window.rect = Scene::Rect();
	}

	void GUIManager::frameEnd()
	{
		m_impl->frameEnd();

		for (auto [idx, layer] : Indexed(m_impl->layers()))
		{
			const HSV color{ ((double)idx / 4 * 360), 0.8, 1.0 };
			for (auto& [id, impl] : layer.container())
			{
				auto& window = impl->window;

				window.rect.drawFrame(1, 0, color);
				Line{ window.rect.tl(), window.rect.br() }.draw(color);
				Line{ window.rect.bl(), window.rect.tr() }.draw(color);

				SimpleGUI::GetFont()(U"{}:{}"_fmt(id, window.displayName))
					.draw(Arg::bottomLeft = window.rect.tl(), color);
			}
		}
	}

	void GUIManager::windowBegin(StringView name, WindowFlag flags)
	{
		WindowLayer layer = flags & WindowFlag::AlwaysForeground
			? WindowLayer::Foreground
			: WindowLayer::Normal;
		auto windowImpl = &m_impl
			->getLayer(layer)
			.defineWindow(name);
		windowImpl->window.flags = flags;
		m_stack.push_back(windowImpl);
	}

	void GUIManager::windowEnd()
	{
		m_stack.pop_back();
	}

	const Window& GUIManager::getDefaultWindow() const
	{
		return m_defaultWindow->window;
	}

	const Window& GUIManager::getCurrentWindow() const
	{
		return m_stack.back()->window;
	}

	void GUIManager::sameLine()
	{
		getCurrentWindowImpl().window.sameLine = true;
	}

	GUIManager::~GUIManager()
	{ }

	// Dummy

	class Dummy : public IControl
	{
	public:

		Dummy(Size size)
			: m_size(size)
		{ }

	private:

		Size m_size;

		Rect m_rect;

		Size computeSize() const override
		{
			return m_size;
		}

		void update(Rect rect, Optional<Vec2>) override
		{
			m_rect = rect;
		}

		void draw() const
		{
			m_rect.drawFrame(1, 0, Palette::Black);
		}
	};

	void GUIManager::dummy(Size size)
	{
		getCurrentWindowImpl()
			.nextStatelessControl(std::make_shared<Dummy>(size));
	}

	// Button

	class Button : public IControl
	{
	public:

		Button(StringView label)
			: m_label(label)
		{ }

		bool clicked() const
		{
			return m_clicked;
		}

	private:

		String m_label;

		Rect m_rect;

		bool m_mouseOver = false;

		bool m_clicked = false;

		Size computeSize() const override
		{
			return SimpleGUI::ButtonRegion(m_label, { 0, 0 }).size.asPoint();
		}

		void update(Rect rect, Optional<Vec2> cursorPos) override
		{
			m_rect = rect;

			m_mouseOver = cursorPos && rect.contains(*cursorPos);
			m_clicked = m_mouseOver && MouseL.down();
		}

		void draw() const
		{
			Mat3x2 cursorMat = m_mouseOver
				? Mat3x2::Translate(Cursor::PosF())
				: Mat3x2::Translate(Cursor::PosF() + Vec2{ 1, 1 });
			const Transformer2D _{ Mat3x2::Translate(m_rect.pos), cursorMat, Transformer2D::Target::PushLocal };
			[[maybe_unused]]
			bool unused = SimpleGUI::Button(m_label, { 0, 0 }, m_rect.w);
		}
	};

	bool GUIManager::button(StringView label)
	{
		auto& button = getCurrentWindowImpl()
			.nextStatelessControl(std::make_shared<Button>(label));
		return button.clicked();
	}

	// SimpleTextBox

	class SimpleTextBox : public IControl
	{
	public:

		SimpleTextBox()
		{ }

	public:

		double width = 200;

		Optional<size_t> maxChars = unspecified;

		TextEditState& state()
		{
			return m_state;
		}

	private:

		mutable TextEditState m_state;

		Vec2 m_pos;

		double m_renderWidth;

		bool m_mouseOver = false;

		Size computeSize() const override
		{
			return SimpleGUI::TextBoxRegion(m_pos, width).size.asPoint();
		}

		void update(Rect rect, Optional<Vec2> cursorPos) override
		{
			m_pos = rect.pos;
			m_renderWidth = rect.w;
			m_mouseOver = cursorPos && rect.contains(*cursorPos);
		}

		void draw() const
		{
			Mat3x2 cursorMat = m_mouseOver
				? Mat3x2::Translate(Cursor::PosF())
				: Mat3x2::Translate(Cursor::PosF() + Vec2{ 1, 1 });
			const Transformer2D _{ Mat3x2::Translate(m_pos), cursorMat, Transformer2D::Target::PushLocal };
			SimpleGUI::TextBox(m_state, { 0, 0 }, m_renderWidth, maxChars);
		}
	};

	TextEditState& GUIManager::simpleTextBox(StringView id, double width, const Optional<size_t>& maxChars)
	{
		auto& textBox = getCurrentWindowImpl()
			.nextStatefulControl<SimpleTextBox>(id.hash());
		textBox.width = width;
		textBox.maxChars = maxChars;
		return textBox.state();
	}

	// Label

	class Label : public IControl
	{
	public:

		Label(StringView text, ColorF color)
			: m_text(SimpleGUI::GetFont()(text))
			, m_color(color)
		{ }

	private:

		DrawableText m_text;

		ColorF m_color;

		Vec2 m_pos;

		Size computeSize() const override
		{
			return m_text.region().size.asPoint();
		}

		void update(Rect rect, Optional<Vec2>) override
		{
			m_pos = rect.pos;
		}

		void draw() const
		{
			m_text.draw(m_pos, m_color);
		}
	};

	void GUIManager::label(StringView text, ColorF color)
	{
		getCurrentWindowImpl()
			.nextStatelessControl(std::make_shared<Label>(text, color));
	}

	// Image

	class Image : public IControl
	{
	public:

		Image(const Texture& texture, ColorF diffuse)
			: m_texture(texture)
			, m_diffuse(diffuse)
		{ }

		Image(const TextureRegion& texture, ColorF diffuse)
			: m_texture(texture)
			, m_diffuse(diffuse)
		{ }

	private:

		const std::variant<Texture, TextureRegion> m_texture;

		ColorF m_diffuse;

		Vec2 m_pos;

		Size computeSize() const override
		{
			return std::visit([](const auto& tex) {
				return tex.region(0.0, 0.0).size.asPoint();
			}, m_texture);
		}

		void update(Rect rect, Optional<Vec2>) override
		{
			m_pos = rect.pos;
		}

		void draw() const
		{
			std::visit([this](const auto& tex) {
				tex.draw(m_pos, m_diffuse);
			}, m_texture);
		}
	};

	void GUIManager::image(Texture texture, ColorF diffuse)
	{
		getCurrentWindowImpl()
			.nextStatelessControl(std::make_shared<Image>(texture, diffuse));
	}

	void GUIManager::image(TextureRegion texture, ColorF diffuse)
	{
		getCurrentWindowImpl()
			.nextStatelessControl(std::make_shared<Image>(texture, diffuse));
	}

	// CheckBox

	class CheckBox : public IControl
	{
	public:

		using Config = Config::CheckBox;

		CheckBox(bool& value, StringView label)
			: m_ref(&value)
			, m_labelText(SimpleGUI::GetFont()(label))
		{ }

		bool valueChanged() const { return m_valueChanged; }

	private:

		bool* m_ref;

		DrawableText m_labelText;

		bool m_value = false;

		bool m_valueChanged = false;

		RectF m_checkRect;

		Vec2 m_labelLeftCenter;

		Size computeSize() const override
		{
			Size labelSize = m_labelText.region().size.asPoint();
			return { labelSize.x + checkboxSize(), labelSize.y };
		}

		void update(Rect rect, Optional<Vec2> cursorPos) override
		{
			m_checkRect = RectF{ Arg::leftCenter = rect.leftCenter(), static_cast<RectF::size_type::value_type>(checkboxSize()) };
			m_labelLeftCenter = m_checkRect.rightCenter();

			if (cursorPos &&
				rect.contains(*cursorPos))
			{
				Cursor::RequestStyle(CursorStyle::Hand);
				m_valueChanged = MouseL.down();
				*m_ref ^= m_valueChanged;
			}

			m_value = *m_ref;
			m_ref = nullptr;
		}

		void draw() const override
		{
			m_checkRect.drawFrame(1, Palette::Black);
			if (m_value)
			{
				Vec2 center = m_checkRect.center();
				int32 size = checkboxSize();
				drawCheckLine(m_checkRect.center(), checkboxSize() * Config::CheckLineScale);
			}
			m_labelText
				.draw(Arg::leftCenter = m_labelLeftCenter, Palette::Black);
		}

		int32 checkboxSize() const
		{
			return static_cast<int32>(m_labelText.font.height() * Config::BoxScale);
		}

		void drawCheckLine(Vec2 center, double scale) const
		{
			LineString line(Arg::reserve = Config::CheckLine.size());
			for (Vec2 p : Config::CheckLine)
			{
				line.push_back(center + p * scale);
			}
			line.draw(LineStyle::Uncapped, Config::CheckLineTickness * scale, Palette::Black);
		}
	};

	bool GUIManager::checkbox(bool& checked, StringView label)
	{
		auto& checkbox = getCurrentWindowImpl()
			.nextStatelessControl(std::make_shared<CheckBox>(checked, label));
		return checkbox.valueChanged();
	}

	// RadioButton

	class RadioButton : public IControl
	{
	public:

		using Config = Config::RadioButton;

		RadioButton(bool selected, StringView label)
			: m_selected(selected)
			, m_labelText(SimpleGUI::GetFont()(label))
		{ }

		bool clicked() const { return m_clicked; }

	private:

		bool m_selected;

		DrawableText m_labelText;

		bool m_clicked = false;

		Circle m_circle;

		Vec2 m_labelLeftCenter;

		Size computeSize() const override
		{
			Size labelSize = m_labelText.region().size.asPoint();
			return { labelSize.x + circleSize(), labelSize.y };
		}

		void update(Rect rect, Optional<Vec2> cursorPos) override
		{
			m_circle = { Arg::leftCenter = rect.leftCenter(), static_cast<RectF::size_type::value_type>(circleSize()) / 2 };
			m_labelLeftCenter = m_circle.right();

			if (cursorPos &&
				rect.contains(*cursorPos))
			{
				Cursor::RequestStyle(CursorStyle::Hand);
				m_clicked = MouseL.down();
				m_selected |= m_clicked;
			}
		}

		void draw() const override
		{
			m_circle.drawFrame(1, Palette::Black);
			if (m_selected)
			{
				Circle{ m_circle.center, m_circle.r * Config::InnerCircleScale }
					.draw(Palette::Black);
			}
			m_labelText
				.draw(Arg::leftCenter = m_labelLeftCenter, Palette::Black);
		}

		int32 circleSize() const
		{
			return static_cast<int32>(m_labelText.font.height() * Config::CircleScale);
		}
	};

	bool GUIManager::radiobutton(bool selected, StringView label)
	{
		auto& radioButton = getCurrentWindowImpl()
			.nextStatelessControl(std::make_shared<RadioButton>(selected, label));
		return radioButton.clicked();
	}

	// Tab

	class Tab : public IControl
	{
	public:

		using Config = Config::Tab;

		Tab(const Array<String>& tabNames, size_t firstIdx)
			: m_tabs(Arg::reserve = tabNames.size())
			, selectedIdx(firstIdx)
		{
			assert(tabNames);

			int32 tabHeight = 0;
			Point tabPos = { Config::FrameThickness, Config::FrameThickness };
			auto& font = SimpleGUI::GetFont();
			for (auto& name : tabNames)
			{
				auto& tab = m_tabs.emplace_back(TabItem{
					.label = font(name)
				});

				Size labelSize = tab.label.region().size.asPoint();
				tab.localRect.pos = tabPos;
				tab.localRect.w = labelSize.x + Config::TabR * 2;

				tabPos.x += tab.localRect.w
					+ Config::FrameThickness
					+ Config::TabSpace
					+ Config::FrameThickness;

				tabHeight = Max(tabHeight, labelSize.y);
			}
			for (auto& tab : m_tabs)
			{
				tab.localRect.h = tabHeight;
			}
		}

		size_t selectedIdx = 0;

	private:

		struct TabItem
		{
			Rect localRect;

			DrawableText label;

			Rect globalRect;

			bool mouseOver = false;
		};

		Array<TabItem> m_tabs;

		Size computeSize() const override
		{
			return m_tabs.back().localRect.br() + Size{ Config::FrameThickness, Config::FrameThickness };
		}

		void update(Rect rect, Optional<Vec2> cursorPos) override
		{
			for (auto [idx, tab] : IndexedRef(m_tabs))
			{
				tab.globalRect = tab.localRect.movedBy(rect.pos);
				tab.mouseOver = cursorPos && tab.globalRect.contains(*cursorPos);

				if (tab.mouseOver && MouseL.down())
				{
					selectedIdx = idx;
				}
			}
		}

		void draw() const override
		{
			for (auto [idx, tab] : IndexedRef(m_tabs))
			{
				bool selected = idx == selectedIdx;

				if (selected)
				{
					tab.globalRect
						.stretched(0, 0, Config::FrameThickness, 0)
						.rounded(Config::TabR, Config::TabR, 0, 0)
						.draw(Config::SelectedTabColor);
				}
				else
				{
					tab.globalRect
						.rounded(Config::TabR, Config::TabR, 0, 0)
						.draw(Config::TabColor);
				}

				LineString frame;
				{
					Polygon p = tab.globalRect
						.stretched(Config::FrameThickness * 0.5, Config::FrameThickness * 0.5, 0, Config::FrameThickness * 0.5)
						.rounded(Config::TabR + Config::FrameThickness * 0.5, Config::TabR + Config::FrameThickness * 0.5, 0, 0);
					auto& outer = p.outer();
					frame.reserve(outer.size());
					frame.push_back(outer.back());
					frame.insert(frame.cend(), outer.begin(), outer.end() - 1);
				}
				frame.draw(LineStyle::Uncapped, Config::FrameThickness, Config::FrameColor);

				tab.label.drawAt(tab.globalRect.center(), Palette::Black);
			}

			if (selectedIdx < m_tabs.size())
			{
				auto& firstTab = m_tabs.front();
				auto& selectedTab = m_tabs[selectedIdx];
				auto& lastTab = m_tabs.back();
				Rect{ firstTab.globalRect.leftX() - Config::FrameThickness, firstTab.globalRect.bottomY(), selectedTab.localRect.leftX(), Config::FrameThickness}
					.draw(Config::FrameColor);
				Rect{ selectedTab.globalRect.br(), lastTab.localRect.rightX() - selectedTab.localRect.rightX() + Config::FrameThickness, Config::FrameThickness }
					.draw(Config::FrameColor);
			}
			else
			{
				auto& firstTab = m_tabs.front();
				auto& lastTab = m_tabs.back();
				Rect{
					firstTab.globalRect.leftX() - Config::FrameThickness,
					firstTab.globalRect.bottomY(),
					lastTab.localRect.rightX() + Config::FrameThickness,
					Config::FrameThickness
				}.draw(Config::FrameColor);
			}
		}
	};

	size_t& GUIManager::tab(StringView id, Array<String> tabNames, size_t firstIdx)
	{
		assert(tabNames);
		auto& tab = getCurrentWindowImpl()
			.nextStatefulControl<Tab>(
				id.hash(),
				[&]() -> std::shared_ptr<IControl> {
					return std::make_shared<Tab>(tabNames, firstIdx);
				}
			);
		return tab.selectedIdx;
	}

	// SimpleColorPicker

	class SimpleColorPicker : public IControl
	{
	public:

		SimpleColorPicker(HSV& ref)
			: m_ref(ref)
		{ }

		bool valueChanged() const { return m_valueChanged; }

	private:

		HSV& m_ref;

		mutable bool m_valueChanged = false;

		Vec2 m_pos;

		Optional<Vec2> m_cursorPos;

		Size computeSize() const override
		{
			return SimpleGUI::ColorPickerRegion({ 0, 0 }).size.asPoint();
		}

		void update(Rect rect, Optional<Vec2> cursorPos) override
		{
			m_pos = rect.pos;
			m_cursorPos = cursorPos && rect.contains(*cursorPos)
				? cursorPos
				: none;
		}

		void draw() const override
		{
			Mat3x2 cursorMat = m_cursorPos
				? Mat3x2::Translate(Cursor::PosF() - *m_cursorPos)
				: Mat3x2::Translate(Cursor::PosF() + Vec2{ 1, 1 });
			const Transformer2D _{ Mat3x2::Identity(), cursorMat, Transformer2D::Target::PushLocal};
			m_valueChanged = SimpleGUI::ColorPicker(m_ref, m_pos);
		}
	};

	bool GUIManager::simpleColorpicker(HSV& value)
	{
		auto& picker = getCurrentWindowImpl()
			.nextStatelessControl(std::make_shared<SimpleColorPicker>(value));
		return picker.valueChanged();
	}

	// SimpleSlider

	class SimpleSlider : public IControl
	{
	public:

		SimpleSlider(double& ref, double width)
			: m_ref(ref)
			, m_width(width)
		{ }

		bool valueChanged() const { return m_valueChanged; }

	private:

		double& m_ref;

		double m_width;

		mutable bool m_valueChanged = false;

		Vec2 m_pos;

		Optional<Vec2> m_cursorPos;

		Size computeSize() const override
		{
			return SimpleGUI::SliderRegion({ 0, 0 }, 0, m_width).size.asPoint();
		}

		void update(Rect rect, Optional<Vec2> cursorPos) override
		{
			m_pos = rect.pos;
			m_cursorPos = cursorPos && rect.contains(*cursorPos)
				? cursorPos
				: none;
		}

		void draw() const override
		{
			Mat3x2 cursorMat = m_cursorPos
				? Mat3x2::Translate(Cursor::PosF() - *m_cursorPos)
				: Mat3x2::Translate(Cursor::PosF() + Vec2{ 1, 1 });
			const Transformer2D _{ Mat3x2::Identity(), cursorMat, Transformer2D::Target::PushLocal };

			m_valueChanged = SimpleGUI::Slider(m_ref, m_pos, m_width);
		}
	};

	bool GUIManager::simpleSlider(double& value, double width)
	{
		auto& slider = getCurrentWindowImpl()
			.nextStatelessControl(std::make_shared<SimpleSlider>(value, width));
		return slider.valueChanged();
	}

	// Custom

	void GUIManager::custom(std::shared_ptr<IControl> control)
	{
		getCurrentWindowImpl().nextStatelessControl(control);
	}
}
