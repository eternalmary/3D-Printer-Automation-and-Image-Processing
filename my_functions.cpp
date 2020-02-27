
// user defined functions

#include <iostream>
#include <cmath>
#include <conio.h>
#include<cstdio>
//#include<strsteam>
//#include<Windows.h>
//#include<serial_com.h>
//#include<imagetransfer2.h>

using namespace std;

// include this header file for basic image transfer functions
#include "image_transfer3.h"

// include this header file for computer vision functions
#include "vision.h"

#include "my_functions.h"
//#define KEY(c) ( GetAsyncKeyState((int)(c)) & (SHORT)0x8000 )


// Edit


#include <strstream> // for string streams

#include <windows.h>

#include "serial_com.h"

#define KEY(c) ( GetAsyncKeyState((int)(c)) & (SHORT)0x8000 )


void home()

{
	HANDLE h1;
	const int NMAX = 64;
	char buffer[NMAX];
	char ch;
	int n;
	int x = 126, y = 159, z = 182;

	// declare a string stream to convert a mixture
	// of numbers and text to a string to send to the Arduino
	// or 3D printer

	// declare an output string stream of max NMAX charaters
	ostrstream sout(buffer, NMAX);

	// note: the serial port driver should be set to the same data (baud) rate
	// in the device manager com port settings.
	// the uploader should also use the same rate.

	// make sure bps setting in serial_com.cpp file
	// is compatible with the printer -- normally 115200 bps

	// should have the same com port as Arduino here
	open_serial("COM3", h1); // COM9 for my 3D printer

	// should wait briefly to complete the send and/or
	// to receive data from Arduino
	Sleep(100); // need to wait for data to come
	// back from arduino or the next loop will fall through

	// print out what the Arduino sends back
	// this is like a mini serial monitor program
	while (serial_available(h1) > 0) {
		serial_recv_char(ch, h1);
		cout << ch;
	}

	cout << "\ninitial 3D printer data transmit is complete\n";

	Sleep(1000);

	// start home ///////////////////

	// we should home printer before sending move commands
	// -- otherwise it will not know where it is to start.
	sout << "G28";
	sout << "\n";

	sout << '\0'; // terminate the string so strlen can function
	// note the C++ compiler seems to hate "\0"

	n = strlen(buffer); // number of bytes to send (excludes \0)

	// for debugging
	cout << "\nn = " << n;
	cout << "\nbuffer = " << buffer;

	// send the command(s) to the 3D printer / Arduino
	serial_send(buffer, n, h1);

	Sleep(10);

	cout << "\nincoming G-code commands:\n\n";

	// print out what the Arduino sends back
	// this is like a mini serial monitor program
	while (serial_available(h1) > 0) {
		serial_recv_char(ch, h1);
		cout << ch;
		Sleep(1); // wait for more characters
	}

	// end home ////////////////

	while (1) {

		if (KEY('X')) break; // press x to exit loop

		if (KEY(VK_UP) || KEY(VK_DOWN)) {

			if (KEY(VK_UP)) x += 5;
			if (KEY(VK_DOWN)) x -= 5;

			// set sout position to beginning so we can use it again
			sout.seekp(0);

			sout << "G0"; // move without extrusion
			sout << " X" << x;
			sout << " Y" << y;
			sout << " Z" << z;
			sout << "\n";

			sout << '\0'; // terminate the string so strlen can function
			// note the C++ compiler seems to hate "\0"

			n = strlen(buffer); // number of bytes to send (excludes \0)

			// for debugging
			cout << "\nn = " << n;
			cout << "\nbuffer = " << buffer;

			// send the move command to the 3D printer
			serial_send(buffer, n, h1);

			// wait 100 ms for a message to be sent back
			// why wait -> give some time for the move to finish
			// also time for the anet arduino board to process
			// the command --> ie we don't want to fill up
			// its incoming message mailbox
			// note the approximate time to move = dist / speed
			// -> maybe use vision system to see when it stops
			// or gets there
			// -- note there is also a G-code command
			// which sends back the current position
			// -> could use that
			// maybe use istrstream ?
			Sleep(100);

			cout << "\nincoming G-code commands:\n\n";

			// print out what the Arduino sends back
			// this is like a mini serial monitor program
			while (serial_available(h1) > 0) {
				serial_recv_char(ch, h1);
				cout << ch;
				Sleep(1); // wait for more characters
			}

		} // end if

	} // end while(1)	

	cout << "\npress a key to complete.";
	getch();

	cout << "\ndone.\n\n";

	close_serial(h1);

	// note when the program ends the 3D printer resets

}



