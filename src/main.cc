
/*

MIT License

Copyright (c) 2022 Philipp K. Janert

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/		  
		  
#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <string>

#include "bitmap.h"
#include "scalenx.h"
#include "xbr.h"
#include "hqx.h"

using std::string;

void print_usage(int err) {
  if( err > 0) {
    std::cerr << "Unknown algorithm" << std::endl << "" << std::endl;
  }
  std::cerr << "Usage: pixelscaler algo infile [outfile]" << std::endl;
  std::cerr << "Algos: copy block2 block3 scale2x scale2xSFX scale3x scale3xSFX hq2xA hq2xB hq3xA hq3xB superXBR" << std::endl;
  std::cerr << "File format: Microsoft Bitmap BMP3 24bits per pixel"<<std::endl;
}

// Takes 2 or 3 arguments: algo infile outfile
// If only two args are present, output filename defaults to "output.bmp"
// The first arg, giving the algo must be present and be one of:...
int main(int argc, char **argv )
{
  string algo = "";
  string infile = "";
  string outfile = "output.bmp";
  
  // in, out = stdin, stdout  
  if( argc > 3 ) { outfile = argv[3]; }
  if( argc > 2 ) { infile = argv[2];  }
  
  if( argc > 1 )  {
    algo = argv[1];
  } else {
    print_usage(0);
    return 0;
  }

  uint32_t factor = 1;
  uint16_t padding = 0;

  if(      algo == "copy" )       { factor = 1; padding = 0; }
  else if( algo == "block2" )     { factor = 2; padding = 0; }
  else if( algo == "block3" )     { factor = 3; padding = 0; }  
  else if( algo == "scale2x" )    { factor = 2; padding = 0; }
  else if( algo == "scale2xPad" ) { factor = 2; padding = 1; }
  else if( algo == "scale2xSFX" ) { factor = 2; padding = 2; }
  else if( algo == "scale3x" )    { factor = 3; padding = 1; }
  else if( algo == "scale3xSFX" ) { factor = 3; padding = 2; } 
  else if( algo == "hq2xA" )      { factor = 2; padding = 0; }
  else if( algo == "hq2xB" )      { factor = 2; padding = 0; }
  else if( algo == "hq3xA" )      { factor = 3; padding = 0; }
  else if( algo == "hq3xB" )      { factor = 3; padding = 0; }
  else if( algo == "superXBR" )   { factor = 2; padding = 0; }  
  else {
    print_usage( 1 );
    return 0;
  }   
  
  // load the input image
  uint16_t width, height;
  uint32_t *image = NULL;
  if( int res = loadBitmapPadded(infile, image, width, height, padding) ) {
    std::cerr << "Loading image failed " << res << std::endl;
    return 1;
  }
    
  // resize the input image using the given scale factor
  uint32_t outputSize = (width * factor) * (height * factor);
  uint32_t *output = new uint32_t[outputSize]();

  std::cerr<<"Scaling now: "<<algo<<" "<<width<<"x"<<height<<std::endl;
  if(      algo == "copy" )       { copy( image, width, height, output ); }	
  else if( algo == "block2" )     { block2( image, width, height, output ); }	
  else if( algo == "block3" )     { block3( image, width, height, output ); } 
  else if( algo == "scale2x" )    { scale2x( image, width, height, output ); }
  else if( algo == "scale2xPad" ) { scale2xPad( image, width, height, output );}
  else if( algo == "scale2xSFX" ) { scale2xSFX( image, width, height, output );}
  else if( algo == "scale3x" )    { scale3xPad( image, width, height, output );}
  else if( algo == "scale3xSFX" ) { scale3xSFX( image, width, height, output );}
  else if( algo == "hq2xA" )      { hq2xA( image, width, height, output ); }
  else if( algo == "hq2xB" )      { hq2xB( image, width, height, output ); }
  else if( algo == "hq3xA" )      { hq3xA( image, width, height, output ); }
  else if( algo == "hq3xB" )      { hq3xB( image, width, height, output ); }
  else if( algo == "superXBR" ) { scaleSuperXBR( image, width, height, output);}
  else {
    // should never happen...
  }

  // saves the resized image
  if( saveBitmap(output, width*factor, height*factor, outfile) != 0 ) {
    std::cerr << "Saving image failed " << std::endl;
  }

  delete[] image;
  delete[] output;
}
