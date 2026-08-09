//its hard programme
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>//for string comparison functions
#include "players.h"
#include "types.h"
#include "board.h"

#ifdef _WIN32
    #include <windows.h>
    void wait_ms(int ms) {
        Sleep(ms); // Windows Sleep uses milliseconds
    }
#else
    #include <unistd.h>
    void wait_ms(int ms) {
        usleep(ms * 1000); // POSIX usleep uses microseconds
    }
#endif


int move_player(Player players[4] , int starting_player_index, int roll_round[4] , int current_idx , int i);//calls for move_player function to move the player based on the roll.
const char* get_name_string(int index); //get the name of the square based on the index
const char* get_type_string(SquareType type); //get the type of the square based on the SquareType enum
const char* get_color_group_string(ColorGroup group); //get the color group of the property based on the ColorGroup enum



int roll_dice() {
    return rand() % 6 + 1; // Returns a random number between 1 and 6
}

//update rent multiplier and remaining rounds for rent boosts
void update_rent_boosts(Player *player ){
    if(player->rent_boosts_rounds_remaining > 0) {
        player->rent_boosts_rounds_remaining--;
        if (player->rent_boosts_rounds_remaining == 0) {
            player->rent_multiplier = 1; // Reset rent multiplier to normal
            printf("%s's rent boost has expired. Rent multiplier reset to %d.\n", player->name, player->rent_multiplier);
        }
    }
}

int roundfunc(Player players[4] , int starting_player_index ,int round) {
    
    int roll_round[4];
    printf("Starting Round %d...\n", round);
    for (int i = 0; i < 4; i++) { //think about 1 player at a time.His position stored in current_idx. Then move to next player.
        int current_idx = (starting_player_index + i) % 4; //wrap around the player index to ensure it stays within bounds
        int die3 = roll_dice();
        int die4 = roll_dice();
        roll_round[i] = die3 + die4;
        printf("%s rolls %d + %d = %d\n", players[current_idx].name, die3, die4, roll_round[i]);        
        move_player(players, starting_player_index, roll_round, current_idx, i);//calls for move_player function to move the player based on the roll.
        update_rent_boosts(&players[current_idx]); // Call the function to update rent boosts for the current player
    }
}

void buy_property_railway(Player *player, const char* current_square_type) { //buy property function to handle property purchase when a player lands on an unowned property or railway
    int railway_price = 1500;
    int property_price = board[player->position].property.purchase_price;
    if(strcmp(current_square_type, "Property") == 0){//property purchase logic
                 player->balance -= property_price;
                 board[player->position].property.owner_id = player->id;//set owner_id of the property to the player's id
                 printf("%s purchased %s for LKR %d. New Balance: LKR %d\n", player->name, board[player->position].name, property_price, player->balance);        
                 printf("%s now owns %s.\n", player->name, board[player->position].name);

    }
    if(strcmp(current_square_type, "Railway") == 0){//railway purchase logic
                 player->balance -= railway_price;
                 board[player->position].property.owner_id = player->id;//set owner_id of the property to the player's id
                 printf("%s purchased %s for LKR %d. New Balance: LKR %d\n", player->name, board[player->position].name, railway_price, player->balance);  
                 printf("%s now owns %s.\n", player->name, board[player->position].name);      

    }
       
}



void pay_rent_property_railway(Player *player, Player *owner, const char* current_square_type, int rent_amount) {   //pay rent function to handle rent payment when a player lands on another player's property
    if(strcmp(current_square_type, "Property") == 0) {
                
                if (player->balance >= rent_amount) {

                    player->balance -= rent_amount;
                    owner->balance += rent_amount;
                    printf("%s paid LKR %d in rent to Player %d. New Balance: LKR %d\n", player->name, rent_amount, owner->id, player->balance);
                } else {
                    printf("%s does not have enough balance to pay the rent of LKR %d. Current Balance: LKR %d\n", player->name, rent_amount, player->balance);
                    // Here you can add logic for bankruptcy or other consequences.
                }
    }
    if(strcmp(current_square_type, "Railway") == 0) {
                
                if (player->balance >= rent_amount) {

                    player->balance -= rent_amount;
                    owner->balance += rent_amount;
                    printf("%s paid LKR %d in railway rent to Player %d. New Balance: LKR %d\n", player->name, rent_amount, owner->id, player->balance);
                } else {
                    printf("%s does not have enough balance to pay the railway rent of LKR %d. Current Balance: LKR %d\n", player->name, rent_amount, player->balance);
                    // Here you can add logic for bankruptcy or other consequences.
                }
    }
}


