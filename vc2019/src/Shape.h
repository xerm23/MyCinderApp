#include "cinder/Rand.h"


using namespace ci;
using namespace ci::app;

enum ShapeType{
	Circle,
	Square,
	Rectangle
};


#pragma region UtilFunctions

//returns true if point(x, y) is inside of a circle(center_x, center_y + radius)
bool inCircle(float center_x, float center_y, float radius, float x, float y) {
	float square_dist = (center_x - x) * (center_x - x) + (center_y - y) * (center_y - y);
	return square_dist <= radius * radius;
}

//returns true if clicked in rectangle (works for square too)
bool inRectangle(float x, float y, float width, float height, float mousePosX, float mousePosY) {
	if ((mousePosX > x - width / 2) && (mousePosX < x + width / 2) &&
		(mousePosY > y - height / 2) && (mousePosY < y + height / 2)) return true;
	return false;
}

#pragma endregion


class Shape {
public:
	Shape() {};
	Shape(float x, float y, ShapeType type, bool moveAllowed, bool rotateAllowed)
		: x(x), y(y), type(type), moveAllowed(moveAllowed), rotateAllowed(rotateAllowed) {
		r = randFloat();
		g = randFloat();
		b = randFloat();
		if (type == Circle) {
			radius = randFloat(30, 70);
		}
		if (type == Square) {
			width = randFloat(50, 150);
			height = width;
		}
		if (type == Rectangle) {
			width = randFloat(50, 150);
			height = randFloat(50, 150);
		}
	};
	Shape(float x, float y, float r, float g, float b, float radius, ShapeType type)
		: x(x), y(y), r(r), g(g), b(b), radius(radius), type(type) {};
	Shape(float x, float y, float r, float g, float b, float width, float height, ShapeType type)
		: x(x), y(y), r(r), g(g), b(b), width(width), height(height), type(type) {};

	ShapeType type;
	float x, y;
	float width=0, height=0;
	float r, g, b;
	float radius=0;
	bool rogue = false;

	//needed for moving object
	vec2 moveVector = vec2(randFloat(-2, 2), randFloat(-2, 2));
	bool moveAllowed = true;

	//needed for rotation of objects 
	float rotationAngle = 0;
	bool rotateAllowed = true;
	float rotationStep = randFloat(-0.15, 0.15);

	//name value needed for easier track in imgui list
	std::string name;


	//this needs to be called from app::draw
	void drawShape() {
		gl::pushModelMatrix();
		gl::translate(vec2(x, y));
		gl::pushModelMatrix();
		gl::rotate(rotationAngle, 0, 0, 1);
		gl::color(r, g, b);
		if (type == Circle)
			gl::drawSolidCircle(vec2(0, 0), radius, 100);
		else gl::drawSolidRect(Rectf(
			- width / 2,
			- height / 2,
			width / 2,
			height / 2));
		gl::popModelMatrix();
		gl::popModelMatrix();

		if (rotateAllowed) {
			if (rotationAngle >= 360) rotationAngle = 0;
			rotationAngle += rotationStep;
		}
		if(moveAllowed)
		moveObject();

	}


	//returns true if we click on the shape -> should be used for deleting object on right click
	bool isClickedOn(float mousePosX, float mousePosY) {
		//check if the circle on top is clicked and use function for circle check
		if (type == Circle) {
			if (inCircle(x, y, radius, mousePosX, mousePosY)) return true;
			
		}
		//if it is a rectangle or square use check function for rectangles
		else {
			if (inRectangle(x, y, width, height, mousePosX, mousePosY)) return true;			
		}
		return false;
	}
private:
	void moveObject() {
		x += moveVector.x;
		y += moveVector.y;

		//if shape is circle -> width = 0
		//if shape is rectangle/square -> radius = 0 so this covers both cases of object on windowedge
		if (x+radius+width/2 > getWindowWidth()) {
			x = getWindowWidth()-radius-width/2;
			moveVector.x = -moveVector.x;
		}
		if (x-radius-width/2 < 0) {
			x = radius+width/2;
			moveVector.x = -moveVector.x;
		}
		if (y+radius+height/2 > getWindowHeight()) {
			y = getWindowHeight()-radius-height/2;
			moveVector.y = -moveVector.y;
		}
		if (y-radius-height/2 < 0) {
			y = radius + height/2;
			moveVector.y = -moveVector.y;
		}


	}

};
