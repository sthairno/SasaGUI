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

		IControl() { }

		virtual Size computeSize() const { return { }; };

		virtual void update(Rect rect) { };

		virtual void draw() const { };

		virtual ~IControl() { };
	};

	namespace detail
	{
		class WindowImpl : Uncopyable
		{
		public:

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
			inline ControlType& nextControl(std::unique_ptr<ControlType> control)
			{
				return reinterpret_cast<ControlType&>(nextControlImpl(std::move(control)));
			}

			void draw() const;

		private:

			String m_id;

			std::vector<std::unique_ptr<IControl>> m_controls;

			IControl& nextControlImpl(std::unique_ptr<IControl> control);
		};
	}

	class Layer
	{
	public:

		using container_type = HashTable<String, std::unique_ptr<WindowImpl>>;

		const container_type& container() const { return m_container; }

		void frameBegin();

		void frameEnd();

		WindowImpl& defineWindow(StringView id);

	private:

		container_type m_container;
	};

	namespace detail
	{
		class GUIImpl
		{
		public:

			std::array<Layer, 3>& layers() { return m_layers; }

			Layer& getLayer(WindowLayer layer);

		private:

			std::array<Layer, 3> m_layers;
		};
	}

	// WindowImpl

	WindowImpl::WindowImpl(StringView id, StringView name)
		: window({
			.displayName = String{ name },
			.rect = { RandomPoint({0, 0, Scene::Size() - Size{100, 100}}), 100, 100 }
		})
		, m_controls()
		, m_id(id)
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

	IControl& WindowImpl::nextControlImpl(std::unique_ptr<IControl> c)
	{
		m_controls.emplace_back(std::move(c));

		auto& control = *m_controls.back();

		Size size = control.computeSize();
		Rect localRect{ nextPos, size };
		Rect globalRect = localRect.movedBy(window.rect.pos);

		control.update(globalRect);

		nextPos.y += size.y;
		nextPos.y += window.space;

		return control;
	}

	// Layer

	void Layer::frameBegin()
	{
		for (auto& [id, impl] : m_container)
		{
			impl->frameBegin();
		}
	}

	void Layer::frameEnd()
	{
		for (auto itr = m_container.begin(); itr != m_container.end();)
		{
			if (itr->second->defined)
			{
				itr++;
			}
			else
			{
				itr = m_container.erase(itr);
			}
		}

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
			itr = tmp;
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

		for (auto& layer : m_impl->layers())
		{
			layer.frameBegin();
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
			.nextControl(std::make_unique<Dummy>(size));
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
			.nextControl(std::make_unique<Button>(label));
		return button.clicked();
	}
}
