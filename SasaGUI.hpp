#pragma once
#include <Siv3D.hpp> // OpenSiv3D v0.4.3
#include <any>

namespace s3d
{
	namespace SasaGUI
	{
		class GUIManager;

		enum WindowFlag
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
			/// コントロールのマージンを0にする
			/// </summary>
			NoMargin = 1 << 8,
			/// <summary>
			/// ウィンドウを隠す
			/// </summary>
			Hide = 1 << 9,
			/// <summary>
			/// 無効
			/// </summary>
			Disable = 1 << 10
		};

		enum class LayoutType
		{
			/// <summary>
			/// 垂直に配置
			/// </summary>
			Vertical,
			/// <summary>
			/// 水平に配置
			/// </summary>
			Horizontal,
			/// <summary>
			/// なし
			/// </summary>
			None
		};

		enum TextInputFlag
		{
			CharType = 0b0011,
			/// <summary>
			/// 数値('0' - '9' , '.' , '-')
			/// </summary>
			Number = 0,
			/// <summary>
			/// 半角(0x00 - 0x7F)
			/// </summary>
			Half = 1,
			/// <summary>
			/// 全て
			/// </summary>
			All = 2,
			/// <summary>
			/// 複数行
			/// </summary>
			MultiLine = 0b0100,
			/// <summary>
			/// タブを許可
			/// </summary>
			AcceptTab = 0b1000,
		};

		struct GUITheme
		{
			Font font;
			ColorF fontCol;
			ColorF fontDisableCol;
			ColorF frameCol;
			//Window
			ColorF windowTitlebarCol;
			ColorF windowBackCol;
			ColorF windowFrameCol;
			ColorF windowTitlebarFontCol;
			//Scrollbar
			ColorF scrollBarBackCol;
			ColorF scrollBarGripCol;
			//Button
			ColorF buttonBackCol;
			ColorF buttonFrameCol;
			ColorF buttonHoverCol;
			ColorF buttonGrabCol;
			ColorF buttonDisableCol;
			ColorF buttonFontCol;
			ColorF buttonDisableFontCol;
			//MenuItem
			ColorF menuItemCol;
			ColorF menuItemFontCol;
			//Accent
			ColorF accentCol;
			ColorF accentDisableCol;
			ColorF accentFontCol;
			ColorF accentDisableFontCol;
			//ProgressBar
			ColorF progressBarCol;
			//TextBox
			ColorF textBoxCol;
			ColorF textBoxSelectCol;
			ColorF textBoxDisableCol;
			ColorF textBoxFontCol;
			ColorF textBoxSelectFontCol;
			ColorF textBoxDisableFontCol;
			ColorF textBoxFrameCol;
			ColorF textBoxActiveFrameCol;
			static GUITheme Light(Font font = Font(16))
			{
				return {
					font,//font
					ColorF(0),//fontCol
					ColorF(0.3),//fontDisableCol
					ColorF(0.6),//frameCol

					ColorF(0.8),//windowTitlebarCol
					ColorF(0.9),//windowBackCol
					ColorF(0.7),//windowFrameCol
					ColorF(0),//windowTitlebarFontCol

					ColorF(0.85),//scrollBarBackCol
					ColorF(0.6),//scrollBarGripCol

					ColorF(1),//buttonBackCol
					ColorF(0.8),//buttonFrameCol
					ColorF(0.93, 0.95, 1.0),//buttonHoverCol
					ColorF(0.8,0.91,1),//buttonGrabCol
					ColorF(0.85),//buttonDisableCol
					ColorF(0),//buttonFontCol
					ColorF(0.3),//buttonDisableFontCol

					ColorF(0.2,0.5,1),//menuItemCol
					ColorF(1),//menuItemFontCol

					ColorF(0.2,0.6,1),//accentCol
					ColorF(0.7),//accentDisableCol
					ColorF(1),//accentFontCol
					ColorF(1),//accentDisableFontCol

					ColorF(0.4),//progressBarCol

					ColorF(1),//textBoxCol
					ColorF(0.1,0.5,0.9),//textBoxSelectCol
					ColorF(0.9),//textBoxDisableCol
					ColorF(0),//textBoxFontCol
					ColorF(1),//textBoxSelectFontCol
					ColorF(0.3),//textBoxDisableFontCol
					ColorF(0.5),//textBoxFrameCol
					ColorF(0.35, 0.7, 1.0)//textBoxActiveFrameCol
				};
			}
			static GUITheme Dark(Font font = Font(16))
			{
				return {
					font,//font
					ColorF(1),//fontCol
					ColorF(0.8),//fontDisableCol
					ColorF(0.4),//frameCol

					ColorF(0.2),//windowTitlebarCol
					ColorF(0.1),//windowBackCol
					ColorF(0.3),//windowFrameCol
					ColorF(1),//windowTitlebarFontCol

					ColorF(0.1),//scrollBarBackCol
					ColorF(0.4),//scrollBarGripCol

					ColorF(0.3),//buttonBackCol
					ColorF(0.2),//buttonFrameCol
					ColorF(0.4),//buttonHoverCol
					ColorF(0.5),//buttonGrabCol
					ColorF(0.3),//buttonDisableCol
					ColorF(1),//buttonFontCol
					ColorF(0.7),//buttonDisableFontCol

					ColorF(0,0.4,0.8),//menuItemCol
					ColorF(1),//menuItemFontCol

					ColorF(0.1,0.4,0.8),//accentCol
					ColorF(0.5),//accentDisableCol
					ColorF(1),//accentFontCol
					ColorF(1),//accentDisableFontCol

					ColorF(0.2),//progressBarCol

					ColorF(0.25),//textBoxCol
					ColorF(0.1,0.4,0.7),//textBoxSelectCol
					ColorF(0.3),//textBoxDisableCol
					ColorF(1),//textBoxFontCol
					ColorF(1),//textBoxSelectFontCol
					ColorF(0.7),//textBoxDisableFontCol
					ColorF(0.3),//textBoxFrameCol
					ColorF(0.3,0.5,0.75)//textBoxActiveFrameCol
				};
			}
		};

		using WindowFlags = int32;
		using ID = size_t;

		namespace detail
		{
			const double WindowR = 3;
			const double WindowPadding = 5;
			const double WindowResizeGripSize = 5;
			const SizeF WindowMinSize = SizeF(100, 100);
			const double CtrlMargin = 5;
			const double ScrollbarR = 8;
			const double ScrollbarPadding = 5;
			const double CheckBoxRadioButtonSize = 16;
			const LineString CheckLine{ {-0.5,-0.14},{-0.21,0.44},{0.64,-0.64} };
			const double CheckTickness = 0.28;
			const Vec2 TextBoxPadding = Vec2(2, 2);
			const Vec2 TextButtonPadding = Vec2(7, 4);
			const Vec2 ImageButtonPadding = Vec2(4, 4);
			const Vec2 TabItemPadding = Vec2(5, 0);
			const Vec2 TreeNodePadding = Vec2(0, 0);

			Vec2 CursorDeltaF()
			{
				//タッチ操作の移動量吹っ飛び対策
				if (MouseL.down())
				{
					return Vec2(0.0, 0.0);
				}
				return Cursor::DeltaF();
			}

			bool KeyRepeat(const Key& key)
			{
				if (key.down())
				{
					return true;
				}
				if (key.pressed())
				{
					const auto duration = key.pressedDuration().count();
					if (duration >= 1.0)
					{
						return true;
					}
				}
				return false;
			}

			void drawTriangle(Vec2 center, double r, double theta, ColorF col)
			{
				Triangle(Circular(r, theta), Circular(r, theta + 120_deg), Circular(r, theta + 240_deg))
					.setCentroid(center)
					.draw(col);
			}

			Optional<RectF> rectAnd(RectF a, RectF b)
			{
				Vec2 bg(Max(a.x, b.x), Max(a.y, b.y));
				Vec2 ed(Min(a.x + a.w, b.x + b.w) - 1, Min(a.y + a.h, b.y + b.h) - 1);
				if (bg.x > ed.x || bg.y > ed.y)
				{
					return unspecified;
				}
				return RectF(bg, ed - bg + Vec2(1, 1));
			}

			class ScopedScissorRect : Uncopyable
			{
			private:
				Rect m_oldScissorRect;
			public:
				ScopedScissorRect(RectF rect, bool transform = true)
					:m_oldScissorRect(Graphics2D::GetScissorRect())
				{
					if (transform)
					{
						rect.pos = Graphics2D::GetLocalTransform().transform(rect.pos);
					}
					auto newRect = rectAnd(rect, m_oldScissorRect);
					if (newRect)
					{
						Graphics2D::SetScissorRect(*newRect);
					}
					else
					{
						Graphics2D::SetScissorRect(Rect(0, 0, 0, 0));
					}
				}

				~ScopedScissorRect()
				{
					Graphics2D::SetScissorRect(m_oldScissorRect);
				}
			};

			enum class CursorStyle
			{
				Arrow,

				IBeam,

				Cross,

				Hand,

				NotAllowed,

				ResizeUpDown,

				ResizeLeftRight,

				Hidden,

				ResizeNESW,

				ResizeNWSE,

				NoRequest,
			};

			static CursorStyle cursor = CursorStyle::NoRequest;

			void CursorRequestStyle(CursorStyle style)
			{
				cursor = style;
			}

			void ResetCursor()
			{
				cursor = CursorStyle::NoRequest;
			}

			void DrawCursor()
			{
				Vec2 cursorPos = Cursor::PosF();
				switch (cursor)
				{
				case CursorStyle::NoRequest:
					break;
				case CursorStyle::ResizeNESW:
					Cursor::RequestStyle(s3d::CursorStyle::Hidden);
					Line(cursorPos, cursorPos + Vec2(-8, 8)).drawArrow(4, Vec2(6, 6), Palette::Black);
					Line(cursorPos, cursorPos + Vec2(8, -8)).drawArrow(4, Vec2(6, 6), Palette::Black);
					Line(cursorPos, cursorPos + Vec2(-7, 7)).drawArrow(2, Vec2(4, 4), Palette::White);
					Line(cursorPos, cursorPos + Vec2(7, -7)).drawArrow(2, Vec2(4, 4), Palette::White);
					break;
				case CursorStyle::ResizeNWSE:
					Cursor::RequestStyle(s3d::CursorStyle::Hidden);
					Line(cursorPos, cursorPos + Vec2(-8, -8)).drawArrow(4, Vec2(6, 6), Palette::Black);
					Line(cursorPos, cursorPos + Vec2(8, 8)).drawArrow(4, Vec2(6, 6), Palette::Black);
					Line(cursorPos, cursorPos + Vec2(-7, -7)).drawArrow(2, Vec2(4, 4), Palette::White);
					Line(cursorPos, cursorPos + Vec2(7, 7)).drawArrow(2, Vec2(4, 4), Palette::White);
					break;
				default:
					Cursor::RequestStyle(static_cast<s3d::CursorStyle>(cursor));
					break;
				}
			}

			//垂直スクロールバー
			class VerticalScrollbar
			{
			private:
				RoundRect m_scarea;
				RoundRect m_grip;
				bool m_grab;
			public:
				double m_max;
				double m_range;
				double m_inc = 1;
				Rect m_rect;
				void update(double& value, const bool hovered)
				{
					if (m_rect.mouseOver() && hovered)
					{
						value += Mouse::Wheel() * m_inc;
					}
					if (m_grab)
					{
						if (!MouseL.pressed())
						{
							m_grab = false;
						}
						else
						{
							value += CursorDeltaF().y * m_max / m_scarea.h;
						}
					}
					else
					{
						if (m_grip.leftClicked() && hovered)
						{
							m_grab = true;
						}
						else if (!m_grip.leftPressed() && m_scarea.leftPressed() && hovered)
						{
							value += Math::Sign(Cursor::PosF().y - m_grip.y - m_grip.h / 2) * m_inc;
						}
					}
					value = Clamp<double>(value, 0, m_max - m_range);
					m_scarea = RoundRect(m_rect.tr() + Vec2(-ScrollbarR * 2 - ScrollbarPadding, ScrollbarPadding), ScrollbarR * 2, m_rect.h - ScrollbarPadding * 2 - ScrollbarR * 2, ScrollbarR);
					m_grip = RoundRect(m_scarea);
					m_grip.y += m_scarea.h * value / m_max;
					m_grip.h *= m_range / m_max;
				}
				void draw(ColorF main = ColorF(0.8), ColorF grip = ColorF(0.6))
				{
					m_scarea.draw(main);
					m_grip.draw(grip);
				}
			};

			//水平スクロールバー
			class HorizontalScrollbar
			{
			private:
				RoundRect m_scarea;
				RoundRect m_grip;
				bool m_grab;
			public:
				double m_max;
				double m_range;
				double m_inc = 1;
				Rect m_rect;
				void update(double& value, const bool hovered)
				{
					if (m_rect.mouseOver() && hovered)
					{
						value += Mouse::WheelH() * m_inc;
					}
					if (m_grab)
					{
						if (!MouseL.pressed())
						{
							m_grab = false;
						}
						else
						{
							value += CursorDeltaF().x * m_max / m_scarea.w;
						}
					}
					else
					{
						if (m_grip.leftClicked() && hovered)
						{
							m_grab = true;
						}
						else if (!m_grip.leftPressed() && m_scarea.leftPressed() && hovered)
						{
							value += Math::Sign(Cursor::PosF().x - m_grip.x - m_grip.w / 2) * m_inc;
						}
					}
					value = Clamp<double>(value, 0, m_max - m_range);
					m_scarea = RoundRect(m_rect.bl() + Vec2(ScrollbarPadding, -ScrollbarR * 2 - ScrollbarPadding), m_rect.w - ScrollbarPadding * 2 - ScrollbarR * 2, ScrollbarR * 2, ScrollbarR);
					m_grip = RoundRect(m_scarea);
					m_grip.x += m_scarea.w * value / m_max;
					m_grip.w *= m_range / m_max;
				}
				void draw(ColorF main = ColorF(0.8), ColorF grip = ColorF(0.6))
				{
					m_scarea.draw(main);
					m_grip.draw(grip);
				}
			};

