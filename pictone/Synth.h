/*
 *  Synth.h
 *  pictone
 *
 *  Created by Shiwei Song on 11/16/09.
 *  Copyright 2009 Baren Studio. All rights reserved.
 *
 */
#pragma once
#include <vector>
#include <utility>
#include "Detector.h"
#include "Plucked.h"
#include "Clarinet.h"
#include "Saxofony.h"
#include "Shakers.h"

using namespace stk;
using namespace std;

//bool resultPredicate(const pair<CvRect,Detector::Type>& d1, const pair<CvRect,Detector::Type>& d2);

class Synth {
public:
    virtual void init(Detector::Result&) {};
    virtual void clear() {};
    virtual StkFloat tick() { return 0; };
    virtual void frame() {};
};

class MultiSynth : public Synth {
public:
    MultiSynth() : enabled(false), ticks(0), progress(0), cmdIdx(0) {
        clarinet = new Clarinet(200);
        saxofony = new Saxofony(200);
        plucked = new Plucked(200);
        shakers = new Shakers();
        current = clarinet;
    };
    ~MultiSynth() { delete clarinet; };
    void init(Detector::Result&);
    void clear();
    StkFloat tick();
private:
    enum mode {
        silence = -1,
        clarinet_noop = 0,
        clarinet_freq,
        clarinet_on,
        clarinet_off,
        saxofony_noop,
        saxofony_freq,
        saxofony_on,
        saxofony_off,
        plucked_noop,
        plucked_freq,
        plucked_on,
        plucked_off,
        shakers_noop,
        shakers_on,
        shakers_off
    };
    struct Cmd {
        mode m;
        float freq;
        float delay;
    };
    vector<Cmd> cmds;
    
    double progress;
    unsigned cmdIdx;
    unsigned long long ticks;
    bool enabled;
    
    Instrmnt *current;
    Clarinet *clarinet;
    Saxofony *saxofony;
    Plucked *plucked;
    Shakers *shakers;
    
    float calculateFrequency(CvRect);
    float calculateFrequency(CvPoint);
};

class PluckSynth : public Synth {
public:
    PluckSynth() : enabled(false), nNotes(0), tickCount(0) { plucked = new Plucked(200); };
    ~PluckSynth() { delete plucked; };
    void init(Detector::Result&);
    void clear();
    StkFloat tick();
    void frame() { if(enabled) tickCount++; };
    
private:
    bool enabled;
    int nNotes;
    float notes[100];
    Plucked *plucked;
    unsigned long tickCount;
};
