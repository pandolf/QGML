#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TLorentzVector.h"

#define mt2_cxx
#include "interface/mt2.h"



int main() {

  std::string path = "root://t3dcachedb.psi.ch//pnfs/psi.ch/cms/trivcat/store/user/pandolf/MT2production/80X/PostProcessed/prodAug16_DYandQCD_T2_postProc_Aug21/";

  TFile* file = TFile::Open(Form("%s/DYJetsToLL_M50_LO_post.root", path.c_str()));
  TTree* tree = (TTree*)file->Get("mt2");

  MT2Tree myTree;
  myTree.loadGenStuff = false;
  myTree.Init(tree);


  TFile* outfile = TFile::Open("treeLite_DY.root", "recreate");
  TTree* tree_lite = new TTree("tlite", "");

  float w;
  float pt;
  float eta;
  float ptd;
  float axis2;
  int   mult;
  float qgl;
  int   partonId;

  tree_lite->Branch("weight"  , &w        , "w/F"        );
  tree_lite->Branch("pt"      , &pt       , "pt/F"       );
  tree_lite->Branch("eta"     , &eta      , "eta/F"      );
  tree_lite->Branch("ptd"     , &ptd      , "ptd/F"      );
  tree_lite->Branch("axis2"   , &axis2    , "axis2/F"    );
  tree_lite->Branch("mult"    , &mult     , "mult/I"     );
  tree_lite->Branch("qgl"     , &qgl      , "qgl/F"      );
  tree_lite->Branch("partonId", &partonId , "partonId/I" );



  int nentries = tree->GetEntries();


  for( unsigned iEntry=0; iEntry<nentries; iEntry++ ) {

    tree->GetEntry(iEntry);

    if( iEntry%50000 == 0 ) std::cout << "  Entry: " << iEntry << " / " << nentries << std::endl;

    Double_t weight = (myTree.isData) ? 1. : myTree.evt_scale1fb;//*cfg.lumi(); 

    if(  myTree.isData && !( myTree.HLT_DoubleMu 
                          || myTree.HLT_DoubleMu_NonIso 
                          || myTree.HLT_SingleMu_NonIso 
                          || myTree.HLT_DoubleEl 
                          || myTree.HLT_DoubleEl33 
                          || myTree.HLT_Photon165_HE10 ) )continue;

    if( myTree.isData && !myTree.passFilters() ) continue;
    if( myTree.isData &&  myTree.isGolden == 0 ) continue;

    //if( !myTree.isData && !myTree.passFiltersMC() ) continue;

    //if( myTree.nJet200MuFrac50DphiMet > 0 ) continue; // new RA2 filter
    //if (myTree.met_miniaodPt/myTree.met_caloPt>5.0  &&  myTree.met_miniaodPt>200.) continue;


    if(myTree.nVert < 1) continue;

    if( !( myTree.nlep==2 ) ) continue; 

    if( !( myTree.lep_pdgId[0]=-myTree.lep_pdgId[1]) ) continue;
    if( !( abs(myTree.lep_pdgId[0])==11 || abs(myTree.lep_pdgId[0])==13 ) ) continue;

    if( myTree.lep_pt[0]<30. ) continue;
    if( myTree.lep_pt[1]<25. ) continue; 

    if( abs(myTree.lep_pdgId[0])==11 && myTree.lep_tightId[0]< 0.5 ) continue;
    if( abs(myTree.lep_pdgId[1])==11 && myTree.lep_tightId[1]< 0.5 ) continue;

    if( fabs(myTree.zll_mass-91.19)>=20. ) continue;

    TLorentzVector lept0, lept1;

    lept0.SetPtEtaPhiM( myTree.lep_pt[0], myTree.lep_eta[0], myTree.lep_phi[0], myTree.lep_mass[0] );
    lept1.SetPtEtaPhiM( myTree.lep_pt[1], myTree.lep_eta[1], myTree.lep_phi[1], myTree.lep_mass[1] );

    TLorentzVector z_boson;
    z_boson = lept0+lept1;

    
    if( myTree.nJet30>1 )
      if( myTree.jet_pt[1]>0.3*z_boson.Pt() ) continue;

    if( fabs(myTree.jet_eta[0])>1.3 ) continue;
    if( myTree.jet_pt[0]<20. ) continue;

    w = weight;
    pt  = z_boson.Pt();
    //pt  = myTree.jet_pt[0];
    eta = myTree.jet_eta[0];
    ptd = myTree.jet_ptd[0];
    axis2 = myTree.jet_axis2[0];
    mult = myTree.jet_mult[0];
    qgl = myTree.jet_qgl[0];
    partonId = myTree.jet_partonId[0];

    tree_lite->Fill();
      
  } // for entries


  outfile->cd();
  tree_lite->Write();
  outfile->Close();

  return 0;

}
