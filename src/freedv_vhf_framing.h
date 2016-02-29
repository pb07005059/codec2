/*---------------------------------------------------------------------------*\

  FILE........: freedv_vhf_framing.h
  AUTHOR......: Brady O'Brien
  DATE CREATED: 11 February 2016

  Framer and deframer for VHF FreeDV modes 'A' and 'B'
  Currently designed for-
  * 40ms ota modem frames
  * 40ms Codec2 1300 frames
  * 52 bits of Codec2 per frame
  * 16 bits of unique word per frame
  * 28 'spare' bits per frame
  *  - 4 spare bits at front and end of frame (8 total) for padding
  *  - 20 'protocol' bits, either for higher layers of 'protocol' or
  *  - 18 'protocol' bits and 2 vericode sidechannel bits
\*---------------------------------------------------------------------------*/

/*
  Copyright (C) 2016 David Rowe

  All rights reserved.

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License version 2.1, as
  published by the Free Software Foundation.  This program is
  distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
  License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with this program; if not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _FREEDV_VHF_FRAMING_H
#define _FREEDV_VHF_FRAMING_H

/* Standard frame type */
#define FREEDV_VHF_FRAME_A 1
struct freedv_vhf_deframer {
    int ftype;          /* Type of frame to be looking for */
    int state;          /* State of deframer */
    uint8_t * bits;     /* Bits currently being decanted */
    uint8_t * invbits;  /* Inversion of bits currently being decanted, for FMFSK */
    
    int bitptr;         /* Pointer into circular bit buffer */
    int miss_cnt;       /* How many UWs have been missed */
    int last_uw;        /* How many bits since the last UW? */
    int frame_size;     /* How big is a frame? */
    int on_inv_bits;    /* Are we using the inverted bits? */
};

/* Init and allocate memory for a freedv-vhf framer/deframer */
struct freedv_vhf_deframer * fvhff_create_deframer(uint8_t frame_type,int enable_bit_flip);

/* Free the memory used by a freedv-vhf framer/deframer */
void fvhff_destroy_deframer(struct freedv_vhf_deframer * def);

/* Place codec and other bits into a frame */
void fvhff_frame_bits(int frame_type,uint8_t bits_out[],uint8_t codec2_in[],uint8_t proto_in[],uint8_t vc_in[]);

/* Find and extract frames from a stream of bits */
int fvhff_deframe_bits(struct freedv_vhf_deframer * def,uint8_t codec2_out[],uint8_t proto_out[],uint8_t vc_out[],uint8_t bits_in[]);

/* Is the de-framer synchronized? */
int fvhff_synchronized(struct freedv_vhf_deframer * def);

#endif //_FREEDV_VHF_FRAMING_H