void move(double &xnew, double &ynew)
{
	HANDLE h1;
	const int NMAX = 64;
	char buffer[NMAX];
	char ch;
	int n;
    int x = xnew, y = ynew, znew = 30;


	// declare a string stream to convert a mixture
	// of numbers and text to a string to send to the Arduino
	// or 3D printer

	// declare an output string stream of max NMAX charaters
	ostrstream sout(buffer, NMAX);

	// note: the serial port driver should be set to the same data (baud) rate
	// in the device manager com port settings.
	// the uploader should also use the same rate.

	// make sure bps setting in serial_com.cpp file
	// is compatible with the printer -- normally 115200 bps

	// should have the same com port as Arduino here
	open_serial("COM3", h1); // COM9 for my 3D printer

	// should wait briefly to complete the send and/or
	// to receive data from Arduino
	Sleep(100); // need to wait for data to come
	// back from arduino or the next loop will fall through

	// print out what the Arduino sends back
	// this is like a mini serial monitor program
	while (serial_available(h1) > 0) {
		serial_recv_char(ch, h1);
		cout << ch;
	}

	cout << "\ninitial 3D printer data transmit is complete\n";

	Sleep(1000);

	// start home ///////////////////

	// we should home printer before sending move commands
	// -- otherwise it will not know where it is to start.
	sout << "G28";
	sout << "\n";

	sout << '\0'; // terminate the string so strlen can function
	// note the C++ compiler seems to hate "\0"

	n = strlen(buffer); // number of bytes to send (excludes \0)

	// for debugging
	cout << "\nn = " << n;
	cout << "\nbuffer = " << buffer;

	// send the command(s) to the 3D printer / Arduino
	serial_send(buffer, n, h1);

	Sleep(10);

	cout << "\nincoming G-code commands:\n\n";

	// print out what the Arduino sends back
	// this is like a mini serial monitor program
	while (serial_available(h1) > 0) {
		serial_recv_char(ch, h1);
		cout << ch;
		Sleep(1); // wait for more characters
	}

	// end home ////////////////

	while (1) {

//		if (KEY('X')) break; // press x to exit loop

		/*if (KEY(VK_UP) || KEY(VK_DOWN)) {

			if (KEY(VK_UP)) x += 5;
			if (KEY(VK_DOWN)) x -= 5;*/

			// set sout position to beginning so we can use it again
			sout.seekp(0);

			sout << "G0"; // move without extrusion
			sout << " X" << xnew;
			sout << " Y" << ynew;
			sout << " Z" << znew;
			sout << "\n";

			sout << '\0'; // terminate the string so strlen can function
			// note the C++ compiler seems to hate "\0"

			n = strlen(buffer); // number of bytes to send (excludes \0)

			// for debugging
			cout << "\nn = " << n;
			cout << "\nbuffer = " << buffer;

			// send the move command to the 3D printer
			serial_send(buffer, n, h1);

			// wait 100 ms for a message to be sent back
			// why wait -> give some time for the move to finish
			// also time for the anet arduino board to proce
			// the command --> ie we don't want to fill up
			// its incoming message mailbox
			// note the approximate time to move = dist / speed
			// -> maybe use vision system to see when it stops
			// or gets there
			// -- note there is also a G-code command
			// which sends back the current position
			// -> could use that
			// maybe use istrstream ?
			Sleep(100);

			cout << "\nincoming G-code commands:\n\n";

			// print out what the Arduino sends back
			// this is like a mini serial monitor program
			while (serial_available(h1) > 0) {
				serial_recv_char(ch, h1);
				cout << ch;
				Sleep(1); // wait for more characters
			}

//		} // end if

	} // end while(1)	

	cout << "\npress a key to complete.";
	getch();

	cout << "\ndone.\n\n";

	close_serial(h1);

	// note when the program ends the 3D printer resets
	
}








// edit


// calculate the average RGB colour of an object with
// a label image value of label_num
void average_colour(image &rgb, image &label_image, int label_num,
	double &R, double &G, double &B)
{
	int i, j, k, label, N;
	int height, width; // ints are 4 bytes on the PC
	ibyte *p; // pointer to colour components in the rgb image
	i2byte *pl; // pointer to the label image

	height = rgb.height;
	width =  rgb.width;

	p = rgb.pdata;
	pl = (i2byte *)label_image.pdata;

	// initialize the summation varibles to compute average colour
	R = 0.0;
	G = 0.0;
	B = 0.0;
	N = 0; // number of pixels with the label number of interest
/*
	for(j=0;j<height;j++) {
		for(i=0;i<width;i++) {
			// equivalent 1D array index k
			k = j*width + i; // pixel number
			// how to get j and i from k ?
			// i = k % width
			// j = (k - i) / width

			// label value for i,j
			label = pl[k]; // method #1 -- 1D array index
//			label = *(pl+k); // method #2 -- 1D array pointer notation

			// collect data if the pixel has the label of interest
			if (label == label_num) {
				N++;
				// 3 bytes per pixel -- colour in order BGR
				B += p[k*3]; // 1st byte in pixel
				G += p[k*3+1]; // 2nd byte in pixel
				R += p[k*3+2]; // 3rd
			}

		} // for i

	} // for j
*/
	// method #3 -- pointers only !
	for(k=0;k<width*height;k++) { // loop for kth pixel

		// how to get j and i from k ?
		i = k % width;
		j = (k - i) / width;
		label = *pl;

		// collect data if the pixel has the label of interest
		if (label == label_num) {
			N++;
			// 3 bytes per pixel -- colour in order BGR
			B += *p; // 1st byte in pixel
			G += *(p+1); // 2nd byte in pixel
			R += *(p+2); // 3rd
		}

		// increment pointers
		p+=3; // 3 bytes per pixel
		pl++;

	}

	// compute average colour
	R = R / N;
	G = G / N;
	B = B / N;

}





