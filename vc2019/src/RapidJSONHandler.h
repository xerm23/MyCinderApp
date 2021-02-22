#include <iostream>
#include <fstream>
#include <cstdlib>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/ostreamwrapper.h>


#include "Shape.h"
#include "cinder/Log.h"

using namespace rapidjson;
using namespace std;
using namespace ci::log;

class RapidJSONHandler {

private:
	Shape* readShape(string stringFromFile) {
		Document d;
		Shape* shapeRead;
		d.Parse(stringFromFile.c_str());
		Value& X = d["X"];
		Value& Y = d["Y"];
		Value& R = d["R"];
		Value& G = d["G"];
		Value& B = d["B"];
		Value& Rogue = d["Rogue"];
		Value& Name = d["Name"];
		Value& ShapeValue = d["Shape"];
		Value& Radius = d["Radius"];
		Value& Width = d["Width"];
		Value& Height = d["Height"];

		if (0 == std::strcmp(ShapeValue.GetString(), "Circle"))
			shapeRead = new Shape(X.GetFloat(), Y.GetFloat(), R.GetFloat(), G.GetFloat(), B.GetFloat(), Radius.GetFloat(), Circle);		
		//if not circle draw rectangle
		//also works for square because width = height from file 
		else shapeRead = new Shape(X.GetFloat(), Y.GetFloat(), R.GetFloat(), G.GetFloat(), B.GetFloat(), Width.GetFloat(), Height.GetFloat(), Rectangle);
		
		shapeRead->rogue = Rogue.GetBool();
		shapeRead->name = Name.GetString();
		return shapeRead;
	}

	//function saves shape as one line string in project.json
	void saveShapeToRJSON(Shape* shapeToWrite) {
		StringBuffer s;
		Writer<StringBuffer> writer(s);

		writer.StartObject();               // Between StartObject()/EndObject(), 
		writer.Key("Shape");				//output a key
		if (shapeToWrite->type == Circle) 
			writer.String("Circle");		//folow by a value	
		else if (shapeToWrite->type == Square)
			writer.String("Square");		
		else if (shapeToWrite->type == Rectangle)
			writer.String("Rectangle");		

		//radius will be 0 if its not circle
		writer.Key("Radius");
		writer.Double(shapeToWrite->radius);

		//width and height will be 0 if its circle
		writer.Key("Width");
		writer.Double(shapeToWrite->width);
		writer.Key("Height");
		writer.Double(shapeToWrite->height);

		writer.Key("X");				
		writer.Double(shapeToWrite->x); 
		writer.Key("Y");
		writer.Double(shapeToWrite->y);
		writer.Key("R");
		writer.Double(shapeToWrite->r);
		writer.Key("G");
		writer.Double(shapeToWrite->g);
		writer.Key("B");
		writer.Double(shapeToWrite->b);
		writer.Key("Rogue");
		writer.Bool(shapeToWrite->rogue);
		writer.Key("Name");
		writer.String(shapeToWrite->name.c_str());
		writer.EndObject();


		std::ofstream outFile;
		outFile.open("project.json", std::ios_base::app);
		outFile << s.GetString() << endl;
		outFile.close();
	}

public:

	//function returns all shapes by reading project.json line by line
	vector<Shape*> readAllShapes() {
		vector<Shape*> tmpVector;
		string line;
		std::ifstream inFile;
		inFile.open("project.json");
		if (inFile.is_open()){
			//get string line from file and turn it into a shape
			while (getline(inFile, line)){
				tmpVector.push_back(readShape(line));
			}
			inFile.close();
		}
		return tmpVector;
	}

	//saves all objects in project.json
	void saveAllShapes(vector<Shape*> shapes) {
		//clear if something inside of it
		std::ofstream outFile;
		outFile.open("project.json", std::ofstream::out | std::ofstream::trunc);
		outFile.close();

		//write each shape object as one line 
		for (size_t i = 0; i < shapes.size(); i++){
			saveShapeToRJSON(shapes[i]);			
		}
	}
};

