#pragma once

#include <SFML/Graphics/Export.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Window/Window.hpp>
#include <string>


namespace sf
{
	class SFML_GRAPHICS_API RenderWindow : public Window, public RenderTarget
	{
	public:
		static RenderWindow* getInstance(VideoMode mode, const String& title, Uint32 style = Style::Default, const ContextSettings& settings = ContextSettings());
		
		virtual ~RenderWindow();
		virtual Vector2u getSize() const;
		bool setActive(bool active = true);
		SFML_DEPRECATED Image capture() const;

	protected:
		RenderWindow();
		RenderWindow(VideoMode mode, const String& title, Uint32 style = Style::Default, const ContextSettings& settings = ContextSettings());
		explicit RenderWindow(WindowHandle handle, const ContextSettings& settings = ContextSettings());

		virtual void onCreate();
		virtual void onResize();

		static RenderWindow* instance;
	};

}