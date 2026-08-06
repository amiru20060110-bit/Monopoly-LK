#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "players.h"
#include "types.h"
#include "board.h"


int move_player(Player players[4] , int starting_player_index, int roll_round[4] , int current_idx , int i);//calls for move_player function to move the player based on the roll.
const char* get_name_string(int index); //get the name of the square based on the index
const char* get_type_string(SquareType type); //get the type of the square based on the SquareType enum
const char* get_color_group_string(ColorGroup group); //get the color group of the property based on the ColorGroup enum



int roll_dice() {
    return rand() % 6 + 1; // Returns a random number between 1 and 6
}

int roundfunc(Player players[4] , int starting_player_index) {
    int round = 1;
    int roll_round[4];
    printf("Starting Round %d...\n", round);
    for (int i = 0; i < 4; i++) { //think about 1 player at a time.His position stored in current_idx. Then move to next player.
        int current_idx = (starting_player_index + i) % 4; //wrap around the player index to ensure it stays within bounds
        int die3 = roll_dice();
        int die4 = roll_dice();
        roll_round[i] = die3 + die4;
        printf("%s rolls %d + %d = %d\n", players[current_idx].name, die3, die4, roll_round[i]);
        move_player(players, starting_player_index, roll_round, current_idx, i);//calls for move_player function to move the player based on the roll.
        
        
    }

}

void buy_property(Player players[4], int current_idx) { //buy property function to handle property purchase when a player lands on an unowned property
    int property_price = board[players[current_idx].position].property.purchase_price;
    if (players[current_idx].balance >= property_price) {

        players[current_idx].balance -= property_price;
        board[players[current_idx].position].property.owner_id = players[current_idx].id;//set owner_id of the property to the player's id
        printf("%s purchased %s for LKR %d. New Balance: LKR %d\n", players[current_idx].name, board[players[current_idx].position].name, property_price, players[current_idx].balance);
    } else {
        printf("%s does not have enough balance to purchase %s. Current Balance: LKR %d, Purchase Price: LKR %d\n", players[current_idx].name, board[players[current_idx].position].name, players[current_idx].balance, property_price);
    }
}

void pay_rent(Player players[4], int current_idx, int owner_idx, int rent_amount) {   //pay rent function to handle rent payment when a player lands on another player's property
                
                if (players[current_idx].balance >= rent_amount) {

                    players[current_idx].balance -= rent_amount;
                    players[owner_idx].balance += rent_amount;
                    printf("%s paid LKR %d in rent to Player %d. New Balance: LKR %d\n", players[current_idx].name, rent_amount, players[owner_idx].id, players[current_idx].balance);
                } else {
                    printf("%s does not have enough balance to pay the rent of LKR %d. Current Balance: LKR %d\n", players[current_idx].name, rent_amount, players[current_idx].balance);
                    // Here you can add logic for bankruptcy or other consequences.
                }
            }



//check the conditions of the square the player landed on and print appropriate messages
void condition_check_squares(const char* current_square_name, const char* current_square_type, const char* current_square_color_group, Player players[4], int current_idx) {


    if (current_square_type == "Property" || current_square_type == "Railway" || current_square_type == "Utility" || current_square_type == "Bank" || current_square_type == "Insurance") {

        if (current_square_type == "Property" && board[players[current_idx].position].property.owner_id == 0) {//property buying logic
            printf("%s landed on an unowned property: %s. It can be purchased.\n", players[current_idx].name, current_square_name); 

            if(players[current_idx].balance >= board[players[current_idx].position].property.purchase_price) {
                printf("%s has enough balance to purchase %s for LKR %d.\n", players[current_idx].name, current_square_name, board[players[current_idx].position].property.purchase_price);
                buy_property(players, current_idx); // Call the function to handle property purchase
            } else {
                printf("%s does not have enough balance to purchase %s. Current Balance: LKR %d, Purchase Price: LKR %d\n", players[current_idx].name, current_square_name, players[current_idx].balance, board[players[current_idx].position].property.purchase_price);
            }
        }
        
        else if (current_square_type == "Property" && board[players[current_idx].position].property.owner_id != players[current_idx].id) {//property rent logic
            printf("%s landed on a property owned by Player %d: %s. Rent must be paid!\n", players[current_idx].name, board[players[current_idx].position].property.owner_id, current_square_name);
            // Call the function to handle rent payment
            int rent_amount = board[players[current_idx].position].property.base_rent;
            pay_rent(players, current_idx, board[players[current_idx].position].property.owner_id, rent_amount);
        } 
        
        else {
            printf("%s landed on their own property: %s.\n", players[current_idx].name, current_square_name);//player landed on their own property
        }


    } else if (current_square_type == "Tax") {
        printf("%s landed on a Tax square: %s. Tax must be paid.\n", players[current_idx].name, current_square_name);    
    }
    
    else if (current_square_type == "Event") {
        printf("%s landed on an Event square: %s. A National Event will occur.\n", players[current_idx].name, current_square_name);
        void card_draw_cycle(Player players[4], int current_idx, Event cards[20]); // Declare the function prototype
        card_draw_cycle(players, current_idx, cards); // Call the function to handle the event

    } 
    
    else if (current_square_type == "Jail") {
        printf("%s landed on Jail: %s. They are just visiting.\n", players[current_idx].name, current_square_name);
    } else if (current_square_type == "Special Square") {
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
        {1, "Aggressive Investor", 30000, 0},
        {2, "Conservative Banker", 30000, 0},
        {3, "Risk-Taker", 30000, 0},
        {4, "Opportunistic Trader", 30000, 0}
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

    roundfunc(players, starting_player_index);  //start the rounds.

}



