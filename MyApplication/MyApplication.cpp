#include <iostream>
#include <vector>
#include <algorithm>

#include <SFML/Window/Event.hpp>

#include "RenderWindow.h"
#include "ShapesFactory.h"
#include "Composite.h"
#include "ShapeCareTaker.h"

using namespace sf;

float SPEED_COEFFICIENT = 0.5f;

void setParams(sf::Shape* shp, sf::Color color, sf::Vector2f pos,
	sf::Vector2f scale);

int main()
{
	sf::RenderWindow* window = sf::RenderWindow::getInstance(sf::VideoMode(1920, 1080), "Course lab!");

	std::unordered_map<int, ShapeCareTaker> snapshots; // k: Shape ID, v: caretaker objects

	ShapesFactory shapes_factory;
	sf::Shape* my_rect = shapes_factory.createPrototype("rectangle");
	my_rect->setToDefault();
	sf::Shape* my_circle = shapes_factory.createPrototype("circle");
	my_circle->setToDefault();
	sf::Shape* my_ellipse = shapes_factory.createPrototype("ellipse");
	my_ellipse->setToDefault();

	Composite container;

	// Shapes array.
	std::vector<sf::Shape*> shapes;

	// Containers to delete.
	std::vector<sf::Shape*> containers_to_delete;

	// Pointer to the activated shape.
	sf::Shape* current_shape = nullptr;

	bool show_tail_switch = false;
	bool transparency_switch = false;
	int color_switch = 1;

	while (window->isOpen())
	{
		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window->close();

			// Set current shape. Press MLK.
			else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
			{
				sf::Vector2f mouse_position;
				mouse_position.x = sf::Mouse::getPosition(*window).x;
				mouse_position.y = sf::Mouse::getPosition(*window).y;

				for (auto& shape : shapes)
				{
					// Set previous shape's outline thickness to 0.
					if (current_shape)
						current_shape->setOutlineThickness(0);

					if (shape->selectedWithMouse(*window))
					{
						current_shape = shape;
						current_shape->setOutlineThickness(5);
						current_shape->setOutlineColor(sf::Color::White);
						break;
					}
					current_shape = nullptr;
				}
			}

			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P)
			{
				int shapeID;
				Json::Value json_shapes_vector(Json::arrayValue);

				for (auto& shape : shapes)
				{
					shapeID = shape->getID();

					if (snapshots.find(shapeID) == snapshots.end())
					{
						snapshots[shapeID] = ShapeCareTaker(shape);
					}

					snapshots[shapeID].createSnapshot();
					auto json_snap = snapshots[shapeID].getJsonSnapshot();

					json_shapes_vector.append(json_snap);
				}

				std::ofstream write_file("snapshot.json");
				Json::StreamWriterBuilder builder;
				builder["commentStyle"] = "None";
				builder["indentation"] = "    ";
				std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());

				writer->write(json_shapes_vector, &write_file);

			}

			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::O)
			{
				int shapeID;
				std::vector<sf::Shape*> toDelete;

				for (auto& shape : shapes)
				{

					shapeID = shape->getID();
					auto it1 = snapshots.find(shapeID);

					// If shape has caretaker.
					if (it1 != snapshots.end())
					{
						it1->second.undo();
					}

					// If shape doesn't have caretaker.
					else
					{
						toDelete.push_back(shape);
					}
				}

				for (auto& to_delete_shape : toDelete)
				{
					auto it2 = std::find(shapes.begin(), shapes.end(), to_delete_shape);
					if (it2 != shapes.end())
					{
						delete to_delete_shape;
						to_delete_shape = nullptr;
						shapes.erase(it2);
					}
				}

				current_shape = nullptr;
			}

			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::J)
			{
				std::ifstream read_file("snapshot.json");
				Json::Value shapes_json;
				read_file >> shapes_json;
				shapes.clear();

				std::string str = shapes_json.toStyledString();

				for (Json::Value::ArrayIndex i = 0; i != shapes_json.size(); i++)
				{
					sf::Color color(
						shapes_json[i]["fill color"]["r"].asUInt(),
						shapes_json[i]["fill color"]["g"].asUInt(),
						shapes_json[i]["fill color"]["b"].asUInt(),
						shapes_json[i]["fill color"]["a"].asUInt());

					sf::Vector2f position(
						shapes_json[i]["position"]["x"].asFloat(),
						shapes_json[i]["position"]["y"].asFloat());

					sf::Vector2f scale(
						shapes_json[i]["scale"]["x"].asFloat(),
						shapes_json[i]["scale"]["y"].asFloat());


					if (shapes_json[i]["type"] == "rectangle")
					{
						auto shape = my_rect->clone();
						setParams(shape, color, position, scale);

						shapes.push_back(shape);
					}
					else if (shapes_json[i]["type"] == "circle")
					{
						auto shape = my_circle->clone();

						setParams(shape, color, position, scale);
						shapes.push_back(shape);
					}
					else if (shapes_json[i]["type"] == "ellipse")
					{
						auto shape = my_ellipse->clone();

						setParams(shape, color, position, scale);
						shapes.push_back(shape);
					}
					else if (shapes_json[i]["type"] == "composite")
					{
						auto container_shape = container.clone();
						auto container_json = shapes_json[i]["composite shapes"];
						std::string str = container_json.toStyledString();

						auto new_container = container.clone();

						for (Json::Value::ArrayIndex j = 0; j != container_json.size(); j++)
						{
							sf::Color color(
								container_json[j]["fill color"]["r"].asUInt(),
								container_json[j]["fill color"]["g"].asUInt(),
								container_json[j]["fill color"]["b"].asUInt(),
								container_json[j]["fill color"]["a"].asUInt());

							sf::Vector2f position(
								container_json[j]["position"]["x"].asFloat(),
								container_json[j]["position"]["y"].asFloat());

							sf::Vector2f scale(
								container_json[j]["scale"]["x"].asFloat(),
								container_json[j]["scale"]["y"].asFloat());

							if (container_json[j]["type"] == "rectangle")
							{
								auto shape = my_rect->clone();

								setParams(shape, color, position, scale);
								new_container->addShape(shape);
							}
							else if (container_json[j]["type"] == "circle")
							{
								auto shape = my_circle->clone();

								setParams(shape, color, position, scale);
								new_container->addShape(shape);
							}
							else if (container_json[j]["type"] == "ellipse")
							{
								auto shape = my_ellipse->clone();

								setParams(shape, color, position, scale);
								new_container->addShape(shape);
							}
						}

						shapes.push_back(new_container);
					}
				}
			}

			// Visualize or remove shapes.
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num1)
			{
				shapes.push_back(my_rect->clone());
			}
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num2)
			{
				shapes.push_back(my_ellipse->clone());
			}
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num3)
			{
				shapes.push_back(my_circle->clone());
			}

			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num4)
			{
				shapes.push_back(current_shape->clone());
			}

			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Delete)
			{
				if (dynamic_cast<Composite*>(current_shape))
				{
					container.getShapes()->clear();
				}

				shapes.erase(std::remove(shapes.begin(),
					shapes.end(), current_shape), shapes.end());
			}

			// Restore shape to default settings.
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R)
			{
				current_shape->setToDefault();
			}

			// Change color.
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::C))
			{
				sf::Color color = current_shape->getFillColor();
				switch (color_switch)
				{
				case 1:
					current_shape->setFillColor(sf::Color::White);
					color_switch++;
					break;
				case 2:
					current_shape->setFillColor(sf::Color::Blue);
					color_switch++;
					break;
				case 3:
					current_shape->setFillColor(sf::Color::Cyan);
					color_switch++;
					break;
				case 4:
					current_shape->setFillColor(sf::Color::Green);
					color_switch++;
					break;
				case 5:
					current_shape->setFillColor(sf::Color::Magenta);
					color_switch++;
					break;
				default:
					color_switch = 1;
					current_shape->setFillColor(color);
					color_switch++;
					break;
				}
			}

			// Make invisible.
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::I)
			{
				if (current_shape && transparency_switch)
				{
					current_shape->setAlpha(255);
					transparency_switch = false;
				}
				else
				{
					current_shape->setAlpha(0);
					transparency_switch = true;
				}
			}

			// Show tail when move.
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::T)
			{
				if (show_tail_switch == false)
					show_tail_switch = true;
				else
					show_tail_switch = false;
			}

			// Add shape to the container.
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::G) // If container doesn't contain selected shape
			{
				// TODO: check if shape is already in container
				// add shape to the container.

				if (dynamic_cast<Composite*>(current_shape))
				{
					auto selected_composite_vector = dynamic_cast<Composite*>(current_shape)->getShapes();

					for (auto& shape : *selected_composite_vector)
					{
						container.addShape(shape);
					}

					containers_to_delete.push_back(current_shape);
				}
				else
				{
					container.addShape(current_shape);
				}

				current_shape = nullptr;
			}

			// Add container to the shapes vector.
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::H)
			{

				for (auto& container_to_delete : containers_to_delete)
				{
					shapes.erase(std::remove(shapes.begin(),
						shapes.end(), container_to_delete), shapes.end());
				}

				shapes.push_back(container.clone());

				// Remove shapes from shapes vector
				for (auto& shape : *container.getShapes())
				{
					shapes.erase(std::remove(shapes.begin(),
						shapes.end(), shape), shapes.end());
				}

				// Clear container.
				container.getShapes()->clear();
			}
		}

		// Enable keyboard move for current shape.
		if (current_shape)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
			{
				current_shape->move(-SPEED_COEFFICIENT, 0.0f);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
			{
				current_shape->move(SPEED_COEFFICIENT, 0.0f);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
			{
				current_shape->move(0.0f, -SPEED_COEFFICIENT);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
			{
				current_shape->move(0.0f, SPEED_COEFFICIENT);
			}
		}

		// Keep moving after reaching window's bound.
		for (auto& shape : shapes)
		{
			shape->boundMoveActivate(*window);
		}

		// Deformate when collide.
		for (auto& shape : shapes)
		{
			if (current_shape && current_shape != shape &&
				current_shape->intersectsWith(shape, *window))
			{
				shape->setScale(1.5f, 1.5f);
			}
		}

		// Move to the clicked point.
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Mouse::isButtonPressed(sf::Mouse::Right))
		{
			if (current_shape)
			{
				sf::Vector2f total_movement(sf::Mouse::getPosition((*window)).x - current_shape->getPosition().x,
					sf::Mouse::getPosition((*window)).y - current_shape->getPosition().y);
				current_shape->move(total_movement * (1.0f / 60.0f));
			}
		}


		if (show_tail_switch == false)
			window->clear();

		for (auto& shape : shapes)
			window->draw(*shape);

		window->display();
	}

	return 0;
}

void setParams(sf::Shape* shp, sf::Color color, sf::Vector2f pos,
	sf::Vector2f scale)
{
	shp->setFillColor(color);
	shp->setPosition(pos);
	shp->setScale(scale);
}