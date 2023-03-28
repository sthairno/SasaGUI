#include "SasaGUI.hpp"

namespace SasaGUI
{
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

	class IControl
	{
	public:

		virtual Size computeSize() const = 0;

		virtual void update(Rect rect, Optional<Vec2> cursorPos) = 0;

		virtual void draw() const = 0;

		virtual ~IControl() { };
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

			using ControlGenerator = std::shared_ptr<IControl>(*)();

			WindowImpl(InputContext& input, StringView id, StringView name);

		public:

			Window window;

			bool defined = false;

			Point nextPos{ 0, 0 };

			Size contentSize{ 0, 0 };

			bool requestMoveToFront = false;

			const String& id() const { return m_id; }

			size_t randomId() const { return m_randomId; }

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

			void draw() const;

		private:

			String m_id;

			size_t m_randomId;

			InputContext* m_input;

			WindowState m_state = WindowState::Default;

			bool m_mouseOver = false;

			Array<std::shared_ptr<IControl>> m_controls;

			std::map<size_t, std::shared_ptr<IControl>> m_savedControls;

			IControl& nextControlImpl(std::shared_ptr<IControl> control);

			IControl& nextStatefulControlImpl(size_t id, ControlGenerator generator);
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
			.rect = { RandomPoint({0, 0, Scene::Size() - Size{100, 100}}), 100, 100 }
		})
		, m_controls()
		, m_input(&input)
		, m_id(id)
		, m_randomId(RandomUint64())
	{ }

	void WindowImpl::frameBegin(InputContext& input)
	{
		auto& font = SimpleGUI::GetFont();
		m_input = &input;

		Rect titlebarRect;
		Rect contentRect;

		if (window.flags & WindowFlag::NoBackground ||
			window.flags & WindowFlag::NoTitlebar)
		{
			titlebarRect = { 0, 0, 0, 0 };
			contentRect = window.rect;
		}
		else
		{
			titlebarRect = {
				window.rect.pos,
				window.rect.w,
				font.height()
			};
			contentRect = {
				window.rect.x,
				window.rect.y + font.height(),
				window.rect.w,
				window.rect.h - font.height()
			};
		}

		if (auto cursor = input.getCursorPos(*this))
		{
			switch (m_state)
			{
			case SasaGUI::WindowState::Default:
				if (titlebarRect.contains(*cursor))
				{
					input.hover(*this);
					if (MouseL.down())
					{
						m_state = WindowState::Moving;
						requestMoveToFront = true;
						input.capture(*this);
					}
				}
				if (contentRect.contains(*cursor))
				{
					input.hover(*this);
					if (MouseL.down())
					{
						requestMoveToFront = true;
					}
				}
				break;
			case SasaGUI::WindowState::Moving:
				if (MouseL.pressed())
				{
					window.rect.pos += Cursor::Delta();
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

		defined = false;
		nextPos = { 0, 0 };
		contentSize = { 0, 0 };
		m_controls.clear();
	}

	void WindowImpl::frameEnd()
	{
		draw();
	}

	void WindowImpl::draw() const
	{
		auto& font = SimpleGUI::GetFont();

		if (not (window.flags & WindowFlag::NoBackground))
		{
			RoundRect{ window.rect, 10 }
				.drawFrame(0, 1, Palette::Lightgray)
				.draw(ColorF{ 0.9 });

			if (not (window.flags & WindowFlag::NoTitlebar))
			{
				RectF{ window.rect.pos, window.rect.w, font.height() }
					.rounded(10, 10, 0, 0)
					.draw(Palette::Lightgray);
				font(window.displayName)
					.draw(Arg::topCenter = window.rect.topCenter(), Palette::Black);
			}
		}

		for (auto& control : m_controls)
		{
			control->draw();
		}
	}

	IControl& WindowImpl::nextControlImpl(std::shared_ptr<IControl> control)
	{
		m_controls.emplace_back(control);

		Size size = control->computeSize();
		Rect localRect{ nextPos, size };
		Rect globalRect = localRect.movedBy(window.rect.pos);

		Optional<Vec2> cursorPos = m_input->getCursorPos(*this);
		if (m_state != WindowState::Default)
		{
			cursorPos.reset();
		}
		control->update(globalRect, cursorPos);

		nextPos.y += size.y;
		nextPos.y += window.space;

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

	// Layer

	void Layer::frameBegin(InputContext& input)
	{
		m_input = &input;

		std::list<String> front;
		for (auto itr = m_windowOrder.rbegin(); itr != m_windowOrder.rend();)
		{
			auto& window = *m_container.at(*itr);

			window.frameBegin(*m_input);

			if (window.requestMoveToFront)
			{
				front.emplace_back(std::move(*itr));
				m_windowOrder.erase(--(itr.base()));
				window.requestMoveToFront = false;
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
		impl.defined = true;
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

			if (itr->second->defined)
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

		Size computeSize() const override
		{
			return SimpleGUI::TextBoxRegion(m_pos, width).size.asPoint();
		}

		void update(Rect rect, Optional<Vec2> cursorPos) override
		{
			m_pos = rect.pos;
			m_renderWidth = rect.w;
		}

		void draw() const
		{
			SimpleGUI::TextBox(m_state, m_pos, m_renderWidth, maxChars);
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
}
