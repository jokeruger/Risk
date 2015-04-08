//
//  Country.cpp
//  Risk
//
//  Created by Joe Kruger on 3/11/15.
//  Copyright (c) 2015 Joe Kruger. All rights reserved.
//

#include "Country.h"
#include "string"

using namespace std;

Country::Country(int x, int y, string name, int r, int g, int b) {
    countryName = name;
    xCoordinate = x;
    yCoordinate = y;
    colorR = r;
    colorG = g;
    colorB = b;
    armies = 0;
    owner = 0;
}

string Country::getName() {
    return countryName;
}

int Country::getX() {
    return xCoordinate;
}

int Country::getY() {
    return yCoordinate;
}

int Country::getR() {
    return colorR;
}

int Country::getG() {
    return colorG;
}

int Country::getB() {
    return colorB;
}

int Country::getArmies() {
    return armies;
}

void Country::addArmies(int n) {
    armies = armies + n;
}

void Country::subArmies(int n) {
    armies = armies - n;
}

void Country::setArmies(int n) {
    armies = n;
}

int Country::getOwner() {
    return owner;
}

void Country::setOwner(int player) {
    owner = player;
}
