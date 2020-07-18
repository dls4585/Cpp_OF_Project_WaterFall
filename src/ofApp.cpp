#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetBackgroundAuto(false);
	ofBackground(255); // 백그라운드의 색을 흰색으로 설정
	ofSetLogLevel(OF_LOG_VERBOSE);
	
}

//--------------------------------------------------------------
void ofApp::update(){
	if (water_flag) {
		//물이 흐르는 중
		if (moving_flag) {
			pos.x += x_increase; 
			pos.y += y_increase; //x, y의 증가량만큼 현재 점의 위치를 변화시킨다.
			if (left_flag) { //물이 왼쪽으로 향할 경우 
				if (pos.x <= w.p2.x) { //목표 지점에 도달
					moving_flag = 0; 
					left_flag = 0;
					right_flag = 0;
					up_flag = 0;
					down_flag = 0; //다시 목표 좌표를 설정할 수 있도록 모든 flag를 unset한다. 
				}
				
			}
			if (right_flag) { //물이 오른쪽으로 향할 경우
				if (pos.x >= w.p2.x) {
					moving_flag = 0;
					left_flag = 0;
					right_flag = 0;
					up_flag = 0;
					down_flag = 0; //left_flag의 경우와 동일
				}
				
			}
			if (up_flag) { //물이 위로 향할 경우
				if (pos.y <= w.p2.y) {
					moving_flag = 0;
					left_flag = 0;
					right_flag = 0;
					up_flag = 0;
					down_flag = 0; //right_flag의 경우와 동일
				}
				
			}
			if (down_flag) { //물이 아래로 향할 경우
				if (pos.y >= w.p2.y) {
					moving_flag = 0;
					left_flag = 0;
					right_flag = 0;
					up_flag = 0;
					down_flag = 0; //up_flag 의 경우와 동일
				
				}
				
			}
			if (line_flag) { //물이 선분 위에서 흐를 경우
				if (pos.y >= w.p2.y) { //목표 지점에 도달
					cross.resize(0); //벡터의 크기를 초기화함.(이전 기록을 지워준다)
					w.gradient = 0; //수직낙하하므로 기울기는 0
					w.p1 = pos; //현재 점으로 시작점을 변경
					w.p1.y++; //현재 선분과 교차점을 구하는 것을 방지하기 위해 y좌표를 1증가시킴
					w.p2.x = w.p1.x; 
					w.p2.y = ofGetHeight(); //시작점에서 수직으로 내려오는 선분으로 목표점을 설정

					//시작점-목표점을 잇는 선분 사이에 교차하는 선분이 있는지 확인
					for (int i = 0; i < line_num; i++)
						if (LineIntersection(w, line[i]))
							crossPoint(w, line[i]);
					//교차하는 선분이 있을 경우 가장 가까운 교차점을 새로운 목표점으로 설정한다.
					if (cross.size() != 0) {
						w.p2 = getMinY(cross);
					}
					pos.x = w.p1.x; 
					pos.y = w.p1.y; //현재 점을 시작점으로 설정
					x_increase = (w.p2.x - w.p1.x) / 1000.0; 
					y_increase = (w.p2.y - w.p1.y) / 1000.0; //x, y 증가량 설정
					down_flag = 1; // 아래로 흐르는 것을 의미
					line_flag = 0; // 선분 위에서 흐르지 않음
					moving_flag = 1; // 물이 다시 흐르기 시작
				}
			}
		}
		//목표 점에 도달
		else { 
			// 왼쪽 벽을 만났을 때
			if (pos.x <= 0) {
				cross.resize(0); //cross 컨테이너의 크기를 0으로 초기화 (이전 기록을 지워준다)
				// 기울기 재설정
				if (w.gradient == 0) { // 수평으로 물이 향하고 있었을 경우
					int degree = ofRandom(-75,75); // -75~75도 사이의 각도를 임의로 설정
					w.gradient = tan(degree * (PI / 180)); // 해당 각도를 갖는 선분의 기울기를 설정
				}
				else { //수평이 아니었을 경우
					w.gradient *= -1; //반대로 향하도록 설정
				}
				w.p1 = w.p2; // 목표점을 시작점으로 재설정
				// 기울기별 목표 좌표
				if (w.gradient < 0) { // 선분이 우상향일 때
					if (w.gradient < getG(w.p1.x, ofGetWidth(), w.p1.y, 0)) { // 오른쪽 상단의 모서리를 잇는 선분보다 위의 영역에 속하는 기울기의 경우
						w.p2.y = 0;
						w.p2.x = getX(w.gradient, w.p1.x, w.p1.y, w.p2.y); //목표점을 설정
						up_flag = 1;
						right_flag = 1; //우상향 flag
					}
					else if (w.gradient == getG(w.p1.x, ofGetWidth(), w.p1.y, 0)) { // 오른쪽 상단의 모서리를 잇는 선분과 동일한 경우
						w.p2.y = 0;
						w.p2.x = ofGetWidth();
						up_flag = 1;
						right_flag = 1;
					}
					else if (w.gradient > getG(w.p1.x, ofGetWidth(), w.p1.y, 0)) { //오른쪽 상단의 모서리를 잇는 선분보다 아래의 영역에 속하는 기울기의 경우
						w.p2.x = ofGetWidth();
						w.p2.y = getY(w.gradient, w.p1.x, w.p1.y, w.p2.x);
						up_flag = 1;
						right_flag = 1;
					}
				}
				else if (w.gradient > 0) { //선분이 우하향일 때
					if (w.gradient > getG(w.p1.x, ofGetWidth(), w.p1.y, ofGetHeight())) { // 오른쪽 하단의 모서리를 잇는 선분보다 아래의 영역에 속하는 기울기의 경우
						w.p2.y = ofGetHeight();
						w.p2.x = getX(w.gradient, w.p1.x, w.p1.y, w.p2.y);
						down_flag = 1;
						right_flag = 1;
					}
					else if (w.gradient == getG(w.p1.x, ofGetWidth(), w.p1.y, ofGetHeight())) { //오른쪽 하단의 모서리를 잇는 선부과 동일한 경우
						w.p2.x = ofGetWidth();
						w.p2.y = ofGetHeight();
						down_flag = 1;
						right_flag = 1;
					}
					else if (w.gradient < getG(w.p1.x, ofGetWidth(), w.p1.y, ofGetHeight())) { // 오른쪽 하단의 모서리를 잇는 선분보다 위의 영역에 속하는 기울기의 경우
						w.p2.x = ofGetWidth();
						w.p2.y = getY(w.gradient, w.p1.x, w.p1.y, w.p2.x);
					}
				}

				for (int i = 0; i < line_num; i++)
					if (LineIntersection(w, line[i]))
						crossPoint(w, line[i]);
				if (cross.size() != 0) {
					w.p2 = getMinX(cross);
					
				}
				x_increase = (w.p2.x - w.p1.x) / 1000.0;
				y_increase = (w.p2.y - w.p1.y) / 1000.0;
				pos.x = w.p1.x;
				pos.y = w.p1.y;
				
			} 
			//오른쪽 벽을 만났을 때
			else if (pos.x >= ofGetWidth()) {
				cross.resize(0); //cross 컨테이너의 크기를 0으로 초기화(이전기록을 비워준다.)
				if (w.gradient == 0) {
					int degree = ofRandom(-75,75);
					w.gradient = tan(degree * (PI / 180));
				}
				else {
					w.gradient *= -1;
				}
				w.p1 = w.p2;
				// 기울기별 목표 좌표
				if (w.gradient > 0) { // 좌상향의 경우
					if (w.gradient > getG(w.p1.x, 0, w.p1.y, 0)) { // 왼쪽 상단의 모서리를 잇는 선분의 기울기보다 위의 영역에 있는 기울기의 경우
						w.p2.y = 0;
						w.p2.x = getX(w.gradient, w.p1.x, w.p1.y, w.p2.y);
						up_flag = 1;
						left_flag = 1;
					}
					else if (w.gradient == getG(w.p1.x, 0, w.p1.y, 0)) { // 왼쪽 상단의 모서리를 잇는 선분의 기울기와 같은 경우
						w.p2.y = 0;
						w.p2.x = 0;
						up_flag = 1;
						left_flag = 1;
					}
					else if (w.gradient < getG(w.p1.x, 0, w.p1.y, 0)) { // 왼쪽 상단의 모서리를 잇는 선분의 기울기보다 아래의 영역에 있는 기울기의 경우
						w.p2.x = 0;
						w.p2.y = getY(w.gradient, w.p1.x, w.p1.y, w.p2.x);
						up_flag = 1;
						left_flag = 1;
					}
				}
				else if (w.gradient < 0) { // 좌하향의 경우
					if (w.gradient < getG(w.p1.x, 0, w.p1.y, ofGetHeight())) { // 왼쪽 하단의 모서리를 잇는 선분의 기울기보다 아래의 영역에 있는 기울기의 경우
						w.p2.y = ofGetHeight();
						w.p2.x = getX(w.gradient, w.p1.x, w.p1.y, w.p2.y);
						down_flag = 1;
						left_flag = 1;
					}
					else if (w.gradient == getG(w.p1.x, 0, w.p1.y, ofGetHeight())) { // 왼쪽 하단의 모서리를 잇는 선분의 기울기와 같은 경우
						w.p2.x = 0;
						w.p2.y = ofGetHeight();
						down_flag = 1;
						left_flag = 1;
					}
					else if (w.gradient > getG(w.p1.x, 0, w.p1.y, ofGetHeight())) { // 왼쪽 하단의 모서리를 잇는 선분의 기울기보다 위의 영역에 있는 기울기의 경우
						w.p2.x = 0;
						w.p2.y = getY(w.gradient, w.p1.x, w.p1.y, w.p2.x);
						down_flag = 1;
						left_flag = 1;
					}
				}
			
				for (int i = 0; i < line_num; i++)
					if (LineIntersection(w, line[i]))
						crossPoint(w, line[i]);
				if (cross.size() != 0) {
					w.p2 = getMaxX(cross);
					
				}
				x_increase = (w.p2.x - w.p1.x) / 1000.0;
				y_increase = (w.p2.y - w.p1.y) / 1000.0;
				pos.x = w.p1.x;
				pos.y = w.p1.y;
				
			}
			//아래 벽을 만났을 때
			else if (pos.y >= ofGetHeight()) {
				cross.resize(0);  // cross 컨테이너의 크기를 0으로 초기화(이전기록을 비워준다.)
				if (w.gradient == 0) {
					int degree = ofRandom(-75,75);
					w.gradient = tan(degree * (PI / 180));
				}
				else {
					w.gradient *= -1;
				}
				w.p1 = w.p2;
				// 기울기별 목표 좌표
				if (w.gradient > 0) { // 좌상향의 경우
					if (w.gradient < getG(w.p1.x, 0, w.p1.y, 0)) { // 왼쪽 상단의 모서리를 잇는 선분의 기울기보다 아래의 영역에 있는 경우 
						w.p2.x = 0;
						w.p2.y = getY(w.gradient, w.p1.x, w.p1.y, w.p2.x);
						left_flag = 1;
						up_flag = 1;
					}
					else if (w.gradient == getG(w.p1.x, 0, w.p1.y, 0)) { // 왼쪽 상단의 모서리를 잇는 선분의 기울기와 같은 경우
						w.p2.x = 0;
						w.p2.y = 0;
						left_flag = 1;
						up_flag = 1;
					}
					else if (w.gradient > getG(w.p1.x, 0, w.p1.y, 0)) { // 왼쪽 상단의 모서리를 잇는 선분의 기울기보다 위의 영역에 있는 경우 
						w.p2.y = 0;
						w.p2.x = getX(w.gradient, w.p1.x, w.p1.y, w.p2.y);
						left_flag = 1;
						up_flag = 1;
					}
				
				}
				else if (w.gradient < 0) { 
					if (w.gradient > getG(w.p1.x, ofGetWidth(), w.p1.y, 0)) { // 오른쪽 상단의 모서리를 잇는 선분의 기울기보다 아래의 영역에 있는 경우 
						w.p2.x = ofGetWidth();
						w.p2.y = getY(w.gradient, w.p1.x, w.p1.y, w.p2.x);
						right_flag = 1;
						up_flag = 1;
					}

					else if (w.gradient == getG(w.p1.x, ofGetWidth(), w.p1.y, 0)) { // 오른쪽 상단의 모서리를 잇는 선분의 기울기와 같은 경우
						w.p2.x = ofGetWidth();
						w.p2.y = ofGetHeight();
						right_flag = 1;
						up_flag = 1;
					}
					else if (w.gradient < getG(w.p1.x, ofGetWidth(), w.p1.y, 0)) { // 오른쪽 상단의 모서리를 잇는 선분의 기울기보다 위의 영역에 있는 경우 
						w.p2.y = 0;
						w.p2.x = getX(w.gradient, w.p1.x, w.p1.y, w.p2.y);
						right_flag = 1;
						up_flag = 1;
					}
				}
				for (int i = 0; i < line_num; i++) 
					if (LineIntersection(w, line[i]))
						crossPoint(w, line[i]);
				if (cross.size() != 0) {
					w.p2 = getMaxY(cross);
					
				}
				x_increase = (w.p2.x - w.p1.x) / 1000.0;
				y_increase = (w.p2.y - w.p1.y) / 1000.0;
				pos.x = w.p1.x;
				pos.y = w.p1.y;
				
			}
			//위 벽을 만났을 때
			else if (pos.y <= 0) {
				cross.resize(0); // cross 컨테이너의 크기를 0으로 초기화(이전기록을 비워준다.)
				if (w.gradient == 0) {
					int degree = ofRandom(-75,75);
					w.gradient = tan(degree * (PI / 180));
				}
				else {
					w.gradient *= -1;
				}
				w.p1 = w.p2;
				// 기울기별 목표 좌표
				if (w.gradient < 0) { // 좌하향의 경우
					if (w.gradient > getG(w.p1.x, 0, w.p1.y, ofGetHeight())) { // 왼쪽 하단의 모서리를 잇는 선분의 기울기보다 위의 영역에 있는 경우
						w.p2.x = 0;
						w.p2.y = getY(w.gradient, w.p1.x, w.p1.y, w.p2.x);
						left_flag = 1;
						down_flag = 1;
					}
					else if (w.gradient == getG(w.p1.x, 0, w.p1.y, ofGetHeight())) { // 왼쪽 하단의 모서리를 잇는 선분의 기울기와 같은 경우
						w.p2.x = 0;
						w.p2.y = ofGetHeight();
						left_flag = 1;
						down_flag = 1;
					}
					else if (w.gradient < getG(w.p1.x, 0, w.p1.y, ofGetHeight())) { // 왼쪽 하단의 모서리를 잇는 선분의 기울기보다 아래의 영역에 있는 경우
						w.p2.y = ofGetHeight();
						w.p2.x = getX(w.gradient, w.p1.x, w.p1.y, w.p2.y);
						left_flag = 1;
						down_flag = 1;
					}
				}
				else if (w.gradient > 0) {
					if (w.gradient < getG(w.p1.x, ofGetWidth(), w.p1.y, ofGetHeight())) { // 오른쪽 하단의 모서리를 잇는 선분의 기울기보다 위의 영역에 있는 경우
						w.p2.x = ofGetWidth();
						w.p2.y = getY(w.gradient, w.p1.x, w.p1.y, w.p2.x);
						right_flag = 1;
						down_flag = 1;
					}
					else if (w.gradient == getG(w.p1.x, ofGetWidth(), w.p1.y, ofGetHeight())) { // 오른쪽 하단의 모서리를 잇는 선분의 기울기와 같은 경우
						w.p2.y = ofGetHeight();
						w.p2.x = ofGetWidth();
						right_flag = 1;
						down_flag = 1;
					}
					else if (w.gradient > getG(w.p1.x, ofGetWidth(), w.p1.y, ofGetHeight())) { // 오른쪽 하단의 모서리를 잇는 선분의 기울기보다 아래의 영역에 있는 경우
						w.p2.y = ofGetHeight();
						w.p2.x = getX(w.gradient, w.p1.x, w.p1.y, w.p2.y);
						right_flag = 1;
						down_flag = 1;
					}
				}
				
				for (int i = 0; i < line_num; i++)
					if (LineIntersection(w, line[i]))
						crossPoint(w, line[i]);
				if (cross.size() != 0) {
					w.p2 = getMinY(cross);
					
				}
				x_increase = (w.p2.x - w.p1.x) / 1000.0;
				y_increase = (w.p2.y - w.p1.y) / 1000.0;
				pos.x = w.p1.x;
				pos.y = w.p1.y;
				
			}
			//선분을 만났을 때
			else { 
				cross.resize(0); // cross 컨테이너의 크기를 0으로 초기화(이전기록을 비워준다.)
				w.p1 = w.p2;
				if (line[w.p2.L_num].p1.y > line[w.p2.L_num].p2.y) { // 선분이 왼쪽으로 기울어져 있는 경우
					w.p2 = line[w.p2.L_num].p1;
				}
				else { // 선분이 오른쪽으로 기울어져 있는 경우
					w.p2 = line[w.p2.L_num].p2;
				}
				x_increase = (w.p2.x - w.p1.x) / 1000.0;
				y_increase = (w.p2.y - w.p1.y) / 1000.0;
				pos.x = w.p1.x;
				pos.y = w.p1.y;
				line_flag = 1;
				
			}
			moving_flag = 1; // 물이 목표점으로 다시 흐르도록 설정
			
		}
		
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	if (draw_flag) {
		ofSetColor(0); //기본색은 검은색 
		ofFill(); //색 채우기
		ofSetLineWidth(5); // 선분의 굵기를 5로 설정
		for (int i = 0; i < line_num; i++) {
			ofDrawLine(line[i].p1.x, line[i].p1.y, line[i].p2.x, line[i].p2.y);
		}

		for (int i = 0; i < dot_num; i++) { 
			if (i == water_dot) { // 물이 나오는 지점은 맨 왼쪽으로 초기화
				 //물이 나오는 지점의 인덱스 값을 기억한다.

				ofSetColor(255, 0, 0); //물이나오는 지점의 색은 빨간색
			}
			else {
				ofSetColor(0x000000); //다른 지점은 검은색
			}
			ofDrawCircle(dot[i].x,dot[i].y, 10); //해당 좌표의 원을 그린다
		}
		draw_flag = 0;
	}
	if (water_flag) {
		ofSetColor(ofColor::blue);
		ofFill();
		ofSetLineWidth(4);
		ofDrawLine(w.p1.x, w.p1.y, pos.x, pos.y); //시작점부터 현재점까지를 잇는 선분을 그린다.

	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == 'd' || key == 'D') { // d 키를 눌렀을 때
		draw_flag = 1; // draw_flag를 set하고
		draw(); // draw함수를 호출한다.
	}
	if (key == 's' || key == 'S') {
		start_flag = 1; //start_flag 를 set한다.
		ofSetColor(ofColor::black);
		//왼쪽 화살표
		ofDrawTriangle(dot[water_dot].x - 50, dot[water_dot].y,
			dot[water_dot].x - 30, dot[water_dot].y - 10,
			dot[water_dot].x - 30, dot[water_dot].y + 10);
		ofSetLineWidth(5);
		ofDrawLine(dot[water_dot].x - 30, dot[water_dot].y,
			dot[water_dot].x - 15, dot[water_dot].y);
		//오른쪽 화살표
		ofDrawTriangle(dot[water_dot].x + 50, dot[water_dot].y,
			dot[water_dot].x + 30, dot[water_dot].y - 10,
			dot[water_dot].x + 30, dot[water_dot].y + 10);
		ofSetLineWidth(5);
		ofDrawLine(dot[water_dot].x + 30, dot[water_dot].y,
			dot[water_dot].x + 15, dot[water_dot].y);
		//위 화살표
		ofDrawTriangle(dot[water_dot].x, dot[water_dot].y - 50,
			dot[water_dot].x - 10, dot[water_dot].y - 30,
			dot[water_dot].x + 10, dot[water_dot].y - 30);
		ofSetLineWidth(5);
		ofDrawLine(dot[water_dot].x, dot[water_dot].y - 30,
			dot[water_dot].x, dot[water_dot].y - 15);
		//아래 화살표
		ofDrawTriangle(dot[water_dot].x, dot[water_dot].y + 50,
			dot[water_dot].x - 10, dot[water_dot].y + 30,
			dot[water_dot].x + 10, dot[water_dot].y + 30);
		ofSetLineWidth(5);
		ofDrawLine(dot[water_dot].x, dot[water_dot].y + 30,
			dot[water_dot].x, dot[water_dot].y + 15);
	}
	if (key == 'e' || key == 'E') { 
		water_flag = 0; //water_flag 를 0으로 unset해 물의 흐름을 중지.
	}
	if (key == OF_KEY_LEFT) {
		if (!start_flag) { // 방향 선택을 하기 이전, 점을 선택할 수 있는 단계
			ofFill();
			ofSetColor(0x000000);
			ofDrawCircle(dot[water_dot].x, dot[water_dot].y, 10);
			water_dot--; //점을 저장한 배열의 인덱스를 감소시킨다.
			if (water_dot < 0) // 맨 왼쪽의 점에서 다시 왼쪽으로 이동한 경우, 가장 우측의 점으로 이동
				water_dot = dot_num - 1;

			ofSetColor(255, 0, 0);
			ofDrawCircle(dot[water_dot].x, dot[water_dot].y, 10);
			cout << "Selected Dot coordinate is (" << dot[water_dot].x
				<< ", " << dot[water_dot].y << ")" << endl;
		}
		else {
			// 화살표 지우기
			if (!water_flag) {
				water_flag = 1;
				ofSetColor(ofColor::white);
				ofDrawTriangle(dot[water_dot].x - 50, dot[water_dot].y,
					dot[water_dot].x - 30, dot[water_dot].y - 10,
					dot[water_dot].x - 30, dot[water_dot].y + 10);
				ofSetLineWidth(5);
				ofDrawLine(dot[water_dot].x - 30, dot[water_dot].y,
					dot[water_dot].x - 15, dot[water_dot].y);
				//오른쪽 화살표
				ofDrawTriangle(dot[water_dot].x + 50, dot[water_dot].y,
					dot[water_dot].x + 30, dot[water_dot].y - 10,
					dot[water_dot].x + 30, dot[water_dot].y + 10);
				ofSetLineWidth(5);
				ofDrawLine(dot[water_dot].x + 30, dot[water_dot].y,
					dot[water_dot].x + 15, dot[water_dot].y);
				//위 화살표
				ofDrawTriangle(dot[water_dot].x, dot[water_dot].y - 50,
					dot[water_dot].x - 10, dot[water_dot].y - 30,
					dot[water_dot].x + 10, dot[water_dot].y - 30);
				ofSetLineWidth(5);
				ofDrawLine(dot[water_dot].x, dot[water_dot].y - 30,
					dot[water_dot].x, dot[water_dot].y - 15);
				//아래 화살표
				ofDrawTriangle(dot[water_dot].x, dot[water_dot].y + 50,
					dot[water_dot].x - 10, dot[water_dot].y + 30,
					dot[water_dot].x + 10, dot[water_dot].y + 30);
				ofSetLineWidth(5);
				ofDrawLine(dot[water_dot].x, dot[water_dot].y + 30,
					dot[water_dot].x, dot[water_dot].y + 15);
			}
			if (!moving_flag) { //맨 처음 방향을 선택한 경우
				left_flag = 1;
				w.p1.x = pos.x = dot[water_dot].x; 
				w.p1.y = pos.y = dot[water_dot].y; //시작점을 물이 나오는 점의 좌표로 설정
				w.p2.x = 0;
				w.p2.y = dot[water_dot].y; //목표점을 왼쪽벽으로 설정

				//시작점-목표점을 잇는 선분 사이에 교차하는 선분이 있는지 확인
				for (int i = 0; i < line_num; i++) 
					if (LineIntersection(w, line[i]))
						crossPoint(w, line[i]);
				//교차하는 선분이 있을 경우 가장 가까운 교차점을 새로운 목표점으로 설정한다.
				if (cross.size() != 0) {
					w.p2 = getMaxX(cross); //<- 방향이기 때문에 x좌표가 가장 큰 값을 선택
				}
				x_increase = (w.p2.x - w.p1.x) / 1000.0; 
				y_increase = (w.p2.y - w.p1.y) / 1000.0; //x,y 축의 증가량
				w.gradient = 0; 
				moving_flag = 1; //물이 흐를 수 있도록 moving_flag 를 set
			}
		}
		
	}
	if (key == OF_KEY_RIGHT) {
		if (!start_flag) {// 방향 선택을 하기 이전, 점을 선택할 수 있는 단계
			ofFill();
			ofSetColor(0x000000);
			ofDrawCircle(dot[water_dot].x, dot[water_dot].y, 10);
			water_dot++; //점을 저장한 배열의 인덱스를 증가시킨다.
			if (water_dot == dot_num) // 맨 오른쪽 점에서 다시 오른쪽으로 이동한 경우, 가장 좌측의 점으로 이동
				water_dot = 0;
			ofSetColor(255, 0, 0);
			ofDrawCircle(dot[water_dot].x, dot[water_dot].y, 10);
			cout << "Selected Dot coordinate is (" << dot[water_dot].x
				<< ", " << dot[water_dot].y << ")" << endl;
		}
		else {
			// 화살표 지우기
			if (!water_flag) {
				water_flag = 1;
				ofSetColor(ofColor::white);
				//왼쪽 화살표
				ofDrawTriangle(dot[water_dot].x - 50, dot[water_dot].y,
					dot[water_dot].x - 30, dot[water_dot].y - 10,
					dot[water_dot].x - 30, dot[water_dot].y + 10);
				ofSetLineWidth(5);
				ofDrawLine(dot[water_dot].x - 30, dot[water_dot].y,
					dot[water_dot].x - 15, dot[water_dot].y);
				//오른쪽 화살표
				ofDrawTriangle(dot[water_dot].x + 50, dot[water_dot].y,
					dot[water_dot].x + 30, dot[water_dot].y - 10,
					dot[water_dot].x + 30, dot[water_dot].y + 10);
				ofSetLineWidth(5);
				ofDrawLine(dot[water_dot].x + 30, dot[water_dot].y,
					dot[water_dot].x + 15, dot[water_dot].y);
				//위 화살표
				ofDrawTriangle(dot[water_dot].x, dot[water_dot].y - 50,
					dot[water_dot].x - 10, dot[water_dot].y - 30,
					dot[water_dot].x + 10, dot[water_dot].y - 30);
				ofSetLineWidth(5);
				ofDrawLine(dot[water_dot].x, dot[water_dot].y - 30,
					dot[water_dot].x, dot[water_dot].y - 15);
				//아래 화살표
				ofDrawTriangle(dot[water_dot].x, dot[water_dot].y + 50,
					dot[water_dot].x - 10, dot[water_dot].y + 30,
					dot[water_dot].x + 10, dot[water_dot].y + 30);
				ofSetLineWidth(5);
				ofDrawLine(dot[water_dot].x, dot[water_dot].y + 30,
					dot[water_dot].x, dot[water_dot].y + 15);
			}
			if (!moving_flag) {
				right_flag = 1;
				w.p1.x = pos.x = dot[water_dot].x;
				w.p1.y = pos.y = dot[water_dot].y; //시작점을 물이 나오는 점의 좌표로 설정
				w.p2.x = ofGetWidth();
				w.p2.y = dot[water_dot].y; //목표점을 오른쪽벽으로 설정

				//시작점-목표점을 잇는 선분 사이에 교차하는 선분이 있는지 확인
				for (int i = 0; i < line_num; i++)
					if (LineIntersection(w, line[i]))
						crossPoint(w, line[i]);
				//교차하는 선분이 있을 경우 가장 가까운 교차점을 새로운 목표점으로 설정한다.
				if (cross.size() != 0) {
					w.p2 = getMinX(cross); //-> 방향이기 때문에 x좌표가 가장 작은 값을 선택
				}
				x_increase = (w.p2.x - w.p1.x) / 1000.0;
				y_increase = (w.p2.y - w.p1.y) / 1000.0;
				w.gradient = 0;
				moving_flag = 1; //물이 흐를 수 있도록 moving_flag 를 set
			}
		}
	}
	if (key == OF_KEY_UP) {
		if (start_flag) {
			// 화살표 지우기
			if (!water_flag) {
				water_flag = 1;
				ofSetColor(ofColor::white);
				ofDrawTriangle(dot[water_dot].x - 50, dot[water_dot].y,
					dot[water_dot].x - 30, dot[water_dot].y - 10,
					dot[water_dot].x - 30, dot[water_dot].y + 10);
				ofSetLineWidth(5);
				ofDrawLine(dot[water_dot].x - 30, dot[water_dot].y,
					dot[water_dot].x - 15, dot[water_dot].y);
				//오른쪽 화살표
				ofDrawTriangle(dot[water_dot].x + 50, dot[water_dot].y,
					dot[water_dot].x + 30, dot[water_dot].y - 10,
					dot[water_dot].x + 30, dot[water_dot].y + 10);
				ofSetLineWidth(5);
				ofDrawLine(dot[water_dot].x + 30, dot[water_dot].y,
					dot[water_dot].x + 15, dot[water_dot].y);
				//위 화살표
				ofDrawTriangle(dot[water_dot].x, dot[water_dot].y - 50,
					dot[water_dot].x - 10, dot[water_dot].y - 30,
					dot[water_dot].x + 10, dot[water_dot].y - 30);
				ofSetLineWidth(5);
				ofDrawLine(dot[water_dot].x, dot[water_dot].y - 30,
					dot[water_dot].x, dot[water_dot].y - 15);
				//아래 화살표
				ofDrawTriangle(dot[water_dot].x, dot[water_dot].y + 50,
					dot[water_dot].x - 10, dot[water_dot].y + 30,
					dot[water_dot].x + 10, dot[water_dot].y + 30);
				ofSetLineWidth(5);
				ofDrawLine(dot[water_dot].x, dot[water_dot].y + 30,
					dot[water_dot].x, dot[water_dot].y + 15);
			}
			if (!moving_flag) {
				up_flag = 1;
				w.p1.x = pos.x = dot[water_dot].x;
				w.p1.y = pos.y = dot[water_dot].y; //시작점을 물이 나오는 점의 좌표로 설정
				w.p2.x = dot[water_dot].x;
				w.p2.y = 0;						   //목표점을 위쪽벽으로 설정

				//시작점-목표점을 잇는 선분 사이에 교차하는 선분이 있는지 확인
				for (int i = 0; i < line_num; i++)
					if (LineIntersection(w, line[i]))
						crossPoint(w, line[i]);
				//교차하는 선분이 있을 경우 가장 가까운 교차점을 새로운 목표점으로 설정한다.
				if (cross.size() != 0)
					w.p2 = getMaxY(cross); //위 방향이기 때문에 y좌표가 가장 큰 값을 선택
				x_increase = (w.p2.x - w.p1.x) / 1000.0;
				y_increase = (w.p2.y - w.p1.y) / 1000.0;
				w.gradient = 0;
				moving_flag = 1; //물이 흐를 수 있도록 moving_flag 를 set
			}
		}
	}
	if (key == OF_KEY_DOWN) {
			if (start_flag) {
				// 화살표 지우기
				if (!water_flag) {
					water_flag = 1;
					ofSetColor(ofColor::white);
					ofDrawTriangle(dot[water_dot].x - 50, dot[water_dot].y,
						dot[water_dot].x - 30, dot[water_dot].y - 10,
						dot[water_dot].x - 30, dot[water_dot].y + 10);
					ofSetLineWidth(5);
					ofDrawLine(dot[water_dot].x - 30, dot[water_dot].y,
						dot[water_dot].x - 15, dot[water_dot].y);
					//오른쪽 화살표
					ofDrawTriangle(dot[water_dot].x + 50, dot[water_dot].y,
						dot[water_dot].x + 30, dot[water_dot].y - 10,
						dot[water_dot].x + 30, dot[water_dot].y + 10);
					ofSetLineWidth(5);
					ofDrawLine(dot[water_dot].x + 30, dot[water_dot].y,
						dot[water_dot].x + 15, dot[water_dot].y);
					//위 화살표
					ofDrawTriangle(dot[water_dot].x, dot[water_dot].y - 50,
						dot[water_dot].x - 10, dot[water_dot].y - 30,
						dot[water_dot].x + 10, dot[water_dot].y - 30);
					ofSetLineWidth(5);
					ofDrawLine(dot[water_dot].x, dot[water_dot].y - 30,
						dot[water_dot].x, dot[water_dot].y - 15);
					//아래 화살표
					ofDrawTriangle(dot[water_dot].x, dot[water_dot].y + 50,
						dot[water_dot].x - 10, dot[water_dot].y + 30,
						dot[water_dot].x + 10, dot[water_dot].y + 30);
					ofSetLineWidth(5);
					ofDrawLine(dot[water_dot].x, dot[water_dot].y + 30,
						dot[water_dot].x, dot[water_dot].y + 15);
				}
				if (!moving_flag) {
					down_flag = 1;
					w.p1.x = pos.x = dot[water_dot].x;
					w.p1.y = pos.y = dot[water_dot].y; //시작점을 물이 나오는 점의 좌표로 설정
					w.p2.x = dot[water_dot].x;
					w.p2.y = ofGetHeight();			   //목표점을 위쪽벽으로 설정

					//시작점-목표점을 잇는 선분 사이에 교차하는 선분이 있는지 확인
					for (int i = 0; i < line_num; i++)
						if (LineIntersection(w, line[i]))
							crossPoint(w, line[i]);
					//교차하는 선분이 있을 경우 가장 가까운 교차점을 새로운 목표점으로 설정한다.
					if (cross.size() != 0)
						w.p2 = getMinY(cross); //아래 방향이기 때문에 y좌표가 가장 작은 값을 선택
					x_increase = (w.p2.x - w.p1.x) / 1000.0;
					y_increase = (w.p2.y - w.p1.y) / 1000.0;
					w.gradient = 0;
					moving_flag = 1; //물이 흐를 수 있도록 moving_flag 를 set
				}
			}
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if (key == 'l' || key == 'L') { // 다이얼로그 창을 띄운다.

		//Open the Open File Dialog
		ofFileDialogResult openFileResult = ofSystemLoadDialog("Select an input file");

		//Check if the user opened a file
		if (openFileResult.bSuccess) {

			ofLogVerbose("User selected a file");

			//We have a file, check it and process it
			processOpenFileSelection(openFileResult); // 파일로부터 값 입력받기 

		}
		else {
			ofLogVerbose("User hit cancel");
		}
	}
	if (key == 'q' || key == 'Q') { //할당된 메모리를 해제하며 프로그램 종료
		if (!water_flag) {
			freeMemory();
			cout << "Memories are freed" << endl;
			std::exit(0); 
		}
	}
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

void ofApp::processOpenFileSelection(ofFileDialogResult openFileResult) {
	//콘솔 로그에 파일 이름과 경로를 출력한다.
	ofLogVerbose("getName(): " + openFileResult.getName()); 
	ofLogVerbose("getPath(): " + openFileResult.getPath());
	
	ifstream file;
	file.open(openFileResult.getPath());

	if (!file.is_open()) 
		cout << "file open error" << endl;
	else 
		cout << "we found the target" << endl;

	int line_or_dot = 0;
	int line_count;
	string _line;
	int n=0,m = 0;
	//파일의 끝까지 파일의 정보를 받아 자료구조에 저장하는 반복문
	while (!file.eof()) {
		//파일로부터 한 줄씩 읽어와 문자열에 저장한 후 띄어쓰기를 기준으로 분리해 저장
		getline(file, _line);
		vector <string> words = ofSplitString(_line, " ");

		if (words.size() == 1) { // 선분의 개수 또는 점의 개수
			if (line_or_dot == 0) { // 0이면 선분, 1이면 점
				line_num = atoi(words[0].c_str());
				line = (Line*)malloc(sizeof(Line)*line_num); // 입력되는 선분의 개수만큼 동적할당
				line_count = line_num; 
				cout << "The number of line is: " << line_num << endl;
			}
			else {
				dot_num = atoi(words[0].c_str());
				dot = new Dot[dot_num]; //입력되는 점의 개수만큼 동적할당
				cout << "The number of dot is: " << dot_num << endl;
			}
		}
		else if (words.size() > 1) {
			if (line_or_dot == 0) {
				line[n].p1.L_num = n;
				line[n].p2.L_num = n;
				line[n].p1.x = atoi(words[0].c_str());
				line[n].p1.y = atoi(words[1].c_str());
				line[n].p2.x = atoi(words[2].c_str());
				line[n].p2.y = atoi(words[3].c_str());
				line[n].gradient = (line[n].p2.y - line[n].p1.y) / (line[n].p2.x - line[n++].p1.x);
				line_count--;
			}
			else {
				dot[m].x = atoi(words[0].c_str());
				dot[m++].y = atoi(words[1].c_str());
			}
		}
		if (line_count == 0) // line_count 가 0이되면 선분의 정보에 대한 입력이 끝났음을 의미 -> 점에 대한 정보를 입력받을 차례
			line_or_dot = 1;
	}


}


//선분의 right의 x좌표가 left의 x좌표보다 크면 True / 작으면 False를 반환
int ofApp:: comparator(Dot left, Dot right) {
	int ret;
	if (left.x == right.x) {
		ret = (left.y <= right.y);
	}
	else {
		ret = (left.x <= right.x);
	}
	return ret;
}

//right의 x좌표가 더 작은 경우 둘을 바꿔준다.
void ofApp:: swap(Line* l) {
	Dot temp;
	temp = l->p1;
	l->p1 = l->p2;
	l->p2 = temp;
}

//두 선분의 교차 여부를 확인한다.
int ofApp:: LineIntersection(Line l1, Line l2) {
	int ret;
	// l1을 기준으로 l2가 교차하는 지 확인한다.

	int l1_l2 = ccw(l1.p1, l1.p2, l2.p1) * ccw(l1.p1, l1.p2, l2.p2);
	// l2를 기준으로 l1이 교차하는 지 확인한다.

	int l2_l1 = ccw(l2.p1, l2.p2, l1.p1) * ccw(l2.p1, l2.p2, l1.p2);


	// l1 과 l2가 일직선 상에 있는 경우
	if (l1_l2 == 0 && l2_l1 == 0) {
		// Line1의 점의 크기 순서를 p1 < p2 순서로 맞춘다.
		if (comparator(l1.p2, l1.p1)) swap(&l1);
		// Line2의 점의 크기 순서를 p1 < p2 순서로 맞춘다.
		if (comparator(l2.p2, l2.p1)) swap(&l2);

		// l1.p1 -----------l1.p2
		// l2.p1 -----------l2.p2
		// 위 조건을 만족하는 지 살펴본다.
		ret = (comparator(l2.p1, l1.p2)) && (comparator(l1.p1, l2.p2));
	}
	// l1과 l2가 일직선 상에 있지 않는 경우
	else {
		ret = (l1_l2 <= 0) && (l2_l1 <= 0);
	}
	return ret; //return 1 ->  교차 return 0 -> 교차 x
}

//ccw 알고리즘을 구현한 함수
int ofApp:: ccw(Dot p1, Dot p2, Dot p3) {
	int cross_product = (p2.x - p1.x) * (p3.y - p1.y) - (p3.x - p1.x) * (p2.y - p1.y);

	if (cross_product > 0) {
		return 1;
	}
	else if (cross_product < 0) { 
		return -1;
	}
	else {
		return 0;
	}
}

//두 선분의 교점을 구하는 함수
ofApp::Dot ofApp::crossPoint(Line l1, Line l2) {
	int x1, x2, x3, x4, y1, y2, y3, y4;
	Dot temp;
	x1 = l1.p1.x;
	x2 = l1.p2.x;
	x3 = l2.p1.x;
	x4 = l2.p2.x;
	y1 = l1.p1.y;
	y2 = l1.p2.y;
	y3 = l2.p1.y;
	y4 = l2.p2.y;


	temp.x = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) / ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));
	temp.y = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) / ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));
	temp.L_num = l2.p1.L_num;
	//해당 교차점을 cross 컨테이너에 push_back 하며 반환
	cross.push_back(temp);
	return temp;
}

