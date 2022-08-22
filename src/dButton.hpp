//
// dButton.hpp
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

#pragma once

#include "ofMain.h"

class dButton {
public:
    void setup(float x, float y, float w, float h, std::string label);
    void setLabel(std::string label);
    
    void handlePressed(float x, float y);
    void handleMove(float x, float y);
    void handleReleased(float x, float y);
    
    void update();
    void draw();
    
    bool hide = false;
    
    ofTrueTypeFont* font;
    bool getClick() {
        return clicked;
    };
    
protected:
    float px;
    float py;
    float width;
    float height;
    std::string labelText;
    
    bool clicked = false;
    bool pressedInside = false;
    bool hover = false;
    
    bool getMouseInside(float x, float y);
};

class dSlider : public dButton {
public:
    void handlePressed(float x, float y);
    void handleMove(float x, float y);
    void handleReleased(float x, float y);
    
    void draw();
    
    float min = 0;
    float max = 1;
    int step = 0;
    
    float getValue() {
        return value;
    };
    
    void setValue(float v) {
        value = v;
    };
    
private:
    float value = 0;
    
    float getValueFromMousePos(float x, float y);
};
