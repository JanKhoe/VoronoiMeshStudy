#include "ofApp.h"


vector<glm::vec2> ofApp::findIntersection(Point p1, Point p2) {

    std::vector<glm::vec2> intersections;

    glm::vec3 vec = (p1.position - p2.position);
    float d = glm::length(vec);
       
    if( d > p1.radius + p2.radius) {
        // no intersection
        return intersections;
	}

	// the common chord of the intersecting circles is always perpendicular to the line connection the centers of the 2 circles
    // that intersects that line at a point where a intersection point  of the circles is projected on the distance between the centers ('a').
    // this is given by a formula derived by the cosine law.

	float a = (p1.radius * p1.radius - p2.radius * p2.radius + d * d) / (2.0 * d);

	//using 'a' construc a right angle triangle going to center of p1, an intersection point and 'a' to find the distance from 'a' to the intersection point(s) 'h'

	float h = sqrt(p1.radius * p1.radius - a * a);

    // calculate the point along the 2 circles that 'a' resides on

    glm::vec2 p0 = p1.position + a * (p2.position - p1.position) / d;

    // find the intersection points by going 'h' distance along a perpendicular line that intersects on 'a'

    glm::vec2 intersection1 = glm::vec2(
        p0.x + h * (p2.position.y - p1.position.y) / d,
        p0.y - h * (p2.position.x - p1.position.x) / d
    );

    glm::vec2 intersection2 = glm::vec2(
        p0.x - h * (p2.position.y - p1.position.y) / d,
        p0.y + h * (p2.position.x - p1.position.x) / d
    );

    if (!(abs(intersection1.x) > BOUND_X || abs(intersection1.y) > BOUND_Y)) {
        bool IsClosest = true;
        for (Point p : points) {
            if (p == p1 || p == p2) continue;
            if ((glm::length(p.position - intersection1) < glm::length(p1.position - intersection1))) {
                IsClosest = false;
                break;
            }
        }
        if (IsClosest) {
            intersections.push_back(intersection1);
        }
        
    }

    

    // if circles touch at one point, intersection1 == intersection2
    if (intersection1 != intersection2 && !(abs(intersection2.x) > BOUND_X || abs(intersection2.y) > BOUND_Y)) {
        bool IsClosest = true;
        for (Point p : points) {
            if (p == p1 || p == p2) continue;
            if ((glm::length(p.position - intersection2) < glm::length(p1.position - intersection2))) {
                IsClosest = false;
                break;
            }
        }
        if (IsClosest) {
            intersections.push_back(intersection2);
        }
    }


    return intersections;

}


//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0);
	ofEnableDepthTest();
    cam.setDistance(2000);
    timer = 0;


    for (int i = 0; i < 50; i++) {
        Point p;
        p.setup(1, ofColor::fromHsb(ofRandom(255), 200, 255)); // random color
        float x = ofRandom(-800, 800);
        float y = ofRandom(-800, 800);
        p.setPosition(x, y, 0);
        points.push_back(p);
    }
}

//--------------------------------------------------------------
void ofApp::update(){

    timer += DELTA;
    if (timer > 1000) return;

    for (Point &p1 : points) {
        for (Point &p2 : points) {
            if (p2 == p1) {
                continue;
            }
            vector<glm::vec2> intersections = findIntersection(p1, p2);
            if (intersections.size() > 0) {

                allEdges.insert(allEdges.end(), intersections.begin(), intersections.end());
            }
        }
    }

    for (Point &p : points) {
        
        p.radius += DELTA;
        
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    cam.begin(); // start camera

    for (Point p : points) {
        if (timer > 1000) break;
        p.draw();
        
    }
    ofSetColor(255, 255, 255);
    for (glm::vec2 e : allEdges) {
        
        ofDrawSphere(glm::vec3(e.x, e.y, 0), 0.5f);
    }
    ofNoFill();            // outline only
    ofSetColor(0, 255, 0); // green
    ofSetLineWidth(2);

    // Draw rectangle from (-800, -800) to (800, 800)
    ofDrawRectangle(-800, -800, 1600, 1600);

    cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