			//テキストボックスの基本機能
			class TextBoxBase
			{
			private:

				bool m_isActive = false;

				bool m_isSelecting = false;

				bool m_textChanged = false;

				bool m_enabled = true;

				size_t m_selectBegin;

				size_t m_selectEnd;

				String m_selectStr;

				size_t m_cursorIndex = 0;

				double m_cursorBeginSec;

				String m_editingText;

				Vec2 m_scroll;

				String m_text;

				Font m_font;

				int32 m_flags;

				RectF m_rect;

				String m_rawInput;

				size_t getHoveringIndex()
				{
					if (m_text)
					{
						const Vec2 mousePos = Cursor::PosF() - m_rect.pos - m_scroll - detail::TextBoxPadding;
						Vec2 penPos(0, 0);
						size_t index = 0;
						for (const auto& glyph : m_font(m_text))
						{
							if (mousePos.x >= penPos.x && mousePos.y >= penPos.y)
							{
								index = glyph.index;
							}
							if (glyph.codePoint == U'\n')
							{
								penPos.x = 0;
								penPos.y += m_font.height();
								continue;
							}
							penPos.x += glyph.xAdvance;
						}
						if (mousePos.x >= penPos.x && mousePos.y >= penPos.y)
						{
							index = static_cast<int32>(m_text.length());
						}
						return index;
					}
					else
					{
						return 0;
					}
				}

				Vec2 getDrawPos(size_t index)
				{
					Vec2 penPos(0, 0);
					if (m_text)
					{
						for (const auto& glyph : m_font(m_text))
						{
							if (index == glyph.index)
							{
								break;
							}
							if (glyph.codePoint == U'\n')
							{
								penPos.x = 0;
								penPos.y += m_font.height();
								continue;
							}
							penPos.x += glyph.xAdvance;
						}
					}
					penPos.x += m_font(m_editingText).region().w;
					return penPos;
				}

				Array<size_t> linesIndex(const String& str)
				{
					size_t bg = 0, ed;
					Array<size_t> result;
					ed = str.indexOf(U'\n', bg);
					while (ed != String::npos)
					{
						result << ed - bg;
						bg = ed + 1;
						ed = str.indexOf(U'\n', bg);
					}
					result << str.length() - bg;
					return result;
				}

				void procCommandKey(bool ctrlPressed)
				{
					if (ctrlPressed)
					{
						if (KeyX.down())
						{
							cut();
						}
						if (KeyC.down())
						{
							copy();
						}
						if (KeyV.down())
						{
							paste();
						}
						if (KeyA.down())
						{
							selectAll();
						}
					}
				}

			public:

				TextBoxBase(const Font& font, const int32 flags, const String& text = U"")
				{
					setText(text);
					m_font = font;
					m_flags = flags;
					m_cursorBeginSec = Scene::Time();
				}

				//アクティブか
				bool isActive()
				{
					return m_isActive;
				}

				//選択しているか
				bool isSelecting()
				{
					return m_isSelecting && m_selectBegin != m_selectEnd;
				}

				//テキストが変更されたか
				bool textChanged()
				{
					return m_textChanged;
				}

				String getSelectingText()
				{
					if (isSelecting())
					{
						const size_t selectBg = Min(m_selectBegin, m_selectEnd);
						const size_t selectEd = Max(m_selectBegin, m_selectEnd);
						return m_text.substr(selectBg, selectEd - selectBg);
					}
					else
					{
						return U"";
					}
				}

				void cut()
				{
					if (isSelecting())
					{
						Clipboard::SetText(getSelectingText());
						m_rawInput += U'\b';
					}
				}

				void copy()
				{
					if (isSelecting())
					{
						Clipboard::SetText(getSelectingText());
					}
				}

				void paste()
				{
					String text;
					if (Clipboard::GetText(text))
					{
						m_rawInput += text;
					}
				}

				void selectAll()
				{
					m_isSelecting = true;
					m_cursorIndex = m_text.length();
					m_selectBegin = 0;
					m_selectEnd = m_cursorIndex;
				}

				//テキストを取得
				String getText()
				{
					return m_text;
				}

				//テキストを設定
				//\rは\nに変換
				void setText(const String& text)
				{
					m_text = text.replaced(U'\r', U'\n');
					m_isSelecting = false;
					m_cursorIndex = 0;
					m_scroll = Vec2(0, 0);
				}

				//更新処理
				void update(bool hovered, RectF rect, bool enabled, bool lPressed = MouseL.pressed(), bool lDown = MouseL.down(), bool rDown = MouseR.down())
				{
					m_rect = rect;
					m_enabled = enabled;
					const bool isHovering = hovered && m_rect.mouseOver();
					m_textChanged = false;

					//アクティブ/非アクティブを切り替え
					if (m_isActive)
					{
						if (!m_enabled || !(isHovering) && (lDown || rDown))
						{
							m_isActive = false;
							m_isSelecting = false;
							m_editingText = U"";
						}
					}
					else
					{
						if (m_enabled && isHovering && (lDown || rDown))
						{
							m_isActive = true;
							m_cursorBeginSec = Scene::Time();
						}
					}

					//キーボード入力
					if (m_isActive)
					{
# if SIV3D_PLATFORM(MACOS)
						const bool ctrlPressed = KeyCommand.pressed();
# else
						const bool ctrlPressed = KeyControl.pressed();
#endif

						procCommandKey(ctrlPressed);

						const size_t hoveringIdx = getHoveringIndex();
						const size_t previousCursorIndex = m_cursorIndex;

						Array<size_t> linesLength;//各行の長さ
						Vector2D<size_t> cursorPos(0, m_cursorIndex);//カーソルの行,列
						{
							size_t bg = 0, ed;
							ed = m_text.indexOf(U'\n', bg);
							while (ed != String::npos)
							{
								linesLength << ed - bg;
								bg = ed + 1;
								if (bg <= m_cursorIndex)
								{
									cursorPos.x++;
									cursorPos.y = m_cursorIndex - bg;
								}
								ed = m_text.indexOf(U'\n', bg);
							}
							linesLength << static_cast<int32>(m_text.length()) - bg;
						}

						if (!ctrlPressed)
						{
							m_rawInput += TextInput::GetRawInput();
						}
						m_editingText = TextInput::GetEditingText();
						//カーソル移動
						if (m_editingText.isEmpty())
						{
							if (m_cursorIndex > 0 && detail::KeyRepeat(KeyLeft))
							{
								m_cursorIndex--;
							}
							if (detail::KeyRepeat(KeyRight))
							{
								m_cursorIndex++;
							}
							if (cursorPos.x > 0 && detail::KeyRepeat(KeyUp))
							{
								m_cursorIndex -= cursorPos.y + 1;
								m_cursorIndex -= Max<size_t>(linesLength[cursorPos.x - 1] - cursorPos.y, 0);
								cursorPos.x--;
							}
							if (cursorPos.x < linesLength.size() - 1 && detail::KeyRepeat(KeyDown))
							{
								m_cursorIndex += linesLength[cursorPos.x] - cursorPos.y + 1;
								m_cursorIndex += Min<size_t>(cursorPos.y, linesLength[cursorPos.x + 1]);
								cursorPos.x++;
							}
						}
						if (lPressed)
						{
							m_cursorIndex = hoveringIdx;
							m_cursorIndex = hoveringIdx;
						}
						m_cursorIndex = Clamp<size_t>(m_cursorIndex, 0, m_text.length());
						//選択
						if (m_cursorIndex == previousCursorIndex)
						{
							if (lDown)
							{
								m_isSelecting = false;
							}
						}
						else
						{
							m_cursorBeginSec = Scene::Time();
							if ((lPressed && !lDown) || KeyShift.pressed())
							{
								m_selectEnd = m_cursorIndex;
								if (!m_isSelecting)
								{
									m_selectBegin = previousCursorIndex;
								}
								m_isSelecting = true;
							}
							else
							{
								m_isSelecting = false;
							}
						}
					}

					//文字列入力
					const size_t selectBg = Min(m_selectBegin, m_selectEnd);
					const size_t selectEd = Max(m_selectBegin, m_selectEnd);
					if (m_isSelecting)
					{
						if (m_rawInput)
						{
							m_text.erase(selectBg, selectEd - selectBg);
							m_rawInput.remove(U'\b');//BackSpace
							m_rawInput.remove(U'\x7f');//Delete
							m_cursorIndex = selectBg;
							m_isSelecting = false;
						}
					}
					for (auto& chr : m_rawInput)
					{
						switch (chr)
						{
						case U'\b'://BackSpace
							if (m_cursorIndex > 0)
							{
								m_cursorIndex--;
								m_text.remove_at(m_cursorIndex);
								m_textChanged = true;
							}
							break;
						case U'\x7f'://Delete
							if (m_cursorIndex < m_text.length())
							{
								m_text.remove_at(m_cursorIndex);
								m_textChanged = true;
							}
							break;
						case U'\t'://Tab
							if (!(m_flags & TextInputFlag::AcceptTab))
							{
								m_isActive = false;
								break;
							}
							m_text.insert(m_cursorIndex, 1, chr);
							m_cursorIndex++;
							m_textChanged = true;
							break;
						case U'\r'://Return
							chr = U'\n';
						case U'\n':
							if (!(m_flags & TextInputFlag::MultiLine))
							{
								m_isActive = false;
								break;
							}
							m_text.insert(m_cursorIndex, 1, chr);
							m_cursorIndex++;
							m_textChanged = true;
							break;
						default:
							switch (m_flags & TextInputFlag::CharType)
							{
							case TextInputFlag::Number:
								if (!InRange<int32>(chr, U'0', U'9') && chr != U'-' && chr != U'.')
								{
									break;
								}
							case TextInputFlag::Half:
								if (!InRange<int32>(chr, 0, 127))
								{
									break;
								}
							case TextInputFlag::All:
							default:
								m_text.insert(m_cursorIndex, 1, chr);
								m_cursorIndex++;
								m_textChanged = true;
								break;
							}
							break;
						}
					}
					m_rawInput = U"";

					//スクロール
					{
						Vec2 drawPos = getDrawPos(m_cursorIndex);
						m_scroll.x = Clamp(m_scroll.x, -drawPos.x, -drawPos.x - 2 + m_rect.w - detail::TextBoxPadding.x * 2);
						m_scroll.y = Clamp(m_scroll.y, -drawPos.y, -drawPos.y - m_font.height() + m_rect.h - detail::TextBoxPadding.y * 2);
					}

					//マウスカーソル
					if (enabled && (isHovering || (m_isSelecting && lPressed)))
					{
						detail::CursorRequestStyle(detail::CursorStyle::IBeam);
					}
				}

				//描画処理
				void draw(const GUITheme& theme)
				{
					const size_t selectBg = Min(m_selectBegin, m_selectEnd);
					const size_t selectEd = Max(m_selectBegin, m_selectEnd);
					bool showCursor = m_isActive && !(Periodic::Square0_1(1s, Scene::Time() - m_cursorBeginSec) == 0.0);
					Vec2 penPos(m_rect.pos + m_scroll + detail::TextBoxPadding);
					Optional<Vec2> cursorPos;
					{
						detail::ScopedScissorRect clip(m_rect);
						if (m_text)
						{
							for (const auto& glyph : m_font(m_text))
							{
								const bool isLn = glyph.codePoint == U'\n';
								if (glyph.index == m_cursorIndex)
								{
									cursorPos = penPos;
									penPos.x += m_font(m_editingText).region().w;
								}
								const Vec2 drawPos = penPos + glyph.offset;
								if (m_isSelecting && selectBg <= glyph.index && glyph.index <= selectEd - 1)
								{
									RectF(penPos, isLn ? m_font.height() / 4 : glyph.xAdvance, m_font.height()).draw(theme.textBoxSelectCol);
									glyph.texture.draw(drawPos, theme.textBoxSelectFontCol);
								}
								else
								{
									glyph.texture.draw(drawPos, m_enabled ? theme.textBoxFontCol : theme.textBoxDisableFontCol);
								}
								if (isLn)
								{
									penPos.x = m_rect.x + m_scroll.x + detail::TextBoxPadding.x;
									penPos.y += m_font.height();
									continue;
								}
								penPos.x += glyph.xAdvance;
							}
							if (m_cursorIndex == m_text.length())
							{
								cursorPos = penPos;
							}
						}
						else
						{
							cursorPos = penPos;
						}
						if (cursorPos)
						{
							if (m_editingText)
							{
								m_font(m_editingText).draw(*cursorPos, theme.textBoxFontCol).bottom().movedBy(0, -0.5).draw(theme.textBoxFontCol);
							}
							else if (showCursor)
							{
								RectF(*cursorPos, SizeF(2, m_font.height())).draw(theme.textBoxFontCol);
							}
						}
					}
				}
			};

