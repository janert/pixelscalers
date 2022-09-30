/*
 * Copyright 2016 Bruno Ribeiro
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Adapted from https://github.com/brunexgeek/hqx
 * and modified by Philipp K. Janert, September 2022
 */

#include <fstream>
#include <cstdlib>
#include <cstdint>
#include <string>

#include "bitmap.h"

using std::ifstream;
using std::ofstream;
using std::string;

#pragma pack(push, 1)

struct BitmapHeader
{
    uint16_t bfType;
    uint32_t bfSize;
    uint32_t bfRes1;
    uint32_t bfOffBits;
};

struct DibHeader
{
	uint32_t biSize;
	uint32_t biWidth;
	uint32_t biHeight;
	uint16_t biPlanes;
	uint16_t biBitCount;
	uint32_t biCompression;
	uint32_t biSizeImage;
	uint32_t biXPelsPerMeter;
	uint32_t biYPelsPerMeter;
	uint32_t biClrUsed;
	uint32_t biClrImportant;
};

#pragma pack(pop)

// Writes a Windows Bitmap image (BMP3, 24 bits) data structure from raw data
int saveBitmap(	const uint32_t *data, uint32_t width, uint32_t height,
		const string &fileName ) {
	BitmapHeader bh;
	DibHeader dh;
	uint16_t suffix;
	uint32_t zero = 0;
	const uint32_t *ptr;

	ofstream output(fileName.c_str(), std::ios_base::binary);	
	if (!output.good()) return -1;

	// suffix = ((width + 3) & ~0x03) - width;
	suffix = ( 4 - (3*width)%4 )%4;
	
	dh.biSize          = sizeof(DibHeader);
	dh.biWidth         = width;
	dh.biHeight        = height;
	dh.biPlanes        = 1;
	dh.biBitCount      = 24;
	dh.biCompression   = 0;
	// dh.biSizeImage     = (uint16_t) ( (width*3+suffix)*height );
	dh.biSizeImage     = (width*3+suffix)*height;
	dh.biXPelsPerMeter = 0x2E23;
	dh.biYPelsPerMeter = dh.biXPelsPerMeter;
	dh.biClrUsed       = 0;
	dh.biClrImportant  = 0;

	bh.bfType    = 0x4D42;
	bh.bfSize    = dh.biSizeImage + 0x0036;
	bh.bfRes1    = 0;
	bh.bfOffBits = 0x0036;
	output.write( (char*) &bh, sizeof(BitmapHeader) );
	output.write( (char*) &dh, sizeof(DibHeader) );

	ptr = data + (width * height);
	for (uint32_t i = 0; i < height; i++)
	{
		ptr -= width;

		for (uint32_t j = 0; j < width; ++j)
			output.write( (char*) (ptr + j), 3 );

		if (suffix > 0)
			output.write( (char*) &zero, suffix );
	}

	output.close();

  return 0;
}

// Allocates memory for and loads an Windows Bitmap image (BMP3, 24 bits)
int loadBitmap(	const string &fileName, uint32_t *&data,
		uint16_t &width, uint16_t &height ) {
	BitmapHeader bh;
	DibHeader dh;
	uint16_t suffix;
	uint32_t zero = 0;
	uint32_t *ptr;
	uint16_t bits;

	ifstream input(fileName.c_str(), std::ios_base::binary);
	if (!input.good()) return -1;

	input.read( (char*) &bh, sizeof(BitmapHeader) );
	if (bh.bfType != 0x4D42) return -1;
	input.read( (char*) &dh.biSize, sizeof(uint32_t) );
	if (dh.biSize != 40) return -2;

	input.read( (char*) &dh.biWidth, sizeof(DibHeader) - sizeof(uint32_t) );
	width  = dh.biWidth;
	height = dh.biHeight;
	if (dh.biBitCount != 24) return -3;

	// suffix = ((width + 3) & ~0x03) - width;
	suffix = ( 4 - (3*width)%4 )%4;
	ptr = data = new uint32_t[width * height]();
	ptr += width * height;
	for (uint32_t i = 0; i < height; i++)
	{
		ptr -= width;

		for (uint32_t j = 0; j < width; ++j)
		{
			input.read( (char*) (ptr + j), 3 );
			*(ptr + j) |= 0xFF000000;
		}

		if (suffix > 0)
			input.read( (char*) &zero, suffix );
	}

	input.close();
	return 0;
}

