//
// dButton.cpp
//
// MIT License
//
// Copyright (c) 2022 DRING
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#include "dButton.hpp"

/* dButton */

// Call this function first in ofApp::setup()
void dButton::setup(float x, float y, float w, float h, std::string label) {
    px = x;
    py = y;
    width = w;
    height = h;
    labelText = label;
}

void dButton::setLabel(std::string label) {
    labelText = label;
}

bool dButton::getMouseInside(float x, float y) {
    return x >= px &&
        x <= px + width &&
        y >= py &&
        y <= py + height;
}

// Call this function in ofApp::mousePressed(...)
void dButton::handlePressed(float x, float y) {
    if (hide) return;
    if (getMouseInside(x, y)) {
        pressedInside = true;
    }
}

// Call this function in ofApp::update()
void dButton::handleMove(float x, float y) {
    if (hide) return;
    hover = getMouseInside(x, y);
}

// Call this function in ofApp::mouseReleased(...)
void dButton::handleReleased(float x, float y) {
    if (hide) return;
    if (!pressedInside) {
        return;
    }
    
    if (getMouseInside(x, y)) {
        clicked = true;
    }
        
    pressedInside = false;
}

// Call this function in ofApp::update()
void dButton::update() {
    clicked = false;
}

// Call this function in ofApp::draw()
void dButton::draw() {
    if (hide) return;
    ofPushStyle();
    
    if (hover && pressedInside) {
        // Active
        ofSetColor(25, 25, 25);
    } else if (hover) {
        // Hover
        ofSetColor(55, 55, 55);
    } else {
        // Default
        ofSetColor(0, 0, 0);
    }
    
    ofDrawRectRounded(px, py, width, height, height / 2.0);
    
    if (hover && pressedInside) {
        // Active
        ofSetColor(235, 235, 235);
    } else if (hover) {
        // Hover
        ofSetColor(255, 255, 255);
    } else {
        // Default
        ofSetColor(200, 200, 200);
    }
    
    float fWidth = font->stringWidth(labelText);
    float aHeight = font->getAscenderHeight();
    font->drawString(labelText, px + width / 2.0 - fWidth / 2.0, py + aHeight + (height - aHeight) / 2.0);
    
    ofPopStyle();
}

/* dSlider */

// Call this function in ofApp::mousePressed(...)
void dSlider::handlePressed(float x, float y) {
    if (hide) return;
    dButton::handlePressed(x, y);
    if (!pressedInside) return;
    value = getValueFromMousePos(x, y);
}

// Call this function in ofApp::update()
void dSlider::handleMove(float x, float y) {
    if (hide) return;
    dButton::handleMove(x, y);
    if (!pressedInside) return;
    value = getValueFromMousePos(x, y);
}

// Call this function in ofApp::mouseReleased(...)
void dSlider::handleReleased(float x, float y) {
    if (hide) return;
    dButton::handleReleased(x, y);
}

float dSlider::getValueFromMousePos(float x, float y) {
    float v = ofClamp((x - px - height / 2.0) / (width - height), 0, 1);
    return (step > 1 ? round(v * step) / (float) step : v) * (max - min) + min;
}

// Call this function in ofApp::draw()
void dSlider::draw() {
    if (hide) return;
    ofPushStyle();
    
    if (hover && pressedInside) {
        // Active
        ofSetColor(25, 25, 25);
    } else if (hover) {
        // Hover
        ofSetColor(55, 55, 55);
    } else {
        // Default
        ofSetColor(0, 0, 0);
    }
    
    ofDrawRectRounded(px, py, width, height, height / 2.0);
    
    ofSetColor(255, 255, 255, 70);
    ofDrawRectRounded(px + 3, py + 3, height + (width - height - 6) * (value - min) / (max - min), height - 6, (height - 6) / 2.0);
    
    if (hover && pressedInside) {
        // Active
        ofSetColor(235, 235, 235);
    } else if (hover) {
        // Hover
        ofSetColor(255, 255, 255);
    } else {
        // Default
        ofSetColor(200, 200, 200);
    }
    
    float aHeight = font->getAscenderHeight();
    std::string valueText;
    if (step > 1) {
        valueText = std::to_string((int) round(value));
    } else {
        valueText = std::to_string((int) round((value - min) / (max - min) * 100))  + "%";
    }
    std::string text = pressedInside ? valueText : labelText;
    float fWidth = font->stringWidth(text);
    font->drawString(text, px + width / 2.0 - fWidth / 2.0, py + aHeight + (height - aHeight) / 2.0);
    
    ofPopStyle();
}
