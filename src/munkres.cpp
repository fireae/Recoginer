/*
 *   Copyright (c) 2007 John Weaver
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */

#include "munkres.h"

#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;

#define Z_NORMAL 0
#define Z_STAR 1
#define Z_PRIME 2

bool 
Munkres::find_uncovered_in_matrix(double item, int &row, int &col) {
	int cols = matrix.cols;
	int rows = matrix.rows;
	for ( row = 0; row < rows; row++ )
	{
		if ( !row_mask[row] )
			for ( col = 0; col < cols; col++ )
				if ( !col_mask[col] )
					if ( matrix.at<double>(row,col) == item )
						return true;
	}
	return false;
}

bool 
Munkres::pair_in_list(const std::pair<int,int> &needle, const std::list<std::pair<int,int> > &haystack) {
	for ( std::list<std::pair<int,int> >::const_iterator i = haystack.begin() ; i != haystack.end() ; i++ ) {
		if ( needle == *i )
			return true;
	}
	
	return false;
}

int 
Munkres::step1(void) {
	int cols = matrix.cols;
	int rows = matrix.rows;
	
	for ( int row = 0 ; row < rows ; row++ )
		for ( int col = 0 ; col < cols ; col++ )
			if ( matrix.at<double>(row,col) == 0 ) {
				bool isstarred = false;
				for ( int nrow = 0 ; nrow < rows ; nrow++ )
					if ( mask_matrix.at<uchar>(nrow,col) == Z_STAR )
						isstarred = true;

				if ( !isstarred ) {
					for ( int ncol = 0 ; ncol < cols ; ncol++ )
						if ( mask_matrix.at<uchar>(row,ncol) == Z_STAR )
							isstarred = true;
				}
							
				if ( !isstarred ) {
					mask_matrix.at<uchar>(row,col) = Z_STAR;
				}
			}

	return 2;
}

