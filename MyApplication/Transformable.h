#pragma once

#include <SFML/Graphics/Export.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <unordered_map>


namespace sf
{
	class SFML_GRAPHICS_API Transformable
	{
	public:
		Transformable();
		virtual ~Transformable();
		virtual void setPosition(float x, float y);
		void setPosition(const Vector2f& position);
		void setRotation(float angle);
		virtual void setScale(float factorX, float factorY);
		virtual void setScale(const Vector2f& factors);
		void setOrigin(float x, float y);
		void setOrigin(const Vector2f& origin);
		virtual const Vector2f& getPosition() const;

		float getRotation() const;
		const Vector2f& getScale() const;
		const Vector2f& getOrigin() const;
		virtual void move(float offsetX, float offsetY);
		virtual void move(const Vector2f& offset);
		void rotate(float angle);
		void scale(float factorX, float factorY);
		void scale(const Vector2f& factor);
		const Transform& getTransform() const;
		const Transform& getInverseTransform() const;

	private:
		Vector2f          m_origin;
		Vector2f          m_position;
		float             m_rotation;
		Vector2f          m_scale;
		mutable Transform m_transform;
		mutable bool      m_transformNeedUpdate;
		mutable Transform m_inverseTransform;
		mutable bool      m_inverseTransformNeedUpdate;
	};
}
