#include "ofApp.h"
//--------------------------------------------------------------
void ofApp::setup(){

    /*
    
    
    
    CHANGE THE LINE BELOW FOR A DIFFERENT SIMULATION TYPE
    
    
    
    */
    simulationType = BISECT;

	ofBackground(0);
	ofEnableDepthTest();
    cam.setDistance(2000);
    timer = 0;


    for (int i = 0; i < NUM_POINTS; i++) {
        std::unique_ptr<Point> p;

        switch (simulationType) {
        case CRYSTAL:
            p = std::make_unique<CrystallizingPoint>();
            break;
        case BISECT:
            p = std::make_unique<BisectingPoint>();
            break;
        default:
            p = nullptr;
            break;
        }
        
        
        p->setup(1, ofColor::fromHsb(ofRandom(255), 200, 255));
        p->setPosition(ofRandom(-BOUND_X, BOUND_X), ofRandom(-BOUND_Y, BOUND_Y), 0);
        points.push_back(std::move(p));
    }

    if (simulationType == BISECT) {
        findCellShape();
    }
}

vector<glm::vec2> ofApp::findIntersection(CrystallizingPoint& p1, CrystallizingPoint& p2) {

    vector<glm::vec2> intersections;

    glm::vec3 vec = (p1.position - p2.position);
    float d = glm::length(vec);
    if (d > p1.radius + p2.radius) return intersections;

    if (d > p1.radius + p2.radius) {
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
        for (auto& p : points) {
            if (*p == p1 || *p == p2) continue;
            if ((glm::length(p->position - intersection1) < glm::length(p1.position - intersection1))) {
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
        for (auto& p : points) {
            if (*p == p1 || *p == p2) continue;
            if ((glm::length(p->position - intersection2) < glm::length(p1.position - intersection2))) {
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
void ofApp::update(){

    timer += DELTA;
    

    if (simulationType == CRYSTAL) {
        checkForIntersections();
    }
    
    

    for (auto &p : points) {
        
        p->update(DELTA);
        
    }
    if (simulationType == BISECT) {
        findCellShape();
    }
}

void ofApp::findCellShape() {
    for (auto& p1 : points) {
        for (auto& p2 : points) {
            if (p2 == p1) {
                continue;
            }
            glm::vec3 eqOfBisect = Bisector(p1->position, p2->position);
            /*ofLog() << "vec3: " << p1->position;
            ofLog() << "vec3: " << p2->position;
            ofLog() << "vec3: " << eqOfBisect[0] << eqOfBisect[1] << eqOfBisect[2];
            ofLog() << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";*/
            BisectingPoint* b1 = dynamic_cast<BisectingPoint*>(p1.get());
            ChangeCell(eqOfBisect, *b1);

        }
    }
}

glm::vec3 ofApp::Bisector(glm::vec3 p, glm::vec3 q) {
    float a = q.x - p.x;
    float b = q.y - p.y;
    float c = -1 * a * ((q.x + p.x) / 2) - b * ((q.y + p.y) / 2);
    //returns the perpendicular bisector between p and q in the form of ax + by + c = 0 
    //std::cout << a << " | " << b << " | " << c;
    //std::cout << "\n" << p << " | " << q;
    return glm::vec3{a, b, c};
}

void ofApp::checkForIntersections() {
    for (auto& p1 : points) {
        for (auto& p2 : points) {
            if (p2 == p1) {
                continue;
            }
            CrystallizingPoint* c1 = dynamic_cast<CrystallizingPoint*>(p1.get());
            CrystallizingPoint* c2 = dynamic_cast<CrystallizingPoint*>(p2.get());
            if (!c1 || !c2) continue; // skip if not CrystallizingPoint
            vector<glm::vec2> intersections = findIntersection(*c1, *c2);
            if (intersections.size() > 0) {

                allEdges.insert(allEdges.end(), intersections.begin(), intersections.end());
            }
        }
    }
}

void ofApp::ChangeCell(glm::vec3 lineEq, BisectingPoint& p) {
    //printf("\n calculating point of intersection");
    for (int i = 0; i < p.lineEdges.size(); i++) {
        //check for intersections along this line
        glm::vec3 A = p.lineEdges[i];
        glm::vec3 B = p.lineEdges[(i + 1) % p.lineEdges.size()];
        glm::vec3 v = glm::vec3{
            (A.y - B.y),
            (B.x - A.x),
            (A.x * B.y) - (B.x * A.y)
        };

        glm::vec3 z = glm::cross(lineEq, v);
        if (z.z != 0) {
            
            glm::vec3 POI = glm::vec3{
                (lineEq.y * v.z - v.y * lineEq.z)  / z.z,
                (lineEq.z * v.x - v.z * lineEq.x) / z.z,
                0
            };
            const float EPS = 1e-4f;
            //std::cout << "POI: " << POI << "between lines: " << A << " and " << B << "\n";
            if (POI.x >= glm::min(A.x, B.x) - EPS &&
                POI.x <= glm::max(A.x, B.x) + EPS &&
                POI.y >= glm::min(A.y, B.y) - EPS &&
                POI.y <= glm::max(A.y, B.y) + EPS)
            {
                p.lineEdges.insert(p.lineEdges.begin() + i+1, POI);
                p.POIindicies.push_back(i+1);
                i++;
                //std::cout << "valid point!";
            }
        }
        
    }

    if (p.POIindicies.size() >= 2) {
        //printf("hello!");
        bool correctSide = whichSideOfLine(p.position, p.lineEdges[p.POIindicies[0]], p.lineEdges[p.POIindicies[1]]);
        vector<glm::vec3> newMesh;
        

        for (int i = 0; i < p.lineEdges.size(); i++) {
            float index = (i + p.POIindicies[1]) % p.lineEdges.size();
            float next = (i + p.POIindicies[1] + 1) % p.lineEdges.size();
            newMesh.push_back(p.lineEdges[index]);
            if (whichSideOfLine(p.position, p.lineEdges[index], p.lineEdges[next]) != correctSide) {
                newMesh = vector<glm::vec3>();
                for (int i2 = 0; i2 < p.lineEdges.size(); i2++) {
                    float index = (i2 + p.POIindicies[0]) % p.lineEdges.size();
                    float next = (i2 + p.POIindicies[0] + 1) % p.lineEdges.size();
                    newMesh.push_back(p.lineEdges[index]);
                    if (next == p.POIindicies[1]) {
                        newMesh.push_back(p.lineEdges[p.POIindicies[1]]);
                        break;
                    }
                }
                break;
            }
            if (next == p.POIindicies[0]) {
                newMesh.push_back(p.lineEdges[p.POIindicies[0]]);
                break;
            }

        }
        p.lineEdges = newMesh;
        p.POIindicies = vector<int>();
        
    }
}

bool ofApp::whichSideOfLine(glm::vec3 point, glm::vec3 A, glm::vec3 B) {
    float D = (B.x - A.x) * (point.y - A.y) - (point.x - A.x) * (B.y - A.y);
    return D < 0;
}

//--------------------------------------------------------------
void ofApp::draw(){
    cam.begin(); // start camera

    for (auto& p : points) {
        //if (timer > 1000) { p->Point::draw(); continue; }
        p->draw();
        
        
        
    }
    ofSetColor(255, 255, 255);
    for (glm::vec2 e : allEdges) {
        ofDrawSphere(glm::vec3(e.x, e.y, 0), 0.5f);
    }
    /*
    ofNoFill();            // outline only
    ofSetColor(0, 255, 0); // green
    ofSetLineWidth(2);

    // Draw rectangle from (-800, -800) to (800, 800)
    ofDrawRectangle(-800, -800, 1600, 1600);
    */
    

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
