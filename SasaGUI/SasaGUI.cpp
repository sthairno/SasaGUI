#include "SasaGUI.hpp"

namespace SasaGUI
{
	using WindowImpl = detail::WindowImpl;
	using GUIImpl = detail::GUIImpl;

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

		virtual void update(Rect rect) = 0;

		virtual void draw() const = 0;

		virtual ~IControl() { };
	};

	namespace detail
	{
		class WindowImpl : Uncopyable
		{
		public:

			using ControlGenerator = std::shared_ptr<IControl>(*)();

			WindowImpl(StringView id, StringView name);

		public:

			Window window;

			bool defined = false;

			Point nextPos{ 0, 0 };

			Size contentSize{ 0, 0 };

			const String& id() const { return m_id; }

			void frameBegin();

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

			size_t m_salt;

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

		void frameBegin();

		void frameEnd();

		WindowImpl& defineWindow(StringView id);

	private:

		container_type m_container;

		std::list<String> m_windowOrder;
	};

	namespace detail
	{
		class GUIImpl
		{
		public:

			std::array<Layer, 3>& layers() { return m_layers; }

			Layer& getLayer(WindowLayer layer);

		private:

			std::array<Layer, 3> m_layers{
				Layer{ WindowLayer::Background },
				Layer{ WindowLayer::Normal },
				Layer{ WindowLayer::Foreground }
			};
		};
	}

	static StringView ToString(WindowLayer layer)
	{
		return std::array{ U"Background", U"Normal", U"Foreground" } [static_cast<int32>(layer)];
	}

	// WindowImpl

	WindowImpl::WindowImpl(StringView id, StringView name)
		: window({
			.displayName = String{ name },
			.rect = { RandomPoint({0, 0, Scene::Size() - Size{100, 100}}), 100, 100 }
		})
		, m_controls()
		, m_id(id)
		, m_salt(RandomUint64())
	{ }

	void WindowImpl::frameBegin()
	{
		defined = false;
		nextPos = { 0, 0 };
		contentSize = { 0, 0 };
		m_controls.clear();
	}

	void WindowImpl::frameEnd()
	{
		
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

		control->update(globalRect);

		nextPos.y += size.y;
		nextPos.y += window.space;

		return *control;
	}

	IControl& WindowImpl::nextStatefulControlImpl(size_t id, ControlGenerator generator)
	{
		s3d::detail::HashCombine(id, m_salt);

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

	void Layer::frameBegin()
	{
		for (auto& id : m_windowOrder)
		{
			m_container.at(id)->frameBegin();
		}
	}

	void Layer::frameEnd()
	{
		m_windowOrder.remove_if([this](const String& id) {
			auto itr = m_container.find(id);

			if (itr->second->defined)
			{
				return false;
			}

			m_container.erase(itr);


			Console << U"[" << ToString(type) << U"][-] " << id;
			return true;
		});

		for (auto& [id, impl] : m_container)
		{
			impl->frameEnd();
		}
	}

	WindowImpl& Layer::defineWindow(StringView id)
	{
		auto itr = m_container.find(id);

		if (itr == m_container.end())
		{
			auto [tmp, _] = m_container.emplace(
				id,
				std::make_unique<WindowImpl>( id, id )
			);
			m_windowOrder.emplace_back(String{ id });
			itr = tmp;

			Console << U"[" << ToString(type) << U"][+] " << itr->first;
		}

		auto& impl = *itr->second;
		impl.defined = true;
		return impl;
	}

	// GUIImpl

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

		auto& layers = m_impl->layers();
		for (auto layerItr = layers.rbegin(); layerItr != layers.rend(); layerItr++)
		{
			layerItr->frameBegin();
		}

		m_defaultWindow = &m_impl
			->getLayer(WindowLayer::Background)
			.defineWindow(U"DefaultWindow");
		m_defaultWindow->window.rect = Scene::Rect();
	}

	void GUIManager::frameEnd()
	{
		for (auto& layer : m_impl->layers())
		{
			layer.frameEnd();
		}

		for (auto [idx, layer] : Indexed(m_impl->layers()))
		{
			const HSV color{ ((double)idx / 4 * 360), 0.8, 1.0 };
			for (auto& [id, impl] : layer.container())
			{
				auto& window = impl->window;

				impl->draw();

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

		void update(Rect rect) override
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

		Vec2 m_pos;

		double m_width;

		bool m_clicked = false;

		Size computeSize() const override
		{
			return SimpleGUI::ButtonRegion(m_label, { 0, 0 }).size.asPoint();
		}

		void update(Rect rect) override
		{
			m_pos = rect.pos;
			m_width = rect.w;

			m_clicked = rect.leftClicked();
		}

		void draw() const
		{
			[[maybe_unused]]
			bool unused = SimpleGUI::Button(m_label, m_pos, m_width);
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

		void update(Rect rect) override
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