//vector cross 컨테이너 내의 x좌표 값이 가장 작은 점을 반환한다.
ofApp::Dot ofApp:: getMinX(vector<Dot> cross) {
	Dot min = cross[0];
	for (int i = 1; i < cross.size(); i++) {
		if (cross[i].x < min.x) min = cross[i];
	}
	return min;
}
//vector cross 컨테이너 내의 y좌표 값이 가장 작은 점을 반환한다.
ofApp::Dot ofApp::getMinY(vector<Dot> cross) {
	Dot min = cross[0];
	for (int i = 1; i < cross.size(); i++) {
		if (cross[i].y < min.y) min = cross[i];
	}
	return min;
}
//vector cross 컨테이너 내의 x좌표 값이 가장 큰 점을 반환한다.
ofApp::Dot ofApp::getMaxX(vector<Dot> cross) {
	Dot max = cross[0];
	for (int i = 1; i < cross.size(); i++) {
		if (cross[i].x > max.x) max = cross[i];
	}
	return max;
}
//vector cross 컨테이너 내의 y좌표 값이 가장 큰 점을 반환한다.
ofApp::Dot ofApp::getMaxY(vector<Dot> cross) {
	Dot max = cross[0];
	for (int i = 1; i < cross.size(); i++) {
		if (cross[i].y > max.y) max = cross[i];
	}
	return max;
}
//기울기와 한 점의 x,y 좌표와 다른 점의 y좌표를 알고 있을 때 해당 점의 x 좌표를 구한다.
double ofApp::getX(double gradient, int x1, int y1, int y2) {
	double c = (y1)-gradient * (x1);
	
	return  ((y2 - c) / gradient);
}
//기울기와 한 점의 x,y 좌표와 다른 점의 x좌표를 알고 있을 때 해당 점의 y 좌표를 구한다.
double ofApp::getY(double gradient, int x1, int y1, int x2) {
	double c = (y1)-gradient * (x1);

	return  ((gradient*x2)+c);
}
//선분의 두 점을 알고 있을 때 기울기를 구한다.
double ofApp::getG(int x1, int x2, int y1, int y2) {
	double ret = ((double)y2 - y1) / (x2 - x1);
	return ret;
}
//할당된 메모리를 해제한다.
void ofApp::freeMemory() {
	free(line);
	delete[] dot;
	vector<Dot>().swap(cross);
}