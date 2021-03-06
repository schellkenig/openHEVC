/*
 * HEVC video Decoder
 *
 * Copyright (C) 2012 Guillaume Martres
 *
 * This file is part of Libav.
 *
 * Libav is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * Libav is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Libav; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "hevc.h"
#include "hevcdsp.h"

#define BIT_DEPTH 8
#include "hevcdsp_template.c"
#undef BIT_DEPTH

#define BIT_DEPTH 9
#include "hevcdsp_template.c"
#undef BIT_DEPTH
static void dequant(int16_t *coeffs, int log2_size, int qp, int bit_depth)
{
    int x, y;
    int size = 1 << log2_size;

    const uint8_t level_scale[] = { 40, 45, 51, 57, 64, 72 };

    //TODO: scaling_list_enabled_flag support

    int m = 16;
    int shift = bit_depth + log2_size - 5;
    int scale = level_scale[qp % 6] << (qp / 6);
    for (y = 0; y < size; y++)
        for (x = 0; x < size; x++)
            coeffs[size*y+x] = av_clip_int16_c(((coeffs[size*y+x] * m * scale) +
                                                (1 << (shift - 1))) >> shift);
}

void ff_hevc_dsp_init(HEVCDSPContext *hevcdsp, int bit_depth)
{
#undef FUNC
#define FUNC(a, depth) a ## _ ## depth

#define HEVC_DSP(depth)                                                     \
    hevcdsp->transquant_bypass = FUNC(transquant_bypass, depth);            \
    hevcdsp->transform_skip = FUNC(transform_skip, depth);                  \
    hevcdsp->transform_4x4_luma_add = FUNC(transform_4x4_luma_add, depth);  \
    hevcdsp->transform_add[0] = FUNC(transform_4x4_add, depth);             \
    hevcdsp->transform_add[1] = FUNC(transform_8x8_add, depth);             \
    hevcdsp->transform_add[2] = FUNC(transform_16x16_add, depth);           \
    hevcdsp->transform_add[3] = FUNC(transform_32x32_add, depth);           \
    hevcdsp->sao_band_filter  = FUNC(sao_band_filter, depth);               \
    hevcdsp->sao_edge_filter  = FUNC(sao_edge_filter, depth);               \
                                                                            \
    hevcdsp->put_hevc_qpel[0][0] = FUNC(put_hevc_qpel_pixels, depth);       \
    hevcdsp->put_hevc_qpel[0][1] = FUNC(put_hevc_qpel_h1, depth);           \
    hevcdsp->put_hevc_qpel[0][2] = FUNC(put_hevc_qpel_h2, depth);           \
    hevcdsp->put_hevc_qpel[0][3] = FUNC(put_hevc_qpel_h3, depth);           \
    hevcdsp->put_hevc_qpel[1][0] = FUNC(put_hevc_qpel_v1, depth);           \
    hevcdsp->put_hevc_qpel[1][1] = FUNC(put_hevc_qpel_h1v1, depth);         \
    hevcdsp->put_hevc_qpel[1][2] = FUNC(put_hevc_qpel_h2v1, depth);         \
    hevcdsp->put_hevc_qpel[1][3] = FUNC(put_hevc_qpel_h3v1, depth);         \
    hevcdsp->put_hevc_qpel[2][0] = FUNC(put_hevc_qpel_v2, depth);           \
    hevcdsp->put_hevc_qpel[2][1] = FUNC(put_hevc_qpel_h1v2, depth);         \
    hevcdsp->put_hevc_qpel[2][2] = FUNC(put_hevc_qpel_h2v2, depth);         \
    hevcdsp->put_hevc_qpel[2][3] = FUNC(put_hevc_qpel_h3v2, depth);         \
    hevcdsp->put_hevc_qpel[3][0] = FUNC(put_hevc_qpel_v3, depth);           \
    hevcdsp->put_hevc_qpel[3][1] = FUNC(put_hevc_qpel_h1v3, depth);         \
    hevcdsp->put_hevc_qpel[3][2] = FUNC(put_hevc_qpel_h2v3, depth);         \
    hevcdsp->put_hevc_qpel[3][3] = FUNC(put_hevc_qpel_h3v3, depth);         \
                                                                            \
    hevcdsp->put_hevc_epel[0][0] = FUNC(put_hevc_epel_pixels, depth);       \
    hevcdsp->put_hevc_epel[0][1] = FUNC(put_hevc_epel_h, depth);            \
    hevcdsp->put_hevc_epel[1][0] = FUNC(put_hevc_epel_v, depth);            \
    hevcdsp->put_hevc_epel[1][1] = FUNC(put_hevc_epel_hv, depth);

    hevcdsp->dequant = dequant;
    if (bit_depth > 8) {
        HEVC_DSP(9);
    } else {
        HEVC_DSP(8);
    }
}
