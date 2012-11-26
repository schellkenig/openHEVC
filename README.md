openHEVC
========


- openHEVC
----------
libav version from smarter (smarter.free.fr)

- openHEVC/wrapper
----------
C wrapper from HM9.0 C++ reference SW (HEVC reference sw)

- openHEVC/wrapper/HM
----------
HM9.0 reference SW


TODO
----------
- test parser with INTRA/ldp/ldmain sequences
- Miminization of 
    memset(s->pu.left_ipm, INTRA_DC, pic_height_in_min_pu);
    memset(s->pu.top_ipm, INTRA_DC, pic_width_in_min_pu);
    memset(s->pu.tab_ipm, INTRA_DC, pic_width_in_min_pu*pic_height_in_min_pu);
- new SE strong_intra_smoothing_enable_flag :
	support for 32x32 intra prediction
- add edges to 80 for compatibility between libav and hm.
- add reconstructed mv
- add deblocking filter
- add SAO from libav/smarter