// Like loadBitmap(), but allocates "pad" pixels on all four sides, and fills
// them with the nearest pixel values. This greatly simplifies edge handling
// in the scaling algos. The only place to add the padding is when the data
// structure is first created and populated. True width and height are
// width+2*pad, height+2*pad. This is not separately reported, client code
// is responsible for providing to algos a data struct w/ required padding.
int loadBitmapPadded( const string &fileName, uint32_t *&data,
		      uint16_t &width, uint16_t &height, uint16_t pad ) {
	BitmapHeader bh;
	DibHeader dh;
	uint16_t suffix;
	uint32_t zero = 0;
	uint32_t *ptr;
	uint16_t bits;

	uint16_t fullWidth, fullHeight, origin;
	
	ifstream input(fileName.c_str(), std::ios_base::binary);
	if (!input.good()) return -1;

	input.read( (char*) &bh, sizeof(BitmapHeader) );
	if (bh.bfType != 0x4D42) return -1;
	input.read( (char*) &dh.biSize, sizeof(uint32_t) );
	if (dh.biSize != 40) return -2;

	input.read( (char*) &dh.biWidth, sizeof(DibHeader) - sizeof(uint32_t) );
	width  = dh.biWidth;
	height = dh.biHeight;
	if (dh.biBitCount != 24) return -3;

//	suffix = ((width + 3) & ~0x03) - width;      // orig
//	suffix = ((3*width + 3) & ~0x03) - 3*width;  // corrected
	suffix = ( 4 - (3*width)%4 )%4;              // pkj

	fullWidth = width + 2*pad;
	fullHeight = height + 2*pad;
	
	ptr = data = new uint32_t[fullWidth*fullHeight]();
	ptr += pad + (pad+height)*fullWidth;
	
	for (uint32_t i = 0; i < height; i++) {
  	        ptr -= fullWidth;

		for (uint32_t j = 0; j < width; ++j) {
			input.read( (char*) (ptr + j), 3 );
			*(ptr + j) |= 0xFF000000;
		}

		if (suffix > 0) {
			input.read( (char*) &zero, suffix );
		}
	}
	input.close();

	// Top and bottom padding
	for( int i=0; i<width; i++ ) {
	  for( int j=0; j<pad; j++ ) {
	    data[j*fullWidth + pad + i] = data[pad*fullWidth + pad + i];
	    data[(pad+height+j)*fullWidth + pad + i] =
	      data[(pad+height-1)*fullWidth + pad + i];
	  }
	}

	// Left and right padding
	for( int i=0; i<height; i++ ) {
	  for( int j=0; j<pad; j++ ) {
	    data[(pad+i)*fullWidth + j] = data[(pad+i)*fullWidth + pad];
	    data[(pad+i)*fullWidth + pad + width + j] =
	      data[(pad+i)*fullWidth + pad + width - 1];
	  }
	}

	// Corner padding
	for( int i=0; i<pad; i++ ) {
	  for( int j=0; j<pad; j++ ) {
	    origin = 0;
	    data[origin + i*fullWidth + j] = data[pad*fullWidth + pad];

	    origin = pad + width; // fullWidth - pad;
	    data[origin + i*fullWidth + j] = data[pad*fullWidth + pad + width];
	      
	    origin = (pad+height)*fullWidth;
	    data[origin + i*fullWidth + j] = data[(pad+height-1)*fullWidth+pad];

	    origin = (pad+height)*fullWidth + pad + width;
	    data[origin + i*fullWidth + j] =
	      data[(pad+height-1)*fullWidth + pad + width];
	  }
	}

	return 0;
}
