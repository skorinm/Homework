// I, Matt Skorina, was unable to get i2c to compile under my own etch a sketch file. So I'm just hijacking this and it's makefile.


/*
    my2cset.c - First try at controlling Adafruit 8x8matrix.
    Mark A. Yoder, 14-Aug-2012.
    Mark A. Yoder, 26-Oct-2012.  Cleaned up.
    Page numbers are from the HT16K33 manual
    http://www.adafruit.com/datasheets/ht16K33v110.pdf
*/
/*
    i2cset.c - A user-space program to write an I2C register.
    Copyright (C) 2001-2003  Frodo Looijaard <frodol@dds.nl>, and
                             Mark D. Studebaker <mdsxyz123@yahoo.com>
    Copyright (C) 2004-2010  Jean Delvare <khali@linux-fr.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
    MA 02110-1301 USA.
*/


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

#include <math.h>

#define left_button 48
#define right_button 3
#define up_button 2
#define down_button 49
#define clear_button 51


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
	
	

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "i2c-dev.h"
#include "i2cbusses.h"

#define TEST
#define BICOLOR		// undef if using a single color display

// The upper btye is RED, the lower is GREEN.
// The single color display responds only to the lower byte
static __u16 smile_bmp[]=
	{0xAA, 0xff, 0xa9, 0x85, 0x85, 0xa9, 0x42, 0x3c}; //0x3c
static __u16 frown_bmp[]=
	{0x3c00, 0x42ff, 0xa900, 0x8500, 0x8500, 0xa900, 0x4200, 0x3c00};
static __u16 neutral_bmp[]=
	{0xff00, 0x00ff, 0xa9a9, 0x8989, 0x8989, 0xa9a9, 0x4242, 0x3c3c};

static void help(void) __attribute__ ((noreturn));

static void help(void) {
	fprintf(stderr, "Usage: matrixLEDi2c (hardwired to bus 3, address 0x70)\n");
	exit(1);
}

static int check_funcs(int file) {
	unsigned long funcs;

	/* check adapter functionality */
	if (ioctl(file, I2C_FUNCS, &funcs) < 0) {
		fprintf(stderr, "Error: Could not get the adapter "
			"functionality matrix: %s\n", strerror(errno));
		return -1;
	}

	if (!(funcs & I2C_FUNC_SMBUS_WRITE_BYTE)) {
		fprintf(stderr, MISSING_FUNC_FMT, "SMBus send byte");
		return -1;
	}
	return 0;
}

// Writes block of data to the display
static int write_block(int file, __u16 *data) {
	int res;
#ifdef BICOLOR
	res = i2c_smbus_write_i2c_block_data(file, 0x00, 16, 
		(__u8 *)data);
	return res;
#else
/*
 * For some reason the single color display is rotated one column, 
 * so pre-unrotate the data.
 */
	int i;
	__u16 block[I2C_SMBUS_BLOCK_MAX];
//	printf("rotating\n");
	for(i=0; i<8; i++) {
		block[i] = (data[i]&0xfe) >> 1 | 
			   (data[i]&0x01) << 7;
	}
	res = i2c_smbus_write_i2c_block_data(file, 0x00, 16, 
		(__u8 *)block);
	return res;
#endif
}

int main(int argc, char *argv[])
{
	int res, i2cbus, address, file;
	char filename[20];
	int force = 0;

	i2cbus = lookup_i2c_bus("1");
	printf("i2cbus = %d\n", i2cbus);
	if (i2cbus < 0)
		help();

	address = parse_i2c_address("0x70");
	printf("address = 0x%2x\n", address);
	if (address < 0)
		help();

	file = open_i2c_dev(i2cbus, filename, sizeof(filename), 0);
//	printf("file = %d\n", file);
	if (file < 0
	 || check_funcs(file)
	 || set_slave_addr(file, address, force))
		exit(1);

	// Check the return value on these if there is trouble
	i2c_smbus_write_byte(file, 0x21); // Start oscillator (p10)
	i2c_smbus_write_byte(file, 0x81); // Disp on, blink off (p11)
	i2c_smbus_write_byte(file, 0xe7); // Full brightness (page 15)

//	Display a series of pictures
	//write_block(file, smile_bmp);
	//sleep(1);
	//char test_pic[] = {0xff00, 0x00ff, 0xa9a9, 0x8989, 0x8989, 0xa9a9, 0x4242, 0x3c3c};
	//write_block(file, test_pic);
	//sleep(5);
	//write_block(file, smile_bmp);


	
	
	
	


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
	/*
	gpio_export(clear_button);
	gpio_set_dir(clear_button, "in");
	gpio_set_edge(clear_button, "falling");
	gpio_fd[4] = gpio_fd_open(left_button, O_RDONLY);
	
*/
	
	for (row = 0; row < width; row++) {
		for (col = 0; col < height; col++) {
			spots[row][col] = 0;
		}
	}
	spots[y][x] = 1;

	while (1) {
		
		updateScreen(spots);
		static __u16 disp[8];
		int m_col, m_row;
		int col_num = 0;
		for (m_col = 0; m_col < width; m_col++) {
			col_num = 0;
			for (m_row = 0; m_row < height; m_row++) {
				col_num = col_num + spots[m_row][m_col]*(1 << m_row);
			}
			disp[m_col] = col_num;
			
		}
		//int dispa[8]= {0x00, 0x42, 0xa9, 0x85, 0x85, 0xa9, 0x42, 0x3c};
		//fprintf(stderr,"Num: %i\n",col_num);
		//fprintf(stderr, "%d\n",x);
		disp[x] = disp[x] + 256*(1 << y);
			
		
		
		//disp[]= {0x00, 0x42, 0xa9, 0x85, 0x85, 0xa9, 0x42, 0x3c};
		write_block(file, disp);
		
		
		
		
		
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
		} else if (control == 4) {
		//fprintf(stderr, "%d\n",x);
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

int rc = poll(fdset, 5, -1);
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
			fprintf(stderr, "%d\n",i);
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
