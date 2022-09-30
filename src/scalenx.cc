
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

#include <cstdint>

#include "scalenx.h"

// Copies input to output, pixel by pixel. No scaling. Mostly for testing.
void copy( uint32_t *img, int w, int h, uint32_t *out ) {
  uint32_t *p = img;
  uint32_t *q = out;
  
  for( int j=0; j<h; j++ ) {
    p = img + j*w;
    q = out + j*w;
    
    for( int i=0; i<w; i++ ) {
      q[i] = p[i];
    }
  }
}

// Expands every input pixel to a 2x2 block. No interpolation.
void block2( uint32_t *img, int w, int h, uint32_t *out ) {
  uint32_t *p = img;
  uint32_t *q = out;
  
  for( int j=0; j<h; j++ ) {
    p = img + j*w;   
    q = out + 2*2*j*w;
    
    for( int i=0; i<w; i++ ) {
      q[2*i] = p[i];
      q[2*i+1] = p[i];
      q[2*i+2*w] = p[i];
      q[2*i+2*w+1] = p[i];      
    }
  }
}

// Expands every input pixel to a 3x3 block. No interpolation.
void block3( uint32_t *img, int w, int h, uint32_t *out ) {
  uint16_t scl = 3;  

  uint32_t *p = img;
  uint32_t *q1 = out;
  uint32_t *q2 = q1 + scl*w;
  uint32_t *q3 = q2 + scl*w;
  
  for( int j=0; j<h; j++ ) {
    for( int i=0; i<w; i++ ) {

      for( int k=0; k<scl; k++ ) {
	q1[scl*i+k] = p[i];
	q2[scl*i+k] = p[i];
	q3[scl*i+k] = p[i];
      }
    }
      
    p += w;
    q1 += scl*scl*w;
    q2 += scl*scl*w;
    q3 += scl*scl*w;    
  }
}

// scale2x algo: http://www.scale2x.it/algorithm
// This version handles boundaries and does not require padded input
void scale2x( uint32_t *img, int W, int H, uint32_t *out ) {
  uint16_t scl = 2;
  
  uint32_t *p = img;
  uint32_t *q1 = out;
  uint32_t *q2 = out + scl*W;

  uint32_t b, d, e, f, h;
  
  for( int j=0; j<H; j++ ) {    
    for( int i=0; i<W; i++ ) {
      b = j>0 ? p[i-W] : p[i];
      d = i>0 ? p[i-1] : p[0];
      e = p[i];
      f = i<W-1 ? p[i+1] : p[i];
      h = j<H-1 ? p[i+W] : p[i];

      // E0 = D == B && B != H && D != F ? D : E;
      // E1 = B == F && B != H && D != F ? F : E;
      // E2 = D == H && B != H && D != F ? D : E;
      // E3 = H == F && B != H && D != F ? F : E;
      
      q1[ scl*i ] = d == b && b != h && d != f ? d : e;
      q1[scl*i+1] = b == f && b != h && d != f ? f : e;
      q2[ scl*i ] = d == h && b != h && d != f ? d : e;
      q2[scl*i+1] = h == f && b != h && d != f ? f : e;
    }

    p += W;
    q1 += scl*scl*W;
    q2 += scl*scl*W;
  }
}

// Same as scale2x, but requires a 1px padding on all four sides.
void scale2xPad( uint32_t *img, uint16_t W, uint16_t H, uint32_t *out ) {
  uint16_t scl = 2;
  uint16_t pad = 1;
  
  uint16_t V = W+2*pad;
  uint32_t *p = img + V + pad;
  uint32_t *q1 = out;
  uint32_t *q2 = out + scl*W;

  uint32_t b, d, e, f, h;
  
  for( int j=0; j<H; j++ ) {    
    for( int i=0; i<W; i++ ) {
      b = p[i-V];
      d = p[i-1];
      e = p[i];
      f = p[i+1];
      h = p[i+V];

      // E0 = D == B && B != H && D != F ? D : E;
      // E1 = B == F && B != H && D != F ? F : E;
      // E2 = D == H && B != H && D != F ? D : E;
      // E3 = H == F && B != H && D != F ? F : E;
      
      q1[ scl*i ] = d == b && b != h && d != f ? d : e;
      q1[scl*i+1] = b == f && b != h && d != f ? f : e;
      q2[ scl*i ] = d == h && b != h && d != f ? d : e;
      q2[scl*i+1] = h == f && b != h && d != f ? f : e;
    }

    p += V;
    q1 += scl*scl*W;
    q2 += scl*scl*W;
  }
}

