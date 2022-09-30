
# PixelArt Scalers

A collection of scaling algorithms for "pixel-art", combined into a single, 
stand-alone tool, using a common, general-purpose programming language.


## Usage

```
pixelscaler algo input.bmp [output.bmp]
```

The input file _must_ be in 
[Bitmap](https://en.wikipedia.org/wiki/BMP_file_format)
(BMP/BMP3) format, Version 3, 24 bits per pixel. 
Generated output uses the same bitmap format. 

The third argument is optional; if it is omitted, the
output file will be named `output.bmp`. 

The second argument selects the scaling algorithm to use, it must
be one of: `block2`, `block3`, `scale2x`, `scale2xSFX`, `scale3x`, 
`scale3xSFX`, `hq2xA`, `hq2xB`, `hq3xA`, `hq3xB`, `superXBR`.

Other file formats must be converted to BMP3 first; many tools (like
ImageMagick or the Gimp) can do that. Just be sure to specify 24bit
colordepth. For example, using ImageMagick, you might use: 
`convert input.gif -type truecolor input.bmp3`.


## Building

```
git clone https://github.com/janert/pixelscalers.git
cd pixelscalers/src
make
```

## Algorithms

This tool combines implementations of several of the well-known
[scaling algorithms](https://en.wikipedia.org/wiki/Pixel-art_scaling_algorithms)
for pixel art.

- `block2` : Each input pixel is expanded into a 2x2 block; no interpolation.
- `block3` : Each input pixel is expanded into a 3x3 block; no interpolation.
- `scale2x` : The [Scale2x](http://www.scale2x.it/algorithm) algorithm, 2x magnification.
- `scale2xSFX` : The improved [`scale2x` algorithm](https://web.archive.org/web/20160527015550/https://libretro.com/forums/archive/index.php?t-1655.html) 
by _Sp00kyFox_, 2x magnification.
- `scale3x` :The [Scale2x](http://www.scale2x.it/algorithm) algorithm, 3x magnification.
- `scale3xSFX` : The improved [`scale3x` algorithm](https://web.archive.org/web/20160527015550/https://libretro.com/forums/archive/index.php?t-1655.html) 
by _Sp00kyFox_, 3x magnification.
- `hq2xA` : The [Hqx algorithm](https://en.wikipedia.org/wiki/Hqx), optimized for simple graphs, 2x magnification.
- `hq2xB` : The [Hqx algorithm](https://en.wikipedia.org/wiki/Hqx), optimized for complex graphs, 2x magnification.
- `hq3xA` : The [Hqx algorithm](https://en.wikipedia.org/wiki/Hqx), optimized for simple graphs, 3x magnification.
- `hq3xB` : The [Hqx algorithm](https://en.wikipedia.org/wiki/Hqx), optimized for complex graphs, 3x magnification.
- `superXBR` : The [Super xBR algorithm](https://en.wikipedia.org/wiki/Pixel-art_scaling_algorithms#xBR_family), 2x magnification.

Not included is the [2×SaI algorithm](https://vdnoort.home.xs4all.nl/emulation/2xsai/). Maybe I will add it at some point.

For some algorithms (in particular the 
[SFX](https://web.archive.org/web/20160527015550/https://libretro.com/forums/archive/index.php?t-1655.html)
versions and the 
[XBR](https://forums.libretro.com/t/xbr-algorithm-tutorial/123)
algorithm),
the primary documentation seem to be forum discussions. Screenshots of those
are included as PDFs in the `docs` directory, to preserve them for reference.


## Examples

These images are BMP files; if they don't show up, try a different browser.

| | | |
|:---:|:---:|:---:|
| | Original <br>&nbsp;<br> ![orig](blob/main/imgs/test_original.gif) | |
| <br>`block2` <br>&nbsp;<br> ![block2](blob/main/imgs/block2.bmp) | | <br>`block3` <br>&nbsp;<br> ![block2](blob/main/imgs/block3.bmp) |
| <br>`scale2x` <br>&nbsp;<br> ![scale2x](blob/main/imgs/scale2x.bmp) | | <br>`scale3x` <br>&nbsp;<br> ![scale3x](blob/main/imgs/scale3x.bmp) |
| <br>`scale2xSFX` <br>&nbsp;<br> ![scale2xSFX](blob/main/imgs/scale2xSFX.bmp) | | <br>`scale3xSFX` <br>&nbsp;<br> ![scale3xSFX](blob/main/imgs/scale3xSFX.bmp) |
| <br>`hq2xA` <br>&nbsp;<br> ![hq2xA](blob/main/imgs/hq2xA.bmp) | | <br>`hq3xA` <br>&nbsp;<br> ![hq3xA](blob/main/imgs/hq3xA.bmp) |
| <br>`hq2xB` <br>&nbsp;<br> ![hq2xB](blob/main/imgs/hq2xB.bmp) | | <br>`hq3xB` <br>&nbsp;<br> ![hq3xB](blob/main/imgs/hq3xB.bmp) |
| <br>`superXBR` <br>&nbsp;<br> ![superXBR](blob/main/imgs/xbr.bmp) | | |



## Issues and Limitations

- IO using standard input/output is not supported, so the tool can
  unfortunately not be used as a filter.

- The size of integer types used in the implementation is not always
  consistent. The implementation may fail for very large images (more
  than 16k pixels along one edge) because of insufficient integer range.

- Since they were not intended for real-time processing, no effort has been 
  made to optimize the execution time of the algorithm implementations.
  
- The code layout has not been unified across the various original
  implementations.


## Genesis

This is largely derivative work. For a personal project, I wanted to
use and _compare_ the effects of a variety of published algorithms,
but existing implementations seemed to be somewhat scattered
about. Some algorithms appear to have only been discussed on forums,
with no implementations in a general-purpose language easily
accessible, or only available via the Wayback Machine. So I thought
it might be useful to collect them in one place and provide a
consistent wrapper around them.

This tool incorporates the works of others. If you are one of the
original authors and you think I am infringing on your rights, let
me know, so we can work it out.


## Acknowledgments

- The implementation of the ScaleNx algorithms follows the descriptions
  [here](http://www.scale2x.it/algorithm), and 
  [here](https://web.archive.org/web/20160527015550/https://libretro.com/forums/archive/index.php?t-1655.html).

- The implementation of the Hqx algorithms, as well as the bitmap
  handling are largely based on [this](https://github.com/brunexgeek/hqx).
  
- The implementation of the Super xBR algorithm is taken from 
  [here](https://pastebin.com/cbH8ZQQT). Also see the 
  [discussion](https://forums.libretro.com/t/xbr-algorithm-tutorial/123).
  
- It's been a long time since I wrote a Makefile; I found this
  [page](https://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/)
  a helpful reminder.


## License

Copyright (c) 2022, Philipp K. Janert. 
All rights reserved.

Unless otherwise stated in individual files, the contents of this 
repository is licensed under the MIT-style license found in the
LICENSE file in this directory.
