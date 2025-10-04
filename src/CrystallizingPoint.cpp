#include "CrystallizingPoint.h"


CrystallizingPoint::CrystallizingPoint() {
    color = ofColor(255);
    radius = 0;
}

void CrystallizingPoint::update(float deltatime) {
    radius += deltatime;
}

void CrystallizingPoint::draw() {
    Point::draw();


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