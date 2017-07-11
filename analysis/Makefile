CC = g++
CFLAGS = -Wall -c -g -fpermissive 

ROOFIT_INCLUDE := $(shell cd $(CMSSW_BASE); scram tool info roofitcore | grep INCLUDE= | sed 's|INCLUDE=||')
ROOFIT_LIBDIR := $(shell cd $(CMSSW_BASE); scram tool info roofitcore | grep LIBDIR= | sed 's|LIBDIR=||')

INCLUDES = -I. -I.. -I$(ROOTSYS)/include -I$(ROOFIT_INCLUDE)/ -I$(CMSSW_BASE)/src

ROOTSYS ?= ERROR_RootSysIsNotDefined

ROOTFLAG = `${ROOTSYS}/bin/root-config --cflags --libs`

EXTRALIBS := -L$(ROOTSYS)/lib -L$(ROOFIT_LIBDIR)/ 
#-L$(CMSSW_BASE)/lib/slc6_amd64_gcc491/ -L/cvmfs/cms.cern.ch/slc6_amd64_gcc491/cms/cmssw/CMSSW_7_4_7/lib/slc6_amd64_gcc491/ -lHtml -lMathCore -lGenVector -lMinuit -lEG  -lPhysics -L/cvmfs/cms.cern.ch/slc6_amd64_gcc491/external/zlib/1.2.8-cms/lib -lz -lTreePlayer -lRooFitCore -lRooFit



createTreeLite: createTreeLite.cpp QGMLCommon.o
	$(CC) -Wall $(INCLUDES) -o createTreeLite createTreeLite.cpp QGMLCommon.o $(ROOTFLAG) $(EXTRALIBS)

drawQGML: drawQGML.cpp QGMLCommon.o
	$(CC) -Wall $(INCLUDES) -o drawQGML drawQGML.cpp QGMLCommon.o $(ROOTFLAG) $(EXTRALIBS)

drawRNNTrainTest: drawRNNTrainTest.cpp QGMLCommon.o
	$(CC) -Wall $(INCLUDES) -o drawRNNTrainTest drawRNNTrainTest.cpp QGMLCommon.o $(ROOTFLAG) $(EXTRALIBS)



QGMLCommon.o: ../src/QGMLCommon.cc
	$(CC) -Wall $(INCLUDES) -c ../src/QGMLCommon.cc $(ROOTFLAG) $(EXTRALIBS)
