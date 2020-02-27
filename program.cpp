
// example program
// showing how to use the vision functions
// discussed in class

// open image_view every time before running the example
// close image_view every time after running the example

// notes from class

// int copy(image *a, image *b);
// copies and converts between different image types

// int scale(image *a, image *b);
// forces the image to use the whole range of pixel values
// 0 to 255, basically enhancing the contrast.
// this is important for consisent behavior of many image 
// processing algorithms, especially ones that have
// a thresholding procedure.
// for more advanced applications, the scaling can
// be applied using information from the histogram of the image
// for instance, stretching the histogram to the top and
// bottom 1% of pixel values.

// int invert(image *a, image *b);
// inverts the image pixels pb = 255-pa
// useful for making dark objects bright

// other functions you can write yourself /////

// addition - useful for removing noise
// pb = (pa(t1) + pa(t2) + ... + pa(tn))/n
// sigma_ave = sigma / sqrt(n)
// watch out for overflow with addition
// final result must be 0 to 255
// also useful for merging objects from two images,
// you can also use OR operator for this purpose
// pc = pa | pb

// subtraction
// pb = pa(t2) - pa(t1)
// useful for detecting differences in an image,
// such as when a process will begin.
// for slow processes you may want to subtract
// images from very different times (use a slow sample rate).
// useful for security as well.

// AND operator 
// pc = pa & pb
// can be used to find the objects
// in two images that intersect, this is useful
// for masking objects -- more on this later...

// up to this point we have considered functions only on
// individual pixels of the image.
// to go any further than this point we need to consider
// the neighbourhood pixels:
// k1 k2 k3
// k4 k5 k6
// k7 k8 k9
// this allows spatial filters to be applied to the image.
// we can also consider 7x7, 9x9, etc. filters,
// but they are more computationally expensive,
// and often the same result can be achieved with
// multiple 3x3 neighbourhood filters.

// int convolution(image *a, image *b, int *k, double s);
// perform a weighted summation of the neighbourhood pixels
// pb = s*(k1*pa1 + k2*pa2 + ... + k9*pa9)
// depending on the values of s, k1, ..., k9 we will
// get a different type of filter

// int lowpass_filter(image *a, image *b);
// k = [1 1 1 1 1 1 1 1 1], s = 1/9.0
// removes high frequency spatial noise (specks, etc.)

// int highpass_filter(image *a, image *b);
// k = [-1 -1 -1 -1 9 -1 -1 -1 -1], s = 1.0
// accentuates high frequency components
// (attenuates low frequency),
// effectively sharpening the image

// int gaussian_filter(image *a, image *b);
// k = [1 2 1 2 4 2 1 2 1], s = 1/16.0
// another type of low pass noise removing filter.
// it has the special property that two passes of this
// filter is equivalent to one 5x5 guasian filter, and so on.
// this filter is more accurate than the
// averaging low pass filter, but you may need to 
// apply it more often since their is higher weighting
// to the center pixel.

#include <cstdio>
#include <cstdlib>
#include <conio.h>
#include<cmath>
#include <stdlib.h>
#include<iostream>

using namespace std;

// include this header file for basic image transfer functions
#include "image_transfer3.h"
#include <stdlib.h>
// include this header file for computer vision functions
#include "vision.h"