int 
Munkres::step2(void) {
	int cols = matrix.cols;
	int rows = matrix.rows;
	
	int covercount = 0;
	for ( int row = 0 ; row < rows ; row++ )
		for ( int col = 0 ; col < cols ; col++ )
			if ( mask_matrix.at<uchar>(row,col) == Z_STAR ) {
				col_mask[col] = true;
				covercount++;
			}
			
	int k = std::min(rows, cols);

	if ( covercount >= k ) {
#ifdef DEBUG
		std::cout << "Final cover count: " << covercount << std::endl;
#endif
		return 0;
	}

#ifdef DEBUG
	std::cout << "Munkres matrix has " << covercount << " of " << k << " Columns covered:" << std::endl;
	for ( int row = 0 ; row < rows ; row++ ) {
		for ( int col = 0 ; col < cols ; col++ ) {
			std::cout.width(8);
			std::cout << matrix.at<double>(row,col) << ",";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
#endif


	return 3;
}

int 
Munkres::step3(void) {
	/*
	Main Zero Search

   1. Find an uncovered Z in the distance matrix and prime it. If no such zero exists, go to Step 5
   2. If No Z* exists in the row of the Z', go to Step 4.
   3. If a Z* exists, cover this row and uncover the column of the Z*. Return to Step 3.1 to find a new Z
	*/
	int cols = matrix.cols;
	int rows = matrix.rows;
	
	if ( find_uncovered_in_matrix(0, saverow, savecol) ) {
		mask_matrix.at<uchar>(saverow,savecol) = Z_PRIME; // prime it.
	} else {
		return 5;
	}

	for ( int ncol = 0 ; ncol < cols ; ncol++ )
		if ( mask_matrix.at<uchar>(saverow,ncol) == Z_STAR ) {
			row_mask[saverow] = true; //cover this row and
			col_mask[ncol] = false; // uncover the column containing the starred zero
			return 3; // repeat
		}

	return 4; // no starred zero in the row containing this primed zero
}

int 
Munkres::step4(void) {
	int cols = matrix.cols;
	int rows = matrix.rows;
	
	std::list<std::pair<int,int> > seq;
	// use saverow, savecol from step 3.
	std::pair<int,int> z0(saverow, savecol);
	std::pair<int,int> z1(-1,-1);
	std::pair<int,int> z2n(-1,-1);
	seq.insert(seq.end(), z0);
	int row, col = savecol;
	/*
	Increment Set of Starred Zeros

   1. Construct the ``alternating sequence'' of primed and starred zeros:

         Z0 : Unpaired Z' from Step 4.2 
         Z1 : The Z* in the column of Z0
         Z[2N] : The Z' in the row of Z[2N-1], if such a zero exists 
         Z[2N+1] : The Z* in the column of Z[2N]

      The sequence eventually terminates with an unpaired Z' = Z[2N] for some N.
	*/
	bool madepair;
	do {
		madepair = false;
		for ( row = 0 ; row < rows ; row++ )
			if ( mask_matrix.at<uchar>(row,col) == Z_STAR ) {
				z1.first = row;
				z1.second = col;
				if ( pair_in_list(z1, seq) )
					continue;
				
				madepair = true;
				seq.insert(seq.end(), z1);
				break;
			}

		if ( !madepair )
			break;

		madepair = false;

		for ( col = 0 ; col < cols ; col++ )
			if ( mask_matrix.at<uchar>(row,col) == Z_PRIME ) {
				z2n.first = row;
				z2n.second = col;
				if ( pair_in_list(z2n, seq) )
					continue;
				madepair = true;
				seq.insert(seq.end(), z2n);
				break;
			}
	} while ( madepair );

	for ( std::list<std::pair<int,int> >::iterator i = seq.begin() ;
		  i != seq.end() ;
		  i++ ) {
		// 2. Unstar each starred zero of the sequence.
		if ( mask_matrix.at<uchar>(i->first,i->second) == Z_STAR )
			mask_matrix.at<uchar>(i->first,i->second) = Z_NORMAL;

		// 3. Star each primed zero of the sequence,
		// thus increasing the number of starred zeros by one.
		if ( mask_matrix.at<uchar>(i->first,i->second) == Z_PRIME )
			mask_matrix.at<uchar>(i->first,i->second) = Z_STAR;
	}

	// 4. Erase all primes, uncover all columns and rows, 
	for ( int row = 0 ; row < rows ; row++ )
		for ( int col = 0 ; col < cols ; col++ )
			if ( mask_matrix.at<uchar>(row,col) == Z_PRIME )
				mask_matrix.at<uchar>(row,col) = Z_NORMAL;
	
	for ( int i = 0 ; i < rows ; i++ ) {
		row_mask[i] = false;
	}

	for ( int i = 0 ; i < cols ; i++ ) {
		col_mask[i] = false;
	}

	// and return to Step 2. 
	return 2;
}

int 
Munkres::step5(void) {
	/*
	New Zero Manufactures

   1. Let h be the smallest uncovered entry in the (modified) distance matrix.
   2. Add h to all covered rows.
   3. Subtract h from all uncovered columns
   4. Return to Step 3, without altering stars, primes, or covers. 
	*/
	int cols = matrix.cols;
	int rows = matrix.rows;
	
	
	double h = 0;
	for ( int row = 0 ; row < rows ; row++ ) {
		if ( !row_mask[row] ) {
			for ( int col = 0 ; col < cols ; col++ ) {
				if ( !col_mask[col] ) {
					if ( (h > matrix.at<double>(row,col) && matrix.at<double>(row,col) != 0) || h == 0 ) {
						h = matrix.at<double>(row,col);
					}
				}
			}
		}
	}

	for ( int row = 0 ; row < rows ; row++ )
		for ( int col = 0 ; col < cols ; col++ ) {
			if ( row_mask[row] )
				matrix.at<double>(row,col) += h;

			if ( !col_mask[col] )
				matrix.at<double>(row,col) -= h;
		}

	return 3;
}

void 
Munkres::solve(cv::Mat &m, vector<int> &result, double &result_val, bool ismin) {
	// Linear assignment problem solution
	// [modifies matrix in-place.]
	// matrix(row,col): row major format assumed.

	// Assignments are remaining 0 values
	// (extra 0 values are replaced with -1)
	int cols = m.cols;
	int rows = m.rows;
	result.resize(rows);
	Mat save_mat = m.clone();
	if (!ismin)
	{
		double max_val = 0.0;
		for (int i = 0; i < rows; i++)
		{
			double *data = m.ptr<double>(i);
			for (int j = 0; j < cols; j++)
			{
				if (data[j] > max_val)
					max_val = data[j];
			}
		}

		for (int i = 0; i < rows; i++)
		{
			double *data = m.ptr<double>(i);
			for (int j = 0; j < cols; j++)
			{
				if (data[j] > max_val)
					data[j] = max_val-data[j];
			}
		}
	}
	bool notdone = true;
	int step = 1;

	this->matrix = m;
	// Z_STAR == 1 == starred, Z_PRIME == 2 == primed
	mask_matrix = Mat::zeros(Size(matrix.rows, matrix.cols), CV_8UC1);

	row_mask = new bool[matrix.rows];
	col_mask = new bool[matrix.cols];
	for ( int i = 0 ; i < matrix.rows ; i++ ) {
		row_mask[i] = false;
	}

	for ( int i = 0 ; i < matrix.cols ; i++ ) {
		col_mask[i] = false;
	}

	while ( notdone ) {
		switch ( step ) {
			case 0:
				notdone = false;
				break;
			case 1:
				step = step1();
				break;
			case 2:
				step = step2();
				break;
			case 3:
				step = step3();
				break;
			case 4:
				step = step4();
				break;
			case 5:
				step = step5();
				break;
		}
	}

	// Store results
	result_val = 0.0;
	for ( int row = 0 ; row < matrix.rows ; row++ )
	{
		for ( int col = 0 ; col < matrix.cols ; col++ )
		{
			if ( mask_matrix.at<uchar>(row,col) == Z_STAR )
			{
				result_val += save_mat.at<double>(row, col);
				result[row] = col;
				matrix.at<double>(row,col) = 0;
			}
			else
			{
				matrix.at<double>(row,col) = -1;
			}
		}
	}
	m = matrix;

	delete [] row_mask;
	delete [] col_mask;
}
