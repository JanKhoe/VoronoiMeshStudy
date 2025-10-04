#pragma once

#include "ofMain.h"
#include "Point.h" 
#include "CrystallizingPoint.h"

#define BOUND_X 800
#define BOUND_Y 800
#define DELTA 0.5

enum simType {
	CRYSTAL
};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		vector<glm::vec2> findIntersection(CrystallizingPoint& p1, CrystallizingPoint& p2);
		vector<glm::vec2> allEdges;
		float timer;

		ofEasyCam cam;
		vector<std::unique_ptr<Point>> points;
		};
