#ifndef TYPES_H
#define TYPES_H

//square types
typedef enum {
    PROPERTY,
    RAILWAY,
    UTILITY,
    BANK,
    INSURANCE,
    TAX,
    EVENT,
    JAIL,
    SQUARE_SPECIAL    
} SquareType;//made a new data type called SquareType which is an enum that contains all the different types of squares on the board

//enum for color groups
typedef enum {
    GROUP_NONE = 0, // For non-property squares
    GROUP_1, // Brown
    GROUP_2,     // Light Blue
    GROUP_3,     // Pink
    GROUP_4,     // Orange
    GROUP_5,     // Red
    GROUP_6,     // Yellow
    GROUP_7,     // Green
    GROUP_8      // Dark Blue
} ColorGroup;//made a new data type called ColorGroup which is an enum that contains all the different color groups of properties on the board
//struct for property
typedef struct {
    ColorGroup group;  // Color group of the property
    int purchase_price;  // Cost to buy
    int mortgage_value;  // Mortgage value
    int base_rent;       // Base rent rate
    int house_cost;     // 0 to 4
    int hotel_cost;     // 0 or 1
    int house_count;    // Number of houses (0-4)
    int hotel_count;    // Number of hotels (0 or 1)
    int owner_id;        // Player ID of the owner (0 if unowned)

} Property;//made a new data type called Property which is a struct that contains all the different properties of a property square on the board

//struct for square

typedef struct {
    int index;
    char name[50];
    SquareType type;
    Property property; // Only relevant for PROPERTY, RAILWAY, UTILITY, BANK, INSURANCE types

} Square;//made a new data type called Square which is a struct that contains a SquareType variable called type

typedef enum{
    Tourism_Hype,
    Fuel_Shortage,
    Heavy_Floods,
    Political_Rally,
    Stock_Market_Rise,
    Economic_Downturn,
    Housing_Subsidy,
    Interest_Rate_Cut,
    Interest_Rate_Increase,
    Tax_Amnesty,
    Power_Failure,
    Foreign_Funding,
    Port_Expansion,
    Festival_Season,
    Labour_Strike,
    Insurance_Discount,
    Property_Revaluation,
    Currency_Depreciation,
    Government_Grant,
    National_Disaster

} EventType;//made a new data type called EventType which is an enum that contains all the different types of events that can occur on an event square on the board

typedef struct {
    int event_id; // Unique identifier for the event
    char description[100]; // Description of the event
    EventType type; // Type of effect (e.g., gain money, lose money, move to a square, etc.)
    int effect_value; // Value associated with the effect (e.g., amount of money gained or lost)
   
} Event;//made a new data type called Event which is a struct that contains all the different attributes of an event square on the board


#endif // TYPES_H





