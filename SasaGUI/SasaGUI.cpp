#include "SasaGUI.hpp"

namespace SasaGUI
{
	using WindowImpl = detail::WindowImpl;
	using GUIImpl = detail::GUIImpl;

	namespace Config
	{
		struct Common
		{
			constexpr static ColorF LabelColor = Palette::Black;
		};

		struct ScrollBar
		{
			// |          ______________     |
			// |  =======|______________|==  |
			// |                             |
			//    ^---------Track---------^
			//           ^-----Thumb----^

			constexpr static ColorF BackgroundColor{ 1, 0 };
			constexpr static ColorF TrackColor{ 0, 0 };
			constexpr static ColorF ThumbColor{ 0.4, 0.7 };
			constexpr static ColorF HoveredBackgroundColor{ 1, 1 };
			constexpr static ColorF HoveredTrackColor{ 0, 0.1 };
			constexpr static ColorF HoveredThumbColor{ 0.4, 1 };

			constexpr static int32 TrackMargin = 3;
			constexpr static int32 ThumbThickness = 6;
			constexpr static int32 ThumbMinLength = 20;

			constexpr static Duration ScrollSmoothTime = 0.1s;
			constexpr static Duration LargeScrollDelay = 0.8s;
			constexpr static Duration LargeScrollInterval = 0.1s;
			constexpr static Duration FadeInDuration = 0.06s;
			constexpr static Duration FadeOutDuration = 0.5s;
			constexpr static Duration FadeOutDelay = 2s;
		};

		struct Window
		{
			constexpr static ColorF BackColor{ 0.9 };
			constexpr static ColorF FrameColor{ 0.7 };
			constexpr static ColorF TitlebarColor = Palette::Lightgray;
			constexpr static ColorF TitlebarLabelColor = Common::LabelColor;

			constexpr static Size DefaultWindowSize{ 100, 100 };
			constexpr static int32 Roundness = 3;
			constexpr static int32 FrameThickness = 2;
			constexpr static int32 ResizeGripSize = 5;
		};

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
			constexpr static ColorF CircleColor = Palette::White;
			constexpr static ColorF CheckedCircleColor = Palette::White;
			constexpr static ColorF HoveredCircleColor{ 0.9 };

			constexpr static ColorF FrameColor{ 0.3 };
			constexpr static ColorF CheckedFrameColor{ 0.35, 0.7, 1.0 };
			constexpr static ColorF HoveredFrameColor = Palette::Black;

			constexpr static ColorF LabelColor = Common::LabelColor;

			constexpr static double CircleScale = 0.6; // r = Font::height() * CircleScale / 2
			constexpr static double InnerCircleScale = 0.6; // r = Font::height() * InnerCircleScale * CircleScale / 2
			constexpr static int32 CircleMargin = 4;
			constexpr static int32 CircleFrameThickness = 2;
		};

		struct Tab
		{
			constexpr static ColorF FrameColor = Palette::Black;

			constexpr static ColorF TabColor = ColorF{ 0.95 };
			constexpr static ColorF HoveredTabColor = ColorF{ 0.8 };
			constexpr static ColorF SelectedTabColor = ColorF::Zero();

			constexpr static ColorF LabelColor = Common::LabelColor;

			constexpr static int32 FrameThickness = 1;
			constexpr static int32 TabR = 5;
			constexpr static int32 TabSpace = 1;
		};

		struct ProgressBar
		{
			constexpr static ColorF BackgroundColor{ 0.8 };
			constexpr static ColorF BarColor = Color{ 13, 110, 253 };
			constexpr static ColorF FrameColor = Color::Zero();

