#include <stdio.h>
#include <stdlib.h>

#define WIDTH 60
#define HEIGHT 25

//intensity vals = ampped from cold to hot

char palette[] = " ...::;;;+++===+*#%%@@@@";
int PALETTE_LEN ;


int fire[HEIGHT][WIDTH];

//fxn make bottom row of heat max -> void / modifies array


void seed_bottom_row() {
	for (int x = 0; x < WIDTH; x++)
	{
		fire[HEIGHT - 1][x] = PALETTE_LEN  - 1; //max heat  
	}

}
void render() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            int intensity = fire[y][x];
            putchar(palette[intensity]);
        }
        putchar('\n');
    }
}

int main() {
    PALETTE_LEN = sizeof(palette) - 1; // -1 drops the null terminator
    seed_bottom_row();
    render();
    return 0;
}