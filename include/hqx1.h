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

#ifndef __JANERT_PIXELSCALERS_HQX1__
#define __JANERT_PIXELSCALERS_HQX1__


/* This is a private header file, only to be included by the hq src files.
   For the public header file, see hqx.h */


#include <cstdint>

uint32_t ARGBtoAYUV( uint32_t value );
bool isDifferentA( uint32_t color1, uint32_t color2,
		   uint32_t trY, uint32_t trU, uint32_t trV, uint32_t trA );
bool isDifferentB( uint32_t color1, uint32_t color2,
		   uint32_t trY, uint32_t trU, uint32_t trV, uint32_t trA );



#define MASK_RB   0x00FF00FF
#define MASK_G    0x0000FF00
#define MASK_A    0xFF000000


/**
 * @brief Mixes two colors using the given weights.
 */
#define HQX_MIX_2(C0,C1,W0,W1) \
	((((C0 & MASK_RB) * W0 + (C1 & MASK_RB) * W1) / (W0 + W1)) & MASK_RB) | \
	((((C0 & MASK_G)  * W0 + (C1 & MASK_G)  * W1) / (W0 + W1)) & MASK_G)  | \
	((((((C0 & MASK_A) >> 8)  * W0 + ((C1 & MASK_A) >> 8) * W1) / (W0 + W1)) << 8) & MASK_A)

/**
 * @brief Mixes three colors using the given weights.
 */
#define HQX_MIX_3(C0,C1,C2,W0,W1,W2) \
	((((C0 & MASK_RB) * W0 + (C1 & MASK_RB) * W1 + (C2 & MASK_RB) * W2) / (W0 + W1 + W2)) & MASK_RB) | \
	((((C0 & MASK_G)  * W0 + (C1 & MASK_G)  * W1 + (C2 & MASK_G)  * W2) / (W0 + W1 + W2)) & MASK_G)  | \
	((((((C0 & MASK_A) >> 8) * W0 + ((C1 & MASK_A) >> 8) * W1 + ((C2 & MASK_A) >> 8) * W2) / (W0 + W1 + W2)) << 8) & MASK_A)


#define MIX_00_4				*output = w[4];
#define MIX_00_4_0_3_1			*output = HQX_MIX_2(w[4],w[0],3U,1U);
#define MIX_00_4_3_3_1			*output = HQX_MIX_2(w[4],w[3],3U,1U);
#define MIX_00_4_1_3_1			*output = HQX_MIX_2(w[4],w[1],3U,1U);
#define MIX_00_3_1_1_1			*output = HQX_MIX_2(w[3],w[1],1U,1U);
#define MIX_00_4_3_1_2_1_1		*output = HQX_MIX_3(w[4],w[3],w[1],2U,1U,1U);
#define MIX_00_4_3_1_2_7_7 		*output = HQX_MIX_3(w[4],w[3],w[1],2U,7U,7U);
#define MIX_00_4_0_1_2_1_1		*output = HQX_MIX_3(w[4],w[0],w[1],2U,1U,1U);
#define MIX_00_4_0_3_2_1_1		*output = HQX_MIX_3(w[4],w[0],w[3],2U,1U,1U);
#define MIX_00_4_1_3_5_2_1		*output = HQX_MIX_3(w[4],w[1],w[3],5U,2U,1U);
#define MIX_00_4_3_1_5_2_1		*output = HQX_MIX_3(w[4],w[3],w[1],5U,2U,1U);
#define MIX_00_4_3_1_6_1_1		*output = HQX_MIX_3(w[4],w[3],w[1],6U,1U,1U);
#define MIX_00_4_3_1_2_3_3		*output = HQX_MIX_3(w[4],w[3],w[1],2U,3U,3U);
#define MIX_00_4_3_1_e_1_1		*output = HQX_MIX_3(w[4],w[3],w[1],14U,1U,1U);