//check the conditions of the square the player landed on and print appropriate messages
void condition_check_squares(const char* current_square_name, const char* current_square_type, const char* current_square_color_group, Player players[4], int current_idx) {
    if (strcmp(current_square_type, "Property") == 0) {

        if ((strcmp(current_square_type, "Property") == 0) && board[players[current_idx].position].property.owner_id == -1) {//property purchase logic
            printf("%s landed on an unowned property: %s. It can be purchased.\n", players[current_idx].name, current_square_name); 

            if(players[current_idx].balance >= board[players[current_idx].position].property.purchase_price) {
                printf("%s has enough balance to purchase %s for LKR %d.\n", players[current_idx].name, current_square_name, board[players[current_idx].position].property.purchase_price);
                buy_property_railway(&players[current_idx], current_square_type); // Call the function to handle property purchase
            } else {
                printf("%s does not have enough balance to purchase %s. Current Balance: LKR %d, Purchase Price: LKR %d\n", players[current_idx].name, current_square_name, players[current_idx].balance, board[players[current_idx].position].property.purchase_price);
            }
        }
        
        else if (strcmp(current_square_type, "Property") == 0 && board[players[current_idx].position].property.owner_id != players[current_idx].id) {//property rent logic
            printf("%s landed on a property owned by Player %d: %s. Rent must be paid!\n", players[current_idx].name, board[players[current_idx].position].property.owner_id, current_square_name);
            int rent_amount = board[players[current_idx].position].property.base_rent*players[current_idx].rent_multiplier; // Calculate rent based on the player's rent multiplier
            
            pay_rent_property_railway(&players[current_idx], &players[board[players[current_idx].position].property.owner_id], current_square_type, rent_amount);
        } 
        else {
            printf("%s landed on their own property: %s.\n", players[current_idx].name, current_square_name);//player landed on their own property
        }
    } 


    if(strcmp(current_square_type, "Railway") == 0){
             if (strcmp(current_square_type, "Railway") == 0 && board[players[current_idx].position].property.owner_id == -1) { //logic for unowned railway squares
             printf("%s landed on an unowned railway: %s. It can be purchased.\n", players[current_idx].name, current_square_name);
             // Call the function to handle railway purchase
             if (players[current_idx].balance >= 1500) {
                  printf("%s has enough balance to purchase %s for LKR %d.\n", players[current_idx].name, current_square_name, 1500);
                  buy_property_railway(&players[current_idx], current_square_type); // Call the function to handle railway purchase
             } else {
                  printf("%s does not have enough balance to purchase %s. Current Balance: LKR %d, Purchase Price: LKR %d\n", players[current_idx].name, current_square_name, players[current_idx].balance, 1500);
             }
             
    
             }
             else if(strcmp(current_square_type, "Railway") == 0 && board[players[current_idx].position].property.owner_id != players[current_idx].id){//railway rent logic
                printf("%s landed on a railway owned by Player %d: %s. Rent must be paid!\n", players[current_idx].name, board[players[current_idx].position].property.owner_id, current_square_name);
                // Call the function to handle rent payment
                int owner_id = board[players[current_idx].position].property.owner_id;
                pay_rent_property_railway(&players[current_idx], &players[owner_id], current_square_type, 250*players[current_idx].rent_multiplier); // Calculate rent based on the player's rent multiplier
             }
             else{
                printf("%s landed on their own railway: %s.\n", players[current_idx].name, current_square_name);//player landed on their own railway
             }
    }

    else if (strcmp(current_square_type, "Tax") == 0) {
        printf("%s landed on a Tax square: %s. Tax must be paid.\n", players[current_idx].name, current_square_name);    
    }
    
    else if (strcmp(current_square_type, "Event") == 0) {
        printf("%s landed on an Event square: %s. A National Event will occur.\n", players[current_idx].name, current_square_name);
        void card_draw_cycle(Player players[4], int current_idx, Event cards[20]); // Declare the function prototype
        card_draw_cycle(players, current_idx, cards); // Call the function to handle the event
    } 
    
    else if (strcmp(current_square_type, "Jail") == 0) {
        printf("%s landed on Jail: %s. They are just visiting.\n", players[current_idx].name, current_square_name);
    } else if (strcmp(current_square_type, "Special Square") == 0) {
        printf("%s landed on a Special square: %s. No action is required.\n", players[current_idx].name, current_square_name);
    }
}