			//ボタンの基本機能
			class ButtonBase
			{
			private:
				bool m_hover = false;
				bool m_grab = false;
				bool m_down = false;
				bool m_click = false;
				bool m_enabled = true;
			public:
				bool hover()
				{
					return m_hover;
				}
				bool grab()
				{
					return m_grab;
				}
				bool down()
				{
					return m_down;
				}
				bool click()
				{
					return m_click;
				}
				bool enabled()
				{
					return m_enabled;
				}
				void update(bool hover, bool down, bool pressed, bool enabled)
				{
					m_enabled = enabled;
					if (m_enabled)
					{
						m_hover = hover;
						m_down = m_hover && down;
						m_click = false;
						if (m_down)
						{
							m_grab = true;
						}
						if (m_grab)
						{
							if (!pressed)
							{
								if (m_hover)
								{
									m_click = true;
								}
								m_grab = false;
							}
						}
					}
					else
					{
						m_hover = false;
						m_down = false;
						m_click = false;
						m_grab = false;
					}
					if (m_hover || m_grab)
					{
						detail::CursorRequestStyle(detail::CursorStyle::Hand);
					}
				}
				ColorF getBackColor(const GUITheme& theme)
				{
					if (!m_enabled)
					{
						return theme.buttonDisableCol;
					}
					else if (m_grab)
					{
						return theme.buttonGrabCol;
					}
					else if (m_hover)
					{
						return theme.buttonHoverCol;
					}
					return theme.buttonBackCol;
				}
				ColorF getFrameColor(const GUITheme& theme)
				{
					return theme.buttonFrameCol;
				}
				ColorF getFontColor(const GUITheme& theme)
				{
					return m_enabled ? theme.buttonFontCol : theme.buttonDisableFontCol;
				}
			};

			//ウィンドウの表示位置
			enum WindowLayer
			{
				Background, Nomal, Foreground
			};

			//ウィンドウの種類
			enum class WindowType
			{
				Nomal,
				ToolTip,
				Menu
			};

			struct Group;
			struct IControl;

			//ウィンドウ
			struct Window
			{
				ID id;
				bool m_used;
				String m_title;
				RectF m_rect;
				Vec2 m_cursor;
				WindowFlags flags;
				WindowFlags prevFlags;
				WindowLayer layer;
				WindowType type = WindowType::Nomal;
				bool itemHovered = false;

				//Layout
				double ctrlMargin;
				LayoutType layout = LayoutType::Horizontal;
				RectF contentRect;
				RectF clientRect;
				Vec2 contentMov;

				//Move
				bool m_grab = false;

				//Resize
				int32 resizeSide;

				//Scrollbar
				bool vScrollbar_show;
				VerticalScrollbar vScrollbar;
				bool hScrollbar_show;
				HorizontalScrollbar hScrollbar;

				//Control
				Array<std::shared_ptr<IControl>> controls;
				std::shared_ptr<IControl> lastUsedCtrl;

				//Group
				Array<Group> groups;
				Array<size_t> groupStack;
			};

			//グループ
			struct Group
			{
				RectF rect;
				String label;
				bool frame;
				bool enabled = true;
				Vec2 cursorBg;
				Vec2 br;
				Optional<size_t> parentIdx = unspecified;
				bool getEnabled(const Window& wnd)
				{
					if (parentIdx)
					{
						return wnd.groups[*parentIdx].enabled && enabled;
					}
					return enabled;
				}
			};

			struct IControl
			{
				ID m_id;
				bool m_used;
				RectF m_rect;
				size_t m_groupIdx;
				bool m_enabled = true;
				bool getEnabled(Window& wnd)
				{
					return m_enabled && wnd.groups[m_groupIdx].getEnabled(wnd) && !(wnd.flags & WindowFlag::Disable);
				}
				virtual void update(GUIManager& mgr, Window& wnd) = 0;
				virtual void draw(GUIManager& mgr, Window& wnd) = 0;
			};
		}

		class GUIManager
		{
		private:
			const size_t DefaultWindow = 0;

			const WindowFlags defaultWindowFlags = NoTitlebar | NoResize | NoMove | NoBackground;

			const WindowFlags tooltipFlags = NoMove | AutoResize | NoTitlebar | AlwaysForeground | NoFocus;

			const WindowFlags dropdownlistFlags = NoMove | AutoResize | NoTitlebar | AlwaysForeground | NoMargin;

			const WindowFlags menuFlags = NoMove | AutoResize | NoTitlebar | AlwaysForeground | NoMargin;

			GUITheme m_theme;

			Array<detail::Window> windows;
			std::array<Array<size_t>, 3> windowOrder;
			Array<size_t> currentWindow;
			Optional<size_t> hoveringWindow;

			bool debug_drawRect = false;
			bool debug_window = false;

			size_t addWindow(detail::Window window, detail::WindowLayer layer)
			{
				const auto idx = windows.size();
				window.layer = layer;
				windows << window;
				windowOrder[layer] << idx;
				updateWindow(window);
				return idx;
			}
			void delWindow(size_t idx)
			{
				if (idx == DefaultWindow)
				{
					return;
				}
				windows.remove_at(idx);
				for (auto& tOrder : windowOrder)
				{
					for (size_t i = 0; i < tOrder.size();)
					{
						auto& order = tOrder[i];
						if (order == idx)
						{
							tOrder.remove_at(i);
							continue;
						}
						if (order > idx)
						{
							order--;
						}
						i++;
					}
				}
			}
			size_t getCurrentWindowIdx()
			{
				return currentWindow[currentWindow.size() - 1];
			}
			detail::Window& getCurrentWindow()
			{
				return windows[getCurrentWindowIdx()];
			}
			void updateControl(detail::Window& window, std::shared_ptr<detail::IControl> ctrl)
			{
				ctrl->m_used = true;
				ctrl->m_groupIdx = window.groupStack[window.groupStack.size() - 1];// &window.groups[window.groups.size() - 1];
				window.lastUsedCtrl = ctrl;
			}
			template<class T>
			std::shared_ptr<T> getControl(detail::Window& window, ID id)
			{
				for (const auto& ctrl : window.controls)
				{
					//IDが同じでダウンキャストができたら
					if (ctrl->m_id == id)
					{
						auto ptr = std::dynamic_pointer_cast<T, detail::IControl>(ctrl);
						if (ptr)
						{
							updateControl(window, ptr);
							return ptr;
						}
					}
				}
				return std::shared_ptr<T>();
			}
			void addControl(detail::Window& window, std::shared_ptr<detail::IControl> ctrl, ID id)
			{
				ctrl->m_id = id;
				window.controls << ctrl;
				updateControl(window, ctrl);
			}
			ID getID(const String& str)
			{
				return std::hash<String>()(str);
			}
			ID getID(const Rect& rect)
			{
				return std::hash<Rect>()(rect);
			}
			ID getID(const s3d::Texture& tex)
			{
				return std::hash<uint32>()(tex.id().value());
			}
			template<class T>
			ID getID(T* ptr)
			{
				return std::hash<T*>()(ptr);
			}
			template<class T>
			ID getID(const s3d::Array<T>& ary)
			{
				size_t total = 0;
				for (const auto& val : ary)
				{
					total += std::hash<T>()(val);
				}
				return total;
			}

			//ウィンドウを更新
			void updateWindow(detail::Window& window)
			{
				const auto titlebarHeight = getTheme().font.height();
				window.m_cursor = Vec2(detail::WindowPadding, detail::WindowPadding);
				window.m_used = false;
				window.lastUsedCtrl = nullptr;
				//自動リサイズ
				if (window.flags & WindowFlag::AutoResize)
				{
					window.m_rect.size = window.contentRect.size;
					if (!(window.flags & WindowFlag::NoTitlebar))
					{
						window.m_rect.h += titlebarHeight;
					}
				}
				//移動
				if (!(window.flags & WindowFlag::NoTitlebar))
				{
					if (!(window.flags & WindowFlag::NoMove))
					{
						auto titleRect = RoundRect(window.m_rect.pos, window.m_rect.w, titlebarHeight, 3);
						if (windowHovered() && titleRect.leftClicked())
						{
							window.m_grab = true;
						}
						if (window.m_grab)
						{
							if (!MouseL.pressed())
							{
								window.m_grab = false;
							}
							else
							{
								auto mov = detail::CursorDeltaF();
								window.m_rect.pos += mov;
								window.m_rect.x = Clamp<double>(window.m_rect.x, -window.m_rect.w + 20, Scene::Width() - 20);
								window.m_rect.y = Clamp<double>(window.m_rect.y, 0, Scene::Height() - titlebarHeight);
							}
						}
					}
				}
				//リサイズ
				if (!(window.flags & WindowFlag::NoResize) &&
					!(window.flags & WindowFlag::AutoResize))
				{
					int32 cursorSide = 0;
					if (window.resizeSide)
					{
						if (MouseL.pressed())
						{
							cursorSide = window.resizeSide;
							Vec2 delta = detail::CursorDeltaF();
							if (window.resizeSide & 0b0001)
							{
								//Top
								delta.y = Clamp(delta.y, -window.m_rect.y, window.m_rect.h - detail::WindowMinSize.y);
								window.m_rect.y += delta.y;
								window.m_rect.h -= delta.y;
							}
							if (window.resizeSide & 0b0010)
							{
								//Bottom
								delta.y = Clamp(delta.y, detail::WindowMinSize.y - window.m_rect.h, Scene::Height() - window.m_rect.y - window.m_rect.h);
								window.m_rect.h += delta.y;
							}
							if (window.resizeSide & 0b0100)
							{
								//Left
								delta.x = Clamp(delta.x, -window.m_rect.x, window.m_rect.w - detail::WindowMinSize.x);
								window.m_rect.x += delta.x;
								window.m_rect.w -= delta.x;
							}
							if (window.resizeSide & 0b1000)
							{
								//Right
								delta.x = Clamp(delta.x, detail::WindowMinSize.x - window.m_rect.w, Scene::Width() - window.m_rect.x - window.m_rect.w);
								window.m_rect.w += delta.x;
							}
						}
						else
						{
							window.resizeSide = 0;
						}
					}
					else
					{
						//Top
						if (RectF(window.m_rect.tl() + Vec2(-detail::WindowResizeGripSize, -detail::WindowResizeGripSize),
							window.m_rect.w + detail::WindowResizeGripSize * 2, detail::WindowResizeGripSize)
							.mouseOver())
						{
							cursorSide |= 0b0001;
						}
						//Bottom
						if (RectF(window.m_rect.bl() + Vec2(-detail::WindowResizeGripSize, 1),
							window.m_rect.w + detail::WindowResizeGripSize * 2, detail::WindowResizeGripSize)
							.mouseOver())
						{
							cursorSide |= 0b0010;
						}
						//Left
						if (RectF(window.m_rect.tl() + Vec2(-detail::WindowResizeGripSize, -detail::WindowResizeGripSize),
							detail::WindowResizeGripSize, window.m_rect.h + detail::WindowResizeGripSize * 2)
							.mouseOver())
						{
							cursorSide |= 0b0100;
						}
						//Right
						if (RectF(window.m_rect.tr() + Vec2(1, -detail::WindowResizeGripSize),
							detail::WindowResizeGripSize, window.m_rect.h + detail::WindowResizeGripSize * 2)
							.mouseOver())
						{
							cursorSide |= 0b1000;
						}

						if (!windowHovered())
						{
							cursorSide = 0;
						}
						else if (MouseL.down() && cursorSide)
						{
							window.resizeSide = cursorSide;
						}
						else if (MouseL.pressed() || MouseR.pressed())
						{
							cursorSide = 0;
						}
					}
					if (cursorSide)
					{
						switch (cursorSide)
						{
						case 0b0101:
						case 0b1010:
							// ＼
							detail::CursorRequestStyle(detail::CursorStyle::ResizeNWSE);
							break;
						case 0b1001:
						case 0b0110:
							// ／
							detail::CursorRequestStyle(detail::CursorStyle::ResizeNESW);
							break;
						case 0b0001:
						case 0b0010:
							// ｜
							detail::CursorRequestStyle(detail::CursorStyle::ResizeUpDown);
							break;
						case 0b0100:
						case 0b1000:
							// ー
							detail::CursorRequestStyle(detail::CursorStyle::ResizeLeftRight);
							break;
						}
					}
				}
				//クライアント領域計算
				window.clientRect = RectF(window.m_rect.size);
				if (!(window.flags & WindowFlag::NoTitlebar))
				{
					window.clientRect.y += titlebarHeight;
					window.clientRect.h -= titlebarHeight;
				}
				window.itemHovered = window.clientRect.movedBy(window.m_rect.pos).mouseOver();
				//スクロールバー処理
				window.hScrollbar.m_rect = window.clientRect;
				window.vScrollbar.m_rect = window.clientRect;
				window.hScrollbar_show = window.contentRect.w > window.clientRect.w && !(window.flags & WindowFlag::NoScrollbar);
				window.vScrollbar_show = window.contentRect.h > window.clientRect.h && !(window.flags & WindowFlag::NoScrollbar);
				if (window.hScrollbar_show)
				{
					window.clientRect.h -= detail::ScrollbarR * 2 + detail::ScrollbarPadding;
				}
				if (window.vScrollbar_show)
				{
					window.clientRect.w -= detail::ScrollbarR * 2 + detail::ScrollbarPadding;
				}
				{
					Transformer2D transform(Mat3x2::Translate(window.m_rect.pos), true);
					if (window.hScrollbar_show)
					{
						window.hScrollbar.m_max = window.contentRect.w;
						window.hScrollbar.m_inc = 10;
						window.hScrollbar.m_range = window.clientRect.w;
						window.hScrollbar.update(window.contentMov.x, windowHovered());
					}
					else
					{
						window.contentMov.x = 0;
					}
					if (window.vScrollbar_show)
					{
						window.vScrollbar.m_max = window.contentRect.h;
						window.vScrollbar.m_range = window.clientRect.h;
						window.vScrollbar.m_inc = 10;
						window.vScrollbar.m_range = window.clientRect.h;
						window.vScrollbar.update(window.contentMov.y, windowHovered());
					}
					else
					{
						window.contentMov.y = 0;
					}
				}
				window.contentRect = RectF(window.clientRect.pos - window.contentMov, Vec2());
				{
					Transformer2D transform(Mat3x2::Translate(window.m_rect.pos + window.contentRect.pos), true);
					for (auto& control : window.controls)
					{
						control->m_used = false;
						control->update(*this, window);
					}
				}
				window.groups.clear();
				window.groupStack.clear();
				window.prevFlags = window.flags;
			}

