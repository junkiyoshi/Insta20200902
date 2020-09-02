#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetLineWidth(2);
	ofEnableDepthTest();

	for (int i = 0; i < 200; i++) {

		ofMesh mesh, frame;
		frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
		glm::vec3 base = glm::vec3(ofRandom(-100, 100), ofRandom(-100, 100), ofRandom(-100, 100));
		base = glm::normalize(base) * 250;
		switch ((int)ofRandom(3)) {
		case 0:
			mesh.addVertex(base + glm::vec3(5, 0, 0));
			mesh.addVertex(base + glm::vec3(-5, 0, 0));
			frame.addVertex(base + glm::vec3(5, 0, 0));
			frame.addVertex(base + glm::vec3(-5, 0, 0));
			break;
		case 1:
			mesh.addVertex(base + glm::vec3(0, 5, 0));
			mesh.addVertex(base + glm::vec3(0, -5, 0));
			frame.addVertex(base + glm::vec3(0, 5, 0));
			frame.addVertex(base + glm::vec3(0, -5, 0));
			break;
		case 2:
			mesh.addVertex(base + glm::vec3(0, 0, 5));
			mesh.addVertex(base + glm::vec3(0, 0, -5));
			frame.addVertex(base + glm::vec3(0, 0, 5));
			frame.addVertex(base + glm::vec3(0, 0, -5));
			break;
		}

		frame.addIndex(0);
		frame.addIndex(1);

		this->face_list.push_back(mesh);
		this->frame_list.push_back(frame);

		auto direction = glm::vec3(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1));
		direction = glm::normalize(direction) * 4;
		this->direction_list.push_back(direction);

		auto noise_seed = glm::vec3(glm::vec3(ofRandom(1000), ofRandom(1000), ofRandom(1000)));
		this->noise_seed_list.push_back(noise_seed);

		this->noise_step_list.push_back(0.f);
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	for (int i = this->face_list.size() - 1; i > -1; i--) {

		while (true) {

			this->noise_step_list[i] += 0.01;

			auto angle_x = ofMap(ofNoise(this->noise_seed_list[i].x, this->noise_step_list[i]), 0, 1, -PI, PI);
			auto angle_y = ofMap(ofNoise(this->noise_seed_list[i].y, this->noise_step_list[i]), 0, 1, -PI, PI);
			auto angle_z = ofMap(ofNoise(this->noise_seed_list[i].z, this->noise_step_list[i]), 0, 1, -PI, PI);

			auto rotation_x = glm::rotate(glm::mat4(), angle_x, glm::vec3(1, 0, 0));
			auto rotation_y = glm::rotate(glm::mat4(), angle_y, glm::vec3(0, 1, 0));
			auto rotation_z = glm::rotate(glm::mat4(), angle_z, glm::vec3(0, 0, 1));

			glm::vec3 rotate_direction = glm::vec4(this->direction_list[i], 0) * rotation_z * rotation_y * rotation_x;

			auto right = this->face_list[i].getVertex(this->face_list[i].getNumVertices() - 2);
			auto left = this->face_list[i].getVertex(this->face_list[i].getNumVertices() - 1);
			auto center = (right + left) / 2;

			auto feature = center + rotate_direction;
			if (glm::length(feature) < 300 && glm::length(feature) > 250) {

				this->face_list[i].addVertex(right + rotate_direction);
				this->face_list[i].addVertex(left + rotate_direction);

				this->frame_list[i].addVertex(right + rotate_direction);
				this->frame_list[i].addVertex(left + rotate_direction);

				break;
			}
		}

		if (this->face_list[i].getNumVertices() > 150) {

			this->face_list[i].removeVertex(0);
			this->face_list[i].removeVertex(1);
		}
		else {

			int index = this->face_list[i].getNumVertices() - 1;
			this->face_list[i].addIndex(index); this->face_list[i].addIndex(index - 2); this->face_list[i].addIndex(index - 3);
			this->face_list[i].addIndex(index); this->face_list[i].addIndex(index - 1); this->face_list[i].addIndex(index - 3);
		}

		if (this->frame_list[i].getNumVertices() > 150) {

			this->frame_list[i].removeVertex(0);
			this->frame_list[i].removeVertex(1);
		}
		else {

			int index = this->frame_list[i].getNumVertices() - 1;
			this->frame_list[i].addIndex(index); this->frame_list[i].addIndex(index - 2);
			this->frame_list[i].addIndex(index - 1); this->frame_list[i].addIndex(index - 3);

			if (index == 149) {

				this->frame_list[i].addIndex(148);
				this->frame_list[i].addIndex(149);
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateY(ofGetFrameNum() * 0.25);

	for (int i = 0; i < this->face_list.size(); i++) {

		ofSetColor(239);
		this->face_list[i].draw();

		ofSetColor(39);
		this->frame_list[i].drawWireframe();
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}