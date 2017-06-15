#include "../interface/QGMLCommon.h"

#include <cmath>



QGMLCommon::QGMLCommon() {}

QGMLCommon::~QGMLCommon() {}


float QGMLCommon::binToDelta( int bin ) {

  float delta = -0.3 + QGMLCommon::pixelSize()*(((float)bin)-1.) + 0.00001; // the small extra to avoid annoying ROOT binning problems
  return delta;
 
}


void QGMLCommon::getEtaPhiBins( int i, int nPix1D, int& etaBin, int& phiBin ) {

  etaBin = i % (nPix1D);
  phiBin = floor((float)i/((float)(nPix1D)));  
  etaBin += 1; // because ROOT binning starts at 1
  phiBin += 1;

}


float QGMLCommon::pixelSize() {

  return 0.005;

}
