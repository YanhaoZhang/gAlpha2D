/*
Author: Srinivasan Kidambi Sridharan, Ashwin Nanjappa, Cao Thanh Tung

Copyright (c) 2012, School of Computing, National University of Singapore. 
All rights reserved.

If you use GAlpha 1.0 and you like it or have comments on its usefulness etc., we 
would love to hear from you at <tants@comp.nus.edu.sg>. You may share with us
your experience and any possibilities that we may improve the work/code.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list of
conditions and the following disclaimer. Redistributions in binary form must reproduce
the above copyright notice, this list of conditions and the following disclaimer
in the documentation and/or other materials provided with the distribution. 

Neither the name of the National University of Singapore nor the names of its contributors
may be used to endorse or promote products derived from this software without specific
prior written permission from the National University of Singapore. 

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES 
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
TO, PROCUREMENT OF SUBSTITUTE  GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
DAMAGE.

*/

#pragma once

#include "kernel.h"

typedef AlphaHVec::iterator MasterList_iterator;
typedef thrust::device_vector<int>::iterator IntDIter;

class Alpha_Shapes_2D
{
private:
	/* Data structures in CPU */

	// Delaunay Triangulation
	PGPUDTOUTPUT							pDT;
	PGPUDTPARAMS							pInput;

	// Alpha Shape
	PointIntervalHVec						point_list;	
	EdgeIntervalHVec						edge_list;
	TriangleIntervalHVec					tri_list;
	
	IntHVec									tri_edge_indicesH;
	IntHVec									spec_pointers;

	/* Data structures in GPU */

	// Delaunay Triangulation
	PointDVec								pDVec;
	TriangleDVec							triDVec;
	IntDVec									ptTriMap;
	
	// Alpha Shape in GPU
	PointIntervalDVec						point_listD;	
	EdgeIntervalDVec						edge_listD;
	TriangleIntervalDVec					tri_listD;
	
	AlphaDVec								master_listD;
	DoubleDVec								spectrumD;
	IntDVec									tri_edge_indicesD;

	// Helper Structure
	IntDVec									tri_edge_countD;	
	BackRefDVec								br;
	IntDVec									edge_tri_map;
	IntDVec									flag;	
	
	int										num_edges;	
	Predicates*								ps;
	bool									data_initialized;
	vector<string>							messages;

protected:		

	// Methods to compute interval map for all k-simplices 0<=k<=2
	void clear_all();
	void computeEdges();
	bool init_alpha_shapes();
	
	void compute_ro();	
	void sort_spectrum();
	void compute_intervals();
	void compute_master_list();
	void CPU_insertion_sort(int specSize);

public:
	DoubleHVec								spectrum;
	REAL									alpha;
	int										alpha_index;
	AlphaHVec								master_list;

	Alpha_Shapes_2D();
	Alpha_Shapes_2D(PGPUDTPARAMS pInput, PGPUDTOUTPUT pDelaunay);
	
	void computeAlphaShapes();

	// Modifier functions
	MasterList_iterator Master_begin();
	MasterList_iterator Master_end();	
	void setAlpha(REAL val);
	REAL getAlpha();

	void dumpValues(string fname);
	
	friend class Visualizer;
	friend void display(void);
};
