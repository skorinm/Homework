// Matt Skorina 9/13/13

// Needed for getch
#include <unistd.h>
#include <termios.h>

// From Yoder's gpio example 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <signal.h>	// Defines signal-handling functions (i.e. trap Ctrl-C)
#include "gpio-utils.c"

// Hold GPIO numbers for buttons
#define left_button 48
#define right_button 3
#define up_button 2
#define down_button 49


char getch();
void updateScreen();
int buttonInput(int *gpio_fd);

int toggle = 0;

int row;
	int col;
int x = 0;
	int y = 0;
	int width = 8;
	int height = 8;
	char spots[8][8]; // must be manually set to [height][width] because will not compile dynamic arrays

void main() {
gpio_set_value(30, toggle);
	system("clear"); // clear screen so future clears don't look weird
	
		int gpio_fd[4];
	
	// Setup buttons
	gpio_export(left_button);
	gpio_set_dir(left_button, "in");
	gpio_set_edge(left_button, "falling");
	gpio_fd[0] = gpio_fd_open(left_button, O_RDONLY);
	
	gpio_export(right_button);
	gpio_set_dir(right_button, "in");
	gpio_set_edge(right_button, "falling");
	gpio_fd[1] = gpio_fd_open(right_button, O_RDONLY);
	
	gpio_export(up_button);
	gpio_set_dir(up_button, "in");
	gpio_set_edge(up_button, "falling");
	gpio_fd[2] = gpio_fd_open(up_button, O_RDONLY);
	
	gpio_export(down_button);
	gpio_set_dir(down_button, "in");
	gpio_set_edge(down_button, "falling");
	gpio_fd[3] = gpio_fd_open(down_button, O_RDONLY);
	
	
	
	
	
	

		
	
	
	
	for (row = 0; row < width; row++) {
		for (col = 0; col < height; col++) {
			spots[row][col] = 0;
		}
	}
	spots[y][x] = 1;

	while (1) {
		
		updateScreen(spots);
		int control = buttonInput(gpio_fd);
		//char* control = "w";
		//char* control = getch(); // Used for keyboard control, this method is blocking so can not be used along with button control
		
		//printf(&control);
		if (control == 0) {
			y--;
			if (y < 0) {
				y = 0;
			}
		} else if (control == 1) {
			y++;
			if (y > height - 1) {
				y = height - 1;
			}
		} else if (control == 2) {
			x--;
			if (x < 0) {
				x = 0;
			}
		} else if (control == 3) {
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
		}
		spots[y][x] = 1;
		system("clear");
	}

}

int buttonInput(int *gpio_fd) {

struct pollfd fdset[4];
memset((void*)fdset, 0, sizeof(fdset));

int i;
for(i = 0; i < 4; i++){
	fdset[i].fd = gpio_fd[i];
	fdset[i].events = POLLPRI;
}

int rc = poll(fdset, 4, -1);
if (rc < 0){
	return -1;
}


int len;
for (i = 0; i < 4; i++){
	if (fdset[i].revents & POLLPRI){
	char buf[64];
	buf[0] = '\0';
		read(fdset[i].fd, buf, 1);
		if (buf[0] == 0) {
			//fprintf(stderr, "%d\n",i);
			return i;
		}
	}
}
return -1; 
	
}


void updateScreen() {
//printf("wasd no wrap\n");
//	printf("q to clear\n");
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
}

// Suggested by Mike McDonald
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