			//ウィンドウを描画
			void drawWindow(detail::Window& window)
			{
				const auto& theme = getTheme();
				const auto titlebarHeight = theme.font.height();
				//自動リサイズ
				if (window.flags & WindowFlag::AutoResize)
				{
					window.m_rect.size = window.contentRect.size;
					if (!(window.flags & WindowFlag::NoTitlebar))
					{
						window.m_rect.h += titlebarHeight;
					}
				}
				//背景描画
				if (!(window.flags & WindowFlag::NoBackground))
				{
					RoundRect(window.m_rect, detail::WindowR).draw(theme.windowBackCol).drawFrame(0, 1, theme.windowFrameCol);
				}
				//タイトルバー描画
				if (!(window.flags & WindowFlag::NoTitlebar))
				{
					auto titleRect = RectF(window.m_rect.pos, window.m_rect.w, titlebarHeight);
					titleRect.rounded(detail::WindowR, detail::WindowR, 0, 0).draw(theme.windowTitlebarCol);
					theme.font(window.m_title).drawAt(titleRect.center(), theme.windowTitlebarFontCol);
				}
				{
					Transformer2D transform(Mat3x2::Translate(window.m_rect.pos), true);
					//コントロール&グループ描画
					auto& controls = window.controls;
					if (controls.size())
					{
						Transformer2D ctrlTransform(Mat3x2::Translate(window.contentRect.pos), true);
						RectF clientRect(window.clientRect.pos - window.contentRect.pos, window.clientRect.size);
						detail::ScopedScissorRect scissorRect(clientRect);
						//グループ描画
						{
							for (auto& group : window.groups)
							{
								if (group.rect.intersects(clientRect))
								{
									if (group.frame)
									{
										auto rect = RoundRect(group.rect, 3);
										auto fontPos = group.rect.pos + Vec2(5, 0);
										if (group.label.length() > 0)
										{
											rect.y += theme.font.height() / 2;
											rect.h -= theme.font.height() / 2;
										}
										rect.drawFrame(1, theme.frameCol);
										theme.font(group.label).region(fontPos).draw(theme.windowBackCol);
										theme.font(group.label).draw(fontPos, theme.fontCol);
									}
									else
									{
										theme.font(group.label).draw(group.rect.pos, theme.fontCol);
									}
								}
							}
						}
						//コントロール描画
						{
							for (auto& control : controls)
							{
								if (control->m_rect.intersects(clientRect))
								{
									control->draw(*this, window);
								}
							}
						}
					}
					//スクロールバー描画
					{
						if (window.vScrollbar_show)
						{
							window.vScrollbar.draw(theme.scrollBarBackCol, theme.scrollBarGripCol);
						}
						if (window.hScrollbar_show)
						{
							window.hScrollbar.draw(theme.scrollBarBackCol, theme.scrollBarGripCol);
						}
					}
					if (debug_drawRect)
					{
						{
							Transformer2D transformDbg(Mat3x2::Translate(window.contentRect.pos));
							for (const auto& control : controls)
							{
								control->m_rect.drawFrame(1, Palette::Pink);
							}
							for (const auto& group : window.groups)
							{
								group.rect.drawFrame(1, Palette::Blue);
							}
						}
						{
							window.clientRect.drawFrame(1, Palette::Yellow);
							window.contentRect.drawFrame(1, Palette::Red);
						}
					}
				}
			}

			Optional<size_t> findWindowIdx(ID id)
			{
				for (size_t i = 1; i < windows.size(); i++)
				{
					auto& window = windows[i];
					if (window.id == id)
					{
						return i;
					}
				}
				return unspecified;
			}

			//ウィンドウ開始
			void windowBegin(size_t idx)
			{
				currentWindow << idx;
				auto& window = getCurrentWindow();
				//Print << U"windowBegin title=\"{}\""_fmt(window.m_title);
				window.m_used = true;
				if (!window.groups)
				{
					groupBegin();
				}
			}

			//座標の計算
			Vec2 calcPos(detail::Window& window, Optional<Vec2> pos, SizeF size)
			{
				Vec2 disppos;
				if (pos)
				{
					disppos = *pos;
					window.m_cursor = *pos;
				}
				else
				{
					disppos = window.m_cursor;
				}
				switch (window.layout)
				{
				case LayoutType::Vertical:
					window.m_cursor.y += size.y + window.ctrlMargin;
					break;
				case LayoutType::Horizontal:
					window.m_cursor.x += size.x + window.ctrlMargin;
					break;
				}
				Vec2 br = disppos + size + Vec2(window.ctrlMargin, window.ctrlMargin);
				window.contentRect.w = Max(window.contentRect.w, br.x);
				window.contentRect.h = Max(window.contentRect.h, br.y);
				if (window.groupStack)
				{
					auto& group = window.groups[window.groupStack[window.groupStack.size() - 1]];
					group.br.x = Max(group.br.x, br.x);
					group.br.y = Max(group.br.y, br.y);
				}
				return disppos;
			}

			struct ButtonStringCtrl : detail::IControl
			{
				detail::ButtonBase m_button;
				String text = U"";
				void update(GUIManager& mgr, detail::Window& wnd) override
				{
					m_button.update(mgr.windowItemHovered() && m_rect.mouseOver(), MouseL.down(), MouseL.pressed(), getEnabled(wnd));
				}
				void draw(GUIManager& mgr, detail::Window&) override
				{
					const auto& theme = mgr.getTheme();
					m_rect.rounded(4)
						.draw(m_button.getBackColor(theme))
						.drawFrame(0, 1, m_button.getFrameColor(theme));
					theme.font(text).draw(m_rect.pos + detail::TextButtonPadding, m_button.getFontColor(theme));
				}
			};

			struct ButtonTextureCtrl : detail::IControl
			{
				detail::ButtonBase m_button;
				ColorF m_color;
				Texture m_texture;
				ButtonTextureCtrl(const s3d::Texture& tex) :m_texture(tex) {}
				void update(GUIManager& mgr, detail::Window& wnd) override
				{
					m_button.update(mgr.windowItemHovered() && m_rect.mouseOver(), MouseL.down(), MouseL.pressed(), getEnabled(wnd));
				}
				void draw(GUIManager& mgr, detail::Window&) override
				{
					const auto& theme = mgr.getTheme();
					m_rect.rounded(4)
						.draw(m_button.getBackColor(theme))
						.drawFrame(0, 1, m_button.getFrameColor(theme));
					m_texture.draw(m_rect.pos + detail::ImageButtonPadding, m_color);
				}
			};

			struct LabelCtrl : detail::IControl
			{
				Optional<ColorF> m_color;
				String m_text;
				void update(GUIManager&, detail::Window&) override
				{

				}
				void draw(GUIManager& mgr, detail::Window& wnd) override
				{
					const auto& theme = mgr.getTheme();
					const auto enabled = getEnabled(wnd);
					ColorF col = m_color ? *m_color : (enabled ? theme.fontCol : theme.fontDisableCol);
					theme.font(m_text).draw(m_rect.pos, col);
				}
			};

			struct TextBoxCtrl : detail::IControl
			{
				detail::TextBoxBase textbox;
				String& m_text;
				String m_prevText;
				String m_hint;

				TextBoxCtrl(String& text, const Font& font, const int32& flags)
					:m_text(text)
					, textbox(font, flags, text)
				{

				}
				void update(GUIManager& mgr, detail::Window& wnd) override
				{
					const auto enabled = getEnabled(wnd);
					const auto hovered = mgr.windowItemHovered();
					if (m_text != m_prevText)
					{
						textbox.setText(m_text);
						m_prevText = m_text;
					}
					textbox.update(hovered, m_rect, enabled);
					if (textbox.textChanged())
					{
						m_text = textbox.getText();
						m_prevText = m_text;
					}
				}
				void draw(GUIManager& mgr, detail::Window& wnd) override
				{
					const auto& theme = mgr.getTheme();
					const auto enabled = getEnabled(wnd);
					m_rect.draw(enabled ? theme.textBoxCol : theme.textBoxDisableCol).drawFrame(0, 1, textbox.isActive() ? theme.textBoxActiveFrameCol : theme.textBoxFrameCol);
					textbox.draw(theme);
					if (!textbox.isActive() && !m_text)
					{
						theme.font(m_hint).draw(m_rect.pos, Palette::Gray);
					}
				}
			};

			struct ImageCtrl : detail::IControl
			{
				ColorF color;
				Texture texture;
				ImageCtrl(const Texture& tex) :texture(tex) {}
				void update(GUIManager&, detail::Window&) override
				{
				}
				void draw(GUIManager&, detail::Window&) override
				{
					texture.draw(m_rect.pos, color);
				}
			};

			struct CheckBoxCtrl : detail::IControl
			{
				bool m_hovered = false;
				bool m_clicked = false;
				bool& m_checked;
				String label;
				RectF checkRect;
				CheckBoxCtrl(bool& checked) :m_checked(checked) {}
				void drawCheck(Vec2 center, double size, ColorF col)
				{
					LineString lines(detail::CheckLine);
					for (auto& line : lines)
					{
						line = center + line * size;
					}
					lines.draw(LineStyle::NoCap, detail::CheckTickness * size, col);
				}
				void update(GUIManager& mgr, detail::Window& wnd) override
				{
					const auto enabled = getEnabled(wnd);
					m_hovered = enabled && mgr.windowItemHovered() && m_rect.mouseOver();
					m_clicked = m_hovered && MouseL.down();

					if (m_hovered)
					{
						Cursor::RequestStyle(CursorStyle::Hand);
					}
					if (m_clicked)
					{
						m_checked = !m_checked;
					}
				}
				void draw(GUIManager& mgr, detail::Window& wnd) override
				{
					const auto& theme = mgr.getTheme();
					const auto enabled = getEnabled(wnd);
					if (m_checked)
					{
						checkRect.draw(enabled ? theme.accentCol : theme.accentDisableCol);
						drawCheck(checkRect.center(), detail::CheckBoxRadioButtonSize / 2, enabled ? theme.accentFontCol : theme.accentDisableFontCol);
					}
					else
					{
						checkRect
							.draw(enabled ? (m_hovered ? theme.buttonHoverCol : theme.buttonBackCol) : theme.buttonDisableCol)
							.drawFrame(1, 0, theme.buttonFrameCol);
					}
					theme.font(label).draw(m_rect.pos + Vec2(detail::CheckBoxRadioButtonSize + 3, 0), enabled ? theme.fontCol : theme.fontDisableCol);
				}
			};

			struct RadioButtonCtrl : detail::IControl
			{
				bool m_hovered = false;
				bool m_clicked = false;
				bool m_checked = false;
				String label;
				Circle circle;
				Circle checkCircle;
				void update(GUIManager& mgr, detail::Window& wnd) override
				{
					const auto enabled = getEnabled(wnd);
					m_hovered = enabled && mgr.windowItemHovered() && m_rect.mouseOver();
					m_clicked = m_hovered && MouseL.down();

					if (m_hovered)
					{
						Cursor::RequestStyle(CursorStyle::Hand);
					}
				}
				void draw(GUIManager& mgr, detail::Window& wnd) override
				{
					const auto& theme = mgr.getTheme();
					const auto enabled = getEnabled(wnd);
					if (m_checked)
					{
						circle.draw(enabled ? theme.accentCol : theme.accentDisableCol);
						checkCircle.draw(enabled ? theme.accentFontCol : theme.accentDisableFontCol);
					}
					else
					{
						circle
							.draw(enabled ? (m_hovered ? theme.buttonHoverCol : theme.buttonBackCol) : theme.buttonDisableCol)
							.drawFrame(1, 0, theme.buttonFrameCol);
					}
					theme.font(label).draw(m_rect.pos + Vec2(detail::CheckBoxRadioButtonSize + 3, 0), enabled ? theme.fontCol : theme.fontDisableCol);
				}
			};

			struct CallbackCtrl : detail::IControl
			{
				std::function<void(RectF)> func;
				void update(GUIManager&, detail::Window&) override
				{

				}
				void draw(GUIManager&, detail::Window&) override
				{
					func(m_rect);
				}
			};

			struct TabCtrl : detail::IControl
			{
				Array<detail::ButtonBase> m_tabButtons;
				Array<RectF> m_tabRect;
				Array<String> names;
				size_t current;
				void update(GUIManager& mgr, detail::Window& wnd) override
				{
					const auto font = mgr.getTheme().font;
					const auto enabled = getEnabled(wnd);
					auto pos = m_rect.pos;
					for (size_t idx = 0; idx < names.size(); idx++)
					{
						auto tabRect = m_tabRect[idx].movedBy(m_rect.pos);
						auto& button = m_tabButtons[idx];
						button.update(mgr.windowItemHovered() && tabRect.mouseOver(), MouseL.down(), MouseL.pressed(), enabled);
						if (button.down())
						{
							current = idx;
						}
					}
				}
				void draw(GUIManager& mgr, detail::Window& wnd) override
				{
					const auto& theme = mgr.getTheme();
					const auto font = theme.font;
					const auto& group = wnd.groups[m_groupIdx];
					const auto enabled = getEnabled(wnd);
					for (size_t idx = 0; idx < names.size(); idx++)
					{
						auto tabRect = m_tabRect[idx].movedBy(m_rect.pos);
						auto& button = m_tabButtons[idx];
						tabRect.rounded(4, 4, 0, 0)
							.drawFrame(2, button.getFrameColor(theme))
							.draw(idx == current ? theme.windowBackCol : button.getBackColor(theme));
						font(names[idx]).draw(tabRect.pos + detail::TabItemPadding, idx == current ? (enabled ? theme.fontCol : theme.fontDisableCol) : button.getFontColor(theme));
					}
					auto currentTabRect = m_tabRect[current].movedBy(m_rect.pos);
					currentTabRect.bottom().movedBy(0, 0.5).draw(theme.windowBackCol);
					Line(m_rect.x - 1, m_rect.y + m_rect.h, currentTabRect.bl()).movedBy(0, 0.5).draw(theme.frameCol);
					Line(currentTabRect.br(), group.rect.x + group.rect.w - wnd.ctrlMargin, m_rect.y + m_rect.h)
						.movedBy(0, 0.5).draw(theme.frameCol);
					/*RectF(
						m_rect.br(),
						group.br.x - m_rect.x - m_rect.w - wnd.ctrlMargin,
						1
					).draw(theme.frameCol);*/
				}
			};