#include "my_functions.h"
void calculate_HSV(int R, int G, int B, double &hue, double &sat, double &value);
void calculate_hue_image(image &rgb, image &hue_image);
int draw_point_RGB(image &rgb, int ip, int jp, int R, int G, int B);
double object_radius(double ic, double jc, double beta, int label_number, image &label_image, image &rgb);
double object_theta(double ic, double jc, int label_number, image &label_image, image &rgb);
int main(int argc, char* argv[])
{ 
	int nhist,j,nlabel;
	double hist[255],hmin,hmax,x,ic1,jc1,ic2,jc2,ic3,jc3,ic4,jc4,ic5,jc5,ic6,jc6,r2,g,x1l,y1l,s,xnew,ynew;
	FILE *fp;
	image a,b,rgb,rgb0; // declare some image structures
	image label;
	int cam_number, height, width;
	double R_ave, G_ave, B_ave;
	
	
	cam_number = 0; 
	width  = 640;
	height = 480;

	activate_camera(cam_number,height,width);	// activate camera

	printf("\npress any key to begin");
	getch();

	rgb.type = RGB_IMAGE;
	rgb.width = width;
	rgb.height = height;

	rgb0.type = RGB_IMAGE;
	rgb0.width = width;
	rgb0.height = height;

	// set the type and size of the images
	a.type = GREY_IMAGE;
	a.width = width;
	a.height = height;

	b.type = GREY_IMAGE;
	b.width = width;
	b.height = height;

	label.type = LABEL_IMAGE;
	label.width = width;
	label.height = height;

	// allocate memory for the images
	allocate_image(a);
	allocate_image(b);
	allocate_image(label);
	allocate_image(rgb);
	allocate_image(rgb0);




	printf("\npress any key to home printer");
	getch();

	home();

	printf("\npress any key to get an image");
	getch();


	acquire_image(rgb,0);

	// copy the original rgb image to use later
	copy(rgb,rgb0);

	view_rgb_image(rgb);
	printf("\ninput image rgb");
	getch();

	// convert RGB image to a greyscale image
	copy(rgb,a);

	copy(a,rgb);    // convert to RGB image format
	view_rgb_image(rgb);
	printf("\ninput image converted to greyscale");
	getch();

	// scale the image to enhance contrast
	scale(a,b);
	copy(b,a); // put result back into image a

	copy(a,rgb);    // convert to RGB image format
	view_rgb_image(rgb);
	printf("\nimage scale function is applied");
	getch();

	save_rgb_image("grey.bmp",rgb);

	// apply a filter to reduce noise
	lowpass_filter(a,b);
	copy(b,a);

	copy(a,rgb);    // convert to RGB image format
	view_rgb_image(rgb);
	printf("\nimage after filter function is applied");
	getch();

	// make a histogram
	nhist = 60; // make 60 bins -- each bin is 255/60 range of intensity
	// eg bin1 = 0-3 
	// bin2 = 4-8,
	// etc.
	histogram(a,hist,nhist,hmin,hmax);
	

	// save to a csv file you can open/plot with microsoft excel
	fp = fopen("hist1.csv","w");
	for(j=0;j<nhist;j++) {
		if(j != 0) fprintf(fp,"\n");
		x = hmin + (hmax-hmin)/nhist*j;
		fprintf(fp,"%lf , %lf",x,hist[j]);
	}
	fclose(fp);

	// use threshold function to make a binary image (0,255)
	threshold(a,b,100);
	copy(b,a);

	copy(a,rgb);    // convert to RGB image format
	view_rgb_image(rgb);
	printf("\nimage after threshold function is applied");
	getch();
 
	// invert the image
	invert(a,b);
	copy(b,a);

	copy(a,rgb);    // convert to RGB image format
	view_rgb_image(rgb);
	printf("\nimage after invert function is applied");
	getch();

	// perfoqrm an erosion function to remove noise (small objects)
	erode(a,b);
	copy(b,a);

	copy(a,rgb);    // convert to RGB image format
	view_rgb_image(rgb);
	printf("\nimage after erosion function is applied");
	getch();

	// perform a dialation function to fill in 
	// and grow the objects
	dialate(a,b);
	copy(b,a);

	dialate(a,b);
	copy(b,a);

	copy(a,rgb);    // conv=ert to RGB image format
	view_rgb_image(rgb);
	printf("\nimage after dialation function is applied");
	getch();

	// label the objects in a binary image
	// labels go from 1 to nlabels
	
	label_image(a,label,nlabel);
	

	

	// compute the centroid of the last object
	centroid(a,label,1,ic1,jc1);
	printf("\ncentroid: icm = %lf , jcm = %lf",ic1,jc1);

	label_image(a, label, nlabel);
	centroid(a, label, 2, ic2, jc2);
	printf("\ncentroid: ic1 = %lf , jc1 = %lf", ic2, jc2);

	label_image(a, label, nlabel);
	centroid(a, label, 3, ic3, jc3);
	printf("\ncentroid: ic2 = %lf , jc2 = %lf", ic3, jc3);

	label_image(a, label, nlabel);
	centroid(a, label, 4, ic4, jc4);
    printf("\ncentroid: ic0 = %lf , jc0 = %lf", ic4, jc4);

	label_image(a, label, nlabel);
	centroid(a, label, 6, ic5, jc5);
	printf("\ncentroid: ic4 = %lf , jc4 = %lf", ic5, jc5);

	label_image(a, label, nlabel);
	centroid(a, label, 7, ic6, jc6);
	printf("\ncentroid: ic3 = %lf , jc3 = %lf", ic6, jc6);

		r2 = (ic4*ic4) + (jc4*jc4);
	s = 0.352747;
	g = -2.11478*pow(10, (-7));
	x1l = s*(1 + (g*r2))*ic4;
	y1l = s*(1 + (g*r2))*jc4;
	xnew = x1l - 16;
	ynew = y1l + 79;
	printf("\n s=%1f,\n g=%1f, \n x1l=%1f,\n yll=%1f", s, g, x1l, y1l);
	printf("\n xnew=%1f,\n ynew=%1f", xnew, ynew);




	move(xnew, ynew);
    
	




	// use the original colour image here
	average_colour(rgb0,label,1,R_ave,G_ave,B_ave);

	printf("\nR_ave = %lf",R_ave);
	printf("\nG_ave = %lf",G_ave);
	printf("\nB_ave = %lf",B_ave);

	// draw a point at centroid location (ic,jc) with intensity 120
	draw_point(a,(int)ic4,(int)jc4,120);

	copy(a,rgb);    // convert to RGB image format
	view_rgb_image(rgb);
	printf("\nimage after a centroid is marked.");
	getch();

	// save the image (make sure to use an RGB image)
	save_rgb_image("aout.bmp",rgb);
	
	object_radius(ic4, jc4, 3.14159 / 4, 4, label, rgb);
	object_radius(ic4, jc4, 3.14159 / 2, 4, label, rgb);
	object_radius(ic4, jc4, 3.14159 / 1, 4, label, rgb);
	object_theta(ic4, jc4, 2, label, rgb);
	shape(ic4, jc4, 4, label, rgb);
	view_rgb_image(rgb);
	
	printf("\nimage after a radius is marked.");
	getch();
	average_colour(rgb0, label, 1, R_ave, G_ave, B_ave);

	printf("\nR_ave = %lf", R_ave);
	printf("\nG_ave = %lf", G_ave);
	printf("\nB_ave = %lf", B_ave);
	// free the image memory before the program completes
	free_image(a);
	free_image(b);
	free_image(label);
	free_image(rgb); 
	free_image(rgb0);

	deactivate_cameras();

	printf("\n\ndone.\n");
	getch();

 	return 0;
}

