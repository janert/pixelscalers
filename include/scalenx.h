
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
		  
#ifndef __JANERT_PIXELSCALERS_SCALENX__
#define __JANERT_PIXELSCALERS_SCALENX__

void copy( uint32_t *img, int w, int h, uint32_t *out );
void block2( uint32_t *img, int w, int h, uint32_t *out );
void block3( uint32_t *img, int w, int h, uint32_t *out );
void scale2x( uint32_t *img, int W, int H, uint32_t *out );
void scale2xPad( uint32_t *img, uint16_t W, uint16_t H, uint32_t *out );
void scale2xSFX( uint32_t *img, int w, int h, uint32_t *out );
void scale3xPad( uint32_t *img, uint16_t w, uint16_t h, uint32_t *out );
void scale3xSFX( uint32_t *img, uint16_t w, uint16_t h, uint32_t *out );

#endif