			struct MenuItemCtrl : detail::IControl
			{
				bool m_hovered = false;
				bool m_clicked = false;
				bool m_hasSubItem = false;
				bool m_showSubItem = false;
				String m_text;
				String m_subtext;
				Stopwatch m_hoverstw;
				void update(GUIManager& mgr, detail::Window& wnd) override
				{
					const auto& group = wnd.groups[m_groupIdx];
					const bool enabled = getEnabled(wnd);
					RectF backRect(m_rect.pos, group.br.x - m_rect.x - wnd.ctrlMargin, m_rect.h);
					bool prevHovered = m_hovered;
					m_hovered = enabled && mgr.windowItemHovered() && backRect.mouseOver();
					m_clicked = m_hovered && MouseL.down();
					if (m_hasSubItem)
					{
						if (m_hovered != prevHovered)
						{
							m_hoverstw.restart();
						}
						if (m_showSubItem)
						{
							if (!m_hovered && m_hoverstw.elapsed() > 1s)
							{
								m_showSubItem = false;
							}
						}
						else
						{
							if (m_hovered && (m_clicked || m_hoverstw.elapsed() > 1s))
							{
								m_showSubItem = true;
							}
						}
					}
				}
				void draw(GUIManager& mgr, detail::Window& wnd) override
				{
					const auto& theme = mgr.getTheme();
					const auto& font = theme.font;
					const auto& group = wnd.groups[m_groupIdx];
					const bool enabled = getEnabled(wnd);
					RectF backRect(m_rect.pos, group.br.x - m_rect.x - wnd.ctrlMargin, m_rect.h);
					ColorF fontCol;

					if (!enabled)
					{
						fontCol = theme.fontDisableCol;
					}
					else if (m_hovered || m_showSubItem)
					{
						backRect.draw(theme.menuItemCol);
						fontCol = theme.menuItemFontCol;
					}
					else
					{
						fontCol = theme.fontCol;
					}

					font(m_text).draw(m_rect.pos, fontCol);
					font(m_subtext).draw(Arg::topRight = backRect.tr() - Vec2(font.height() / 2, 0), Palette::Gray);
					if (m_hasSubItem)
					{
						detail::drawTriangle(backRect.rightCenter() - Vec2(font.height() / 4, 0), font.height() / 4, -30_deg, fontCol);
					}
				}
			};

			struct DropdownListCtrl : detail::IControl
			{
				detail::ButtonBase m_button;
				bool m_showItem;
				String m_valueText;

				void update(GUIManager& mgr, detail::Window& wnd) override
				{
					m_button.update(mgr.windowItemHovered() && m_rect.mouseOver(), MouseL.down(), MouseL.pressed(), getEnabled(wnd));
					if (m_button.click())
					{
						m_showItem = !m_showItem;
					}
				}
				void draw(GUIManager& mgr, detail::Window&) override
				{
					const auto& theme = mgr.getTheme();
					const auto& font = theme.font;

					m_rect.rounded(4)
						.draw(m_button.getBackColor(theme))
						.drawFrame(0, 1, m_button.getFrameColor(theme));
					font(m_valueText).draw(m_rect.pos + Vec2(7, 4), m_button.getFontColor(theme));

					detail::drawTriangle(m_rect.rightCenter() - Vec2(7 + font.height() / 2, 0), font.height() / 4, 60_deg, m_button.getFontColor(theme));
				}
			};

			struct ColorpickerCtrl : detail::IControl
			{
				RectF m_svRect;
				HSV& m_color;
				detail::ButtonBase m_svGrip;
				Circle m_svCircle;
				detail::ButtonBase m_hueGrip;
				Circle m_hueCircle;
				bool m_colorChanged = false;
				ColorpickerCtrl(HSV& color) :m_color(color) {}
				void drawLine(const Line& line, double tickness, const Array<ColorF>& colors)
				{
					Vec2 bg = line.begin, ed;
					for (size_t i = 1; i < colors.size(); i++)
					{
						ed = line.begin.lerp(line.end, (double)i / (colors.size() - 1));
						Line(bg, ed).draw(LineStyle::NoCap, tickness, colors[i - 1], colors[i]);
						bg = ed;
					}
				}
				void update(GUIManager& mgr, detail::Window& wnd) override
				{
					const auto hovered = mgr.windowItemHovered();
					const auto enabled = getEnabled(wnd);
					const auto prevCol = m_color;
					m_svRect.pos = m_rect.pos + Vec2(10, 10);
					const Line hueLine(m_svRect.tr() + Vec2(20, 10), m_svRect.br() + Vec2(20, -10));
					m_svCircle = Circle(m_svRect.pos + Vec2(m_color.s, 1 - m_color.v) * (m_svRect.size - Vec2(1, 1)), 10);
					m_hueCircle = Circle(hueLine.begin.lerp(hueLine.end, m_color.h / 360.0), 10);
					m_svGrip.update(hovered && m_svCircle.mouseOver(), MouseL.down(), MouseL.pressed(), enabled);
					m_hueGrip.update(hovered && m_hueCircle.mouseOver(), MouseL.down(), MouseL.pressed(), enabled);
					if (enabled)
					{
						if (m_svGrip.grab())
						{
							m_color.s = Clamp(m_color.s + detail::CursorDeltaF().x / m_svRect.w, 0.0, 1.0);
							m_color.v = Clamp(m_color.v - detail::CursorDeltaF().y / m_svRect.h, 0.0, 1.0);
						}
						else if (hovered && !m_svGrip.hover() && m_svRect.leftClicked())
						{
							m_color.s = Clamp((Cursor::PosF().x - m_svRect.x) / m_svRect.w, 0.0, 1.0);
							m_color.v = Clamp(1 - (Cursor::PosF().y - m_svRect.y) / m_svRect.h, 0.0, 1.0);
						}
						if (m_hueGrip.grab())
						{
							m_color.h = Clamp(m_color.h + detail::CursorDeltaF().y * 360.0 / hueLine.length(), 0.0, 360.0);
						}
						else if (hovered && !m_hueGrip.hover() && RectF(hueLine.begin - Vec2(10, 0), 20, hueLine.end.y + 1).leftClicked())
						{
							m_color.h = Clamp((Cursor::PosF().y - hueLine.begin.y) * 360.0 / (hueLine.end.y - hueLine.begin.y), 0.0, 360.0);
						}
					}
					m_colorChanged = m_color != prevCol;
				}
				void draw(GUIManager& mgr, detail::Window&) override
				{
					const auto& theme = mgr.getTheme();
					const Line hueLine(m_svRect.tr() + Vec2(20, 10), m_svRect.br() + Vec2(20, -10));

					m_svRect
						.draw(Arg::left = ColorF(1), Arg::right = HSV(m_color.h, 1, 1, 1))
						.draw(Arg::top = ColorF(0, 0), Arg::bottom = ColorF(0, 1))
						.drawFrame(0, 1, theme.buttonFrameCol);
					m_svCircle.draw(m_svGrip.getBackColor(theme)).drawFrame(0, 1, m_svGrip.getFrameColor(theme));
					Circle(m_svCircle.x, m_svCircle.y, m_svCircle.r - 4).draw(m_color);

					hueLine
						.draw(LineStyle::RoundCap, 22, theme.buttonFrameCol)
						.draw(LineStyle::RoundCap, 20, Palette::Red);
					drawLine(hueLine, 20, { HSV(0),HSV(60),HSV(120),HSV(180),HSV(240),HSV(300),HSV(360) });
					m_hueCircle.draw(m_hueGrip.getBackColor(theme)).drawFrame(0, 1, m_hueGrip.getFrameColor(theme));
					Circle(m_hueCircle.x, m_hueCircle.y, m_hueCircle.r - 4).draw(HSV(m_color.h));
				}
			};

			struct LinkCtrl : detail::IControl
			{
				String text;
				bool m_clicked;
				bool m_hovered;
				void update(GUIManager& mgr, detail::Window&) override
				{
					m_hovered = mgr.windowItemHovered() && m_rect.mouseOver();
					m_clicked = m_hovered && MouseL.down();

					if (m_hovered)
					{
						Cursor::RequestStyle(CursorStyle::Hand);
					}
				}
				void draw(GUIManager& mgr, detail::Window&) override
				{
					mgr.getTheme().font(text).draw(m_rect.pos, Palette::Blue);
					m_rect.bottom().draw(Palette::Blue);
				}
			};

			struct SliderCtrl : detail::IControl
			{
				detail::ButtonBase m_grip;
				RectF m_gripRect;
				Line m_line;
				double m_per;
				bool m_valueChanged = false;
				std::any m_prevVal;

				void update(GUIManager& mgr, detail::Window& wnd) override
				{
					const SizeF gripSize(10, m_rect.h);
					const double lineY = m_rect.y + (m_rect.h - 1) / 2;
					const double prevPer = m_per;
					const bool enabled = getEnabled(wnd);
					m_valueChanged = false;

					m_line = Line(m_rect.x + gripSize.x / 2, lineY, m_rect.x + m_rect.w - gripSize.x / 2, lineY);
					m_gripRect = RectF(Arg::center = m_line.begin.lerp(m_line.end, m_per), gripSize);
					m_grip.update(mgr.windowItemHovered() && m_gripRect.mouseOver(), MouseL.down(), MouseL.pressed(), enabled);

					if (m_grip.grab())
					{
						m_per += detail::CursorDeltaF().x / m_line.length();
					}
					else if (enabled && m_rect.leftClicked() && mgr.windowItemHovered())
					{
						m_per = (Cursor::PosF().x - m_line.begin.x) / (m_line.end.x - m_line.begin.x + 1);
					}
					if (m_per != prevPer)
					{
						m_valueChanged = true;
						m_per = Clamp(m_per, 0.0, 1.0);
					}
				}
				void draw(GUIManager& mgr, detail::Window& wnd) override
				{
					const auto& theme = mgr.getTheme();
					const auto enabled = getEnabled(wnd);
					m_line.draw(LineStyle::RoundCap, 6, Palette::Gray);
					m_gripRect.rounded(2).draw(m_grip.getBackColor(theme)).drawFrame(0, 1, m_grip.getFrameColor(theme));
				}
			};

			struct SpinBoxCtrl : detail::IControl
			{
				detail::TextBoxBase m_textBox;
				detail::ButtonBase m_up;
				detail::ButtonBase m_down;
				RectF m_textBoxRect;
				RectF m_upRect;
				RectF m_downRect;
				int m_mov = 0;
				bool m_valueChanged = false;
				std::any m_prevVal;

				SpinBoxCtrl(const String& text, const Font& font) : m_textBox(font, TextInputFlag::Number, text)
				{

				}
				void update(GUIManager& mgr, detail::Window& wnd) override
				{
					const bool hovered = mgr.windowItemHovered();
					const bool enabled = getEnabled(wnd);
					bool prevActive = m_textBox.isActive();

					const SizeF btnSize(20, (m_rect.h - 1) / 2);
					m_upRect = RectF(m_rect.x + m_rect.w - btnSize.x, m_rect.y, btnSize);
					m_downRect = RectF(m_rect.x + m_rect.w - btnSize.x, m_rect.y + btnSize.y + 1, btnSize);
					m_textBoxRect = m_rect;
					m_textBoxRect.w -= btnSize.x + 1;

					m_up.update(hovered && m_upRect.mouseOver(), detail::KeyRepeat(MouseL), MouseL.pressed(), enabled);
					m_down.update(hovered && m_downRect.mouseOver(), detail::KeyRepeat(MouseL), MouseL.pressed(), enabled);

					m_mov = (m_up.down() ? 1 : 0) + (m_down.down() ? -1 : 0);
					m_textBox.update(hovered, m_textBoxRect, enabled);
					if (m_textBox.isActive())
					{
						m_mov += (detail::KeyRepeat(KeyUp) ? 1 : 0) + (detail::KeyRepeat(KeyDown) ? -1 : 0);
					}
					m_valueChanged = prevActive && m_textBox.isActive() != prevActive;
				}
				void draw(GUIManager& mgr, detail::Window& wnd) override
				{
					const auto& theme = mgr.getTheme();
					const auto& font = theme.font;
					const bool enabled = getEnabled(wnd);
					m_rect.draw(enabled ? theme.textBoxCol : theme.textBoxDisableCol).drawFrame(0, 1, m_textBox.isActive() ? theme.textBoxActiveFrameCol : theme.textBoxFrameCol);

					m_upRect
						.draw(m_up.getBackColor(theme))
						.drawFrame(0, 1, m_up.getFrameColor(theme));

					detail::drawTriangle(m_upRect.center(), font.height() / 8, 0_deg, m_up.getFontColor(theme));

					m_downRect
						.draw(m_down.getBackColor(theme))
						.drawFrame(0, 1, m_down.getFrameColor(theme));

					detail::drawTriangle(m_downRect.center(), font.height() / 8, 60_deg, m_down.getFontColor(theme));

					m_rect.drawFrame(0, 1, m_textBox.isActive() ? theme.textBoxActiveFrameCol : theme.textBoxFrameCol);

					m_textBox.draw(theme);
				}
			};

			struct SplitCtrl : detail::IControl
			{
				void update(GUIManager&, detail::Window&) override
				{

				}
				void draw(GUIManager& mgr, detail::Window& wnd) override
				{
					const auto& group = wnd.groups[m_groupIdx];
					RectF(
						m_rect.pos,
						group.br.x - m_rect.x - wnd.ctrlMargin,
						1
					).draw(mgr.getTheme().frameCol);
				}
			};

