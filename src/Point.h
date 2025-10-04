#pragma once
#include "ofMain.h"

class Point {
public:
    Point();                          
    void setup(float radius, ofColor col);
    void update();
    void draw();

    void setPosition(float x, float y, float z);
    void setColor(ofColor col);
    
    float radius;
	glm::vec3 position;
    float timer;

    bool operator==(const Point& other) const {
        return position == other.position;
    }


private:
    ofSpherePrimitive sphere;
    ofColor color;
	
};
