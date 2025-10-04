#include "Point.h"

Point::Point() {
    color = ofColor(255); // default white
    radius = 0;
}



void Point::setup(float radius, ofColor col) {
    sphere.setRadius(radius);
    color = col;
    radius = 0;
}

void Point::update() {
    // put animations or transformations here if needed
}

void Point::draw() {
    ofSetColor(color);
    sphere.drawWireframe();   // or sphere.draw() for solid

    ofSetColor(0, 0, 255);
    

    ofPolyline circle;
    int numSegments = 64;        // more = smoother
    float cirRadius = radius;
    glm::vec3 center = position;

    for (int i = 0; i < numSegments; i++) {
        float angle = TWO_PI * i / numSegments;
        float x = center.x + cirRadius * cos(angle);
        float y = center.y + cirRadius * sin(angle);
        float z = center.z;      // flat in XY plane, adjust for other planes if needed
        circle.addVertex(x, y, z);
    }
    circle.close();
    circle.draw();

}

void Point::setPosition(float x, float y, float z) {
    sphere.setPosition(x, y, z);
	position = glm::vec3(x, y, z);
}

void Point::setColor(ofColor col) {
    color = col;
}