int move_player(Player players[4] , int starting_player_index, int roll_round[4] , int current_idx , int i) {
    //move to new square based on roll_round[i] and update player position
    players[current_idx].position = (players[current_idx].position + roll_round[i]) % 40; // Assuming a board with 40 squares
        printf("%s moves from square %d to square %d\n", players[current_idx].name, players[current_idx].position - roll_round[i], players[current_idx].position);
        int x = players[current_idx].position;
        const char* current_square_name = get_name_string(x);
        const char* current_square_type = get_type_string(board[x].type);
        const char* current_square_color_group = get_color_group_string(board[x].property.group);
        printf("%s landed on square %d: %s (Type: %s, Color Group: %s)\n", players[current_idx].name, x, current_square_name, current_square_type, current_square_color_group);
        condition_check_squares(current_square_name, current_square_type, current_square_color_group, players, current_idx);

        //passing go
        if (players[current_idx].position < roll_round[i]) {
            printf("%s passed GO and collects LKR2000!\n", players[current_idx].name);
            players[current_idx].balance += 2000; // Assuming passing GO gives LKR2000
            printf("%s passed GO. Collected LKR 2000.00\n Current Balance: LKR %d\n", players[current_idx].name, players[current_idx].balance);
            
        }
}



const char* get_name_string(int index) { //get the name of the square based on the index
    if (index < 0 || index >= 40) {
        return "Invalid Square Index";
    }
    return board[index].name;
}

const char* get_type_string(SquareType type) { //get the type of the square based on the SquareType enum
    switch (type) {
        case PROPERTY: return "Property";
        case RAILWAY: return "Railway";
        case UTILITY: return "Utility";
        case BANK: return "Bank";
        case INSURANCE: return "Insurance";
        case TAX: return "Tax";
        case EVENT: return "Event";
        case JAIL: return "Jail";
        case SQUARE_SPECIAL: return "Special Square";
        default: return "Unknown Type";
    }
}

const char* get_color_group_string(ColorGroup group) {  //get the color group of the property based on the ColorGroup enum
    switch (group) {
        case GROUP_NONE: return "None";
        case GROUP_1: return "Brown";
        case GROUP_2: return "Light Blue";
        case GROUP_3: return "Pink";
        case GROUP_4: return "Orange";
        case GROUP_5: return "Red";
        case GROUP_6: return "Yellow";
        case GROUP_7: return "Green";
        case GROUP_8: return "Dark Blue";
        default: return "Unknown Group";
    }
}


int main() {
    srand(time(NULL)); // Seed the random number generator
    Player players[4] = {
        {0, "Aggressive Investor", 30000, 0, 1, 0},
        {1, "Conservative Banker", 30000, 0, 1, 0},
        {2, "Risk-Taker", 30000, 0, 1, 0},
        {3, "Opportunistic Trader", 30000, 0, 1, 0}
    };
    int roll[4];
    int max_roll = 0;
    int starting_player_index = 0;

    printf("Rolling 6 sided 2 dices 4 times\n");
    for (int i = 0; i < 4; i++) { 
        int die1 = roll_dice();
        int die2 = roll_dice();
        roll[i] = die1 + die2;
        printf("%s rolls %d\n", players[i].name, roll[i]); 
        if (roll[i] > max_roll) {
            max_roll = roll[i];
            starting_player_index = i;
        }
    }   
    printf("%s who rolled %d will start the game.\n", players[starting_player_index].name, max_roll);
    printf("turn order:\n");
    for (int i = 0; i < 4; i++) {  //wrap around the player index to ensure it stays within bounds
        int player_index = (starting_player_index + i) % 4;
        printf("%s\n", players[player_index].name);
    }


    for (int round = 1; round <= 20; round++) {  // Simulate 20 rounds
        printf("\n--- Round %d ---\n", round);
        roundfunc(players, starting_player_index,round);  //start the rounds
        //time delay between rounds        
        wait_ms(1000); // Wait for 1 second
    }

    wait_ms(10000); // Wait for 10 seconds before ending the game
    //end of this round
}



