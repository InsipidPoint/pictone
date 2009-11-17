/*
 *  Synth.cpp
 *  pictone
 *
 *  Created by Shiwei Song on 11/16/09.
 *  Copyright 2009 Baren Studio. All rights reserved.
 *
 */

#include "Synth.h"

using namespace std;

static bool resultPredicate(const pair<CvRect,Detector::Type>& d1, const pair<CvRect,Detector::Type>& d2) {
    return d1.first.x < d2.first.x;
}

void PluckSynth::init(Detector::Result& result) {
    sort(result.begin(), result.end(), resultPredicate);
    nNotes = result.size();
    for (int i = 0; i < nNotes; i++) {
        notes[i] = 200+100*result.at(i).second;
    }
    enabled = true;
    tickCount = 0;
}

void PluckSynth::clear() {
    enabled = false;
    tickCount = 0;
    nNotes = 0;
}

float PluckSynth::tick() {
    if (enabled) {
        if (notes[tickCount/90]>=0) {
            if(tickCount%90==0) {
                plucked->setFrequency(notes[tickCount/90]);
                plucked->pluck(1);
            }
            tickCount++;
        } else {
            tickCount = 0;
        }
        return plucked->tick();
    } else {
        return 0;
    }
}
