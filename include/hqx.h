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

#ifndef __JANERT_PIXELSCALERS_HQX__
#define __JANERT_PIXELSCALERS_HQX__

#include <cstdint>

void hq2xA( uint32_t *img, int w, int h, uint32_t *out );
void hq2xB( uint32_t *img, int w, int h, uint32_t *out );

void hq3xA( uint32_t *img, int w, int h, uint32_t *out );
void hq3xB( uint32_t *img, int w, int h, uint32_t *out );

#endif