			constexpr static int32 FrameThickness = 0;
			constexpr static int32 Height = 16;
		};
	}

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

		bool hover(const WindowImpl& window);

		bool capture(const WindowImpl& window, bool capture = true);

		Optional<Vec2> getCursorPos() const;

		Optional<Vec2> getCursorPos(const WindowImpl& window) const;

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
		Default, Moving, Resizing, Scrolling
	};

	enum class Side : int32
	{
		Top,
		Right,
		Bottom,
		Left,
		Count
	};

	enum class ResizeFlag : uint8
	{
		None = 0,
		Top = 1 << static_cast<int32>(Side::Top),
		Right = 1 << static_cast<int32>(Side::Right),
		Bottom = 1 << static_cast<int32>(Side::Bottom),
		Left = 1 << static_cast<int32>(Side::Left)
	};
	DEFINE_BITMASK_OPERATORS(ResizeFlag);

	struct WindowLayout
	{
		// タイトルバー領域
		Rect titlebarRect;

		// ウィンドウ内の領域
		Rect clientRect;

		// ウィンドウ内のスクロールバーを除いた領域
		Rect contentRect;

		// リサイズの四角形
		std::array<Rect, static_cast<int32>(Side::Count)> resizeGripRect;
	};

	class Delay
	{
	public:

		constexpr Delay(Duration duration) noexcept
			: m_duration(duration.count())
			, m_time(m_duration)
		{
			assert(duration >= 0s);
		}

		bool update(bool in, double d = Scene::DeltaTime()) noexcept
		{
			if (in)
			{
				m_time = 0.0;
			}
			else
			{
				m_time += d;
			}
			return in || m_time <= m_duration;
		}

	private:

		double m_duration;

		double m_time;
	};

	struct Repeat
	{
	public:

		Repeat(Duration interval, Duration delay = 0s)
			: m_interval(interval.count())
			, m_delay(delay.count())
		{
			assert(interval > 0s);
			assert(delay >= 0s);
		}

		bool update(bool in, double d = Scene::DeltaTime()) noexcept
		{
			if (not in)
			{
				m_first = true;
				m_delayed = false;
				return false;
			}

			bool out = false;

			if (m_first)
			{
				out = true;
				m_accumulation = 0.0;
				m_first = false;
			}

			m_accumulation += d;

			if (not m_delayed)
			{
				if (m_accumulation < m_delay)
				{
					return out;
				}

				out = true;
				m_accumulation -= m_delay;
				m_delayed = true;
			}

			double count = std::floor(m_accumulation / m_interval);
			out |= count > 0.0;
			m_accumulation -= count * m_interval;

			return out;
		}

	private:

		double m_interval;

		double m_delay;

		bool m_first = true;

		bool m_delayed = false;

		double m_accumulation = 0.0;
	};

	enum class Orientation
	{
		Horizontal, Vertical
	};

	class ScrollBar
	{
	public:

		using Config = Config::ScrollBar;

		constexpr static int32 Thickness = Config::TrackMargin * 2 + Config::ThumbThickness;
		constexpr static int32 MinLength = Config::TrackMargin * 2 + Config::ThumbMinLength;
		constexpr static double ThumbRoundness = Config::ThumbThickness * 0.5;

		ScrollBar(Orientation orientation)
			: orientation(orientation)
		{
			updateLayout({ 0, 0, 0, 0 });
		}

	public:

		const Orientation orientation;

		Rect rect() const { return m_rect; }

		double minimum() const { return m_minimum; }

		double maximum() const { return m_maximum; }

		double value() const { return m_value; }

		double viewportSize() const { return m_viewportSize; }

		void updateLayout(Rect rect)
		{
			m_rect = rect;
			switch (orientation)
			{
			case Orientation::Horizontal:
				m_rect.w = Max(m_rect.w, MinLength);
				m_rect.h = Thickness;
				break;
			case Orientation::Vertical:
				m_rect.w = Thickness;
				m_rect.h = Max(m_rect.h, MinLength);
				break;
			}
			m_trackRect = m_rect.stretched(-Config::TrackMargin);
		}

		void updateConstraints(double minimum, double maximum, double viewportSize)
		{
			m_minimum = minimum;
			m_maximum = maximum;
			m_viewportSize = viewportSize;
			m_value = Clamp(m_value, m_minimum, m_maximum - m_viewportSize);
		}

		void show()
		{
			m_colorTransitionDelay.update(true);
		}

		void scroll(double delta)
		{
			if (delta == 0.0)
			{
				return;
			}

			if (Math::Sign(delta) != Math::Sign(m_scrollVelocity))
			{
				m_scrollVelocity = 0.0;
			}
			m_scrollTarget = Clamp(m_value + delta, m_minimum, m_maximum - m_viewportSize);
		}

		void moveTo(double value)
		{
			m_value = Clamp(value, m_minimum, m_maximum - m_viewportSize);
			m_scrollTarget = m_value;
			m_scrollVelocity = 0.0;
		}

		void update(Optional<Vec2> cursorPos = Cursor::PosF(), double deltaTime = Scene::DeltaTime())
		{
			bool rectHovered = false;
			Optional<double> hoveredPos;
			RectF thumbRect = getThumbRect();

			if (not MouseL.pressed())
			{
				m_thumbGrabbed = false;
				m_trackPressed = false;
			}

			if (cursorPos.has_value())
			{
				rectHovered = m_rect.contains(*cursorPos);
				hoveredPos = getHoveredPos(*cursorPos);

				if (MouseL.down() && thumbRect.area() > 0.0)
				{
					if (thumbRect.contains(*cursorPos))
					{
						m_thumbGrabbed = true;
					}
					else if (rectHovered)
					{
						m_largeScrollDirection = Math::Sign(*hoveredPos - m_value);
						m_largeScrollTarget = *hoveredPos;
						m_trackPressed = true;
					}
				}
			}

			if (m_thumbGrabbed && hoveredPos)
			{
				moveTo(*hoveredPos - m_viewportSize * 0.5);
			}

			if (m_largeScrollTimer.update(m_trackPressed, deltaTime))
			{
				scroll(m_largeScrollDirection * m_viewportSize);
				if (m_value <= m_largeScrollTarget &&
					m_largeScrollTarget <= m_value + m_viewportSize)
				{
					m_trackPressed = false;
				}
				if (m_value <= m_minimum ||
					m_maximum - m_viewportSize <= m_value)
				{
					m_trackPressed = false;
				}
			}

			m_value = Math::SmoothDamp(m_value, m_scrollTarget, m_scrollVelocity, Config::ScrollSmoothTime.count());

			bool show = (rectHovered || m_thumbGrabbed || m_trackPressed) && thumbRect.area() > 0.0;
			m_colorTransition.update(m_colorTransitionDelay.update(show, deltaTime), deltaTime);
		}

		void draw() const
		{
			RectF thumbRect = getThumbRect();

			double f = m_colorTransition.value();
			ColorF backColor = Config::BackgroundColor.lerp(Config::HoveredBackgroundColor, f);
			ColorF trackColor = Config::TrackColor.lerp(Config::HoveredTrackColor, f);
			ColorF thumbColor = Config::ThumbColor.lerp(Config::HoveredThumbColor, f);
			m_rect
				.draw(backColor);
			m_trackRect
				.rounded(ThumbRoundness)
				.draw(trackColor);
			if (thumbRect.area() > 0.0)
			{
				thumbRect
					.rounded(ThumbRoundness)
					.draw(thumbColor);
			}
		}

	private:

		double m_minimum = 0.0;

		double m_maximum = 1.0;

		double m_value = 0.0;

		double m_viewportSize = 1.0;

		Rect m_rect;

		Rect m_trackRect;

		bool m_thumbGrabbed = false;

		bool m_trackPressed = false;

		double m_scrollTarget = 0.0;

		double m_scrollVelocity = 0.0;

		int32 m_largeScrollDirection = 0;

		double m_largeScrollTarget = 0.0;

		Repeat m_largeScrollTimer{ Config::LargeScrollInterval, Config::LargeScrollDelay };

		Transition m_colorTransition{ Config::FadeInDuration, Config::FadeOutDuration };

		Delay m_colorTransitionDelay{ Config::FadeOutDelay };

		RectF getThumbRect() const
		{
			double length = m_viewportSize / (m_maximum - m_minimum);
			double pos = m_value / (m_maximum - m_minimum);

			if (length >= 1.0)
			{
				return { 0, 0, 0, 0 };
			}

			switch (orientation)
			{
			case Orientation::Horizontal:
				return {
					m_trackRect.x + m_trackRect.w * pos,
					m_trackRect.y,
					m_trackRect.w * length,
					m_trackRect.h
				};
			case Orientation::Vertical:
				return {
					m_trackRect.x,
					m_trackRect.y + m_trackRect.h * pos,
					m_trackRect.w,
					m_trackRect.h * length
				};
			}
		}

		double getHoveredPos(Vec2 cursorPos) const
		{
			double value = 0.0;
			switch (orientation)
			{
			case Orientation::Horizontal:
				value = (cursorPos.x - m_trackRect.x) / m_trackRect.w;
				break;
			case Orientation::Vertical:
				value = (cursorPos.y - m_trackRect.y) / m_trackRect.h;
				break;
			}
			value *= m_maximum - m_minimum;
			return value;
		}
	};

	namespace detail
	{
		class WindowImpl : Uncopyable
		{
		public:

			using Config = Config::Window;

			using ControlGenerator = std::function<std::shared_ptr<IControl>()>;

			constexpr static int32 ContentMinSize = ScrollBar::MinLength + ScrollBar::Thickness;

			WindowImpl(InputContext& input, StringView id, StringView name);

		public:

			Window window;

			const String& id() const { return m_id; }

			size_t randomId() const { return m_randomId; }

			const Rect& rect() const { return window.rect; }

			int32 titlebarHeight() const
			{
				if (window.flags & WindowFlag::NoBackground ||
					window.flags & WindowFlag::NoTitlebar)
				{
					return 0;
				}
				else
				{
					return window.font.height();
				}
			}

			Size minSize() const
			{
				if (window.flags & WindowFlag::NoBackground)
				{
					return { 0, 0 };
				}
				else
				{
					return { Max(Config::Roundness * 2, ContentMinSize), Max(Config::Roundness * 2, titlebarHeight() + ContentMinSize) };
				}
			}

			Optional<Vec2> getLocalCursorPos() const;

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

		private:

			String m_id;

			size_t m_randomId;

			InputContext* m_input;

			WindowState m_state = WindowState::Default;

			ResizeFlag m_resizeFlag = ResizeFlag::None;

			bool m_isContentHovered = false;

			bool m_firstFrame = true;

			Optional<WindowLayout> m_layout;

			std::array<ScrollBar, 2> m_scrollBars;

			Rect m_contentLocalRect{ 0, 0 };

			Array<std::shared_ptr<IControl>> m_controls;

			std::map<size_t, std::shared_ptr<IControl>> m_savedControls;

			void draw() const;

			void updateWindowState();

			void handleMovingState();

			void handleResizingState();

			void updateLayout();

			void autoResize();

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

	static CursorStyle ToCursorStyle(ResizeFlag flag)
	{
		switch (flag)
		{
		case ResizeFlag::Top:
		case ResizeFlag::Bottom:
			return CursorStyle::ResizeUpDown;
		case ResizeFlag::Top | ResizeFlag::Right:
		case ResizeFlag::Bottom | ResizeFlag::Left:
			return CursorStyle::ResizeNESW;
		case ResizeFlag::Right:
		case ResizeFlag::Left:
			return CursorStyle::ResizeLeftRight;
		case ResizeFlag::Right | ResizeFlag::Bottom:
		case ResizeFlag::Left | ResizeFlag::Top:
			return CursorStyle::ResizeNWSE;
		default:
			return CursorStyle::Default;
		}
	}

	// InputContext

	bool InputContext::hover(const WindowImpl& window)
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

	bool InputContext::capture(const WindowImpl& window, bool capture)
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

	Optional<Vec2> InputContext::getCursorPos() const
	{
		if (m_hoveredItemId == 0)
		{
			return m_cursorPos;
		}

		return none;
	}

	Optional<Vec2> InputContext::getCursorPos(const WindowImpl& window) const
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
			.rect = { 0, 0, Config::DefaultWindowSize }
		})
		, m_scrollBars({
			ScrollBar{Orientation::Horizontal},
			ScrollBar{Orientation::Vertical}
		})
		, m_controls()
		, m_input(&input)
		, m_id(id)
		, m_randomId(RandomUint64())
	{
		updateLayout();
	}

	Optional<Vec2> WindowImpl::getLocalCursorPos() const
	{
		if (auto globalCursorPos = m_input->getCursorPos(*this))
		{
			if (m_state == WindowState::Default &&
				m_isContentHovered)
			{
				return globalCursorPos->movedBy(-m_layout->contentRect.pos - m_contentLocalRect.pos);
			}
		}
	}

	void WindowImpl::frameBegin(InputContext& input)
	{
		m_input = &input;

		updateWindowState();
		handleMovingState();
		handleResizingState();
		updateLayout();

		for (auto& scrollBar : m_scrollBars)
		{
			scrollBar.update(input.getCursorPos(*this).map([this](Vec2 v) { return v.movedBy(-m_layout->clientRect.pos); }));
		}

		window.defined = false;
		window.lineRect = { window.padding, window.padding, 0, 0 };
		m_contentLocalRect = {
			-static_cast<int32>(m_scrollBars[0].value()),
			-static_cast<int32>(m_scrollBars[1].value()),
			0, 0
		};
		m_controls.clear();
		window.sameLine = false;
	}

	void WindowImpl::frameEnd()
	{
		if (not m_controls.empty())
		{
			m_contentLocalRect.size += { window.padding, window.padding };
		}

		autoResize();
		updateLayout();
		draw();
		if (m_resizeFlag != ResizeFlag::None)
		{
			Cursor::RequestStyle(ToCursorStyle(m_resizeFlag));
		}

		m_firstFrame = false;
	}

	void WindowImpl::draw() const
	{
		if (not (window.flags & WindowFlag::NoBackground))
		{
			RoundRect{ window.rect, Config::Roundness }
				.drawFrame(0, Config::FrameThickness, Config::FrameColor)
				.draw(ColorF{ 0.9 });

			if (not (window.flags & WindowFlag::NoTitlebar))
			{
				m_layout->titlebarRect
					.rounded(Config::Roundness, Config::Roundness, 0, 0)
					.draw(Config::TitlebarColor);
				window.font(window.displayName)
					.draw(Arg::topCenter = window.rect.topCenter(), Config::TitlebarLabelColor);
			}
		}

		{
			ScopedViewport2D sv{ m_layout->clientRect };
			{
				Transformer2D t{ Mat3x2::Translate(m_contentLocalRect.pos), Transformer2D::Target::SetLocal };
				for (auto& control : m_controls)
				{
					control->draw();
				}
			}
			{
				Transformer2D t{ Mat3x2::Identity(), Transformer2D::Target::SetLocal };
				for (auto& scrollBar : m_scrollBars)
				{
					scrollBar.draw();
				}
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
			m_contentLocalRect.w = Max(m_contentLocalRect.w, br.x);
			m_contentLocalRect.h = Max(m_contentLocalRect.h, br.y);
			window.lineRect.w = Max(window.lineRect.w, br.x - window.lineRect.x);
			window.lineRect.h = Max(window.lineRect.h, br.y - window.lineRect.y);
		}

		Optional<Vec2> localCursorPos;
		if (auto globalCursorPos = m_input->getCursorPos(*this))
		{
			if (m_state == WindowState::Default &&
				m_isContentHovered)
			{
				localCursorPos = globalCursorPos->movedBy(-m_layout->contentRect.pos - m_contentLocalRect.pos);
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

	void WindowImpl::updateWindowState()
	{
		m_isContentHovered = false;

		Rect& windowRect = window.rect;
		if (auto cursor = m_input->getCursorPos(*this))
		{
			if (rect().contains(*cursor))
			{
				m_input->hover(*this);
				window.requestMoveToFront |= MouseL.down();
			}

			switch (m_state)
			{
			case SasaGUI::WindowState::Default:

				m_resizeFlag = ResizeFlag::None;
				for (int32 i = 0; i < static_cast<int32>(Side::Count); i++)
				{
					auto& grip = m_layout->resizeGripRect[i];

					if (grip.contains(*cursor))
					{
						m_resizeFlag |= static_cast<ResizeFlag>(1 << i);
					}
				}
				if (m_resizeFlag != ResizeFlag::None)
				{
					if (MouseL.down())
					{
						m_state = WindowState::Resizing;
						window.requestMoveToFront = true;
						m_input->capture(*this, true);
					}
					m_input->hover(*this);
					break;
				}

				if (m_layout->titlebarRect.contains(*cursor))
				{
					if (MouseL.down())
					{
						m_state = WindowState::Moving;
						m_input->capture(*this, true);
					}
					break;
				}

				if (m_layout->contentRect.contains(*cursor))
				{
					m_scrollBars[0].scroll(Mouse::WheelH() * window.font.height());
					m_scrollBars[1].scroll(Mouse::Wheel() * window.font.height());
				}

				if (m_scrollBars[0].rect().contains(*cursor))
				{
					if (MouseL.down())
					{
						m_state = WindowState::Scrolling;
						m_input->capture(*this, true);
					}
					break;
				}

				if (m_scrollBars[0].rect().contains(*cursor))
				{
					if (MouseL.down())
					{
						m_state = WindowState::Scrolling;
						m_input->capture(*this, true);
					}
					break;
				}

				m_isContentHovered = m_layout->contentRect.contains(*cursor);

				break;
			case SasaGUI::WindowState::Moving:
			case SasaGUI::WindowState::Resizing:
			case SasaGUI::WindowState::Scrolling:
				if (not MouseL.pressed())
				{
					m_state = WindowState::Default;
					m_input->capture(*this, false);
				}
				break;
			}
		}
	}

	void WindowImpl::handleMovingState()
	{
		if (m_state != WindowState::Moving)
		{
			return;
		}

		window.rect.pos += Cursor::Delta();
	}

	void WindowImpl::handleResizingState()
	{
		if (m_state != WindowState::Resizing)
		{
			return;
		}

		auto delta = Cursor::Delta();

		if (m_resizeFlag & ResizeFlag::Left)
		{
			delta.x = Min(delta.x, window.rect.w - ContentMinSize);
			window.rect.x += delta.x;
			window.rect.w -= delta.x;
		}
		else if (m_resizeFlag & ResizeFlag::Right)
		{
			window.rect.w = Max(window.rect.w + delta.x, ContentMinSize);
		}

		if (m_resizeFlag & ResizeFlag::Top)
		{
			delta.y = Min(delta.y, window.rect.h - ContentMinSize - titlebarHeight());
			window.rect.y += delta.y;
			window.rect.h -= delta.y;
		}
		else if (m_resizeFlag & ResizeFlag::Bottom)
		{
			window.rect.h = Max(window.rect.h + delta.y, ContentMinSize + titlebarHeight());
		}
	}

	void WindowImpl::updateLayout()
	{
		if (window.flags & WindowFlag::NoBackground ||
			window.flags & WindowFlag::NoTitlebar)
		{
			m_layout = WindowLayout{
				.titlebarRect = { 0, 0, 0, 0 },
				.clientRect = window.rect,
				.contentRect = window.rect
			};

			// ResizeGrip

			if (not (window.flags & WindowFlag::NoResize))
			{
				// Top
				m_layout->resizeGripRect[0] = {
					window.rect.leftX() - Config::ResizeGripSize,
					window.rect.topY() - Config::ResizeGripSize,
					window.rect.w + Config::ResizeGripSize * 2,
					Config::ResizeGripSize
				};
				// Right
				m_layout->resizeGripRect[1] = {
					window.rect.rightX(),
					window.rect.topY() - Config::ResizeGripSize,
					Config::ResizeGripSize,
					window.rect.h + Config::ResizeGripSize * 2,
				};
				// Bottom
				m_layout->resizeGripRect[2] = {
					window.rect.leftX() - Config::ResizeGripSize,
					window.rect.bottomY(),
					window.rect.w + Config::ResizeGripSize * 2,
					Config::ResizeGripSize
				};
				// Left
				m_layout->resizeGripRect[3] = {
					window.rect.leftX() - Config::ResizeGripSize,
					window.rect.topY() - Config::ResizeGripSize,
					Config::ResizeGripSize,
					window.rect.h + Config::ResizeGripSize * 2,
				};
			}
		}
		else
		{
			Rect client{
				window.rect.x,
				window.rect.y + titlebarHeight(),
				window.rect.w,
				window.rect.h - titlebarHeight()
			};
			m_layout = WindowLayout{
				.titlebarRect = {
					window.rect.pos,
					window.rect.w,
					titlebarHeight()
				},
				.clientRect = client,
				.contentRect = client
			};

			// ResizeGrip

			if (not (window.flags & WindowFlag::NoResize))
			{
				// Top
				m_layout->resizeGripRect[0] = {
					window.rect.leftX() - Config::ResizeGripSize,
					window.rect.topY(),
					window.rect.w + Config::ResizeGripSize * 2,
					Config::ResizeGripSize
				};
				// Right
				m_layout->resizeGripRect[1] = {
					window.rect.rightX(),
					window.rect.topY(),
					Config::ResizeGripSize,
					window.rect.h + Config::ResizeGripSize,
				};
				// Bottom
				m_layout->resizeGripRect[2] = {
					window.rect.leftX() - Config::ResizeGripSize,
					window.rect.bottomY(),
					window.rect.w + Config::ResizeGripSize * 2,
					Config::ResizeGripSize
				};
				// Left
				m_layout->resizeGripRect[3] = {
					window.rect.leftX() - Config::ResizeGripSize,
					window.rect.topY(),
					Config::ResizeGripSize,
					window.rect.h + Config::ResizeGripSize,
				};
			}
		}

		//スクロールバー

		bool hbar = false;
		bool vbar = false;

		if (not (window.flags & WindowFlag::NoScrollbar))
		{
			hbar = m_contentLocalRect.w > m_layout->contentRect.w;
			vbar = m_contentLocalRect.h > m_layout->contentRect.h;

			// バーの表示によってコントロールが隠れるときはスクロールバーを表示

			if (hbar)
			{
				vbar |= m_contentLocalRect.h > m_layout->contentRect.h - ScrollBar::Thickness;
			}
			if (vbar)
			{
				hbar |= m_contentLocalRect.w > m_layout->contentRect.w - ScrollBar::Thickness;
			}
		}

		// バーの大きさとコントロールが表示できる領域を決定

		if (hbar && vbar)
		{
			m_scrollBars[0].updateLayout({
				0,
				m_layout->clientRect.h - ScrollBar::Thickness,
				m_layout->clientRect.w - ScrollBar::Thickness,
				ScrollBar::Thickness
			});
			m_scrollBars[1].updateLayout({
				m_layout->clientRect.w - ScrollBar::Thickness,
				0,
				ScrollBar::Thickness,
				m_layout->clientRect.h - ScrollBar::Thickness
			});
			m_layout->contentRect.h -= ScrollBar::Thickness;
			m_layout->contentRect.w -= ScrollBar::Thickness;
		}
		else
		{
			if (hbar)
			{
				m_layout->contentRect.h -= ScrollBar::Thickness;
			}

			if (vbar)
			{
				m_layout->contentRect.w -= ScrollBar::Thickness;
			}

			m_scrollBars[0].updateLayout({
				0,
				m_layout->clientRect.h - ScrollBar::Thickness,
				m_layout->clientRect.w - ScrollBar::Thickness,
				ScrollBar::Thickness
			});

			m_scrollBars[1].updateLayout({
				m_layout->clientRect.w - ScrollBar::Thickness,
				0,
				ScrollBar::Thickness,
				m_layout->clientRect.h - ScrollBar::Thickness
			});
		}

		// スクロールバーの上限, 下限を更新

		if (hbar)
		{
			m_scrollBars[1].updateConstraints(0.0, m_contentLocalRect.h + ScrollBar::Thickness, m_layout->contentRect.h);
		}
		else
		{
			m_scrollBars[1].updateConstraints(0.0, m_contentLocalRect.h, m_layout->contentRect.h);
		}

		if (vbar)
		{
			m_scrollBars[0].updateConstraints(0.0, m_contentLocalRect.w + ScrollBar::Thickness, m_layout->contentRect.w);
		}
		else
		{
			m_scrollBars[0].updateConstraints(0.0, m_contentLocalRect.w, m_layout->contentRect.w);
		}
	}

	void WindowImpl::autoResize()
	{
		Size& windowSize = window.rect.size;
		if (window.flags & WindowFlag::AutoResize/* ||
			m_firstFrame*/)
		{
			windowSize = { 0, 0 };
			if (not (window.flags & WindowFlag::NoBackground) &&
				not (window.flags & WindowFlag::NoTitlebar))
			{
				windowSize.y += titlebarHeight();
			}
			windowSize += m_contentLocalRect.size;

			Size min = minSize();
			windowSize.x = Max(windowSize.x, min.x);
			windowSize.y = Max(windowSize.y, min.y);
		}
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
		// Print << m_input.m_hoveredItemId << (m_input.m_captured ? U"🔒" : U"");
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

		//for (auto [idx, layer] : Indexed(m_impl->layers()))
		//{
		//	const HSV color{ ((double)idx / 4 * 360), 0.8, 1.0 };
		//	for (auto& [id, impl] : layer.container())
		//	{
		//		auto& window = impl->window;

		//		window.rect.drawFrame(1, 0, color);
		//		Line{ window.rect.tl(), window.rect.br() }.draw(color);
		//		Line{ window.rect.bl(), window.rect.tr() }.draw(color);

		//		SimpleGUI::GetFont()(U"{}:{}"_fmt(id, window.displayName))
		//			.draw(Arg::bottomLeft = window.rect.tl(), color);
		//	}
		//}
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

		bool m_hovered;

		DrawableText m_labelText;

		bool m_clicked = false;

		Circle m_circle;

		Vec2 m_labelLeftCenter;

		Size computeSize() const override
		{
			Size labelSize = m_labelText.region().size.asPoint();
			return { labelSize.x + circleSize() + Config::CircleMargin, labelSize.y };
		}

		void update(Rect rect, Optional<Vec2> cursorPos) override
		{
			m_circle = { Arg::leftCenter = rect.leftCenter(), static_cast<RectF::size_type::value_type>(circleSize()) / 2};
			m_labelLeftCenter = m_circle.right() + Vec2{ Config::CircleMargin, 0 };

			m_hovered = cursorPos && rect.contains(*cursorPos);
			m_clicked = m_hovered && MouseL.down();
			m_selected |= m_clicked;

			if (m_hovered)
			{
				Cursor::RequestStyle(CursorStyle::Hand);
			}
		}

		void draw() const override
		{
			ColorF color, frameColor;
			if (m_selected)
			{
				color = Config::CheckedCircleColor;
				frameColor = Config::CheckedFrameColor;
			}
			else if (m_hovered)
			{
				color = Config::HoveredCircleColor;
				frameColor = Config::HoveredFrameColor;
			}
			else
			{
				color = Config::CircleColor;
				frameColor = Config::FrameColor;
			}

			m_circle
				.draw(color)
				.drawFrame(Config::CircleFrameThickness, frameColor);
			if (m_selected)
			{
				Circle{ m_circle.center, m_circle.r * Config::InnerCircleScale }
					.draw(frameColor);
			}
			m_labelText
				.draw(Arg::leftCenter = m_labelLeftCenter, Config::LabelColor);
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

				// タブ内部

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
						.draw(tab.mouseOver ? Config::HoveredTabColor : Config::TabColor);
				}

				// タブ枠線

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

				// タブ文字列

				tab.label.drawAt(tab.globalRect.center(), Config::LabelColor);
			}

			// タブ下部下線

			if (selectedIdx < m_tabs.size())
			{
				// 選択されているタブの下は空ける

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
				// 範囲外のときは空けない

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

	// ProgressBar

	class ProgressBar : public IControl
	{
	public:

		using Config = Config::ProgressBar;

		ProgressBar(double value, int32 width)
			: m_value(Clamp(value, 0.0, 1.0))
			, m_width(Max(Config::Height, width))
		{ }

	private:

		double m_value;

		int32 m_width;

		Rect m_rect;

		Size computeSize() const override
		{
			return { m_width, Config::Height };
		}

		void update(Rect rect, Optional<Vec2>) override
		{
			m_rect = rect;
		}

		void draw() const override
		{
			drawBackground();
			drawBar(m_value);
		}

		void drawBackground() const
		{
			constexpr double circleR = Config::Height * 0.5;

			RoundRect{ m_rect.pos, m_rect.w, Config::Height, circleR }
				.draw(Config::BackgroundColor)
				.drawFrame(0, Config::FrameThickness, Config::FrameColor);
		}

		void drawBar(double value) const
		{
			constexpr double circleR = Config::Height * 0.5;
			double barWidth = m_rect.w * value;

			if (barWidth <= Config::Height)
			{
				// 欠けた円を描画
				Circle{ m_rect.x + circleR, m_rect.y + circleR, circleR }
					.drawSegment(270_deg, barWidth, Config::BarColor);
			}
			else
			{
				// 両端が丸い線を描画
				RoundRect{ m_rect.pos, barWidth, Config::Height, circleR }
					.draw(Config::BarColor);
			}
		}
	};

	void GUIManager::progressbar(double value, int32 width)
	{
		getCurrentWindowImpl()
			.nextStatelessControl(std::make_shared<ProgressBar>(value, width));
	}

	// Custom

	void GUIManager::custom(std::shared_ptr<IControl> control)
	{
		getCurrentWindowImpl().nextStatelessControl(control);
	}
}
