//
//  Player.cpp
//  Risk
//
//  Created by Joe Kruger on 3/19/15.
//  Copyright (c) 2015 Joe Kruger. All rights reserved.
//

#include "Player.h"

Player::Player() {
    
    armies = 0;
    alive = false;
    countriesOwned = 0;
}

int Player::getArmies() {
    return armies;
}

void Player::addArmy() {
    armies++;
}

bool Player::isAlive() {
    return alive;
}

void Player::setAlive(bool status) {
    alive = status;
}

void Player::addCountry() {
    countriesOwned++;
}

void Player::subCountry() {
    countriesOwned--;
}

int Player::getCountries() {
    return countriesOwned;
}