			struct ProgressBarCtrl : detail::IControl
			{
				ColorF m_color;
				double m_per;
				void update(GUIManager&, detail::Window&) override
				{

				}
				void draw(GUIManager& mgr, detail::Window& wnd) override
				{
					const double barWidth = m_rect.w * m_per;
					const auto& theme = mgr.getTheme();
					const auto enabled = getEnabled(wnd);
					m_rect.draw(theme.progressBarCol).drawFrame(0, 1, theme.frameCol);
					RectF(m_rect.pos, barWidth, m_rect.h).draw(enabled ? m_color : theme.accentDisableCol);
				}
			};

			struct TreeNodeCtrl : detail::IControl
			{
				String m_text;
				bool m_expand;
				bool m_hovered;
				bool m_clicked;
				void update(GUIManager& mgr, detail::Window&) override
				{
					m_hovered = mgr.windowItemHovered() && m_rect.mouseOver();
					m_clicked = m_hovered && MouseL.down();
					if (m_clicked)
					{
						m_expand = !m_expand;
					}
					if (m_hovered)
					{
						Cursor::RequestStyle(CursorStyle::Hand);
					}
				}
				void draw(GUIManager& mgr, detail::Window& wnd) override
				{
					const auto& theme = mgr.getTheme();
					const auto enabled = getEnabled(wnd);
					const auto fontCol = enabled ? theme.fontCol : theme.buttonDisableFontCol;
					detail::drawTriangle(m_rect.leftCenter() + Vec2(detail::TreeNodePadding.x + theme.font.height() / 2, 0), theme.font.height() / 4, m_expand ? 0_deg : 60_deg, fontCol);
					theme.font(m_text).draw(m_rect.pos + detail::TreeNodePadding + Vec2(theme.font.height(), 0), fontCol);
				}
			};
		public:
			GUIManager()
				:GUIManager(GUITheme::Light())
			{

			}

			GUIManager(GUITheme theme)
			{
				auto window = detail::Window{ .id = getID(U"DefaultWindow"),.m_used = true,.m_title = U"DefaultWindow", .m_rect = Scene::Rect(), .flags = defaultWindowFlags ,.ctrlMargin = detail::CtrlMargin };
				addWindow(window, detail::Background);
				setTheme(theme);
				frameBegin();
			}

			GUITheme& getTheme()
			{
				return m_theme;
			}

			void setTheme(GUITheme theme)
			{
				m_theme = theme;
			}

			/// <summary>
			/// フレーム開始処理
			/// </summary>
			void frameBegin()
			{
				detail::ResetCursor();
				windows[DefaultWindow].m_rect = Scene::Rect();

				hoveringWindow = unspecified;
				//unsigned型の除算ができないのでint64で対応
				for (int64 type = 2; type >= 0; type--)
				{
					auto& order = windowOrder[type];
					for (int64 idx = order.size() - 1; idx >= 0; idx--)
					{
						currentWindow = { order[idx] };
						auto& window = getCurrentWindow();
						if (!(window.flags & WindowFlag::Hide) && !(window.flags & WindowFlag::NoFocus))
						{
							RectF windowRect = window.m_rect;
							if (!((window.flags & WindowFlag::NoResize) || (window.flags & WindowFlag::AutoResize)))
							{
								windowRect.pos -= Vec2(detail::WindowResizeGripSize, detail::WindowResizeGripSize);
								windowRect.size += Vec2(detail::WindowResizeGripSize * 2, detail::WindowResizeGripSize * 2);
							}
							if (!(window.flags & WindowFlag::NoFocus) && (windowRect.intersects(Cursor::PosF()) || window.resizeSide))
							{
								hoveringWindow = getCurrentWindowIdx();
								if (MouseL.down() || MouseR.down())
								{
									windowSetFocus();
								}
								goto _break;
							}
						}
					}
				}
			_break:

				for (size_t idx = 0; idx < windows.size(); idx++)
				{
					currentWindow = { idx };
					updateWindow(getCurrentWindow());
				}

				currentWindow = {};
				windowBegin(DefaultWindow);
			}

			/// <summary>
			/// フレーム終了処理
			/// </summary>
			void frameEnd(const bool draw = true)
			{
				while (currentWindow.size() > 1)
				{
					windowEnd();
				}
				//ウィンドウの削除
				for (size_t idx = 0; idx < windows.size();)
				{
					if (!windows[idx].m_used)
					{
						delWindow(idx);
						continue;
					}
					idx++;
				}
				//ウィンドウのフレーム終了処理
				for (size_t idx = 0; idx < windows.size(); idx++)
				{
					auto& window = windows[idx];
					currentWindow = { idx };
					window.controls.remove_if([&](std::shared_ptr<detail::IControl> ctrl) {return !ctrl->m_used; });
					while (window.groupStack.size())
					{
						groupEnd();
					}
					window.contentRect.w += detail::WindowPadding - window.ctrlMargin;
					window.contentRect.h += detail::WindowPadding - window.ctrlMargin;
				}
				//ウィンドウの描画
				if (draw)
				{
					RasterizerState rasterizerState = Graphics2D::GetRasterizerState();
					rasterizerState.scissorEnable = true;
					ScopedRenderStates2D renderState(rasterizerState);
					Graphics2D::SetScissorRect(Scene::Rect());
					for (int type = 0; type < 3; type++)
					{
						for (const auto order : windowOrder[type])
						{
							auto& window = windows[order];
							if (!(window.flags & WindowFlag::Hide))
							{
								currentWindow = { order };
								drawWindow(window);
							}
						}
					}
					detail::DrawCursor();
				}
			}

			void groupBegin(const String& label = U"", const bool& frame = false, const bool& enabled = true)
			{
				auto& window = getCurrentWindow();
				auto& theme = getTheme();
				//Print << U"{}groupBegin label=\"{}\""_fmt(String(window.groupStack.size() + 1, U'　'), label);
				window.groups.push_back(detail::Group{ RectF(window.m_cursor,SizeF()), label, frame ,enabled });
				auto idx = window.groups.size() - 1;
				auto& group = window.groups[idx];

				if (window.groupStack)
				{
					group.parentIdx = window.groupStack[window.groupStack.size() - 1];
				}
				window.groupStack.push_back(idx);

				//カーソル移動
				window.m_cursor = group.rect.pos + Vec2(window.ctrlMargin, window.ctrlMargin);
				if (label.length() > 0)
				{
					window.m_cursor.y += theme.font.height();
				}
				group.cursorBg = window.m_cursor;
				group.br = window.m_cursor;
			}

			void groupEnd()
			{
				auto& window = getCurrentWindow();
				if (!window.groupStack)
				{
					return;
				}
				auto& group = window.groups[window.groupStack[window.groupStack.size() - 1]];
				//Print << U"{}groupEnd"_fmt(String(window.groupStack.size(), U'　'));
				group.rect.size = group.br - group.rect.pos;
				window.m_cursor = group.rect.pos;
				window.groupStack.pop_back();
				calcPos(window, unspecified, group.rect.size);
			}

			/// <summary>
			/// 新しい行に移動
			/// </summary>
			void newLine()
			{
				auto& window = getCurrentWindow();
				auto group = window.groups[window.groupStack[window.groupStack.size() - 1]];

				switch (window.layout)
				{
				case LayoutType::Vertical:
					window.m_cursor.x = group.br.x;
					window.m_cursor.y = group.cursorBg.y;
					break;
				case LayoutType::Horizontal:
					window.m_cursor.x = group.cursorBg.x;
					window.m_cursor.y = group.br.y;
					break;
				}
			}

			void toolTipBegin()
			{
				Vec2 pos = Cursor::Pos();
				windowBegin(U"ToolTip", tooltipFlags, Size(300, 300), pos);
				auto& window = getCurrentWindow();
				window.type = detail::WindowType::ToolTip;
				pos.x = Clamp(pos.x, 0.0, Scene::Width() - window.m_rect.w);
				pos.y = Clamp(pos.y, 0.0, Scene::Height() - window.m_rect.h);
				window.m_rect.pos = pos;
			}

			void toolTipEnd()
			{
				windowEnd();
			}

			/// <summary>
			/// ウィンドウ開始
			/// </summary>
			/// <param name="title">タイトル</param>
			/// <param name="pos">初期位置</param>
			/// <param name="size">初期サイズ</param>
			/// <param name="flags">ウィンドウ設定</param>
			void windowBegin(const String& title, WindowFlags flags = WindowFlag::None, SizeF size = SizeF(300, 300), Vec2 pos = Vec2(0, 0))
			{
				ID id = getID(title);
				Optional<size_t> idx = findWindowIdx(id);
				if (!idx)
				{
					idx = addWindow(detail::Window{
						.id = id,
						.m_title = title,
						.m_rect = RectF(pos,size),
						.flags = flags ,
						.ctrlMargin = (flags & WindowFlag::NoMargin) ? 0 : detail::CtrlMargin
						}, (flags & WindowFlag::AlwaysForeground) ? detail::Foreground : detail::Nomal);
				}
				windowBegin(*idx);
			}

			/// <summary>
			/// ウィンドウ開始
			/// </summary>
			/// <param name="id">ウィンドウID</param>
			/// <param name="title">タイトル</param>
			/// <param name="pos">初期位置</param>
			/// <param name="size">初期サイズ</param>
			/// <param name="flags">ウィンドウ設定</param>
			void windowBegin(ID id, const String& title, WindowFlags flags = WindowFlag::None, SizeF size = SizeF(300, 300), Vec2 pos = Vec2(0, 0))
			{
				Optional<size_t> idx = findWindowIdx(id);
				if (!idx)
				{
					idx = addWindow(detail::Window{
							.id = id,
							.m_title = title,
							.m_rect = RectF(pos,size),
							.flags = flags ,
							.ctrlMargin = (flags & WindowFlag::NoMargin) ? 0 : detail::CtrlMargin
						}, (flags & WindowFlag::AlwaysForeground) ? detail::Foreground : detail::Nomal);
				}
				windowBegin(*idx);
			}

			/// <summary>
			/// ウィンドウ終了
			/// </summary>
			void windowEnd()
			{
				if (currentWindow.size() <= 1)
				{
					return;
				}
				//Print << U"windowEnd";
				currentWindow.pop_back();
			}

			SizeF windowGetSize()
			{
				return getCurrentWindow().m_rect.size;
			}

			/// <summary>
			/// ウィンドウのサイズを変更
			/// </summary>
			void windowSetSize(SizeF size)
			{
				getCurrentWindow().m_rect.size = size;
			}

			Vec2 windowGetPos()
			{
				return getCurrentWindow().m_rect.pos;
			}

			/// <summary>
			/// ウィンドウの位置を変更
			/// </summary>
			void windowSetPos(Vec2 pos)
			{
				getCurrentWindow().m_rect.pos = pos;
			}

			/// <summary>
			/// 現在のウィンドウ上にカーソルがあるか
			/// </summary>
			bool windowHovered()
			{
				return getCurrentWindowIdx() == hoveringWindow;
			}

			/// <summary>
			/// 現在のウィンドウのクライアント領域にカーソルがあるか
			/// </summary>
			bool windowItemHovered()
			{
				return getCurrentWindow().itemHovered && windowHovered();
			}

			/// <summary>
			/// 現在のウィンドウを最前面に移動する
			/// </summary>
			void windowSetFocus()
			{
				size_t current = getCurrentWindowIdx();
				auto& window = getCurrentWindow();
				windowOrder[window.layer].remove_if([&](size_t idx) {return idx == current; });
				windowOrder[window.layer].push_back(current);
			}

			/// <summary>
			/// ウィンドウのカーソルを取得
			/// </summary>
			Vec2& windowGetNextItemPos()
			{
				return getCurrentWindow().m_cursor;
			}

			/// <summary>
			/// ウィンドウのカーソルを設定
			/// </summary>
			void windowSetNextItemPos(const Vec2& cursor)
			{
				getCurrentWindow().m_cursor = cursor;
			}

			LayoutType windowGetLayoutType()
			{
				return getCurrentWindow().layout;
			}

			void windowSetLayoutType(const LayoutType& type)
			{
				getCurrentWindow().layout = type;
			}

			void windowSetEnabled(bool enabled)
			{
				detail::Window& window = getCurrentWindow();
				window.flags &= ~WindowFlag::Disable;
				if (!enabled)
				{
					window.flags |= WindowFlag::Disable;
				}
			}

			void windowSetEnabled(String title, bool enabled)
			{
				auto idx = findWindowIdx(getID(title));
				if (!idx)
				{
					return;
				}
				detail::Window& window = windows[*idx];
				window.flags &= ~WindowFlag::Disable;
				if (!enabled)
				{
					window.flags |= WindowFlag::Disable;
				}
			}

			void windowSetEnabled(ID id, bool enabled)
			{
				auto idx = findWindowIdx(id);
				if (!idx)
				{
					return;
				}
				detail::Window& window = windows[*idx];
				window.flags &= ~WindowFlag::Disable;
				if (!enabled)
				{
					window.flags |= WindowFlag::Disable;
				}
			}

			bool windowGetEnabled()
			{
				return !(getCurrentWindow().flags & WindowFlag::Disable);
			}

			bool windowGetEnabled(String title)
			{
				auto idx = findWindowIdx(getID(title));
				if (!idx)
				{
					return false;
				}
				return !(windows[*idx].flags & WindowFlag::Disable);
			}

			bool windowGetEnabled(ID id)
			{
				auto idx = findWindowIdx(id);
				if (!idx)
				{
					return false;
				}
				return !(windows[*idx].flags & WindowFlag::Disable);
			}

			void windowSetVisible(bool visible)
			{
				detail::Window& window = getCurrentWindow();
				window.flags &= ~WindowFlag::Hide;
				if (!visible)
				{
					window.flags |= WindowFlag::Hide;
				}
			}

