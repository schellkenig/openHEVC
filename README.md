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
- add edges to 80 for compatibility between libav and hm.
- add reconstructed mv
- add deblocking filter
- add SAO from libav/smarter
- check if we have to initialize the array such as memset(s->rc.significant_coeff_group_flag, 0, 9*9);
- check if the size of the array has to be [9] [9] for significant_coeff_group_flag 