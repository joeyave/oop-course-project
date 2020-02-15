#pragma once

// My Shape.
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>

#include "Transformable.h"
#include "RenderWindow.h"
#include "ShapeState.h"
#include <map>
#include <fstream>
#include <json/json.h>

namespace sf
{
	class Shape : public Drawable, public Transformable
	{
	public:

		// MEMENTO PATTERN FUNCTIONS.
		class Memento
		{
		public:
			Memento() {}
			Memento(ShapeState state)
			{
				this->state = state;
			}

			ShapeState getSavedState()
			{
				return state;
			}

		protected:
			friend class Shape;

			ShapeState state;
		};

		virtual Memento saveToMemento()
		{
			ShapeState newState;

			newState.shapeID = this->getID();

			newState.fillColor = this->getFillColor();
			newState.position = this->getPosition();
			newState.scale = this->getScale();

			return Memento(newState);
		}

		virtual void restoreFromMemento(Memento memento)
		{
			ShapeState state = memento.getSavedState();

			setFillColor(state.fillColor);
			setPosition(state.position);
			setScale(state.scale);
		}

		virtual Json::Value saveToJson()
		{
			Json::Value shape;

			shape["shape id"] = this->getID();
			shape["scale"]["x"] = this->getScale().x;
			shape["scale"]["y"] = this->getScale().y;
			shape["fill color"]["a"] = this->getFillColor().a;
			shape["fill color"]["b"] = this->getFillColor().b;
			shape["fill color"]["g"] = this->getFillColor().g;
			shape["fill color"]["r"] = this->getFillColor().r;
			shape["position"]["x"] = this->getPosition().x;
			shape["position"]["y"] = this->getPosition().y;
			shape["type"] = this->getType();
			
			return shape;
		}

		virtual void restoreFromJson(Json::Value shape)
		{

			sf::Color color(
				shape["fill color"]["r"].asUInt(),
				shape["fill color"]["g"].asUInt(),
				shape["fill color"]["b"].asUInt(),
				shape["fill color"]["a"].asUInt());

			sf::Vector2f position(
				shape["position"]["x"].asFloat(),
				shape["position"]["y"].asFloat());

			sf::Vector2f scale(
				shape["scale"]["x"].asFloat(),
				shape["scale"]["y"].asFloat());

			setFillColor(color);
			setPosition(position);
			setScale(scale);
		}

		// De/Constructors.
		virtual ~Shape();

		// Setters.
		void incrementID()
		{
			ID = counter++;
		}

		void setTexture(const Texture* texture, bool resetRect = false);
		void setTextureRect(const IntRect& rect);
		virtual void setFillColor(const Color& color);
		void setOutlineColor(const Color& color);
		virtual void setOutlineThickness(float thickness);

		// Getters.
		virtual int getID()
		{
			return ID;
		}

		const Texture* getTexture() const;
		const IntRect& getTextureRect() const;
		virtual const Color& getFillColor() const;
		const Color& getOutlineColor() const;
		float getOutlineThickness() const;
		virtual std::size_t getPointCount() const;
		virtual Vector2f getPoint(std::size_t index) const;
		FloatRect getLocalBounds() const;
		virtual FloatRect getGlobalBounds() const;
		virtual std::string getType() = 0;
		
		// Other methods.
		virtual Shape* clone() = 0;
		virtual void setToDefault() = 0;

		virtual void addShape(sf::Shape* shp) {}

		virtual bool selectedWithMouse(RenderWindow& wnd)
		{
			sf::Vector2f mouse_position;
			mouse_position.x = sf::Mouse::getPosition(wnd).x;
			mouse_position.y = sf::Mouse::getPosition(wnd).y;

			if (getGlobalBounds().contains(mouse_position))
				return true;

			return false;
		}

		virtual void boundMoveActivate(RenderWindow& wnd)
		{
			if (getPosition().x < 0)
			{
				setPosition(sf::Vector2f(wnd.getSize().x, getPosition().y));
			}
			if (getPosition().x > wnd.getSize().x)
			{
				setPosition(sf::Vector2f(0.0f, getPosition().y));
			}
			if (getPosition().y > wnd.getSize().y)
			{
				setPosition(sf::Vector2f(getPosition().x, 0.0f));
			}
			if (getPosition().y < 0)
			{
				setPosition(sf::Vector2f(getPosition().x, wnd.getSize().y));
			}
		}

		virtual bool intersectsWith(sf::Shape* shp, RenderWindow& wnd)
		{
			if (getGlobalBounds().intersects(shp->getGlobalBounds()))
				return true;
			return false;
		}

		virtual void setAlpha(const sf::Uint8 index)
		{
			Color color = getFillColor();
			color.a = index;
			setFillColor(color);
		}

	protected:
		Shape();
		void update();
		static int counter;

	private:
		virtual void draw(RenderTarget& target, RenderStates states) const;
		void updateFillColors();
		void updateTexCoords();
		void updateOutline();
		void updateOutlineColors();

		const Texture* m_texture;
		IntRect        m_textureRect;
		Color          m_fillColor;
		Color          m_outlineColor;
		float          m_outlineThickness;
		VertexArray    m_vertices;
		VertexArray    m_outlineVertices;
		FloatRect      m_insideBounds;
		FloatRect      m_bounds;

		int ID;
	};
}
