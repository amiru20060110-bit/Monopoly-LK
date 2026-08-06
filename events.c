#include <stdio.h>
#include "types.h"
#include "players.h"
#include "board.h"

void execute_event_effect(Player* player, EventType event_type,int *rent_amount, Square board[40], int current_idx); // Function prototype for executing event effects
static int current_card_index = 0;  //get the top card from the deck and apply its effect to the current player

void card_draw_cycle(Player players[4], int current_idx, Event cards[20]) {
    Event drawn_card = cards[current_card_index]; // Get the drawn card
    printf("%s drew a card: %s\n", players[current_idx].name, drawn_card.description);
    
    //Execute card effect on the player
    execute_event_effect(&players[current_idx], drawn_card.type, &drawn_card.effect_value, board, current_idx);

    //put card at bottom of the deck
    current_card_index = (current_card_index + 1) % 20;  
}

void execute_event_effect(Player* player, EventType event_type,int *rent_amount, Square board[40], int current_idx) {
    switch (event_type) {
        case Tourism_Hype:
            *rent_amount = *rent_amount * 2; // Double the rent amount for this turn
            printf("%s's hotel rent amount is doubled to LKR %d for this turn.\n", player->name, *rent_amount);
            break;
        case Fuel_Shortage:
            *rent_amount = *rent_amount * 2; // Double the rent amount for this turn
            printf("%s's railway rent doubled to LKR %d\n", player->name, *rent_amount);
            break;
        case Heavy_Floods:
            player->balance -= 1500;
            printf("%s's balance decreased by LKR 1500. New balance: LKR %d\n", player->name, player->balance);
            break;
        case Political_Rally:
            player->balance += 800;
            printf("%s's balance increased by LKR 800. New balance: LKR %d\n", player->name, player->balance);
            break;
        case Stock_Market_Rise:
            player->balance += 1200;
            printf("%s's balance increased by LKR 1200. New balance: LKR %d\n", player->name, player->balance);
            break;
        case Economic_Downturn:
            player->balance -= 1000;
            printf("%s's balance decreased by LKR 1000. New balance: LKR %d\n", player->name, player->balance);
            break;
        case Housing_Subsidy:
            player->balance += 700;
            printf("%s's balance increased by LKR 700. New balance: LKR %d\n", player->name, player->balance);
            break;
        case Interest_Rate_Cut:
            player->balance += 600;
            printf("%s's balance increased by LKR 600. New balance: LKR %d\n", player->name, player->balance);
            break;
        case Interest_Rate_Increase:
            player->balance -= 400;
            printf("%s's balance decreased by LKR 400. New balance: LKR %d\n", player->name, player->balance);
            break;
        case Tax_Amnesty:
            player->balance += 2000;
            printf("%s's balance increased by LKR 2000. New balance: LKR %d\n", player->name, player->balance); 
        case Power_Failure:
            player->balance -= 300;
            printf("%s's balance decreased by LKR 300. New balance: LKR %d\n", player->name, player->balance);
            break;
        case Foreign_Funding:
            player->balance += 1000;
            printf("%s's balance increased by LKR 1000. New balance: LKR %d\n", player->name, player->balance);
            break;
        case Port_Expansion:
            player->balance += 800; 
            printf("%s's port rent amount is increased to LKR %d for this turn.\n", player->name, *rent_amount);
        case Festival_Season:
            player->balance += 500;
            printf("%s's balance increased by LKR 500. New balance: LKR %d\n", player->name, player->balance);
            break;       
        case Labour_Strike:
            player->balance -= 700;
            printf("%s's balance decreased by LKR 700. New balance: LKR %d\n", player->name, player->balance);
            break;
        case Insurance_Discount:
            player->balance += 400;
            printf("%s's balance increased by LKR 400. New balance: LKR %d\n", player->name, player->balance);
            break;
        case Property_Revaluation:
            player->balance += 700;
            printf("%s's balance increased by LKR 700. New balance: LKR %d\n", player->name, player->balance);
            break;
        case Currency_Depreciation:
            player->balance -= 500;
            printf("%s's balance decreased by LKR 500. New balance: LKR %d\n", player->name, player->balance);
            break;
        case Government_Grant:
            player->balance += 5000;    
            printf("%s's balance increased by LKR 5000. New balance: LKR %d\n", player->name, player->balance);   
            break;
        case National_Disaster:
            player->balance -= 1500;
            printf("%s's balance decreased by LKR 1500. New balance: LKR %d\n", player->name, player->balance);     
            break;                          
        default:
            printf("Unknown event type.\n");
    }
}