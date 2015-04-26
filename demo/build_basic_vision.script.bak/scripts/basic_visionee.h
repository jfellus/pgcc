///////////////////////////////////////////////
// PGCC AUTO-GENERATED SCRIPT IMPLEMENTATION //
///////////////////////////////////////////////

/*
	This file was auto-generated by PGCC.
	You shouldn't edit anything in it.
*/


#ifndef __PG_SCRIPT_basic_visionee_H
#define __PG_SCRIPT_basic_visionee_H

//////////////
// INCLUDES //
//////////////

#include "../utils/sync_semaphores.h"
#include "/home/jfellus/workspace/pg_vision/src/Camera.h"
#include "/home/jfellus/workspace/pg_vision/src/SaveImage.h"
#include "/home/jfellus/workspace/pg_vision/src/Gradient.h"
#include "/home/jfellus/workspace/pg_vision/src/Grayscale.h"
#include "/home/jfellus/workspace/pg_vision/src/ImageExtractChannel.h"
#include "/home/jfellus/workspace/pg_vision/src/Binarize.h"
#include "/home/jfellus/workspace/pg_vision/src/Ptcg.h"
#include "/home/jfellus/workspace/pg_vision/src/Convolution.h"
#include "/home/jfellus/workspace/pg_vision/src/DoG.h"
#include "/home/jfellus/workspace/pg_vision/src/Gaussian.h"
#include "/home/jfellus/workspace/pg_vision/src/GiveFocuspoint.h"
#include "/home/jfellus/workspace/pg_vision/src/LogPolar.h"
#include "/home/jfellus/workspace/pg_vision/src/ThetaPopulationCoding.h"
#include "/home/jfellus/workspace/pg_basics/src/Sleep.h"
#include "/home/jfellus/workspace/pg_basics/src/PrintMessage.h"
#include "/home/jfellus/workspace/pg_basics/src/ReadFileLines.h"
#include "/home/jfellus/workspace/pg_basics/src/WriteFileLines.h"
#include "/home/jfellus/workspace/pg_gtk/src/GtkImage.h"
#include "/home/jfellus/workspace/pg_gtk/src/GtkImageOverlay.h"
#include "/home/jfellus/workspace/pg_gtk/src/GtkImageOverlayText.h"
#include "/home/jfellus/workspace/pg_gtk/src/GtkHistogram.h"
#include "/home/jfellus/workspace/pg_gtk/src/GtkPlot.h"
#include "/home/jfellus/workspace/pg_learning/src/SVQ_R.h"
#include "/home/jfellus/workspace/pg_learning/src/SVQ.h"
#include "/home/jfellus/workspace/pg_learning/src/Normalize.h"
#include "/home/jfellus/workspace/pg_learning/src/Histogram.h"
#include "/home/jfellus/workspace/pg_learning/src/SigmaPi.h"
#include "/home/jfellus/workspace/pg_algebra/src/Sum.h"
#include "/home/jfellus/workspace/pg_algebra/src/Divide.h"
#include "/home/jfellus/workspace/pg_algebra/src/Multiply.h"
#include "/home/jfellus/workspace/pg_algebra/src/Pow.h"
#include "/home/jfellus/workspace/pg_algebra/src/Inverse.h"
#include "/home/jfellus/workspace/pg_algebra/src/NormalizeMinMax.h"
#include "/home/jfellus/workspace/pg_algebra/src/kMax.h"
#include "/home/jfellus/workspace/pg_algebra/src/Vector.h"


/////////////////////////////////////////////
// IMPLEMENTATION OF SCRIPT basic_visionee //
/////////////////////////////////////////////

class Script_basic_visionee {
public:
	Camera camera;
	Grayscale grayscale;
	Gradient gradient;
	Ptcg ptcg;
	GiveFocuspoints focuspoints;
	LogPolar logpol;
	Normalize L2_normalization;
	SVQ SVQ_Landmarks;
	kMax kmax;
	ThetaPopulationCoding theta;
	SigmaPi PrPh;
	RowVector place_vector;
	SVQ SVQ_PlaceCells;
	GtkPlot ViewerPlaceCells;



	Script_basic_visionee() {
		camera.path = "/dev/video0";
		gradient.alpha = 0.8;
		ptcg.competition_radius = 30;
		ptcg.mask_radius = 4;
		ptcg.theta1 = 3;
		ptcg.theta2 = 1.5;
		ptcg.threshold = 0.01;
		focuspoints.threshold = 0;
		SVQ_Landmarks.K = 32;
		kmax.K = 1;
		theta.nbNeurons = 30;
		SVQ_PlaceCells.K = 3;
		ViewerPlaceCells.w = 20;
	}

	void init() {
		camera.init();
		grayscale.init();
		gradient.init();
		ptcg.init();
		focuspoints.init();
		logpol.init();
		theta.init();
		L2_normalization.init();
		SVQ_Landmarks.init();
		kmax.init();
		PrPh.init();
		place_vector.init();
		SVQ_PlaceCells.init();
		ViewerPlaceCells.init();
	}


/////////////////
// THREADS (1) //
/////////////////

	void process_thread_0() {
		camera.process();
		grayscale.process(camera);
		gradient.process(grayscale);
		ptcg.process(gradient);
		focuspoints.process(ptcg);
		logpol.process(grayscale, focuspoints);
		theta.process(focuspoints);
		L2_normalization.process(logpol);
		SVQ_Landmarks.process(L2_normalization);
		kmax.process(SVQ_Landmarks);
		PrPh.process(kmax, theta);
		place_vector.process(PrPh);
		SVQ_PlaceCells.process(place_vector);
		ViewerPlaceCells.process(SVQ_PlaceCells);
	}

};

#endif // __PG_SCRIPT_basic_visionee_H