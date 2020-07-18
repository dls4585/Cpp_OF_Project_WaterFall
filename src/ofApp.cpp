#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetBackgroundAuto(false);
	ofBackground(255); // ��׶����� ���� ������� ����
	ofSetLogLevel(OF_LOG_VERBOSE);
	
}

//--------------------------------------------------------------
void ofApp::update(){
	if (water_flag) {
		//���� �帣�� ��
		if (moving_flag) {
			pos.x += x_increase; 
			pos.y += y_increase; //x, y�� ��������ŭ ���� ���� ��ġ�� ��ȭ��Ų��.
			if (left_flag) { //���� �������� ���� ��� 
				if (pos.x <= w.p2.x) { //��ǥ ������ ����
					moving_flag = 0; 
					left_flag = 0;
					right_flag = 0;
					up_flag = 0;
					down_flag = 0; //�ٽ� ��ǥ ��ǥ�� ������ �� �ֵ��� ��� flag�� unset�Ѵ�. 
				}
				
			}
			if (right_flag) { //���� ���������� ���� ���
				if (pos.x >= w.p2.x) {
					moving_flag = 0;
					left_flag = 0;
					right_flag = 0;
					up_flag = 0;
					down_flag = 0; //left_flag�� ���� ����
				}
				
			}
			if (up_flag) { //���� ���� ���� ���
				if (pos.y <= w.p2.y) {
					moving_flag = 0;
					left_flag = 0;
					right_flag = 0;
					up_flag = 0;
					down_flag = 0; //right_flag�� ���� ����
				}
				
			}
			if (down_flag) { //���� �Ʒ��� ���� ���
				if (pos.y >= w.p2.y) {
					moving_flag = 0;
					left_flag = 0;
					right_flag = 0;
					up_flag = 0;
					down_flag = 0; //up_flag �� ���� ����
				
				}
				
			}
			if (line_flag) { //���� ���� ������ �带 ���
				if (pos.y >= w.p2.y) { //��ǥ ������ ����
					cross.resize(0); //������ ũ�⸦ �ʱ�ȭ��.(���� ����� �����ش�)
					w.gradient = 0; //���������ϹǷ� ����� 0
					w.p1 = pos; //���� ������ �������� ����
					w.p1.y++; //���� ���а� �������� ���ϴ� ���� �����ϱ� ���� y��ǥ�� 1������Ŵ
					w.p2.x = w.p1.x; 
					w.p2.y = ofGetHeight(); //���������� �������� �������� �������� ��ǥ���� ����

					//������-��ǥ���� �մ� ���� ���̿� �����ϴ� ������ �ִ��� Ȯ��
					for (int i = 0; i < line_num; i++)
						if (LineIntersection(w, line[i]))
							crossPoint(w, line[i]);
					//�����ϴ� ������ ���� ��� ���� ����� �������� ���ο� ��ǥ������ �����Ѵ�.
					if (cross.size() != 0) {
						w.p2 = getMinY(cross);
					}
					pos.x = w.p1.x; 
					pos.y = w.p1.y; //���� ���� ���������� ����
					x_increase = (w.p2.x - w.p1.x) / 1000.0; 
					y_increase = (w.p2.y - w.p1.y) / 1000.0; //x, y ������ ����
					down_flag = 1; // �Ʒ��� �帣�� ���� �ǹ�
					line_flag = 0; // ���� ������ �帣�� ����
					moving_flag = 1; // ���� �ٽ� �帣�� ����
				}
			}
		}
		//��ǥ ���� ����
		else { 
			// ���� ���� ������ ��
			if (pos.x <= 0) {
				cross.resize(0); //cross �����̳��� ũ�⸦ 0���� �ʱ�ȭ (���� ����� �����ش�)
				// ���� �缳��
				if (w.gradient == 0) { // �������� ���� ���ϰ� �־��� ���
					int degree = ofRandom(-75,75); // -75~75�� ������ ������ ���Ƿ� ����
					w.gradient = tan(degree * (PI / 180)); // �ش� ������ ���� ������ ���⸦ ����
				}
				else { //������ �ƴϾ��� ���
					w.gradient *= -1; //�ݴ�� ���ϵ��� ����
				}
				w.p1 = w.p2; // ��ǥ���� ���������� �缳��
				// ���⺰ ��ǥ ��ǥ
				if (w.gradient < 0) { // ������ ������� ��
					if (w.gradient < getG(w.p1.x, ofGetWidth(), w.p1.y, 0)) { // ������ ����� �𼭸��� �մ� ���к��� ���� ������ ���ϴ� ������ ���
						w.p2.y = 0;
						w.p2.x = getX(w.gradient, w.p1.x, w.p1.y, w.p2.y); //��ǥ���� ����
						up_flag = 1;
						right_flag = 1; //����� flag
					}
					else if (w.gradient == getG(w.p1.x, ofGetWidth(), w.p1.y, 0)) { // ������ ����� �𼭸��� �մ� ���а� ������ ���
						w.p2.y = 0;
						w.p2.x = ofGetWidth();
						up_flag = 1;
						right_flag = 1;
					}
					else if (w.gradient > getG(w.p1.x, ofGetWidth(), w.p1.y, 0)) { //������ ����� �𼭸��� �մ� ���к��� �Ʒ��� ������ ���ϴ� ������ ���
						w.p2.x = ofGetWidth();
						w.p2.y = getY(w.gradient, w.p1.x, w.p1.y, w.p2.x);
						up_flag = 1;
						right_flag = 1;
					}
				}
				else if (w.gradient > 0) { //������ �������� ��
					if (w.gradient > getG(w.p1.x, ofGetWidth(), w.p1.y, ofGetHeight())) { // ������ �ϴ��� �𼭸��� �մ� ���к��� �Ʒ��� ������ ���ϴ� ������ ���
						w.p2.y = ofGetHeight();
						w.p2.x = getX(w.gradient, w.p1.x, w.p1.y, w.p2.y);
						down_flag = 1;
						right_flag = 1;
					}
					else if (w.gradient == getG(w.p1.x, ofGetWidth(), w.p1.y, ofGetHeight())) { //������ �ϴ��� �𼭸��� �մ� ���ΰ� ������ ���
						w.p2.x = ofGetWidth();
						w.p2.y = ofGetHeight();
						down_flag = 1;
						right_flag = 1;
					}
					else if (w.gradient < getG(w.p1.x, ofGetWidth(), w.p1.y, ofGetHeight())) { // ������ �ϴ��� �𼭸��� �մ� ���к��� ���� ������ ���ϴ� ������ ���
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
			//������ ���� ������ ��
			else if (pos.x >= ofGetWidth()) {
				cross.resize(0); //cross �����̳��� ũ�⸦ 0���� �ʱ�ȭ(��������� ����ش�.)
				if (w.gradient == 0) {
					int degree = ofRandom(-75,75);
					w.gradient = tan(degree * (PI / 180));
				}
				else {
					w.gradient *= -1;
				}
				w.p1 = w.p2;
				// ���⺰ ��ǥ ��ǥ
				if (w.gradient > 0) { // �»����� ���
					if (w.gradient > getG(w.p1.x, 0, w.p1.y, 0)) { // ���� ����� �𼭸��� �մ� ������ ���⺸�� ���� ������ �ִ� ������ ���
						w.p2.y = 0;
						w.p2.x = getX(w.gradient, w.p1.x, w.p1.y, w.p2.y);
						up_flag = 1;
						left_flag = 1;
					}
					else if (w.gradient == getG(w.p1.x, 0, w.p1.y, 0)) { // ���� ����� �𼭸��� �մ� ������ ����� ���� ���
						w.p2.y = 0;
						w.p2.x = 0;
						up_flag = 1;
						left_flag = 1;
					}
					else if (w.gradient < getG(w.p1.x, 0, w.p1.y, 0)) { // ���� ����� �𼭸��� �մ� ������ ���⺸�� �Ʒ��� ������ �ִ� ������ ���
						w.p2.x = 0;
						w.p2.y = getY(w.gradient, w.p1.x, w.p1.y, w.p2.x);
						up_flag = 1;
						left_flag = 1;
					}
				}
				else if (w.gradient < 0) { // �������� ���
					if (w.gradient < getG(w.p1.x, 0, w.p1.y, ofGetHeight())) { // ���� �ϴ��� �𼭸��� �մ� ������ ���⺸�� �Ʒ��� ������ �ִ� ������ ���
						w.p2.y = ofGetHeight();
						w.p2.x = getX(w.gradient, w.p1.x, w.p1.y, w.p2.y);
						down_flag = 1;
						left_flag = 1;
					}
					else if (w.gradient == getG(w.p1.x, 0, w.p1.y, ofGetHeight())) { // ���� �ϴ��� �𼭸��� �մ� ������ ����� ���� ���
						w.p2.x = 0;
						w.p2.y = ofGetHeight();
						down_flag = 1;
						left_flag = 1;
					}
					else if (w.gradient > getG(w.p1.x, 0, w.p1.y, ofGetHeight())) { // ���� �ϴ��� �𼭸��� �մ� ������ ���⺸�� ���� ������ �ִ� ������ ���
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
			//�Ʒ� ���� ������ ��
			else if (pos.y >= ofGetHeight()) {
				cross.resize(0);  // cross �����̳��� ũ�⸦ 0���� �ʱ�ȭ(��������� ����ش�.)
				if (w.gradient == 0) {
					int degree = ofRandom(-75,75);
					w.gradient = tan(degree * (PI / 180));
				}
				else {
					w.gradient *= -1;
				}
				w.p1 = w.p2;
				// ���⺰ ��ǥ ��ǥ
				if (w.gradient > 0) { // �»����� ���
					if (w.gradient < getG(w.p1.x, 0, w.p1.y, 0)) { // ���� ����� �𼭸��� �մ� ������ ���⺸�� �Ʒ��� ������ �ִ� ��� 
						w.p2.x = 0;
						w.p2.y = getY(w.gradient, w.p1.x, w.p1.y, w.p2.x);
						left_flag = 1;
						up_flag = 1;
					}
					else if (w.gradient == getG(w.p1.x, 0, w.p1.y, 0)) { // ���� ����� �𼭸��� �մ� ������ ����� ���� ���
						w.p2.x = 0;
						w.p2.y = 0;
						left_flag = 1;
						up_flag = 1;
					}
					else if (w.gradient > getG(w.p1.x, 0, w.p1.y, 0)) { // ���� ����� �𼭸��� �մ� ������ ���⺸�� ���� ������ �ִ� ��� 
						w.p2.y = 0;
						w.p2.x = getX(w.gradient, w.p1.x, w.p1.y, w.p2.y);
						left_flag = 1;
						up_flag = 1;
					}
				
				}
				else if (w.gradient < 0) { 
					if (w.gradient > getG(w.p1.x, ofGetWidth(), w.p1.y, 0)) { // ������ ����� �𼭸��� �մ� ������ ���⺸�� �Ʒ��� ������ �ִ� ��� 
						w.p2.x = ofGetWidth();
						w.p2.y = getY(w.gradient, w.p1.x, w.p1.y, w.p2.x);
						right_flag = 1;
						up_flag = 1;
					}

					else if (w.gradient == getG(w.p1.x, ofGetWidth(), w.p1.y, 0)) { // ������ ����� �𼭸��� �մ� ������ ����� ���� ���
						w.p2.x = ofGetWidth();
						w.p2.y = ofGetHeight();
						right_flag = 1;
						up_flag = 1;
					}
					else if (w.gradient < getG(w.p1.x, ofGetWidth(), w.p1.y, 0)) { // ������ ����� �𼭸��� �մ� ������ ���⺸�� ���� ������ �ִ� ��� 
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
			//�� ���� ������ ��
			else if (pos.y <= 0) {
				cross.resize(0); // cross �����̳��� ũ�⸦ 0���� �ʱ�ȭ(��������� ����ش�.)
				if (w.gradient == 0) {
					int degree = ofRandom(-75,75);
					w.gradient = tan(degree * (PI / 180));
				}
				else {
					w.gradient *= -1;
				}
				w.p1 = w.p2;
				// ���⺰ ��ǥ ��ǥ
				if (w.gradient < 0) { // �������� ���
					if (w.gradient > getG(w.p1.x, 0, w.p1.y, ofGetHeight())) { // ���� �ϴ��� �𼭸��� �մ� ������ ���⺸�� ���� ������ �ִ� ���
						w.p2.x = 0;
						w.p2.y = getY(w.gradient, w.p1.x, w.p1.y, w.p2.x);
						left_flag = 1;
						down_flag = 1;
					}
					else if (w.gradient == getG(w.p1.x, 0, w.p1.y, ofGetHeight())) { // ���� �ϴ��� �𼭸��� �մ� ������ ����� ���� ���
						w.p2.x = 0;
						w.p2.y = ofGetHeight();
						left_flag = 1;
						down_flag = 1;
					}
					else if (w.gradient < getG(w.p1.x, 0, w.p1.y, ofGetHeight())) { // ���� �ϴ��� �𼭸��� �մ� ������ ���⺸�� �Ʒ��� ������ �ִ� ���
						w.p2.y = ofGetHeight();
						w.p2.x = getX(w.gradient, w.p1.x, w.p1.y, w.p2.y);
						left_flag = 1;
						down_flag = 1;
					}
				}
				else if (w.gradient > 0) {
					if (w.gradient < getG(w.p1.x, ofGetWidth(), w.p1.y, ofGetHeight())) { // ������ �ϴ��� �𼭸��� �մ� ������ ���⺸�� ���� ������ �ִ� ���
						w.p2.x = ofGetWidth();
						w.p2.y = getY(w.gradient, w.p1.x, w.p1.y, w.p2.x);
						right_flag = 1;
						down_flag = 1;
					}
					else if (w.gradient == getG(w.p1.x, ofGetWidth(), w.p1.y, ofGetHeight())) { // ������ �ϴ��� �𼭸��� �մ� ������ ����� ���� ���
						w.p2.y = ofGetHeight();
						w.p2.x = ofGetWidth();
						right_flag = 1;
						down_flag = 1;
					}
					else if (w.gradient > getG(w.p1.x, ofGetWidth(), w.p1.y, ofGetHeight())) { // ������ �ϴ��� �𼭸��� �մ� ������ ���⺸�� �Ʒ��� ������ �ִ� ���
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
			//������ ������ ��
			else { 
				cross.resize(0); // cross �����̳��� ũ�⸦ 0���� �ʱ�ȭ(��������� ����ش�.)
				w.p1 = w.p2;
				if (line[w.p2.L_num].p1.y > line[w.p2.L_num].p2.y) { // ������ �������� ������ �ִ� ���
					w.p2 = line[w.p2.L_num].p1;
				}
				else { // ������ ���������� ������ �ִ� ���
					w.p2 = line[w.p2.L_num].p2;
				}
				x_increase = (w.p2.x - w.p1.x) / 1000.0;
				y_increase = (w.p2.y - w.p1.y) / 1000.0;
				pos.x = w.p1.x;
				pos.y = w.p1.y;
				line_flag = 1;
				
			}
			moving_flag = 1; // ���� ��ǥ������ �ٽ� �帣���� ����
			
		}
		
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	if (draw_flag) {
		ofSetColor(0); //�⺻���� ������ 
		ofFill(); //�� ä���
		ofSetLineWidth(5); // ������ ���⸦ 5�� ����
		for (int i = 0; i < line_num; i++) {
			ofDrawLine(line[i].p1.x, line[i].p1.y, line[i].p2.x, line[i].p2.y);
		}

		for (int i = 0; i < dot_num; i++) { 
			if (i == water_dot) { // ���� ������ ������ �� �������� �ʱ�ȭ
				 //���� ������ ������ �ε��� ���� ����Ѵ�.

				ofSetColor(255, 0, 0); //���̳����� ������ ���� ������
			}
			else {
				ofSetColor(0x000000); //�ٸ� ������ ������
			}
			ofDrawCircle(dot[i].x,dot[i].y, 10); //�ش� ��ǥ�� ���� �׸���
		}
		draw_flag = 0;
	}
	if (water_flag) {
		ofSetColor(ofColor::blue);
		ofFill();
		ofSetLineWidth(4);
		ofDrawLine(w.p1.x, w.p1.y, pos.x, pos.y); //���������� ������������ �մ� ������ �׸���.

	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == 'd' || key == 'D') { // d Ű�� ������ ��
		draw_flag = 1; // draw_flag�� set�ϰ�
		draw(); // draw�Լ��� ȣ���Ѵ�.
	}
	if (key == 's' || key == 'S') {
		start_flag = 1; //start_flag �� set�Ѵ�.
		ofSetColor(ofColor::black);
		//���� ȭ��ǥ
		ofDrawTriangle(dot[water_dot].x - 50, dot[water_dot].y,
			dot[water_dot].x - 30, dot[water_dot].y - 10,
			dot[water_dot].x - 30, dot[water_dot].y + 10);
		ofSetLineWidth(5);
		ofDrawLine(dot[water_dot].x - 30, dot[water_dot].y,
			dot[water_dot].x - 15, dot[water_dot].y);
		//������ ȭ��ǥ
		ofDrawTriangle(dot[water_dot].x + 50, dot[water_dot].y,
			dot[water_dot].x + 30, dot[water_dot].y - 10,
			dot[water_dot].x + 30, dot[water_dot].y + 10);
		ofSetLineWidth(5);
		ofDrawLine(dot[water_dot].x + 30, dot[water_dot].y,
			dot[water_dot].x + 15, dot[water_dot].y);
		//�� ȭ��ǥ
		ofDrawTriangle(dot[water_dot].x, dot[water_dot].y - 50,
			dot[water_dot].x - 10, dot[water_dot].y - 30,
			dot[water_dot].x + 10, dot[water_dot].y - 30);
		ofSetLineWidth(5);
		ofDrawLine(dot[water_dot].x, dot[water_dot].y - 30,
			dot[water_dot].x, dot[water_dot].y - 15);
		//�Ʒ� ȭ��ǥ
		ofDrawTriangle(dot[water_dot].x, dot[water_dot].y + 50,
			dot[water_dot].x - 10, dot[water_dot].y + 30,
			dot[water_dot].x + 10, dot[water_dot].y + 30);
		ofSetLineWidth(5);
		ofDrawLine(dot[water_dot].x, dot[water_dot].y + 30,
			dot[water_dot].x, dot[water_dot].y + 15);
	}
	if (key == 'e' || key == 'E') { 
		water_flag = 0; //water_flag �� 0���� unset�� ���� �帧�� ����.
	}
	if (key == OF_KEY_LEFT) {
		if (!start_flag) { // ���� ������ �ϱ� ����, ���� ������ �� �ִ� �ܰ�
			ofFill();
			ofSetColor(0x000000);
			ofDrawCircle(dot[water_dot].x, dot[water_dot].y, 10);
			water_dot--; //���� ������ �迭�� �ε����� ���ҽ�Ų��.
			if (water_dot < 0) // �� ������ ������ �ٽ� �������� �̵��� ���, ���� ������ ������ �̵�
				water_dot = dot_num - 1;

			ofSetColor(255, 0, 0);
			ofDrawCircle(dot[water_dot].x, dot[water_dot].y, 10);
			cout << "Selected Dot coordinate is (" << dot[water_dot].x
				<< ", " << dot[water_dot].y << ")" << endl;
		}
		else {
			// ȭ��ǥ �����
			if (!water_flag) {
				water_flag = 1;
				ofSetColor(ofColor::white);
				ofDrawTriangle(dot[water_dot].x - 50, dot[water_dot].y,
					dot[water_dot].x - 30, dot[water_dot].y - 10,
					dot[water_dot].x - 30, dot[water_dot].y + 10);
				ofSetLineWidth(5);
				ofDrawLine(dot[water_dot].x - 30, dot[water_dot].y,
					dot[water_dot].x - 15, dot[water_dot].y);
				//������ ȭ��ǥ
				ofDrawTriangle(dot[water_dot].x + 50, dot[water_dot].y,
					dot[water_dot].x + 30, dot[water_dot].y - 10,
					dot[water_dot].x + 30, dot[water_dot].y + 10);
				ofSetLineWidth(5);
				ofDrawLine(dot[water_dot].x + 30, dot[water_dot].y,
					dot[water_dot].x + 15, dot[water_dot].y);
				//�� ȭ��ǥ
				ofDrawTriangle(dot[water_dot].x, dot[water_dot].y - 50,
					dot[water_dot].x - 10, dot[water_dot].y - 30,
					dot[water_dot].x + 10, dot[water_dot].y - 30);
				ofSetLineWidth(5);
				ofDrawLine(dot[water_dot].x, dot[water_dot].y - 30,
					dot[water_dot].x, dot[water_dot].y - 15);
				//�Ʒ� ȭ��ǥ
				ofDrawTriangle(dot[water_dot].x, dot[water_dot].y + 50,
					dot[water_dot].x - 10, dot[water_dot].y + 30,
					dot[water_dot].x + 10, dot[water_dot].y + 30);
				ofSetLineWidth(5);
				ofDrawLine(dot[water_dot].x, dot[water_dot].y + 30,
					dot[water_dot].x, dot[water_dot].y + 15);
			}
			if (!moving_flag) { //�� ó�� ������ ������ ���
				left_flag = 1;
				w.p1.x = pos.x = dot[water_dot].x; 
				w.p1.y = pos.y = dot[water_dot].y; //�������� ���� ������ ���� ��ǥ�� ����
				w.p2.x = 0;
				w.p2.y = dot[water_dot].y; //��ǥ���� ���ʺ����� ����

				//������-��ǥ���� �մ� ���� ���̿� �����ϴ� ������ �ִ��� Ȯ��
				for (int i = 0; i < line_num; i++) 
					if (LineIntersection(w, line[i]))
						crossPoint(w, line[i]);
				//�����ϴ� ������ ���� ��� ���� ����� �������� ���ο� ��ǥ������ �����Ѵ�.
				if (cross.size() != 0) {
					w.p2 = getMaxX(cross); //<- �����̱� ������ x��ǥ�� ���� ū ���� ����
				}
				x_increase = (w.p2.x - w.p1.x) / 1000.0; 
				y_increase = (w.p2.y - w.p1.y) / 1000.0; //x,y ���� ������
				w.gradient = 0; 
				moving_flag = 1; //���� �带 �� �ֵ��� moving_flag �� set
			}
		}
		
	}
	if (key == OF_KEY_RIGHT) {
		if (!start_flag) {// ���� ������ �ϱ� ����, ���� ������ �� �ִ� �ܰ�
			ofFill();
			ofSetColor(0x000000);
			ofDrawCircle(dot[water_dot].x, dot[water_dot].y, 10);
			water_dot++; //���� ������ �迭�� �ε����� ������Ų��.
			if (water_dot == dot_num) // �� ������ ������ �ٽ� ���������� �̵��� ���, ���� ������ ������ �̵�
				water_dot = 0;
			ofSetColor(255, 0, 0);
			ofDrawCircle(dot[water_dot].x, dot[water_dot].y, 10);
			cout << "Selected Dot coordinate is (" << dot[water_dot].x
				<< ", " << dot[water_dot].y << ")" << endl;
		}
		else {
			// ȭ��ǥ �����
			if (!water_flag) {
				water_flag = 1;
				ofSetColor(ofColor::white);
				//���� ȭ��ǥ
				ofDrawTriangle(dot[water_dot].x - 50, dot[water_dot].y,
					dot[water_dot].x - 30, dot[water_dot].y - 10,
					dot[water_dot].x - 30, dot[water_dot].y + 10);
				ofSetLineWidth(5);
				ofDrawLine(dot[water_dot].x - 30, dot[water_dot].y,
					dot[water_dot].x - 15, dot[water_dot].y);
				//������ ȭ��ǥ
				ofDrawTriangle(dot[water_dot].x + 50, dot[water_dot].y,
					dot[water_dot].x + 30, dot[water_dot].y - 10,
					dot[water_dot].x + 30, dot[water_dot].y + 10);
				ofSetLineWidth(5);
				ofDrawLine(dot[water_dot].x + 30, dot[water_dot].y,
					dot[water_dot].x + 15, dot[water_dot].y);
				//�� ȭ��ǥ
				ofDrawTriangle(dot[water_dot].x, dot[water_dot].y - 50,
					dot[water_dot].x - 10, dot[water_dot].y - 30,
					dot[water_dot].x + 10, dot[water_dot].y - 30);
				ofSetLineWidth(5);
				ofDrawLine(dot[water_dot].x, dot[water_dot].y - 30,
					dot[water_dot].x, dot[water_dot].y - 15);
				//�Ʒ� ȭ��ǥ
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
				w.p1.y = pos.y = dot[water_dot].y; //�������� ���� ������ ���� ��ǥ�� ����
				w.p2.x = ofGetWidth();
				w.p2.y = dot[water_dot].y; //��ǥ���� �����ʺ����� ����

				//������-��ǥ���� �մ� ���� ���̿� �����ϴ� ������ �ִ��� Ȯ��
				for (int i = 0; i < line_num; i++)
					if (LineIntersection(w, line[i]))
						crossPoint(w, line[i]);
				//�����ϴ� ������ ���� ��� ���� ����� �������� ���ο� ��ǥ������ �����Ѵ�.
				if (cross.size() != 0) {
					w.p2 = getMinX(cross); //-> �����̱� ������ x��ǥ�� ���� ���� ���� ����
				}
				x_increase = (w.p2.x - w.p1.x) / 1000.0;
				y_increase = (w.p2.y - w.p1.y) / 1000.0;
				w.gradient = 0;
				moving_flag = 1; //���� �带 �� �ֵ��� moving_flag �� set
			}
		}
	}
	if (key == OF_KEY_UP) {
		if (start_flag) {
			// ȭ��ǥ �����
			if (!water_flag) {
				water_flag = 1;
				ofSetColor(ofColor::white);
				ofDrawTriangle(dot[water_dot].x - 50, dot[water_dot].y,
					dot[water_dot].x - 30, dot[water_dot].y - 10,
					dot[water_dot].x - 30, dot[water_dot].y + 10);
				ofSetLineWidth(5);
				ofDrawLine(dot[water_dot].x - 30, dot[water_dot].y,
					dot[water_dot].x - 15, dot[water_dot].y);
				//������ ȭ��ǥ
				ofDrawTriangle(dot[water_dot].x + 50, dot[water_dot].y,
					dot[water_dot].x + 30, dot[water_dot].y - 10,
					dot[water_dot].x + 30, dot[water_dot].y + 10);
				ofSetLineWidth(5);
				ofDrawLine(dot[water_dot].x + 30, dot[water_dot].y,
					dot[water_dot].x + 15, dot[water_dot].y);
				//�� ȭ��ǥ
				ofDrawTriangle(dot[water_dot].x, dot[water_dot].y - 50,
					dot[water_dot].x - 10, dot[water_dot].y - 30,
					dot[water_dot].x + 10, dot[water_dot].y - 30);
				ofSetLineWidth(5);
				ofDrawLine(dot[water_dot].x, dot[water_dot].y - 30,
					dot[water_dot].x, dot[water_dot].y - 15);
				//�Ʒ� ȭ��ǥ
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
				w.p1.y = pos.y = dot[water_dot].y; //�������� ���� ������ ���� ��ǥ�� ����
				w.p2.x = dot[water_dot].x;
				w.p2.y = 0;						   //��ǥ���� ���ʺ����� ����

				//������-��ǥ���� �մ� ���� ���̿� �����ϴ� ������ �ִ��� Ȯ��
				for (int i = 0; i < line_num; i++)
					if (LineIntersection(w, line[i]))
						crossPoint(w, line[i]);
				//�����ϴ� ������ ���� ��� ���� ����� �������� ���ο� ��ǥ������ �����Ѵ�.
				if (cross.size() != 0)
					w.p2 = getMaxY(cross); //�� �����̱� ������ y��ǥ�� ���� ū ���� ����
				x_increase = (w.p2.x - w.p1.x) / 1000.0;
				y_increase = (w.p2.y - w.p1.y) / 1000.0;
				w.gradient = 0;
				moving_flag = 1; //���� �带 �� �ֵ��� moving_flag �� set
			}
		}
	}
	if (key == OF_KEY_DOWN) {
			if (start_flag) {
				// ȭ��ǥ �����
				if (!water_flag) {
					water_flag = 1;
					ofSetColor(ofColor::white);
					ofDrawTriangle(dot[water_dot].x - 50, dot[water_dot].y,
						dot[water_dot].x - 30, dot[water_dot].y - 10,
						dot[water_dot].x - 30, dot[water_dot].y + 10);
					ofSetLineWidth(5);
					ofDrawLine(dot[water_dot].x - 30, dot[water_dot].y,
						dot[water_dot].x - 15, dot[water_dot].y);
					//������ ȭ��ǥ
					ofDrawTriangle(dot[water_dot].x + 50, dot[water_dot].y,
						dot[water_dot].x + 30, dot[water_dot].y - 10,
						dot[water_dot].x + 30, dot[water_dot].y + 10);
					ofSetLineWidth(5);
					ofDrawLine(dot[water_dot].x + 30, dot[water_dot].y,
						dot[water_dot].x + 15, dot[water_dot].y);
					//�� ȭ��ǥ
					ofDrawTriangle(dot[water_dot].x, dot[water_dot].y - 50,
						dot[water_dot].x - 10, dot[water_dot].y - 30,
						dot[water_dot].x + 10, dot[water_dot].y - 30);
					ofSetLineWidth(5);
					ofDrawLine(dot[water_dot].x, dot[water_dot].y - 30,
						dot[water_dot].x, dot[water_dot].y - 15);
					//�Ʒ� ȭ��ǥ
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
					w.p1.y = pos.y = dot[water_dot].y; //�������� ���� ������ ���� ��ǥ�� ����
					w.p2.x = dot[water_dot].x;
					w.p2.y = ofGetHeight();			   //��ǥ���� ���ʺ����� ����

					//������-��ǥ���� �մ� ���� ���̿� �����ϴ� ������ �ִ��� Ȯ��
					for (int i = 0; i < line_num; i++)
						if (LineIntersection(w, line[i]))
							crossPoint(w, line[i]);
					//�����ϴ� ������ ���� ��� ���� ����� �������� ���ο� ��ǥ������ �����Ѵ�.
					if (cross.size() != 0)
						w.p2 = getMinY(cross); //�Ʒ� �����̱� ������ y��ǥ�� ���� ���� ���� ����
					x_increase = (w.p2.x - w.p1.x) / 1000.0;
					y_increase = (w.p2.y - w.p1.y) / 1000.0;
					w.gradient = 0;
					moving_flag = 1; //���� �带 �� �ֵ��� moving_flag �� set
				}
			}
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if (key == 'l' || key == 'L') { // ���̾�α� â�� ����.

		//Open the Open File Dialog
		ofFileDialogResult openFileResult = ofSystemLoadDialog("Select an input file");

		//Check if the user opened a file
		if (openFileResult.bSuccess) {

			ofLogVerbose("User selected a file");

			//We have a file, check it and process it
			processOpenFileSelection(openFileResult); // ���Ϸκ��� �� �Է¹ޱ� 

		}
		else {
			ofLogVerbose("User hit cancel");
		}
	}
	if (key == 'q' || key == 'Q') { //�Ҵ�� �޸𸮸� �����ϸ� ���α׷� ����
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
	//�ܼ� �α׿� ���� �̸��� ��θ� ����Ѵ�.
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
	//������ ������ ������ ������ �޾� �ڷᱸ���� �����ϴ� �ݺ���
	while (!file.eof()) {
		//���Ϸκ��� �� �پ� �о�� ���ڿ��� ������ �� ���⸦ �������� �и��� ����
		getline(file, _line);
		vector <string> words = ofSplitString(_line, " ");

		if (words.size() == 1) { // ������ ���� �Ǵ� ���� ����
			if (line_or_dot == 0) { // 0�̸� ����, 1�̸� ��
				line_num = atoi(words[0].c_str());
				line = (Line*)malloc(sizeof(Line)*line_num); // �ԷµǴ� ������ ������ŭ �����Ҵ�
				line_count = line_num; 
				cout << "The number of line is: " << line_num << endl;
			}
			else {
				dot_num = atoi(words[0].c_str());
				dot = new Dot[dot_num]; //�ԷµǴ� ���� ������ŭ �����Ҵ�
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
		if (line_count == 0) // line_count �� 0�̵Ǹ� ������ ������ ���� �Է��� �������� �ǹ� -> ���� ���� ������ �Է¹��� ����
			line_or_dot = 1;
	}


}


//������ right�� x��ǥ�� left�� x��ǥ���� ũ�� True / ������ False�� ��ȯ
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

//right�� x��ǥ�� �� ���� ��� ���� �ٲ��ش�.
void ofApp:: swap(Line* l) {
	Dot temp;
	temp = l->p1;
	l->p1 = l->p2;
	l->p2 = temp;
}

//�� ������ ���� ���θ� Ȯ���Ѵ�.
int ofApp:: LineIntersection(Line l1, Line l2) {
	int ret;
	// l1�� �������� l2�� �����ϴ� �� Ȯ���Ѵ�.

	int l1_l2 = ccw(l1.p1, l1.p2, l2.p1) * ccw(l1.p1, l1.p2, l2.p2);
	// l2�� �������� l1�� �����ϴ� �� Ȯ���Ѵ�.

	int l2_l1 = ccw(l2.p1, l2.p2, l1.p1) * ccw(l2.p1, l2.p2, l1.p2);


	// l1 �� l2�� ������ �� �ִ� ���
	if (l1_l2 == 0 && l2_l1 == 0) {
		// Line1�� ���� ũ�� ������ p1 < p2 ������ �����.
		if (comparator(l1.p2, l1.p1)) swap(&l1);
		// Line2�� ���� ũ�� ������ p1 < p2 ������ �����.
		if (comparator(l2.p2, l2.p1)) swap(&l2);

		// l1.p1 -----------l1.p2
		// l2.p1 -----------l2.p2
		// �� ������ �����ϴ� �� ���캻��.
		ret = (comparator(l2.p1, l1.p2)) && (comparator(l1.p1, l2.p2));
	}
	// l1�� l2�� ������ �� ���� �ʴ� ���
	else {
		ret = (l1_l2 <= 0) && (l2_l1 <= 0);
	}
	return ret; //return 1 ->  ���� return 0 -> ���� x
}

//ccw �˰����� ������ �Լ�
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

//�� ������ ������ ���ϴ� �Լ�
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
	//�ش� �������� cross �����̳ʿ� push_back �ϸ� ��ȯ
	cross.push_back(temp);
	return temp;
}

//vector cross �����̳� ���� x��ǥ ���� ���� ���� ���� ��ȯ�Ѵ�.
ofApp::Dot ofApp:: getMinX(vector<Dot> cross) {
	Dot min = cross[0];
	for (int i = 1; i < cross.size(); i++) {
		if (cross[i].x < min.x) min = cross[i];
	}
	return min;
}
//vector cross �����̳� ���� y��ǥ ���� ���� ���� ���� ��ȯ�Ѵ�.
ofApp::Dot ofApp::getMinY(vector<Dot> cross) {
	Dot min = cross[0];
	for (int i = 1; i < cross.size(); i++) {
		if (cross[i].y < min.y) min = cross[i];
	}
	return min;
}
//vector cross �����̳� ���� x��ǥ ���� ���� ū ���� ��ȯ�Ѵ�.
ofApp::Dot ofApp::getMaxX(vector<Dot> cross) {
	Dot max = cross[0];
	for (int i = 1; i < cross.size(); i++) {
		if (cross[i].x > max.x) max = cross[i];
	}
	return max;
}
//vector cross �����̳� ���� y��ǥ ���� ���� ū ���� ��ȯ�Ѵ�.
ofApp::Dot ofApp::getMaxY(vector<Dot> cross) {
	Dot max = cross[0];
	for (int i = 1; i < cross.size(); i++) {
		if (cross[i].y > max.y) max = cross[i];
	}
	return max;
}
//����� �� ���� x,y ��ǥ�� �ٸ� ���� y��ǥ�� �˰� ���� �� �ش� ���� x ��ǥ�� ���Ѵ�.
double ofApp::getX(double gradient, int x1, int y1, int y2) {
	double c = (y1)-gradient * (x1);
	
	return  ((y2 - c) / gradient);
}
//����� �� ���� x,y ��ǥ�� �ٸ� ���� x��ǥ�� �˰� ���� �� �ش� ���� y ��ǥ�� ���Ѵ�.
double ofApp::getY(double gradient, int x1, int y1, int x2) {
	double c = (y1)-gradient * (x1);

	return  ((gradient*x2)+c);
}
//������ �� ���� �˰� ���� �� ���⸦ ���Ѵ�.
double ofApp::getG(int x1, int x2, int y1, int y2) {
	double ret = ((double)y2 - y1) / (x2 - x1);
	return ret;
}
//�Ҵ�� �޸𸮸� �����Ѵ�.
void ofApp::freeMemory() {
	free(line);
	delete[] dot;
	vector<Dot>().swap(cross);
}