			void windowSetVisible(String title, bool visible)
			{
				auto idx = findWindowIdx(getID(title));
				if (!idx)
				{
					return;
				}
				detail::Window& window = windows[*idx];
				window.flags &= ~WindowFlag::Hide;
				if (!visible)
				{
					window.flags |= WindowFlag::Hide;
				}
			}

			void windowSetVisible(ID id, bool visible)
			{
				auto idx = findWindowIdx(id);
				if (!idx)
				{
					return;
				}
				detail::Window& window = windows[*idx];
				window.flags &= ~WindowFlag::Hide;
				if (!visible)
				{
					window.flags |= WindowFlag::Hide;
				}
			}

			bool windowGetVisible()
			{
				return !(getCurrentWindow().flags & WindowFlag::Hide);
			}

			bool windowGetVisible(String title)
			{
				auto idx = findWindowIdx(getID(title));
				if (!idx)
				{
					return false;
				}
				return !(windows[*idx].flags & WindowFlag::Hide);
			}

			bool windowGetVisible(ID id)
			{
				auto idx = findWindowIdx(id);
				if (!idx)
				{
					return false;
				}
				return !(windows[*idx].flags & WindowFlag::Hide);
			}

			bool itemHovered()
			{
				auto& window = getCurrentWindow();
				if (window.lastUsedCtrl)
				{
					return windowItemHovered() &&
						window.lastUsedCtrl->m_rect.movedBy(window.m_rect.pos + window.contentRect.pos).mouseOver();
				}
				return false;
			}

