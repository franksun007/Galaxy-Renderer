#ifndef _SPECREND_H
#define _SPECREND_H

/* A colour system is defined by the CIE x and y coordinates of
   its three primary illuminants and the x and y coordinates of
   the white point. */
struct colourSystem {
  const char *name;   /* Colour system name */
  float xRed, yRed,  /* Red x, y */
      xGreen, yGreen, /* Green x, y */
      xBlue, yBlue,   /* Blue x, y */
      xWhite, yWhite, /* White point x, y */
      gamma;          /* Gamma correction for system */
};

extern colourSystem NTSCsystem;
extern colourSystem EBUsystem;
extern colourSystem SMPTEsystem;
extern colourSystem CIEsystem;
extern colourSystem Rec709system;
extern float bbTemp; /* Hidden temperature argument */

void spectrum_to_xyz(float (*spec_intens)(float wavelength), float *x,
                     float *y, float *z);
extern void xyz_to_rgb(struct colourSystem *cs, float xc, float yc, float zc,
                       float *r, float *g, float *b);
extern float bb_spectrum(float wavelength);
extern void norm_rgb(float *r, float *g, float *b);
extern float bb_spectrum(float wavelength);

#endif
