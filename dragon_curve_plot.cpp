/*
	Use OpenCV to draw dragon curve.
	I use OpenCV 2.4.7 with opencv_core247.dll, opencv_imgproc247.dll, and opencv_highgui247.dll.
	by J.Shine.Yamaha.Chenkerymiowstone
	2016.2.17
*/

# include <stdio.h>
# include <malloc.h>
# include "opencv/highgui.h"
# include "opencv/cv.h"

# define winx	1000
# define winy	600
# define startx	(winx/4)
# define endx	(winx*3/4)
# define horizy	(winy*0.65)
# define dimishrate	3

CvPoint2D32f rotate45d2(CvPoint2D32f v, int sideswitch)
{
	CvPoint2D32f v2;
	v2.x = (v.x-v.y*sideswitch)/2;
	v2.y = (v.y+v.x*sideswitch)/2;
	return v2;
}

int main()
{
	IplImage *fr, *div;
	CvPoint2D32f *p, v;
	CvSize winsz;
	CvScalar white, blue, yellow, color;
	CvFont font;
	int i, step, vertexamount, vertexamount0, sideswitch, fonttype, fthk;
	char key, text[10];
	
	// inform
	printf("\n\n");
	printf("\t Dragon Curve Plot\n\n");
	printf("\t I use OpenCV 2.4.7 with:\n");
	printf("\t\t opencv_core247.dll\n");
	printf("\t\t opencv_imgproc247.dll\n");
	printf("\t\t opencv_highgui247.dll\n\n");
	printf("\t by J.S.Y.Chenkerymiowstone  2016.2.17");
	
	// memory allocation of array
	size_t arraysz = 100000000;
	p = (CvPoint2D32f *)malloc(arraysz*sizeof(CvPoint2D32f));
	
	// frame size
	winsz = cvSize(winx, winy);
	fr  = cvCreateImage(winsz, 8, 3);
	div = cvCreateImage(winsz, 8, 3);
	cvSet(div, CV_RGB(dimishrate,dimishrate,dimishrate));
	
	// font type
	fonttype = 0;
	fthk = 5;
	cvInitFont(&font,  fonttype, 1, 2, 0, fthk,  CV_AA);
	
	// initialize before loop
	key = 0;
	white  = CV_RGB(255, 255, 255);
	blue   = CV_RGB(110, 150, 220);
	yellow = CV_RGB(250, 230, 120);
	p[0] = cvPoint2D32f(startx, horizy);
	p[1] = cvPoint2D32f(endx,   horizy);
	step = 1;
	vertexamount0 = 2;
	vertexamount = 3;
	while(vertexamount<arraysz)
	{
		// data arrangement
		for(i=vertexamount0-1; i>0; i--) {
			p[i*2] = p[i];
		}
		
		// plot loop
		sprintf(text, "STEP %2d", step);
		cvPutText(fr, text, cvPoint(winx*0.73,winy*0.2), &font, white);
		color = (step%2==1) ? blue : yellow;
		sideswitch = -1;
		for(i=0; i<vertexamount-1; i+=2) {
			// direction vector
			v.x = p[i+2].x - p[i].x;
			v.y = p[i+2].y - p[i].y;
			// interpolation
			v = rotate45d2(v, sideswitch);
			p[i+1].x = p[i].x + v.x;
			p[i+1].y = p[i].y + v.y;
			// plot
			cvLine(fr, cvPointFrom32f(p[i  ]), cvPointFrom32f(p[i+1]), color, 2, CV_AA, 0);
			cvLine(fr, cvPointFrom32f(p[i  ]), cvPointFrom32f(p[i+1]), white, 1, CV_AA, 0);
			cvLine(fr, cvPointFrom32f(p[i+1]), cvPointFrom32f(p[i+2]), color, 2, CV_AA, 0);
			cvLine(fr, cvPointFrom32f(p[i+1]), cvPointFrom32f(p[i+2]), white, 1, CV_AA, 0);
			/*if(step<8 || i%((vertexamount-1)/64)==0)*/
			cvShowImage("Dragon Curve", fr);
			// check whether key something or not
			key = cvWaitKey( (step<7)?100:1 );
			if(key!=-1) break;
			// swich side between left and right
			sideswitch*=-1;
		}
		if(key!=-1) break;
		
		// blur processing
		cvSmooth(fr, fr, CV_BLUR, 5, 5, 0.0, 0.0);
		cvDiv(fr, div, fr);
		
		// update parameter
		step++;
		vertexamount0 = vertexamount;
		vertexamount = vertexamount*2-1;
	}
	
	if(key==-1) cvWaitKey(0);
	cvDestroyWindow("Dragon Curve");
	
	// free memory
	free(p);
	cvReleaseImage(&fr);
	cvReleaseImage(&div);
	return 0;
}