#define MIX_01_4			*(output + 1) = w[4];
#define MIX_01_4_2_3_1		*(output + 1) = HQX_MIX_2(w[4],w[2],3U,1U);
#define MIX_01_4_1_3_1		*(output + 1) = HQX_MIX_2(w[4],w[1],3U,1U);
#define MIX_01_1_4_3_1		*(output + 1) = HQX_MIX_2(w[1],w[4],3U,1U);
#define MIX_01_4_5_3_1		*(output + 1) = HQX_MIX_2(w[4],w[5],3U,1U);
#define MIX_01_4_1_7_1		*(output + 1) = HQX_MIX_2(w[4],w[1],7U,1U);
#define MIX_01_4_1_5_2_1_1	*(output + 1) = HQX_MIX_3(w[4],w[1],w[5],2U,1U,1U);
#define MIX_01_4_2_5_2_1_1	*(output + 1) = HQX_MIX_3(w[4],w[2],w[5],2U,1U,1U);
#define MIX_01_4_2_1_2_1_1	*(output + 1) = HQX_MIX_3(w[4],w[2],w[1],2U,1U,1U);
#define MIX_01_4_5_1_5_2_1	*(output + 1) = HQX_MIX_3(w[4],w[5],w[1],5U,2U,1U);
#define MIX_01_4_1_5_5_2_1	*(output + 1) = HQX_MIX_3(w[4],w[1],w[5],5U,2U,1U);
#define MIX_01_4_1_5_6_1_1	*(output + 1) = HQX_MIX_3(w[4],w[1],w[5],6U,1U,1U);
#define MIX_01_4_1_5_2_3_3	*(output + 1) = HQX_MIX_3(w[4],w[1],w[5],2U,3U,3U);
#define MIX_01_4_1_5_e_1_1	*(output + 1) = HQX_MIX_3(w[4],w[1],w[5],14U,1U,1U);

#define MIX_02_4			*(output + 2) = w[4];
#define MIX_02_4_2_3_1		*(output + 2) = HQX_MIX_2(w[4],w[2],3U,1U);
#define MIX_02_4_1_3_1		*(output + 2) = HQX_MIX_2(w[4],w[1],3U,1U);
#define MIX_02_4_5_3_1  	*(output + 2) = HQX_MIX_2(w[4],w[5],3U,1U);
#define MIX_02_4_1_5_2_1_1	*(output + 2) = HQX_MIX_3(w[4],w[1],w[5],2U,1U,1U);
#define MIX_02_4_1_5_2_7_7	*(output + 2) = HQX_MIX_3(w[4],w[1],w[5],2U,7U,7U);
#define MIX_02_1_5_1_1		*(output + 2) = HQX_MIX_2(w[1],w[5],1U,1U);

#define MIX_10_4			*(output + lineSize) = w[4];
#define MIX_10_4_6_3_1		*(output + lineSize) = HQX_MIX_2(w[4],w[6],3U,1U);
#define MIX_10_4_7_3_1		*(output + lineSize) = HQX_MIX_2(w[4],w[7],3U,1U);
#define MIX_10_4_3_3_1		*(output + lineSize) = HQX_MIX_2(w[4],w[3],3U,1U);
#define MIX_10_4_7_3_2_1_1	*(output + lineSize) = HQX_MIX_3(w[4],w[7],w[3],2U,1U,1U);
#define MIX_10_4_6_3_2_1_1	*(output + lineSize) = HQX_MIX_3(w[4],w[6],w[3],2U,1U,1U);
#define MIX_10_4_6_7_2_1_1	*(output + lineSize) = HQX_MIX_3(w[4],w[6],w[7],2U,1U,1U);
#define MIX_10_4_3_7_5_2_1	*(output + lineSize) = HQX_MIX_3(w[4],w[3],w[7],5U,2U,1U);
#define MIX_10_4_7_3_5_2_1	*(output + lineSize) = HQX_MIX_3(w[4],w[7],w[3],5U,2U,1U);
#define MIX_10_4_7_3_6_1_1	*(output + lineSize) = HQX_MIX_3(w[4],w[7],w[3],6U,1U,1U);
#define MIX_10_4_7_3_2_3_3	*(output + lineSize) = HQX_MIX_3(w[4],w[7],w[3],2U,3U,3U);
#define MIX_10_4_7_3_e_1_1	*(output + lineSize) = HQX_MIX_3(w[4],w[7],w[3],14U,1U,1U);
#define MIX_10_4_3_7_1  	*(output + lineSize) = HQX_MIX_2(w[4],w[3],7U,1U);
#define MIX_10_3_4_3_1  	*(output + lineSize) = HQX_MIX_2(w[3],w[4],3U,1U);

