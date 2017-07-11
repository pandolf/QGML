#ifndef QGMLCommon_h
#define QGMLCommon_h

#include "TPaveText.h"



class QGMLCommon {

 public:

  QGMLCommon();
  ~QGMLCommon();

  static void setStyle();

  static TPaveText* get_labelTop();
  static TPaveText* get_labelCMS();


  static float binToDelta( int bin );
  static void getEtaPhiBins( int i, int nPix1D, int& etaBin, int& phiBin );
  static float pixelSize();

 private:


};



#endif
