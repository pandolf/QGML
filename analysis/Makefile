CC = g++
CFLAGS = -Wall -c -g -fpermissive 

ROOFIT_INCLUDE := $(shell cd $(CMSSW_BASE); scram tool info roofitcore | grep INCLUDE= | sed 's|INCLUDE=||')
ROOFIT_LIBDIR := $(shell cd $(CMSSW_BASE); scram tool info roofitcore | grep LIBDIR= | sed 's|LIBDIR=||')

INCLUDES = -I. -I.. -I$(ROOTSYS)/include -I$(ROOFIT_INCLUDE)/ -I$(CMSSW_BASE)/src

ROOTSYS ?= ERROR_RootSysIsNotDefined

ROOTFLAG = `${ROOTSYS}/bin/root-config --cflags --libs`

EXTRALIBS := -L$(ROOTSYS)/lib -L$(ROOFIT_LIBDIR)/ 
#-L$(CMSSW_BASE)/lib/slc6_amd64_gcc491/ -L/cvmfs/cms.cern.ch/slc6_amd64_gcc491/cms/cmssw/CMSSW_7_4_7/lib/slc6_amd64_gcc491/ -lHtml -lMathCore -lGenVector -lMinuit -lEG  -lPhysics -L/cvmfs/cms.cern.ch/slc6_amd64_gcc491/external/zlib/1.2.8-cms/lib -lz -lTreePlayer -lRooFitCore -lRooFit



createTreeLiteDY: createTreeLiteDY.cpp QGLikelihoodCalculatorMixedPDFs.o Bins.o 
	$(CC) -Wall $(INCLUDES) -o createTreeLiteDY createTreeLiteDY.cpp QGLikelihoodCalculatorMixedPDFs.o Bins.o $(ROOTFLAG) $(EXTRALIBS)

createTreeLiteQCD: createTreeLiteQCD.cpp QGLikelihoodCalculatorMixedPDFs.o Bins.o
	$(CC) -Wall $(INCLUDES) -o createTreeLiteQCD createTreeLiteQCD.cpp QGLikelihoodCalculatorMixedPDFs.o Bins.o $(ROOTFLAG) $(EXTRALIBS)

createPDFs_mixed: createPDFs_mixed.cpp Bins.o
	$(CC) -Wall $(INCLUDES) -o createPDFs_mixed createPDFs_mixed.cpp Bins.o $(ROOTFLAG) $(EXTRALIBS)

drawPDFmixed: drawPDFmixed.cpp QGMLCommon.o Bins.o
	$(CC) -Wall $(INCLUDES) -o drawPDFmixed drawPDFmixed.cpp QGMLCommon.o Bins.o $(ROOTFLAG) $(EXTRALIBS)

drawTreeLite: drawTreeLite.cpp QGMLCommon.o
	$(CC) -Wall $(INCLUDES) -o drawTreeLite drawTreeLite.cpp QGMLCommon.o $(ROOTFLAG) $(EXTRALIBS)

createTreeLite: createTreeLite.cpp QGMLCommon.o QGLikelihoodCalculatorMixedPDFs.o 
	$(CC) -Wall $(INCLUDES) -o createTreeLite createTreeLite.cpp QGMLCommon.o QGLikelihoodCalculatorMixedPDFs.o $(ROOTFLAG) $(EXTRALIBS)

drawQGML: drawQGML.cpp QGMLCommon.o
	$(CC) -Wall $(INCLUDES) -o drawQGML drawQGML.cpp QGMLCommon.o $(ROOTFLAG) $(EXTRALIBS)

drawRNNTrainTest: drawRNNTrainTest.cpp QGMLCommon.o
	$(CC) -Wall $(INCLUDES) -o drawRNNTrainTest drawRNNTrainTest.cpp QGMLCommon.o $(ROOTFLAG) $(EXTRALIBS)




QGMLCommon.o: ../src/QGMLCommon.cc
	$(CC) -Wall $(INCLUDES) -c ../src/QGMLCommon.cc $(ROOTFLAG) $(EXTRALIBS)

Bins.o: ../../QGLikelihood/src/Bins.cc
	$(CC) -Wall $(INCLUDES) -c ../../QGLikelihood/src/Bins.cc $(ROOTFLAG) $(EXTRALIBS)

QGLikelihoodCalculatorMixedPDFs.o: ../../QGLikelihood/src/QGLikelihoodCalculatorMixedPDFs.cc
	$(CC) -Wall $(INCLUDES) -c ../../QGLikelihood/src/QGLikelihoodCalculatorMixedPDFs.cc $(ROOTFLAG) $(EXTRALIBS)
