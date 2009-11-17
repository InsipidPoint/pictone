/*
 *  Synth.h
 *  pictone
 *
 *  Created by Shiwei Song on 11/16/09.
 *  Copyright 2009 Baren Studio. All rights reserved.
 *
 */
#pragma once
#include "Detector.h"
#include "Plucked.h"

using namespace stk;

//bool resultPredicate(const pair<CvRect,Detector::Type>& d1, const pair<CvRect,Detector::Type>& d2);

class Synth {
public:
    virtual void init(Detector::Result&) {};
    virtual void clear() {};
    virtual float tick() { return 0; };
};

class PluckSynth : public Synth {
public:
    PluckSynth() : enabled(false), nNotes(0), tickCount(0) { plucked = new Plucked(200); };
    ~PluckSynth() { delete plucked; };
    void init(Detector::Result&);
    void clear();
    float tick();
    
private:
    bool enabled;
    int nNotes;
    float notes[100];
    Plucked *plucked;
    unsigned long tickCount;
};