// Improved scale2x by Sp00kyFox. Impl requires 2px padding on all four sides. 
// https://web.archive.org/web/20160527015550/https://libretro.com/forums/archive/index.php?t-1655.html
void scale2xSFX( uint32_t *img, int w, int h, uint32_t *out ) {
  uint16_t pad = 2;
  uint16_t scl = 2;  

  uint16_t V = w + 2*pad;

  uint32_t *p = img + pad*V + pad;
  uint32_t *q1 = out;
  uint32_t *q2 = out + scl*w;
  
  uint32_t A, B, C, D, E, F, G, H, I, J, K, L, M;
  uint32_t e0, e1, e2, e3;
  
  for( int j=0; j<h; j++ ) {
    for( int i=0; i<w; i++ ) {
      J = p[i-2*V];
      
      A = p[i-V-1];
      B = p[i-V];
      C = p[i-V+1];

      K = p[i-2];
      D = p[i-1];
      E = p[i];
      F = p[i+1];
      L = p[i+2];

      G = p[i+V-1];
      H = p[i+V];
      K = p[i+V+1];

      M = p[i+2*V];
      
      /*      
      E0 = B=D & B!=F & D!=H & (E!=A | E=C | E=G | A=J | A=K) ? 0.5*(B+D) : E
      E1 = B=F & B!=D & F!=H & (E!=C | E=A | E=I | C=J | C=L) ? 0.5*(B+F) : E
      E2 = D=H & B!=D & F!=H & (E!=G | E=A | E=I | G=K | G=M) ? 0.5*(D+H) : E
      E3 = F=H & B!=F & D!=H & (E!=I | E=C | E=G | I=L | I=M) ? 0.5*(F+H) : E
      */

      e0 = B==D && B!=F && D!=H && (E!=A || E==C || E==G || A==J || A==K) ?D:E;
      e1 = B==F && B!=D && F!=H && (E!=C || E==A || E==I || C==J || C==L) ?F:E;
      e2 = D==H && B!=D && F!=H && (E!=G || E==A || E==I || G==K || G==M) ?D:E;
      e3 = F==H && B!=F && D!=H && (E!=I || E==C || E==G || I==L || I==M) ?F:E;
      
      q1[ scl*i ] = e0;
      q1[scl*i+1] = e1;
      q2[ scl*i ] = e2;
      q2[scl*i+1] = e3;
    }

    p += V;
    q1 += scl*scl*w;
    q2 += scl*scl*w;
  }
}


// scale3x algo: http://www.scale2x.it/algorithm
// Impl requires 1px padding on all four sides.
void scale3xPad( uint32_t *img, uint16_t w, uint16_t h, uint32_t *out ) {
  uint16_t pad = 1;
  uint16_t scl = 3;  
  uint16_t V = w+2*pad;
  
  uint32_t *p = img + V + pad;
  uint32_t *q1 = out;
  uint32_t *q2 = q1 + scl*w;
  uint32_t *q3 = q2 + scl*w;

  uint32_t A, B, C, D, E, F, G, H, I;
  uint32_t E0, E1, E2, E3, E4, E5, E6, E7, E8;
  
  for( int j=0; j<h; j++ ) {    
    for( int i=0; i<w; i++ ) {
      A = p[i-V-1];
      B = p[i-V];
      C = p[i-V+1];
	
      D = p[i-1];
      E = p[i];
      F = p[i+1];
      
      G = p[i+V-1];
      H = p[i+V];
      I = p[i+V+1];

      if (B != H && D != F) {
	E0 = D == B ? D : E;
	E1 = (D == B && E != C) || (B == F && E != A) ? B : E;
	E2 = B == F ? F : E;
	E3 = (D == B && E != G) || (D == H && E != A) ? D : E;
	E4 = E;
	E5 = (B == F && E != I) || (H == F && E != C) ? F : E;
	E6 = D == H ? D : E;
	E7 = (D == H && E != I) || (H == F && E != G) ? H : E;
	E8 = H == F ? F : E;
      } else {
	E0 = E;
	E1 = E;
	E2 = E;
	E3 = E;
	E4 = E;
	E5 = E;
	E6 = E;
	E7 = E;
	E8 = E;
      }
      
      q1[ scl*i ] = E0;
      q1[scl*i+1] = E1;
      q1[scl*i+2] = E2;
	    
      q2[ scl*i ] = E3;
      q2[scl*i+1] = E4;
      q2[scl*i+2] = E5;

      q3[ scl*i ] = E6;
      q3[scl*i+1] = E7;
      q3[scl*i+2] = E8;
    }

    p += V;
    q1 += scl*scl*w;
    q2 += scl*scl*w;
    q3 += scl*scl*w;    
  }
}

