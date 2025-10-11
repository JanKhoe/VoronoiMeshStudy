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
    float angle = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2*PI));
    dir = glm::vec3(cos(angle), sin(angle), 0);
}

void BisectingPoint::update(float deltatime) {
    lineEdges = {
        glm::vec3(-BOUND_X, -BOUND_Y, 0),
        glm::vec3(-BOUND_X, BOUND_Y, 0),
        glm::vec3(BOUND_X, BOUND_Y, 0),
        glm::vec3(BOUND_X, -BOUND_Y, 0)
    };
    POIindicies = vector<int>();

    position += dir * deltatime;
    if (position.x < -BOUND_X) {
        glm::vec3 normal = glm::vec3(1, 0, 0);
        dir = glm::reflect(dir, normal);

        position.x = -BOUND_X + 0.1f;
    }
    if (position.x > BOUND_X) {
        glm::vec3 normal = glm::vec3(-1, 0, 0);
        dir = glm::reflect(dir, normal);
        position.x = BOUND_X - 0.1f;
    }
    if (position.y < -BOUND_Y) {
        glm::vec3 normal = glm::vec3(0, 1, 0);
        dir = glm::reflect(dir, normal);
        position.y = -BOUND_Y + 0.1f;
    }
    else if (position.y > BOUND_Y) {
        glm::vec3 normal = glm::vec3(0, 1, 0);
        dir = glm::reflect(dir, normal);
        position.y = BOUND_Y - 0.1f;
    }
    setPosition(position.x, position.y, position.z);


}



void BisectingPoint::draw() {
    Point::draw();

    ofSetColor(255);
    ofPolyline cell;
    cell.addVertices(lineEdges);
    cell.close();
    cell.draw();


}