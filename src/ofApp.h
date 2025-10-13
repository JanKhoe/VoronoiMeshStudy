#pragma once

#include "ofMain.h"
#include "Point.h" 
#include "CrystallizingPoint.h"
#include "BisectingPoint.h"
#include "3DPoint.h"

#define BOUND_X 800
#define BOUND_Y 800
#define BOUND_Z 800
#define DELTA 1
#define NUM_POINTS 10

enum simType {
	CRYSTAL,
	BISECT,
	THREED
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

		void checkForIntersections();
		void findCellShape();
		void findIntersectingPlanes();
		void ChangeCell(glm::vec3 lineEq, BisectingPoint& p);
		glm::vec3 Bisector(glm::vec3 p, glm::vec3 q);
		bool whichSideOfLine(glm::vec3 point, glm::vec3 A, glm::vec3 B);
		
		vector<glm::vec2> findIntersection(CrystallizingPoint& p1, CrystallizingPoint& p2);
		vector<glm::vec2> allEdges;
		std::vector<std::pair<int, int>> choose2(int n);
		float timer;

		simType simulationType;

		ofEasyCam cam;
		vector<std::unique_ptr<Point>> points;
		};
