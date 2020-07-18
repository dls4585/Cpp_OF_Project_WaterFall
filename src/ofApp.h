#pragma once

#include "ofMain.h"

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
		void processOpenFileSelection(ofFileDialogResult openFileResult);
		int draw_flag = 0, water_flag = 0;
		int start_flag = 0, moving_flag = 0, line_flag = 0;
		int right_flag = 0, left_flag = 0, up_flag = 0, down_flag = 0;
		int water_dot = 0;
		int line_num, dot_num;
		double x_increase, y_increase;
		typedef struct Dot {
			double x, y;
			int L_num;
		} Dot;
		typedef struct Line {
			Dot p1;
			Dot p2;
			double gradient; //<0 up / >0 down
		} Line;
		Line* line;
		Dot* dot;
		Dot pos;
		Line w; // w.x1(y1) = start / w.x2(y2) = pos /
		vector <Dot> cross;

		int comparator(Dot left, Dot right);
		void swap(Line* l);
		int LineIntersection(Line l1, Line l2);
		int ccw(Dot p1, Dot p2, Dot p3);
		Dot crossPoint(Line l1, Line l2);
		Dot getMinX(vector<Dot> cross);
		Dot getMinY(vector<Dot> cross);
		Dot getMaxX(vector<Dot> cross);
		Dot getMaxY(vector<Dot> cross);
		double getX(double gradient, int x1, int y1, int y2);
		double getY(double gradient, int x1, int y1, int x2);
		double getG(int x1, int x2, int y1, int y2);
		void freeMemory();
};