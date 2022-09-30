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

#ifndef __JANERT_PIXELSCALERS_BITMAP__
#define __JANERT_PIXELSCALERS_BITMAP__

#include <cstdint>
#include <string>

int saveBitmap(	const uint32_t *data, uint32_t width, uint32_t height,
		const std::string &fileName );
int loadBitmap(	const std::string &fileName, uint32_t *&data,
		uint16_t &width, uint16_t &height );
int loadBitmapPadded( const std::string &fileName, uint32_t *&data,
		      uint16_t &width, uint16_t &height, uint16_t pad );

#endif
