//
//  Country.h
//  Risk
//
//  Created by Joe Kruger on 3/11/15.
//  Copyright (c) 2015 Joe Kruger. All rights reserved.
//

#ifndef __Risk__Country__
#define __Risk__Country__

#include <stdio.h>
#include "string"

using namespace std;

#endif /* defined(__Risk__Country__) */

class Country {

    string countryName;
    int xCoordinate;
    int yCoordinate;
    
    int colorR;
    int colorG;
    int colorB;
    
    int armies;
    int owner;
public:
    Country(int x, int y, string name, int r, int g, int b);
    
    string getName();
    int getX();
    int getY();
    int getR();
    int getG();
    int getB();
    int getArmies();
    void addArmies(int n);
    void subArmies(int n);
    int getOwner();
    void setOwner(int player);
    void setArmies(int n);
};