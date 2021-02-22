#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Log.h"
#include "cinder/Rand.h"
#include "cinder/CinderImGui.h"

using namespace ci;
using namespace ci::app;
using namespace ci::log;


//my custom RapidJSON controller used to save/open shapes 
#include "RapidJSONHandler.h"
//my custom ImGui control defined as ImGui::MyCheckBox
#include "CustomCheckBox.h"

//this is used to know which type of shape we instantiate
ShapeType currentShape = Circle;

//this is used for imgui properties window
Shape* selected;

bool moveAllowed = true;
bool rotateAllowed = true;

class BasicApp : public App {
	RapidJSONHandler RJSON;

public:
	void setup() override;
	void mouseDown(MouseEvent event) override;
	void keyDown(KeyEvent event) override;

	void update() override;
	void draw() override;

	std::vector<Shape*> shapes;
};

void BasicApp::setup(){
	setWindowPos(0, 50);
	setWindowSize(1800, 900);
	ImGui::Initialize();
}

void BasicApp::keyDown(KeyEvent event) {
	if (!shapes.empty()) {
		switch (event.getCode()) {
		case KeyEvent::KEY_w:
			shapes[shapes.size() - 1]->y -= 5;
			break;
		case KeyEvent::KEY_a:
			shapes[shapes.size() - 1]->x -= 5;
			break;
		case KeyEvent::KEY_s:
			shapes[shapes.size() - 1]->y += 5;
			break;
		case KeyEvent::KEY_d:
			shapes[shapes.size() - 1]->x += 5;
			break;
		case KeyEvent::KEY_ESCAPE:
			selected = NULL;
			break;
		case KeyEvent::KEY_1:
			currentShape = Circle;
			break;
		case KeyEvent::KEY_2:
			currentShape = Square;
			break;
		case KeyEvent::KEY_3:
			currentShape = Rectangle;
			break;
		case KeyEvent::KEY_b:
			moveAllowed = !moveAllowed;
			for (int i = 0; i < shapes.size(); i++)
				shapes[i]->moveAllowed = moveAllowed;			
			break;
		case KeyEvent::KEY_r:
			rotateAllowed = !rotateAllowed;
			for (int i = 0; i < shapes.size(); i++)
				shapes[i]->rotateAllowed = rotateAllowed;
			break;
		}
	}
}
void BasicApp::mouseDown(MouseEvent event) {
	if (event.isLeftDown()) {
		Shape* shape = new Shape(event.getPos().x, event.getPos().y, currentShape, moveAllowed, rotateAllowed);
		shape->name = "Shape: " + std::to_string(shapes.size());
		shapes.push_back(shape);

	}
	if (event.isRightDown()) {		
		//iterate through shapes from the last one, so the shape on top is deleted
		std::vector<Shape*>::iterator it = shapes.end();
		while (it != shapes.begin()){
			--it;
			if ((*it)->isClickedOn(event.getPos().x, event.getPos().y)){
				shapes.erase(it);
				selected = NULL;
				break;				
			}
		}

		//rename all shapes based on their index in vector, this way makes it easier to track shapes in imgui list
		int index = 0;
		for (auto it = shapes.begin(); it != shapes.end(); ++it) {
			(*it)->name = "Shape: " + std::to_string(index++);
		}
	}
	CI_LOG_D(shapes.size());

}


void BasicApp::update() {

	//list window
	ImGui::Begin("List");
	ImGui::ListBoxHeader("Shapes");
	std::string name = "Shape: " + std::to_string(shapes.size());
	for(Shape* item : shapes){
		if (ImGui::Selectable(item->name.c_str(), true))
			selected = item;
	}
	ImGui::ListBoxFooter();
	ImGui::End();

	//properties window
	ImGui::Begin("Properties");
	if (selected != NULL) {
		ImGui::MyCheckbox("Rogue", &selected->rogue);
		if (!selected->rogue) {
			ImGui::Text("Location");
			ImGui::SliderFloat("X", &selected->x, 0.0f + selected->radius + selected->width/2, getWindowSize().x - selected->radius - selected->width/2); 
			ImGui::SliderFloat("Y", &selected->y, 0.0f + selected->radius + selected->height / 2, getWindowSize().y - selected->radius - selected->height / 2);

			float *shapeColor[3] = {&selected->r, &selected->g, &selected->b};
			ImGui::ColorEdit3("Color picker", *shapeColor);

		}
	}
	ImGui::End();

	//main menu bar 
	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("File")) {
		if (ImGui::MenuItem("Save"))
			RJSON.saveAllShapes(shapes);
		if (ImGui::MenuItem("Open"))
			shapes = RJSON.readAllShapes();
		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();
}

void BasicApp::draw(){
	gl::clear(Color::gray(0.1f));

	for (int i = 0; i < shapes.size(); i++)
		shapes[i]->drawShape();
}

CINDER_APP(BasicApp, RendererGl)