// Improved scale3x by Sp00kyFox. Impl requires 2px padding on all four sides. 
// https://web.archive.org/web/20160527015550/https://libretro.com/forums/archive/index.php?t-1655.html
void scale3xSFX( uint32_t *img, uint16_t w, uint16_t h, uint32_t *out ) {
  uint16_t pad = 2;
  uint16_t scl = 3;  
  uint16_t V = w+2*pad;
  
  uint32_t *p = img + pad*V + pad;
  uint32_t *q1 = out;
  uint32_t *q2 = q1 + scl*w;
  uint32_t *q3 = q2 + scl*w;

  uint32_t A, B, C, D, E, F, G, H, I, J, K, L, M;
  uint32_t E0, E1, E2, E3, E4, E5, E6, E7, E8;  
  
  for( int j=0; j<h; j++ ) {
    for( int i=0; i<w; i++ ) {
      J = p[i-2*V];
      
      A = p[i-V-1];
      B = p[i-V];
      C = p[i-V+1];

      K = p[i-2];
      D = p[i-1];
      E = p[i];
      F = p[i+1];
      L = p[i+2];

      G = p[i+V-1];
      H = p[i+V];
      I = p[i+V+1];

      M = p[i+2*V];      
      
      E4 = E;

      E0 = (B==D && B!=F && D!=H && (E!=A || E==C || E==G || A==J || A==K)) || (B==D && C==E && C!=J && A!=E) || (B==D && E==G && A!=E && G!=K) ? (B+D)/2 : E;
      E2 = (B==F && B!=D && F!=H && (E!=C || E==A || E==I || C==J || C==L)) || (B==F && A==E && A!=J && C!=E) || (B==F && E==I && C!=E && I!=L) ? (B+F)/2 : E;
      E6 = (D==H && B!=D && F!=H && (E!=G || E==A || E==I || G==K || G==M)) || (D==H && A==E && A!=K && E!=G) || (D==H && E==I && E!=G && I!=M) ? (D+H)/2 : E;
      E8 = (F==H && B!=F && D!=H && (E!=I || E==C || E==G || I==L || I==M)) || (F==H && C==E && C!=L && E!=I) || (F==H && E==G && E!=I && G!=M) ? (F+H)/2 : E;
      
      E1 = (B==D && B!=F && D!=H && (E!=A || E==C || E==G || A==J || A==K) & E!=C) || (B==F && B!=D && F!=H && (E!=C || E==A || E==I || C==J || C==L) && E!=A) ? B : E;
      E3 = (B==D && B!=F && D!=H && (E!=A || E==C || E==G || A==J || A==K) & E!=G) || (D==H && B!=D && F!=H && (E!=G || E==A || E==I || G==K || G==M) && E!=A) ? D : E;
      E5 = (F==H && B!=F && D!=H && (E!=I || E==C || E==G || I==L || I==M) & E!=C) || (B==F && B!=D && F!=H && (E!=C || E==A || E==I || C==J || C==L) && E!=I) ? F : E;
      E7 = (F==H && B!=F && D!=H && (E!=I || E==C || E==G || I==L || I==M) & E!=G) || (D==H && B!=D && F!=H && (E!=G || E==A || E==I || G==K || G==M) && E!=I) ? H : E;
      
      q1[ scl*i ] = E0;
      q1[scl*i+1] = E1;
      q1[scl*i+2] = E2;
	    
      q2[ scl*i ] = E3;
      q2[scl*i+1] = E4;
      q2[scl*i+2] = E5;

      q3[ scl*i ] = E6;
      q3[scl*i+1] = E7;
      q3[scl*i+2] = E8;
    }

    p += V;
    q1 += scl*scl*w;
    q2 += scl*scl*w;
    q3 += scl*scl*w;    
  }
}
