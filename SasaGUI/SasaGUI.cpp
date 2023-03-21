#include "SasaGUI.hpp"

namespace SasaGUI
{
	enum class WindowLayer : int32
	{
		Background,
		Normal,
		Foreground
	};

	class WindowImpl
	{
	public:

		WindowImpl(StringView id, StringView name);

	public:

		Window window;

		bool defined = false;

		const String& id() const { return m_id; }

		void draw() const;

	private:

		String m_id;
	};

	class Layer
	{
	public:

		using container_type = HashTable<String, WindowImpl>;

		const container_type& container() const { return m_container; }

		WindowImpl& defineWindow(StringView id);

		void frameBegin();

		void frameEnd();

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
		, m_id(id)
	{ }

	void WindowImpl::draw() const
	{
		
	}

	// Layer

	WindowImpl& Layer::defineWindow(StringView id)
	{
		auto itr = m_container.find(id);

		if (itr == m_container.end())
		{
			auto [tmp, _] = m_container.emplace(
				id,
				WindowImpl{ id, id }
			);
			itr = tmp;
		}

		auto& impl = itr->second;
		impl.defined = true;
		return itr->second;
	}

	void Layer::frameBegin()
	{
		for (auto& [id, impl] : m_container)
		{
			impl.defined = false;
		}
	}

	void Layer::frameEnd()
	{
		for (auto itr = m_container.begin(); itr != m_container.end();)
		{
			if (itr->second.defined)
			{
				itr++;
			}
			else
			{
				itr = m_container.erase(itr);
			}
		}
	}

	// GUIImpl

	Layer& detail::GUIImpl::getLayer(WindowLayer layer)
	{
		return m_layers[static_cast<int32>(layer)];
	}

	// GUIManager

	GUIManager::GUIManager()
		: m_impl(new detail::GUIImpl())
	{
		m_defaultWindow = &m_impl
			->getLayer(WindowLayer::Background)
			.defineWindow(U"DefaultWindow")
			.window;
		m_defaultWindow->flags =
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

		for (auto layer : m_impl->layers())
		{
			layer.frameBegin();
		}

		m_defaultWindow = &m_impl
			->getLayer(WindowLayer::Background)
			.defineWindow(U"DefaultWindow")
			.window;
		m_defaultWindow->rect = Scene::Rect();
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
				auto& window = impl.window;

				impl.draw();

				window.rect.drawFrame(1, 0, color);
				Line{ window.rect.tl(), window.rect.br() }.draw(color);
				Line{ window.rect.bl(), window.rect.tr() }.draw(color);

				SimpleGUI::GetFont()(window.displayName)
					.draw(Arg::topCenter = window.rect.topCenter(), color);
			}
		}
	}

	void GUIManager::windowBegin(StringView name, WindowFlag flags)
	{
		WindowLayer layer = flags & WindowFlag::AlwaysForeground
			? WindowLayer::Foreground
			: WindowLayer::Normal;
		auto window = &m_impl
			->getLayer(layer)
			.defineWindow(name)
			.window;
		window->flags = flags;
		m_stack.push_back(window);
	}

	void GUIManager::windowEnd()
	{
		m_stack.pop_back();
	}

	GUIManager::~GUIManager()
	{ }
}
