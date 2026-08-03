#include <stdio.h>

#ifdef _WIN32
    #include <windows.h>
    void wait_ms(int ms) {
        Sleep(ms);
    }
#endif

int main() {
    printf("MONOPOLY-LK Simulation\n");
    wait_ms(2000); // Wait for 1 second
    printf("Player 1 : Aggressive Investor\n");
    printf("Player 2 : Conservative Banker\n");
    printf("Player 3 : Risk-Taker\n");
    printf("Player 4 : Opportunistic Trader\n");

    wait_ms(2000); // Wait for 1 second
    printf("Each player begins with Rs 30000\n");
    return 0;
}