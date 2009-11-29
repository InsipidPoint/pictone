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

static bool resultPredicate(const Detector::Shape& d1, const Detector::Shape& d2) {
    return d1.rect.x < d2.rect.x;
}

void MultiSynth::init(Detector::Result& result) {
    enabled = false;
//    clarinet->noteOn(440, 1);
    sort(result.begin(), result.end(), resultPredicate);
    Cmd prev;
    Cmd c, back;
    for (int i = 0; i < result.size(); i++) {
        CvRect rect = result.at(i).rect;
//        float y = rect.y + rect.height/2.0;
        switch (result.at(i).type) {
            case Detector::TRIANGLE_UP:
                c.m = clarinet_on; c.freq = calculateFrequency(rect); c.delay = 0.6;
                cmds.push_back(c);
                c.m = clarinet_off; c.delay = 0.4;
                cmds.push_back(c);
                break;
            case Detector::SQUARE:
                c.m = saxofony_on; c.freq = calculateFrequency(rect); c.delay = 0.6;
                cmds.push_back(c);
                c.m = saxofony_off; c.delay = 0.4;
                cmds.push_back(c);
                break;
            case Detector::PIN:
                c.m = plucked_on; c.freq = calculateFrequency(rect); c.delay = 0.6;
                cmds.push_back(c);
                c.m = plucked_off; c.delay = 0.4;
                cmds.push_back(c);
                break;
            case Detector::STAR:
                c.m = shakers_on; c.freq = 440; c.delay = 0.6;
                cmds.push_back(c);
                c.m = shakers_off; c.delay = 0.4;
                cmds.push_back(c);
                break;
            case Detector::UNKNOWN:
                if (cmds.size() > 0) {
                    prev = cmds.back();
                    cmds.pop_back();
                    back = cmds.back();
                    back.delay = 0.1;
                    cmds.pop_back();
                    cmds.push_back(back);
                    c.m = freq;
                    for (int j = 0; j < result.at(i).pts.size(); j+=2) {
                        c.freq = calculateFrequency(result.at(i).pts.at(j)); c.delay = 0.05;
                        cmds.push_back(c);
                    }
                    cmds.push_back(prev);
                }
                break;
            default:
                break;
        }
    }
    ticks = 0;
    progress = 0;
    cmdIdx = 0;
    enabled = true;
}

void MultiSynth::clear() {
//    clarinet->noteOff(0.1);
    
    enabled = false;
    ticks = 0;
    cmds.clear();
    clarinet->clear();
    progress = 0;
    cmdIdx = 0;
}

#define t 8820
StkFloat MultiSynth::tick() {
    if (enabled) {
//        return clarinet->tick();
        if (progress == 0) {
            switch (cmds.at(cmdIdx).m) {
                case clarinet_on:
                    clarinet->noteOn(cmds.at(cmdIdx).freq, 1);
                    current = clarinet;
                    break;
                case clarinet_off:
                    clarinet->noteOff(1);
                    break;
                case saxofony_on:
                    saxofony->noteOn(cmds.at(cmdIdx).freq, 1);
                    current = saxofony;
                    break;
                case saxofony_off:
                    saxofony->noteOff(1);
                    break;
                case plucked_on:
                    plucked->noteOn(cmds.at(cmdIdx).freq, 1);
                    current = plucked;
                    break;
                case plucked_off:
                    plucked->noteOff(1);
                    break;
                case shakers_on:
                    shakers->noteOn(5, 1);
                    shakers->controlChange(1, cmds.at(cmdIdx).freq);
                    current = shakers;
                    break;
                case shakers_off:
                    shakers->noteOff(1);
                    break;
                case freq:
                    current->setFrequency(cmds.at(cmdIdx).freq);
                    break;
                default:
                    break;
            }
        }

        ticks++;
        progress = ticks/44100.0;
        
        if (progress >= cmds.at(cmdIdx).delay) {
            progress = 0;
            cmdIdx = (cmdIdx+1)%cmds.size();
            ticks = 0;
        }
        
        return current->tick();
    } else {
        return 0;
    }

}

float MultiSynth::calculateFrequency(CvRect rect) {
    float y = rect.y + rect.height/2.0;
    
    return 220*pow(2, (316-y)/151.68);
}
                                       
float MultiSynth::calculateFrequency(CvPoint pt) {   
   return 220*pow(2, (316-pt.y)/151.68);
}
                               
void PluckSynth::init(Detector::Result& result) {
    sort(result.begin(), result.end(), resultPredicate);
    nNotes = result.size();
    for (int i = 0; i < nNotes; i++) {
        notes[i] = 200+100*result.at(i).type;
    }
    enabled = true;
    tickCount = 0;
}

void PluckSynth::clear() {
    enabled = false;
    tickCount = 0;
    nNotes = 0;
}

StkFloat PluckSynth::tick() {
    if (enabled) {
        if (tickCount/90 < nNotes) {
            if(tickCount%90==0) {
                plucked->setFrequency(notes[tickCount/90]);
                plucked->pluck(1);
            }
        } else {
            tickCount = 0;
        }
        return plucked->tick();
    } else {
        return 0;
    }
}
