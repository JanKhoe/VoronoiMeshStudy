#pragma once
#include "ofMain.h"
#include "Point.h"

class CrystallizingPoint : public Point{
public:
    CrystallizingPoint();
    void draw();
    void update(float deltatime) override;

    float radius;

};
