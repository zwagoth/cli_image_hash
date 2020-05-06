#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/********* generated code snippet *********/

#define N 32

static inline void fdct_1d(float *dst, const float *src,
                           int dst_stridea, int dst_strideb,
                           int src_stridea, int src_strideb)
{
    int i;

    for (i = 0; i < N; i++) {
        const float x00 = src[ 0*src_stridea] + src[31*src_stridea];
        const float x11 = src[ 1*src_stridea] + src[30*src_stridea];
        const float x12 = src[ 2*src_stridea] + src[29*src_stridea];
        const float x13 = src[ 3*src_stridea] + src[28*src_stridea];
        const float x14 = src[ 4*src_stridea] + src[27*src_stridea];
        const float x15 = src[ 5*src_stridea] + src[26*src_stridea];
        const float x16 = src[ 6*src_stridea] + src[25*src_stridea];
        const float x17 = src[ 7*src_stridea] + src[24*src_stridea];
        const float x18 = src[ 8*src_stridea] + src[23*src_stridea];
        const float x19 = src[ 9*src_stridea] + src[22*src_stridea];
        const float x1a = src[10*src_stridea] + src[21*src_stridea];
        const float x1b = src[11*src_stridea] + src[20*src_stridea];
        const float x1c = src[12*src_stridea] + src[19*src_stridea];
        const float x1d = src[13*src_stridea] + src[18*src_stridea];
        const float x1e = src[14*src_stridea] + src[17*src_stridea];
        const float x1f = src[15*src_stridea] + src[16*src_stridea];
        const float x01 = src[ 0*src_stridea] - src[31*src_stridea];
        const float x02 = src[ 1*src_stridea] - src[30*src_stridea];
        const float x03 = src[ 2*src_stridea] - src[29*src_stridea];
        const float x04 = src[ 3*src_stridea] - src[28*src_stridea];
        const float x05 = src[ 4*src_stridea] - src[27*src_stridea];
        const float x06 = src[ 5*src_stridea] - src[26*src_stridea];
        const float x07 = src[ 6*src_stridea] - src[25*src_stridea];
        const float x08 = src[ 7*src_stridea] - src[24*src_stridea];
        const float x09 = src[ 8*src_stridea] - src[23*src_stridea];
        const float x0a = src[ 9*src_stridea] - src[22*src_stridea];
        const float x0b = src[10*src_stridea] - src[21*src_stridea];
        const float x0c = src[11*src_stridea] - src[20*src_stridea];
        const float x0d = src[12*src_stridea] - src[19*src_stridea];
        const float x0e = src[13*src_stridea] - src[18*src_stridea];
        const float x0f = src[14*src_stridea] - src[17*src_stridea];
        const float x10 = src[15*src_stridea] - src[16*src_stridea];
        const float x62 = x00 + x1f;
        const float x63 = x11 + x1e;
        const float x64 = x12 + x1d;
        const float x65 = x13 + x1c;
        const float x66 = x14 + x1b;
        const float x67 = x15 + x1a;
        const float x68 = x16 + x19;
        const float x69 = x17 + x18;
        const float x6a = x00 - x1f;
        const float x6b = x11 - x1e;
        const float x6c = x12 - x1d;
        const float x6d = x13 - x1c;
        const float x6e = x14 - x1b;
        const float x6f = x15 - x1a;
        const float x70 = x16 - x19;
        const float x71 = x17 - x18;
        const float x72 = x62 + x69;
        const float x73 = x63 + x68;
        const float x74 = x64 + x67;
        const float x75 = x65 + x66;
        const float x76 = x62 - x69;
        const float x77 = x63 - x68;
        const float x78 = x64 - x67;
        const float x79 = x65 - x66;
        const float x7a = x72 + x75;
        const float x7b = x73 + x74;
        const float x7c = x72 - x75;
        const float x7d = x73 - x74;
        const float x7e = 1.38703984532215*x76 + 0.275899379282943*x79;
        const float x7f = 1.17587560241936*x77 + 0.785694958387102*x78;
        const float x80 = -0.785694958387102*x77 + 1.17587560241936*x78;
        const float x81 = 0.275899379282943*x76 - 1.38703984532215*x79;
        const float x82 = 0.176776695296637 * (x7e - x7f);
        const float x83 = 0.176776695296637 * (x80 - x81);
        const float x84 = 1.40740373752638*x6a + 0.138617169199091*x71;
        const float x85 = 1.35331800117435*x6b + 0.410524527522357*x70;
        const float x86 = 1.24722501298667*x6c + 0.666655658477747*x6f;
        const float x87 = 1.09320186700176*x6d + 0.897167586342636*x6e;
        const float x88 = -0.897167586342636*x6d + 1.09320186700176*x6e;
        const float x89 = 0.666655658477747*x6c - 1.24722501298667*x6f;
        const float x8a = -0.410524527522357*x6b + 1.35331800117435*x70;
        const float x8b = 0.138617169199091*x6a - 1.40740373752638*x71;
        const float x8c = x84 + x87;
        const float x8d = x85 + x86;
        const float x8e = x84 - x87;
        const float x8f = x85 - x86;
        const float x90 = 0.176776695296637 * (x8c - x8d);
        const float x91 = 0.230969883127822*x8e + 0.0956708580912725*x8f;
        const float x92 = 0.0956708580912725*x8e - 0.230969883127822*x8f;
        const float x93 = x88 + x8b;
        const float x94 = x89 + x8a;
        const float x95 = x88 - x8b;
        const float x96 = x89 - x8a;
        const float x97 = 0.176776695296637 * (x93 - x94);
        const float x98 = 0.230969883127822*x95 + 0.0956708580912725*x96;
        const float x99 = 0.0956708580912725*x95 - 0.230969883127822*x96;
        const float x20 = 1.41251008020198*x01 + 0.0693921705079407*x10;
        const float x21 = 1.39890683597308*x02 + 0.207508226988212*x0f;
        const float x22 = 1.37183135419349*x03 + 0.34362586580705*x0e;
        const float x23 = 1.33154438655373*x04 + 0.476434199693161*x0d;
        const float x24 = 1.27843391857524*x05 + 0.604654211790801*x0c;
        const float x25 = 1.21301143309781*x06 + 0.72705107329128*x0b;
        const float x26 = 1.13590698442014*x07 + 0.842446035509419*x0a;
        const float x27 = 1.04786313053259*x08 + 0.949727781877754*x09;
        const float x28 = -0.949727781877754*x08 + 1.04786313053259*x09;
        const float x29 = 0.842446035509419*x07 - 1.13590698442014*x0a;
        const float x2a = -0.72705107329128*x06 + 1.21301143309781*x0b;
        const float x2b = 0.604654211790801*x05 - 1.27843391857524*x0c;
        const float x2c = -0.476434199693161*x04 + 1.33154438655373*x0d;
        const float x2d = 0.34362586580705*x03 - 1.37183135419349*x0e;
        const float x2e = -0.207508226988212*x02 + 1.39890683597308*x0f;
        const float x2f = 0.0693921705079407*x01 - 1.41251008020198*x10;
        const float x30 = x20 + x27;
        const float x31 = x21 + x26;
        const float x32 = x22 + x25;
        const float x33 = x23 + x24;
        const float x34 = x20 - x27;
        const float x35 = x21 - x26;
        const float x36 = x22 - x25;
        const float x37 = x23 - x24;
        const float x3a = x30 + x33;
        const float x3b = x31 + x32;
        const float x3c = x30 - x33;
        const float x3d = x31 - x32;
        const float x3e = 0.176776695296637 * (x3a - x3b);
        const float x3f = 0.230969883127822*x3c + 0.0956708580912725*x3d;
        const float x40 = 0.0956708580912725*x3c - 0.230969883127822*x3d;
        const float x41 = 1.38703984532215*x34 + 0.275899379282943*x37;
        const float x42 = 1.17587560241936*x35 + 0.785694958387102*x36;
        const float x43 = -0.785694958387102*x35 + 1.17587560241936*x36;
        const float x44 = 0.275899379282943*x34 - 1.38703984532215*x37;
        const float x45 = 0.176776695296637 * (x41 + x42);
        const float x46 = 0.176776695296637 * (x41 - x42);
        const float x47 = 0.176776695296637 * (x43 + x44);
        const float x48 = 0.176776695296637 * (x43 - x44);
        const float x38 = 0.707106781186547 * (x46 - x48);
        const float x39 = 0.707106781186547 * (x46 + x48);
        const float x49 = x28 + x2f;
        const float x4a = x29 + x2e;
        const float x4b = x2a + x2d;
        const float x4c = x2b + x2c;
        const float x4d = x28 - x2f;
        const float x4e = x29 - x2e;
        const float x4f = x2a - x2d;
        const float x50 = x2b - x2c;
        const float x53 = x49 + x4c;
        const float x54 = x4a + x4b;
        const float x55 = x49 - x4c;
        const float x56 = x4a - x4b;
        const float x57 = 0.176776695296637 * (x53 - x54);
        const float x58 = 0.230969883127822*x55 + 0.0956708580912725*x56;
        const float x59 = 0.0956708580912725*x55 - 0.230969883127822*x56;
        const float x5a = 1.38703984532215*x4d + 0.275899379282943*x50;
        const float x5b = 1.17587560241936*x4e + 0.785694958387102*x4f;
        const float x5c = -0.785694958387102*x4e + 1.17587560241936*x4f;
        const float x5d = 0.275899379282943*x4d - 1.38703984532215*x50;
        const float x5e = 0.176776695296637 * (x5a + x5b);
        const float x5f = 0.176776695296637 * (x5a - x5b);
        const float x60 = 0.176776695296637 * (x5c + x5d);
        const float x61 = 0.176776695296637 * (x5c - x5d);
        const float x51 = 0.707106781186547 * (x5f - x61);
        const float x52 = 0.707106781186547 * (x5f + x61);
        dst[ 0*dst_stridea] = 0.176776695296637 * (x7a + x7b);
        dst[ 1*dst_stridea] = 0.176776695296637 * (x3a + x3b);
        dst[ 2*dst_stridea] = 0.176776695296637 * (x8c + x8d);
        dst[ 3*dst_stridea] = 0.707106781186547 * (x45 - x60);
        dst[ 4*dst_stridea] = 0.176776695296637 * (x7e + x7f);
        dst[ 5*dst_stridea] = 0.707106781186547 * (x45 + x60);
        dst[ 6*dst_stridea] = 0.707106781186547 * (x91 - x99);
        dst[ 7*dst_stridea] = 0.707106781186547 * (x3f + x59);
        dst[ 8*dst_stridea] = 0.230969883127822*x7c + 0.0956708580912725*x7d;
        dst[ 9*dst_stridea] = 0.707106781186547 * (x3f - x59);
        dst[10*dst_stridea] = 0.707106781186547 * (x91 + x99);
        dst[11*dst_stridea] = 0.707106781186547 * (x38 - x52);
        dst[12*dst_stridea] = 0.707106781186547 * (x82 - x83);
        dst[13*dst_stridea] = 0.707106781186547 * (x38 + x52);
        dst[14*dst_stridea] = 0.707106781186547 * (x90 + x97);
        dst[15*dst_stridea] = 0.707106781186547 * (x3e + x57);
        dst[16*dst_stridea] = 0.176776695296637 * (x7a - x7b);
        dst[17*dst_stridea] = 0.707106781186547 * (x3e - x57);
        dst[18*dst_stridea] = 0.707106781186547 * (x90 - x97);
        dst[19*dst_stridea] = 0.707106781186547 * (x39 - x51);
        dst[20*dst_stridea] = 0.707106781186547 * (x82 + x83);
        dst[21*dst_stridea] = 0.707106781186547 * (x39 + x51);
        dst[22*dst_stridea] = 0.707106781186547 * (x92 - x98);
        dst[23*dst_stridea] = 0.707106781186547 * (x40 + x58);
        dst[24*dst_stridea] = 0.0956708580912725*x7c - 0.230969883127822*x7d;
        dst[25*dst_stridea] = 0.707106781186547 * (x40 - x58);
        dst[26*dst_stridea] = 0.707106781186547 * (x92 + x98);
        dst[27*dst_stridea] = 0.707106781186547 * (x47 - x5e);
        dst[28*dst_stridea] = 0.176776695296637 * (x80 + x81);
        dst[29*dst_stridea] = 0.707106781186547 * (x47 + x5e);
        dst[30*dst_stridea] = 0.176776695296637 * (x93 + x94);
        dst[31*dst_stridea] = 0.176776695296637 * (x53 + x54);
        dst += dst_strideb;
        src += src_strideb;
    }
}

void fdct(float dst[static 1024], const float src[static 1024])
{
    float tmp[1024];

    fdct_1d(tmp, src, 1, N, 1, N);
    fdct_1d(dst, tmp, N, 1, N, 1);
}