#define MIX_11_4			*(output + lineSize + 1) = w[4];
#define MIX_11_4_8_3_1		*(output + lineSize + 1) = HQX_MIX_2(w[4],w[8],3U,1U);
#define MIX_11_4_5_3_1		*(output + lineSize + 1) = HQX_MIX_2(w[4],w[5],3U,1U);
#define MIX_11_4_7_3_1		*(output + lineSize + 1) = HQX_MIX_2(w[4],w[7],3U,1U);
#define MIX_11_4_5_7_2_1_1	*(output + lineSize + 1) = HQX_MIX_3(w[4],w[5],w[7],2U,1U,1U);
#define MIX_11_4_8_7_2_1_1	*(output + lineSize + 1) = HQX_MIX_3(w[4],w[8],w[7],2U,1U,1U);
#define MIX_11_4_8_5_2_1_1	*(output + lineSize + 1) = HQX_MIX_3(w[4],w[8],w[5],2U,1U,1U);
#define MIX_11_4_7_5_5_2_1	*(output + lineSize + 1) = HQX_MIX_3(w[4],w[7],w[5],5U,2U,1U);
#define MIX_11_4_5_7_5_2_1	*(output + lineSize + 1) = HQX_MIX_3(w[4],w[5],w[7],5U,2U,1U);
#define MIX_11_4_5_7_6_1_1	*(output + lineSize + 1) = HQX_MIX_3(w[4],w[5],w[7],6U,1U,1U);
#define MIX_11_4_5_7_2_3_3	*(output + lineSize + 1) = HQX_MIX_3(w[4],w[5],w[7],2U,3U,3U);
#define MIX_11_4_5_7_e_1_1	*(output + lineSize + 1) = HQX_MIX_3(w[4],w[5],w[7],14U,1U,1U);

#define MIX_12_4			*(output + lineSize + 2) = w[4];
#define MIX_12_4_5_3_1		*(output + lineSize + 2) = HQX_MIX_2(w[4],w[5],3U,1U);
#define MIX_12_4_5_7_1		*(output + lineSize + 2) = HQX_MIX_2(w[4],w[5],7U,1U);
#define MIX_12_5_4_3_1		*(output + lineSize + 2) = HQX_MIX_2(w[5],w[4],3U,1U);

#define MIX_20_4			*(output + lineSize + lineSize) = w[4];
#define MIX_20_4_6_3_1		*(output + lineSize + lineSize) = HQX_MIX_2(w[4],w[6],3U,1U);
#define MIX_20_4_7_3_1		*(output + lineSize + lineSize) = HQX_MIX_2(w[4],w[7],3U,1U);
#define MIX_20_4_3_3_1		*(output + lineSize + lineSize) = HQX_MIX_2(w[4],w[3],3U,1U);
#define MIX_20_4_7_3_2_1_1	*(output + lineSize + lineSize) = HQX_MIX_3(w[4],w[7],w[3],2U,1U,1U);
#define MIX_20_4_7_3_2_7_7	*(output + lineSize + lineSize) = HQX_MIX_3(w[4],w[7],w[3],2U,7U,7U);
#define MIX_20_7_3_1_1		*(output + lineSize + lineSize) = HQX_MIX_2(w[7],w[3],1U,1U);

#define MIX_21_4			*(output + lineSize + lineSize + 1) = w[4];
#define MIX_21_4_7_3_1		*(output + lineSize + lineSize + 1) = HQX_MIX_2(w[4],w[7],3U,1U);
#define MIX_21_4_7_7_1		*(output + lineSize + lineSize + 1) = HQX_MIX_2(w[4],w[7],7U,1U);
#define MIX_21_7_4_3_1		*(output + lineSize + lineSize + 1) = HQX_MIX_2(w[7],w[4],3U,1U);

#define MIX_22_4			*(output + lineSize + lineSize + 2) = w[4];
#define MIX_22_4_8_3_1		*(output + lineSize + lineSize + 2) = HQX_MIX_2(w[4],w[8],3U,1U);
#define MIX_22_4_7_3_1		*(output + lineSize + lineSize + 2) = HQX_MIX_2(w[4],w[7],3U,1U);
#define MIX_22_4_5_3_1		*(output + lineSize + lineSize + 2) = HQX_MIX_2(w[4],w[5],3U,1U);
#define MIX_22_4_5_7_2_1_1	*(output + lineSize + lineSize + 2) = HQX_MIX_3(w[4],w[5],w[7],2U,1U,1U);
#define MIX_22_4_5_7_2_7_7	*(output + lineSize + lineSize + 2) = HQX_MIX_3(w[4],w[5],w[7],2U,7U,7U);
#define MIX_22_5_7_1_1		*(output + lineSize + lineSize + 2) = HQX_MIX_2(w[5],w[7],1U,1U);


#endif





