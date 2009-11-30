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
#include "Bowed.h"
#include "BlowBotl.h"

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
    MultiSynth() : enabled(false), ticks(0), progress(0), cmdIdx(0), playIdx(-1) {
        clarinet = new Clarinet(200);
        saxofony = new Saxofony(200);
        plucked = new Plucked(200);
        bowed = new Bowed(200);
        bottle = new BlowBotl();
        current = clarinet;
    };
    ~MultiSynth() { delete clarinet; };
    void init(Detector::Result&);
    void clear();
    StkFloat tick();
    int playIdx;
private:
    enum mode {
        freq = -2,
        silence = -1,
        clarinet_on = 0,
        clarinet_off,
        saxofony_on,
        saxofony_off,
        plucked_on,
        plucked_off,
        bowed_on,
        bowed_off,
        bottle_on,
        bottle_off
    };
    struct Cmd {
        mode m;
        float freq;
        float delay;
        int idx;
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
    Bowed *bowed;
    BlowBotl *bottle;
    
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
