#include "RenderWindow.h"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/GLCheck.hpp>
#include <SFML/Graphics/RenderTextureImplFBO.hpp>

namespace sf
{
	RenderWindow::RenderWindow()
	{
		// Nothing to do
	}

	RenderWindow::RenderWindow(VideoMode mode, const String& title, Uint32 style, const ContextSettings& settings)
	{
		// Don't call the base class constructor because it contains virtual function calls
		create(mode, title, style, settings);
	}

	RenderWindow::RenderWindow(WindowHandle handle, const ContextSettings& settings)
	{
		// Don't call the base class constructor because it contains virtual function calls
		create(handle, settings);
	}

	RenderWindow* RenderWindow::instance = nullptr;
	
	RenderWindow* RenderWindow::getInstance(VideoMode mode, const String& title, Uint32 style,
		const ContextSettings& settings)
	{
		if (instance == nullptr)
		{
			instance = new RenderWindow(mode, title, style, settings);
		}
		return instance;
	}

	RenderWindow::~RenderWindow()
	{
		// Nothing to do
	}

	Vector2u RenderWindow::getSize() const
	{
		return Window::getSize();
	}

	bool RenderWindow::setActive(bool active)
	{
		bool result = Window::setActive(active);

		// Update RenderTarget tracking
		if (result)
			RenderTarget::setActive(active);

		// If FBOs are available, make sure none are bound when we
		// try to draw to the default framebuffer of the RenderWindow
		if (active && result && priv::RenderTextureImplFBO::isAvailable())
		{
			priv::RenderTextureImplFBO::unbind();

			return true;
		}

		return result;
	}

	Image RenderWindow::capture() const
	{
		Vector2u windowSize = getSize();

		Texture texture;
		texture.create(windowSize.x, windowSize.y);
		texture.update(*this);

		return texture.copyToImage();
	}

	void RenderWindow::onCreate()
	{
		// Just initialize the render target part
		RenderTarget::initialize();
	}

	void RenderWindow::onResize()
	{
		// Update the current view (recompute the viewport, which is stored in relative coordinates)
		setView(getView());
	}

}
