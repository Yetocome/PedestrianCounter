//
//  Preserver.cpp
//  PedestrianDetetion
//
//  Created by XMH on 2/25/16.
//  Copyright © 2016 XMH. All rights reserved.
//

#include "Preserver.hpp"
#define DEFAULT_PATH ""
#define DEFAULT_NAME ""

Preserver::Preserver(string Name) : file_name(Name) {
    if (file_name.empty()) {
        file_name = DEFAULT_NAME;
    }
    path = DEFAULT_PATH;
}
Preserver::~Preserver() {
    
}

void Preserver::setFilePath(string Path) {
    path = Path;
}
void Preserver::process(PDSeq& dead) {
    
}
void Preserver::process(string record) {
    
}
void Preserver::process(string name, unsigned long num) {
    
}