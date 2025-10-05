#pragma once
#include "ofMain.h"
#include "Point.h"


class BisectingPoint : public Point {
public:
    BisectingPoint();
    void draw() override;
    void update(float deltatime) override;

    vector<glm::vec3> lineEdges;
    vector<int> POIindicies;


};
