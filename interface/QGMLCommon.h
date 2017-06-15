#ifndef QGMLCommon_h
#define QGMLCommon_h




class QGMLCommon {

 public:

  QGMLCommon();
  ~QGMLCommon();

  static float binToDelta( int bin );
  static void getEtaPhiBins( int i, int nPix1D, int& etaBin, int& phiBin );
  static float pixelSize();

 private:


};



#endif
