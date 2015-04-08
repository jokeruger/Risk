//
//  Player.h
//  Risk
//
//  Created by Joe Kruger on 3/19/15.
//  Copyright (c) 2015 Joe Kruger. All rights reserved.
//

#ifndef __Risk__Player__
#define __Risk__Player__

#include <stdio.h>

#endif /* defined(__Risk__Player__) */

class Player {
    
    int armies;
    bool alive;
    int countriesOwned;
    
public:
    Player();
    
    int getArmies();
    void addArmy();
    bool isAlive();
    void setAlive(bool status);
    void addCountry();
    void subCountry();
    int getCountries();
};