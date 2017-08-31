#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TLorentzVector.h"

#define mt2_cxx
#include "interface/mt2.h"



std::string type="qcdpt";

void createTreeLiteQCD( const std::string& path, const std::string& name );


int main( int argc, char* argv[] ) {


  if( argc>1 ) {
    std::string typetype(argv[1]);
    type = typetype;
  }

  std::string path = "root://t3dcachedb.psi.ch//pnfs/psi.ch/cms/trivcat/store/user/pandolf/MT2production/80X/PostProcessed/prodAug16_DYandQCD_T2_postProc_Aug21/";

  if( type=="qcdpt" ) {
    createTreeLiteQCD( path, "QCD_Pt15to30" );
    createTreeLiteQCD( path, "QCD_Pt30to50" );
    createTreeLiteQCD( path, "QCD_Pt50to80" );
    createTreeLiteQCD( path, "QCD_Pt80to120" );
    createTreeLiteQCD( path, "QCD_Pt120to170" );
    createTreeLiteQCD( path, "QCD_Pt170to300" );
    createTreeLiteQCD( path, "QCD_Pt300to470" );
    createTreeLiteQCD( path, "QCD_Pt470to600" );
    createTreeLiteQCD( path, "QCD_Pt600to800" );
    createTreeLiteQCD( path, "QCD_Pt800to1000" );
    createTreeLiteQCD( path, "QCD_Pt1000to1400" );
    createTreeLiteQCD( path, "QCD_Pt1400to1800" );
    createTreeLiteQCD( path, "QCD_Pt1800to2400" );
    createTreeLiteQCD( path, "QCD_Pt2400to3200" );
    createTreeLiteQCD( path, "QCD_Pt3200" );
  } else if( type=="qcdht" ) {
    createTreeLiteQCD( path, "QCD_HT100to200" );
    createTreeLiteQCD( path, "QCD_HT300to500" );
    createTreeLiteQCD( path, "QCD_HT500to700" );
    createTreeLiteQCD( path, "QCD_HT700to1000" );
    createTreeLiteQCD( path, "QCD_HT1000to1500" );
    createTreeLiteQCD( path, "QCD_HT1500to2000" );
    createTreeLiteQCD( path, "QCD_HT2000toInf" );
  }

  return 0;

}


void createTreeLiteQCD( const std::string& path, const std::string& name ) {

  std::cout << "-> Starting: " << name << std::endl;

  TFile* file = TFile::Open(Form("%s/%s_post.root", path.c_str(), name.c_str()));
  TTree* tree = (TTree*)file->Get("mt2");

  MT2Tree myTree;
  myTree.loadGenStuff = false;
  myTree.Init(tree);


  TFile* outfile = TFile::Open(Form("treeLite_%s.root", name.c_str()), "recreate");
  TTree* tree_lite = new TTree("tlite", "");

  float w;
  float rho;
  float pt;
  float eta;
  float ptd;
  float axis2;
  int   mult;
  float qgl;
  int   partonId;

  tree_lite->Branch("weight"  , &w        , "w/F"        );
  tree_lite->Branch("rho"     , &rho      , "rho/F"      );
  tree_lite->Branch("pt"      , &pt       , "pt/F"       );
  tree_lite->Branch("eta"     , &eta      , "eta/F"      );
  tree_lite->Branch("ptd"     , &ptd      , "ptd/F"      );
  tree_lite->Branch("axis2"   , &axis2    , "axis2/F"    );
  tree_lite->Branch("mult"    , &mult     , "mult/I"     );
  tree_lite->Branch("qgl"     , &qgl      , "qgl/F"      );
  tree_lite->Branch("partonId", &partonId , "partonId/I" );



  int nentries = tree->GetEntries();

  if( type=="qcdht" ) nentries = 1000000;

  for( unsigned iEntry=0; iEntry<nentries; iEntry++ ) {

    tree->GetEntry(iEntry);

    if( iEntry%50000 == 0 ) std::cout << "  Entry: " << iEntry << " / " << nentries << std::endl;

    Double_t weight = (myTree.isData) ? 1. : myTree.evt_scale1fb;//*cfg.lumi(); 

    if( myTree.isData && !myTree.passFilters() ) continue;
    if( myTree.isData &&  myTree.isGolden == 0 ) continue;

    //if( !myTree.isData && !myTree.passFiltersMC() ) continue;

    //if( myTree.nJet200MuFrac50DphiMet > 0 ) continue; // new RA2 filter
    //if (myTree.met_miniaodPt/myTree.met_caloPt>5.0  &&  myTree.met_miniaodPt>200.) continue;


    if( myTree.nVert < 1 ) continue;

    if( myTree.nJet30<2 ) continue;

    if( myTree.nJet30>2 )
      if( myTree.jet_pt[2]>0.30*0.5*(myTree.jet_pt[0]+myTree.jet_pt[1]) ) continue;

    w = weight;
    rho = myTree.rho;

    if( fabs(myTree.jet_eta[0])<1.3 && myTree.jet_pt[0]>30. ) { // use pt of other

      pt  = myTree.jet_pt[0];
      eta = myTree.jet_eta[0];
      ptd = myTree.jet_ptd[0];
      axis2 = myTree.jet_axis2[0];
      mult = myTree.jet_mult[0];
      qgl = myTree.jet_qgl[0];
      partonId = myTree.jet_partonId[0];

      tree_lite->Fill();

    }

    if( fabs(myTree.jet_eta[1])<1.3 && myTree.jet_pt[1]>30. ) { // use pt of other

      pt  = myTree.jet_pt[1];
      eta = myTree.jet_eta[1];
      ptd = myTree.jet_ptd[1];
      axis2 = myTree.jet_axis2[1];
      mult = myTree.jet_mult[1];
      qgl = myTree.jet_qgl[1];
      partonId = myTree.jet_partonId[1];

      tree_lite->Fill();

    }


      
  } // for entries


  outfile->cd();
  tree_lite->Write();
  outfile->Close();

}
