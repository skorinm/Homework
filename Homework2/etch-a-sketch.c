// Matt Skorina 9/13/13

#include <unistd.h>
#include <termios.h>

char getch();

void main() {

	int row;
	int col;
	system("clear");
	
	int x = 0;
	int y = 0;
	int width = 8;
	int height = 8;
	char spots[8][8]; // must be manually set to [height][width] because will not compile dynamic arrays
	for (row = 0; row < width; row++) {
		for (col = 0; col < height; col++) {
			spots[row][col] = 0;
		}
	}
	spots[y][x] = 1;

	while (1) {
		printf("wasd no wrap\n");
	printf("q to clear\n");
		for (row = 0; row < width + 1; row++) {
			for (col = 0; col < height + 1; col++) {
				if (row == 0) {
					if (col > 0) {
						printf("%i", col-1);
					} else {
						printf("  ");
					}
				} else if (col == 0) {
					printf("%i:", row-1);
				} else {
					printf("%i", spots[row-1][col-1]);
				}
			}
			printf("\n");
		}
		char* control = getch();
		//printf(&control);
		if (control == 'w') {
			y--;
			if (y < 0) {
				y = 0;
			}
		} else if (control == 's') {
			y++;
			if (y > height - 1) {
				y = height - 1;
			}
		} else if (control == 'a') {
			x--;
			if (x < 0) {
				x = 0;
			}
		} else if (control == 'd') {
			x++;
			if (x > width - 1) {
				x = width - 1;
			}
		} else if (control == 'q') {
			for (row = 0; row < width; row++) {
				for (col = 0; col < height; col++) {
					spots[row][col] = 0;
				}
			}
			//spots[y][x] = "*";
		}
		spots[y][x] = 1;
		system("clear");
	}

}

char getch() {
        char buf = 0;
        struct termios old = {0};
        if (tcgetattr(0, &old) < 0)
                perror("tcsetattr()");
        old.c_lflag &= ~ICANON;
        old.c_lflag &= ~ECHO;
        old.c_cc[VMIN] = 1;
        old.c_cc[VTIME] = 0;
        if (tcsetattr(0, TCSANOW, &old) < 0)
                perror("tcsetattr ICANON");
        if (read(0, &buf, 1) < 0)
                perror ("read()");
        old.c_lflag |= ICANON;
        old.c_lflag |= ECHO;
        if (tcsetattr(0, TCSADRAIN, &old) < 0)
                perror ("tcsetattr ~ICANON");
        return (buf);
}
