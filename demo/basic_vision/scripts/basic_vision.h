///////////////////////////////////////////////
// PGCC AUTO-GENERATED SCRIPT IMPLEMENTATION //
///////////////////////////////////////////////

/*
	This file was auto-generated by PGCC.
	You shouldn't edit anything in it.
*/


#ifndef __PG_SCRIPT_basic_vision_H
#define __PG_SCRIPT_basic_vision_H

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


///////////////////////////////////////////
// IMPLEMENTATION OF SCRIPT basic_vision //
///////////////////////////////////////////

class Script_basic_vision {
public:
	Camera c;
	Grayscale g;
	Gradient gradient;
	Ptcg ptcg;
	GiveFocuspoints focuspoints;
	LogPolar lp;
	Normalize norm;
	SVQ vq;
	kMax km;
	ThetaPopulationCoding theta;
	SigmaPi prph;
	RowVector pcpattern;
	SVQ pc;
	GtkPlot vpc;



	Script_basic_vision() {
		c.path = "/dev/video1";
		gradient.alpha = 0.8;
		ptcg.competition_radius = 30;
		ptcg.mask_radius = 4;
		ptcg.theta1 = 3;
		ptcg.theta2 = 1.5;
		ptcg.threshold = 0.01;
		focuspoints.threshold = 0;
		vq.K = 32;
		km.K = 1;
		theta.nbNeurons = 30;
		pc.K = 3;
		vpc.w = 20;
	}

	void init() {
		c.init();
		g.init();
		gradient.init();
		ptcg.init();
		focuspoints.init();
		lp.init();
		theta.init();
		norm.init();
		vq.init();
		km.init();
		prph.init();
		pcpattern.init();
		pc.init();
		vpc.init();
	}


/////////////////
// THREADS (1) //
/////////////////

	void process_thread_0() {
		c.process();
		g.process(c);
		gradient.process(g);
		ptcg.process(gradient);
		focuspoints.process(ptcg);
		lp.process(g, focuspoints);
		theta.process(focuspoints);
		norm.process(lp);
		vq.process(norm);
		km.process(vq);
		prph.process(km, theta);
		pcpattern.process(prph);
		pc.process(pcpattern);
		vpc.process(pc);
	}

};

#endif // __PG_SCRIPT_basic_vision_H
