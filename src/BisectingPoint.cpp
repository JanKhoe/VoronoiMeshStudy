#include "BisectingPoint.h"
#include "ofApp.h"

BisectingPoint::BisectingPoint() {
    color = ofColor(255);
    lineEdges = {
        glm::vec3(-BOUND_X, -BOUND_Y, 0), 
        glm::vec3(-BOUND_X, BOUND_Y, 0), 
        glm::vec3(BOUND_X, BOUND_Y, 0), 
        glm::vec3(BOUND_X, -BOUND_Y, 0)
    };
}

void BisectingPoint::update(float deltatime) {

}



void BisectingPoint::draw() {
    Point::draw();

    ofSetColor(255);
    ofPolyline cell;
    cell.addVertices(lineEdges);
    cell.close();
    cell.draw();


}