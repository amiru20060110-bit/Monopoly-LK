#ifndef PLAYERS_H
#define PLAYERS_H
#include <stdio.h>

typedef struct {
    int id;
    char name[50];
    int balance;
    int position;
    int rent_multiplier; // Multiplier for rent due to event effects
    int rent_boosts_rounds_remaining; // Number of turns the rent boost effect remains active
    float market_rise_multiplier; // Multiplier for property value due to event effects
    float market_downturn_multiplier; // Multiplier for property value due to event effects
    int active_loan; //loan state
    int loan_size; //loan size
    int loan_rounds_remaining; //check available rounds for loan ending
    

} Player;

#endif // PLAYERS_H