			/// <summary>
			/// ボタン
			/// </summary>
			/// <param name="text">ボタン内に表示するテキスト</param>
			/// <param name="enabled">アクティブ状態</param>
			/// <param name="pos">ウィンドウ内の座標,unspecifiedで自動</param>
			/// <returns>ボタンが押されたらtrue,それ以外はfalse</returns>
			bool button(const String& text, const bool enabled = true, Optional<Vec2> pos = unspecified)
			{
				detail::Window& window = getCurrentWindow();
				ID id = getID(text);
				std::shared_ptr<ButtonStringCtrl> ctrl = getControl<ButtonStringCtrl>(window, id);
				if (!ctrl)
				{
					ctrl = std::make_shared<ButtonStringCtrl>();
					ctrl->m_rect.size = getTheme().font(text).region().size + detail::TextButtonPadding * 2;
					ctrl->text = text;
					addControl(window, ctrl, id);
				}
				ctrl->m_rect.pos = calcPos(window, pos, ctrl->m_rect.size);
				ctrl->m_enabled = enabled;
				return ctrl->m_button.click();
			}
			/// <summary>
			/// ボタン
			/// </summary>
			/// <param name="texture">ボタン内に表示するテクスチャ</param>
			/// <param name="color">テクスチャに乗算する色</param>
			/// <param name="enabled">アクティブ状態</param>
			/// <param name="pos">ウィンドウ内の座標,unspecifiedで自動</param>
			/// <returns>ボタンが押されたらtrue,それ以外はfalse</returns>
			bool button(const Texture& texture, const ColorF& color = Palette::White, const bool enabled = true, Optional<Vec2> pos = unspecified)
			{
				detail::Window& window = getCurrentWindow();
				ID id = getID(texture);
				std::shared_ptr<ButtonTextureCtrl> ctrl = getControl<ButtonTextureCtrl>(window, id);
				if (!ctrl)
				{
					ctrl = std::make_shared<ButtonTextureCtrl>(texture);
					ctrl->m_rect.size = texture.size() + detail::ImageButtonPadding * 2;
					addControl(window, ctrl, id);
				}
				ctrl->m_rect.pos = calcPos(window, pos, ctrl->m_rect.size);
				ctrl->m_enabled = enabled;
				ctrl->m_color = color;
				return ctrl->m_button.click();
			}
			/// <summary>
			/// ラベル
			/// </summary>
			/// <param name="text">表示するテキスト</param>
			/// <param name="color">テキストの色,unspecifiedでテーマに従う</param>
			/// <param name="enabled">アクティブ状態</param>
			/// <param name="pos">ウィンドウ内の座標,unspecifiedで自動</param>
			void label(const String& text, Optional<ColorF> color = unspecified, const bool enabled = true, Optional<Vec2> pos = unspecified)
			{
				detail::Window& window = getCurrentWindow();
				ID id = getID(text);
				std::shared_ptr<LabelCtrl> ctrl = getControl<LabelCtrl>(window, id);
				if (!ctrl)
				{
					ctrl = std::make_shared<LabelCtrl>();
					ctrl->m_rect.size = getTheme().font(text).region().size;
					addControl(window, ctrl, id);
				}
				ctrl->m_rect.pos = calcPos(window, pos, ctrl->m_rect.size);
				ctrl->m_enabled = enabled;
				ctrl->m_text = text;
				ctrl->m_color = color;
			}
			/// <summary>
			/// テキストボックス
			/// </summary>
			/// <param name="text">値を編集する変数</param>
			/// <param name="hint">文字列が空のときに表示するテキスト</param>
			/// <param name="flags">テキストボックスのフラグ(TextInputFlag)</param>
			/// <param name="enabled">アクティブ状態</param>
			/// <param name="pos">ウィンドウ内の座標,unspecifiedで自動</param>
			/// <returns>テキストが変更されたらtrue,それ以外はfalse</returns>
			bool textBox(String& text, const String& hint = U"", const int32 flags = TextInputFlag::All, const double width = 200, const int32 lineCnt = 3, const bool enabled = true, Optional<Vec2> pos = unspecified)
			{
				detail::Window& window = getCurrentWindow();
				const auto& theme = getTheme();
				ID id = getID(&text);
				std::shared_ptr<TextBoxCtrl> ctrl = getControl<TextBoxCtrl>(window, id);
				if (!ctrl)
				{
					ctrl = std::make_shared<TextBoxCtrl>(text, theme.font, flags);
					addControl(window, ctrl, id);
				}
				ctrl->m_rect.w = width;
				ctrl->m_rect.h = theme.font.height() * ((flags & TextInputFlag::MultiLine) ? lineCnt : 1);
				ctrl->m_rect.h += detail::TextBoxPadding.y * 2;
				ctrl->m_rect.pos = calcPos(window, pos, ctrl->m_rect.size);
				ctrl->m_enabled = enabled;
				ctrl->m_hint = hint;
				return ctrl->textbox.textChanged();
			}
			/// <summary>
			/// 画像
			/// </summary>
			/// <param name="texture">表示するテクスチャ</param>
			/// <param name="enabled">アクティブ状態</param>
			/// <param name="pos">ウィンドウ内の座標,unspecifiedで自動</param>
			void image(const Texture& texture, const ColorF& color = Palette::White, const bool enabled = true, Optional<Vec2> pos = unspecified)
			{
				if (texture.isEmpty())
				{
					return;
				}
				detail::Window& window = getCurrentWindow();
				ID id = getID(texture);
				std::shared_ptr<ImageCtrl> ctrl = getControl<ImageCtrl>(window, id);
				if (!ctrl)
				{
					ctrl = std::make_shared<ImageCtrl>(texture);
					ctrl->m_rect.size = texture.size();
					addControl(window, ctrl, id);
				}
				ctrl->m_rect.pos = calcPos(window, pos, ctrl->m_rect.size);
				ctrl->m_enabled = enabled;
				ctrl->color = color;
			}
			/// <summary>
			/// チェックボックス
			/// </summary>
			/// <param name="checked">値を変更する変数</param>
			/// <param name="enabled">アクティブ状態</param>
			/// <param name="pos">ウィンドウ内の座標,unspecifiedで自動</param>
			/// <returns>値が変わったらtrue,それ以外はfalse</returns>
			bool checkBox(bool& checked, const String& label, const bool enabled = true, Optional<Vec2> pos = unspecified)
			{
				detail::Window& window = getCurrentWindow();
				ID id = getID(&checked);
				std::shared_ptr<CheckBoxCtrl> ctrl = getControl<CheckBoxCtrl>(window, id);
				if (!ctrl)
				{
					ctrl = std::make_shared<CheckBoxCtrl>(checked);
					ctrl->m_rect.size = getTheme().font(label).region().size;
					ctrl->m_rect.w += detail::CheckBoxRadioButtonSize + 3;
					ctrl->m_rect.h = Max(ctrl->m_rect.h, detail::CheckBoxRadioButtonSize);
					ctrl->label = label;
					addControl(window, ctrl, id);
				}
				ctrl->m_rect.pos = calcPos(window, pos, ctrl->m_rect.size);
				ctrl->m_enabled = enabled;
				ctrl->checkRect = RectF(Arg::leftCenter = ctrl->m_rect.leftCenter(), detail::CheckBoxRadioButtonSize);
				return ctrl->m_clicked;
			}
			/// <summary>
			/// ラジオボタン
			/// </summary>
			/// <param name="variable">値を変更する変数</param>
			/// <param name="value">選択状態にする条件(variable==valueのとき選択状態になります)</param>
			/// <param name="label">表示するテキスト</param>
			/// <param name="enabled">アクティブ状態</param>
			/// <param name="pos">ウィンドウ内の座標,unspecifiedで自動</param>
			/// <returns>選択状態のときtrue,それ以外のときfalse</returns>
			template<class value_type>
			bool radioButton(value_type& variable, const value_type& value, const String& label, const bool enabled = true, Optional<Vec2> pos = unspecified)
			{
				detail::Window& window = getCurrentWindow();
				ID id = getID(&variable) + getID(label);
				std::shared_ptr<RadioButtonCtrl> ctrl = getControl<RadioButtonCtrl>(window, id);
				if (!ctrl)
				{
					ctrl = std::make_shared<RadioButtonCtrl>();
					ctrl->m_rect.size = getTheme().font(label).region().size;
					ctrl->m_rect.w += detail::CheckBoxRadioButtonSize + 3;
					ctrl->m_rect.h = Max(ctrl->m_rect.h, detail::CheckBoxRadioButtonSize);
					ctrl->label = label;
					addControl(window, ctrl, id);
				}
				ctrl->m_rect.pos = calcPos(window, pos, ctrl->m_rect.size);
				ctrl->m_enabled = enabled;
				ctrl->circle = Circle(Arg::leftCenter = ctrl->m_rect.leftCenter(), detail::CheckBoxRadioButtonSize / 2);
				ctrl->checkCircle = Circle(ctrl->circle.center, ctrl->circle.r - 4);
				if (ctrl->m_clicked)
				{
					variable = value;
				}
				ctrl->m_checked = variable == value;
				return ctrl->m_checked;
			}
			/// <summary>
			/// コールバック関数
			/// </summary>
			/// <param name="func">描画時に呼び出すコールバック関数(RectF:描画する領域)</param>
			/// <param name="size">描画する大きさ</param>
			/// <param name="enabled">アクティブ状態</param>
			/// <param name="pos">ウィンドウ内の座標,unspecifiedで自動</param>
			void callback(const std::function<void(RectF)>& func, SizeF size, const bool enabled = true, Optional<Vec2> pos = unspecified)
			{
				detail::Window& window = getCurrentWindow();
				auto ctrl = std::make_shared<CallbackCtrl>();
				addControl(window, ctrl, 0);
				ctrl->m_rect.size = size;
				ctrl->m_rect.pos = calcPos(window, pos, ctrl->m_rect.size);
				ctrl->m_enabled = enabled;
				ctrl->func = func;
			}
			/// <summary>
			/// タブ
			/// </summary>
			/// <param name="names">各タブの名前を格納した配列</param>
			/// <param name="enabled">アクティブ状態</param>
			/// <param name="pos">ウィンドウ内の座標,unspecifiedで自動</param>
			/// <returns>選択されているタブのインデックス番号</returns>
			size_t tab(const Array<String>& names, const bool enabled = true, Optional<Vec2> pos = unspecified)
			{
				if (names.size() == 0)
				{
					return 0;
				}
				detail::Window& window = getCurrentWindow();
				ID id = getID(names);
				std::shared_ptr<TabCtrl> ctrl = getControl<TabCtrl>(window, id);
				const auto theme = getTheme();
				if (!ctrl)
				{
					ctrl = std::make_shared<TabCtrl>();
					ctrl->names = names;
					ctrl->m_tabButtons = Array<detail::ButtonBase>(names.size());
					ctrl->current = 0;
					ctrl->m_rect.w = 0;
					ctrl->m_rect.h = theme.font.height() + detail::TabItemPadding.y * 2;
					Vec2 rectPos(0, 0);
					for (const auto& name : names)
					{
						RectF rect = RectF(rectPos, theme.font(name).region().w, theme.font.height());
						rect.size += detail::TabItemPadding * 2;
						ctrl->m_tabRect << rect;
						rectPos.x += rect.w + 1;
						ctrl->m_rect.w += rect.w + 1;
					}
					addControl(window, ctrl, id);
				}
				ctrl->m_rect.pos = calcPos(window, pos, ctrl->m_rect.size);
				ctrl->m_enabled = enabled;
				return ctrl->current;
			}
			/// <summary>
			/// メニュー要素
			/// </summary>
			/// <param name="text">表示するテキスト</param>
			/// <param name="subtext">追加情報を表示するテキスト</param>
			/// <param name="subitem">子のメニューを持っているか</param>
			/// <param name="enabled">アクティブ状態</param>
			/// <param name="pos">ウィンドウ内の座標,unspecifiedで自動</param>
			/// <returns>subitemがfalseのとき,押されたらtrue、subitemがtrueのとき,子メニューが表示されるときtrue</returns>
			bool menuItem(const String& text, const String& subtext = U"", bool subitem = false, const bool enabled = true, Optional<Vec2> pos = unspecified)
			{
				detail::Window& window = getCurrentWindow();
				ID id = getID(text);
				std::shared_ptr<MenuItemCtrl> ctrl = getControl<MenuItemCtrl>(window, id);
				if (!ctrl)
				{
					const auto& theme = getTheme();
					ctrl = std::make_shared<MenuItemCtrl>();
					ctrl->m_rect.w =
						theme.font(text).region().size.x +
						theme.font(subtext).region().size.x +
						theme.font.height() / 2;
					ctrl->m_rect.h = theme.font.height();
					ctrl->m_hasSubItem = subitem;
					ctrl->m_text = text;
					ctrl->m_subtext = subtext;
					addControl(window, ctrl, id);
				}
				ctrl->m_rect.pos = calcPos(window, pos, ctrl->m_rect.size);
				ctrl->m_enabled = enabled;
				if (ctrl->m_hasSubItem)
				{
					return ctrl->m_showSubItem;
				}
				else
				{
					if (window.type == detail::WindowType::Menu)
					{
						windowSetVisible(false);
					}
					return ctrl->m_clicked;
				}
			}
			/// <summary>
			/// ドロップダウンリスト
			/// </summary>
			/// <param name="value">値を変更する変数</param>
			/// <param name="values">値の候補を格納した配列</param>
			/// <param name="enabled">アクティブ状態</param>
			/// <param name="pos">ウィンドウ内の座標,unspecifiedで自動</param>
			/// <returns>値が変更されたときtrue,それ以外のときfalse</returns>
			template<class T>
			bool dropdownList(T& value, const Array<T>& values, const bool enabled = true, Optional<Vec2> pos = unspecified)
			{
				const auto& font = getTheme().font;
				detail::Window& window = getCurrentWindow();
				ID id = getID(values);
				std::shared_ptr<DropdownListCtrl> ctrl = getControl<DropdownListCtrl>(window, id);
				if (!ctrl)
				{
					ctrl = std::make_shared<DropdownListCtrl>();
					ctrl->m_rect.size.x = font(value).region().w;
					ctrl->m_rect.size.y = font.height();
					for (const auto& val : values)
					{
						ctrl->m_rect.size.x = Max<double>(ctrl->m_rect.size.x, font(val).region().w);
					}
					ctrl->m_rect.size += SizeF(14 + font.height(), 8);
					ctrl->m_showItem = false;
					addControl(window, ctrl, id);
				}
				ctrl->m_rect.pos = calcPos(window, pos, ctrl->m_rect.size);
				ctrl->m_enabled = enabled;
				ctrl->m_valueText = Format(value);

				bool valueChanged = false;
				if (ctrl->m_showItem)
				{
					windowBegin(id, U"", dropdownlistFlags);
					windowSetPos(window.m_rect.pos + window.contentRect.pos + ctrl->m_rect.bl() + Vec2(0, 2));
					for (size_t idx = 0; idx < values.size(); idx++)
					{
						const String text = Format(values[idx]);
						if (menuItem(text))
						{
							ctrl->m_valueText = text;
							ctrl->m_showItem = false;
							value = values[idx];
							valueChanged = true;
							break;
						}
						newLine();
					}
					windowEnd();
				}
				return valueChanged;
			}
			/// <summary>
			/// カラーピッカー
			/// </summary>
			/// <param name="hsv">色を変更する変数</param>
			/// <param name="enabled">アクティブ状態</param>
			/// <param name="pos">ウィンドウ内の座標,unspecifiedで自動</param>
			/// <returns>値が変更されたときtrue,それ以外のときfalse</returns>
			bool colorPicker(HSV& hsv, const bool enabled = true, Optional<Vec2> pos = unspecified)
			{
				detail::Window& window = getCurrentWindow();
				ID id = getID(&hsv);
				std::shared_ptr<ColorpickerCtrl> ctrl = getControl<ColorpickerCtrl>(window, id);
				if (!ctrl)
				{
					ctrl = std::make_shared<ColorpickerCtrl>(hsv);
					ctrl->m_rect.size = SizeF(240, 220);
					ctrl->m_svRect.size = SizeF(200, 200);
					addControl(window, ctrl, id);
				}
				ctrl->m_rect.pos = calcPos(window, pos, ctrl->m_rect.size);
				ctrl->m_enabled = enabled;
				return ctrl->m_colorChanged;
			}
			/// <summary>
			/// スペース
			/// </summary>
			/// <param name="size">大きさ</param>
			/// <param name="pos">ウィンドウ内の座標,unspecifiedで自動</param>
			void space(const SizeF& size, Optional<Vec2> pos = unspecified)
			{
				calcPos(getCurrentWindow(), pos, size);
			}
			/// <summary>
			/// リンク
			/// </summary>
			/// <param name="text">表示するテキスト</param>
			/// <param name="enabled">アクティブ状態</param>
			/// <param name="pos">ウィンドウ内の座標,unspecifiedで自動</param>
			/// <returns>リンクが押されたときtrue,それ以外のときfalse</returns>
			bool link(const String& text, const bool enabled = true, Optional<Vec2> pos = unspecified)
			{
				detail::Window& window = getCurrentWindow();
				ID id = getID(text);
				std::shared_ptr<LinkCtrl> ctrl = getControl<LinkCtrl>(window, id);
				if (!ctrl)
				{
					ctrl = std::make_shared<LinkCtrl>();
					ctrl->m_rect.size = getTheme().font(text).region().size;
					addControl(window, ctrl, id);
				}
				ctrl->m_rect.pos = calcPos(window, pos, ctrl->m_rect.size);
				ctrl->m_enabled = enabled;
				ctrl->text = text;
				return ctrl->m_clicked;
			}
			/// <summary>
			/// スライダー
			/// </summary>
			/// <param name="value"></param>
			/// <param name="enabled">アクティブ状態</param>
			/// <param name="pos">ウィンドウ内の座標,unspecifiedで自動</param>
			/// <returns>値が変更されたときtrue,それ以外のときfalse</returns>
			template<class T>
			bool slider(T& value, const T min, const T max, const double width = 200, const bool enabled = true, Optional<Vec2> pos = unspecified)
			{
				detail::Window& window = getCurrentWindow();
				ID id = getID(&value);
				std::shared_ptr<SliderCtrl> ctrl = getControl<SliderCtrl>(window, id);
				if (!ctrl)
				{
					ctrl = std::make_shared<SliderCtrl>();
					ctrl->m_rect.size = SizeF(width, 20);
					ctrl->m_prevVal = value;
					addControl(window, ctrl, id);
				}
				ctrl->m_rect.pos = calcPos(window, pos, ctrl->m_rect.size);
				ctrl->m_enabled = enabled;
				if (ctrl->m_valueChanged)
				{
					value = min + static_cast<T>(ctrl->m_per * static_cast<double>(max - min));
				}
				else
				{
					if (std::any_cast<T>(ctrl->m_prevVal) != value)
					{
						ctrl->m_per = Clamp(static_cast<double>(value - min) / static_cast<double>(max - min), 0.0, 1.0);
					}
				}
				ctrl->m_prevVal = value;
				return ctrl->m_valueChanged;
			}
			/// <summary>
			/// スピンボックス
			/// </summary>
			/// <param name="value">値を操作する変数</param>
			/// <param name="min">最小値</param>
			/// <param name="max">最大値</param>
			/// <param name="width">横幅</param>
			/// <param name="enabled">アクティブ状態</param>
			/// <param name="pos">ウィンドウ内の座標,unspecifiedで自動</param>
			/// <returns>値が変更されたときtrue,それ以外のときfalse</returns>
			template<class T>
			bool spinBox(T& value, const T& min, const T& max, const T& inc = 1, const double width = 200, const bool enabled = true, Optional<Vec2> pos = unspecified)
			{
				detail::Window& window = getCurrentWindow();
				ID id = getID(&value);
				std::shared_ptr<SpinBoxCtrl> ctrl = getControl<SpinBoxCtrl>(window, id);
				if (!ctrl)
				{
					ctrl = std::make_shared<SpinBoxCtrl>(Format(value), getTheme().font);
					ctrl->m_prevVal = value;
					addControl(window, ctrl, id);
				}
				ctrl->m_rect.w = width;
				ctrl->m_rect.h = getTheme().font.height();
				ctrl->m_rect.h += detail::TextBoxPadding.y * 2;
				ctrl->m_rect.pos = calcPos(window, pos, ctrl->m_rect.size);
				ctrl->m_enabled = enabled;
				//テキストが変わったとき
				if (ctrl->m_valueChanged)
				{
					value = ParseOr<T>(ctrl->m_textBox.getText(), 0);
				}
				//上下ボタンが押されたとき
				if (ctrl->m_mov)
				{
					value = value + inc * ctrl->m_mov;
				}
				if (ctrl->m_mov || ctrl->m_valueChanged)
				{
					value = Clamp<T>(value, min, max);
					ctrl->m_textBox.setText(Format(value));
				}
				else
				{
					if (!ctrl->m_textBox.isActive() && std::any_cast<T>(ctrl->m_prevVal) != value)
					{
						ctrl->m_textBox.setText(Format(value));
					}
				}
				ctrl->m_prevVal = value;
				return ctrl->m_mov || ctrl->m_valueChanged;
			}
			/// <summary>
			/// 区切り線
			/// </summary>
			/// <param name="pos">ウィンドウ内の座標,unspecifiedで自動</param>
			void split(Optional<Vec2> pos = unspecified)
			{
				detail::Window& window = getCurrentWindow();
				auto ctrl = std::make_shared<SplitCtrl>();
				ctrl->m_rect.size = SizeF(1, 1);
				addControl(window, ctrl, 0);
				ctrl->m_rect.pos = calcPos(window, pos, ctrl->m_rect.size);
			}
			/// <summary>
			/// プログレスバー
			/// </summary>
			/// <param name="value">min以上,max以下の数値</param>
			/// <param name="min">最小値</param>
			/// <param name="max">最大値</param>
			/// <param name="width">横幅</param>
			/// <param name="color">バーの色</param>
			/// <param name="enabled">アクティブ状態</param>
			/// <param name="pos">ウィンドウ内の座標,unspecifiedで自動</param>
			template<class T>
			void progressBar(const T& value, const T& min, const T& max, double width = 200, Optional<ColorF> color = unspecified, const bool enabled = true, Optional<Vec2> pos = unspecified)
			{
				detail::Window& window = getCurrentWindow();
				auto ctrl = std::make_shared<ProgressBarCtrl>();
				ctrl->m_rect.size = SizeF(width, 10);
				addControl(window, ctrl, getID(&value));
				ctrl->m_rect.pos = calcPos(window, pos, ctrl->m_rect.size);
				ctrl->m_enabled = enabled;
				ctrl->m_color = color ? *color : getTheme().accentCol;
				ctrl->m_per = Clamp(static_cast<double>(value - min) / static_cast<double>(max - min), 0.0, 1.0);
			}
			/// <summary>
			/// ツリーノード
			/// </summary>
			/// <param name="text">表示するテキスト</param>
			/// <param name="expand">最初から展開するならtrue,展開しないならfalse</param>
			/// <returns>展開しているならtrue,展開していないならfalse</returns>
			bool treeNode(const String& text, bool expand = false, const bool enabled = true, Optional<Vec2> pos = unspecified)
			{
				detail::Window& window = getCurrentWindow();
				ID id = getID(text);
				std::shared_ptr<TreeNodeCtrl> ctrl = getControl<TreeNodeCtrl>(window, id);
				if (!ctrl)
				{
					const auto& theme = getTheme();
					ctrl = std::make_shared<TreeNodeCtrl>();
					ctrl->m_rect.size = theme.font(text).region().size + detail::TreeNodePadding * 2;
					ctrl->m_rect.w += theme.font.height();
					ctrl->m_expand = expand;
					addControl(window, ctrl, id);
				}
				ctrl->m_rect.pos = calcPos(window, pos, ctrl->m_rect.size);
				ctrl->m_enabled = enabled;
				ctrl->m_text = text;
				return ctrl->m_expand;
			}
			void showDebugWindow()
			{
				windowBegin(U"Debug");
				windowSetLayoutType(LayoutType::Vertical);
				label(U"hoveringWindow:{}"_fmt(hoveringWindow));
				label(U"WindowHovered:{}"_fmt(windowHovered()));
				label(U"WindowItemHovered:{}"_fmt(windowItemHovered()));
				label(U"currentWindow:{}"_fmt(currentWindow));
				checkBox(debug_drawRect, U"debug_drawRect");
				checkBox(debug_window, U"Window");
				if (debug_window)
				{
					label(U"  Windows");
					for (size_t idx = 0; idx < windows.size(); idx++)
					{
						auto& window = windows[idx];
						label(U"　　{}:"_fmt(idx));
						label(U"　　　id={}"_fmt(window.id));
						label(U"　　　title=\"{}\""_fmt(window.m_title));
						label(U"　　　flags={:#b}{}"_fmt(window.flags, String(idx, U' ')));
						if (itemHovered())
						{
							toolTipBegin();
							windowSetLayoutType(LayoutType::Vertical);
							if (window.flags)
							{
								int32 bit = 1;
								for (const auto& name : { U"NoTitlebar",U"NoResize",U"NoMove",U"NoBackground",U"NoScrollbar",U"AutoResize",U"AlwaysForeground",U"NoFocus",U"NoMargin",U"Hide",U"Disable" })
								{
									if (window.flags & (1 << bit))
									{
										label(name);
									}
									bit++;
								}
							}
							else
							{
								label(U"None");
							}
							toolTipEnd();
						}
						label(U"　　　rect={}{}"_fmt(window.m_rect, String(idx, U' ')));
					}
					label(U"  WindowOrder");
					label(U"    Background: {}"_fmt(windowOrder[detail::Background]));
					label(U"    Nomal: {}"_fmt(windowOrder[detail::Nomal]));
					label(U"    Forground: {}"_fmt(windowOrder[detail::Foreground]));
				}
				windowEnd();
			}
		};
	}
}