double object_radius(double ic, double jc, double beta, int label_number, image &label_image, image &rgb)
{
	double r = 0.0, dr, r_max;
	int i, j, k, width, height;
	i2byte *pl;
	float r2, s, g, z1l, y1l, x1l;
	double xnew, ynew, znew;
	znew = 10;


	width = label_image.width;
	height = label_image.height;
	pl = (i2byte *)label_image.pdata;

	dr = 0.5; // use 0.5 pixels just to be sure

	r_max = 70; // limit the max object radius size to something reasonable

	for (r = dr; r < r_max; r += dr) {
		i = ic + r*cos(beta);
		j = jc + r*sin(beta);

		// limit i and j in case it gets out of bounds -> wild pointer
		if (i < 0) i = 0;
		if (i >= width) i = width;
		if (j < 0) j = 0;
		if (j >= height) j = height;

		// convert i,j to image coord k
		k = j*width + i;

		// check when label is done -> r is just beyond the object radius
		if (pl[k] != label_number) break;
	}

	// mark point in rgb image for testing / debugging
	draw_point_RGB(rgb, i, j, 255,0 , 0);
	r2 = (i*i) + (j*j);
	s = 0.352747;
	g = -2.11478*pow(10, (-7));
	x1l = s*(1 + (g*r2))*i;
	y1l = s*(1 + (g*r2))*j;
	xnew = x1l - 16;
	ynew = y1l + 79;
	printf("\n s=%1f,\n g=%1f, \n x1l=%1f,\n yll=%1f", s, g, x1l, y1l);
	printf("\n xnew=%1f,\n ynew=%1f", xnew, ynew);




	move(xnew, ynew);

	return r;
}


double object_theta(double ic, double jc, int label_number, image &label_image, image &rgb)
{
	double th, dth, th_max = 0.0, r_max = 0.0, r;
	int n = 45, i, j,ic4,jc4;

	dth = 3.14159 / n;

	for (th = 0.0; th < 2 * 3.141591; th += dth) {
		// call your homies !
		r = object_radius(ic, jc, th, label_number, label_image, rgb);
		if (r > r_max) {
			r_max = r;
			th_max = th;
		}
	}

	// mark point in rbg for debugging / testing
	i = ic + r_max*cos(th_max);
	j = jc + r_max*sin(th_max);
	draw_point_RGB(rgb, i, j, 255, 0, 0);
	draw_point_RGB(rgb, (int)ic4, (int)jc4, 0, 255, 255);

	return th_max;
	printf("/ n theta = % 1f", th_max);
}

int draw_point_RGB(image &rgb, int ip, int jp, int R, int G, int B)
{
	ibyte *p;
	int i, j, w = 2, pixel;

	// initialize pointer
	p = rgb.pdata;

	if (rgb.type != RGB_IMAGE) {
		printf("\nerror in draw_point_RGB: input type not valid!");
		return 1;
	}

	// limit out of range values
	if (ip < w) ip = w;
	if (ip > rgb.width - w - 1) ip = rgb.width - w - 1;
	if (jp < w) jp = w;
	if (jp > rgb.height - w - 1) jp = rgb.height - w - 1;

	for (i = -w; i <= w; i++) {
		for (j = -w; j <= w; j++) {
			pixel = rgb.width*(jp + j) + (ip + i);
			p[3 * pixel] = B;
			p[3 * pixel + 1] = G;
			p[3 * pixel + 2] = R;
		}
	}

	return 0;
}
