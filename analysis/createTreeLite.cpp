#include <iostream>
#include "TFile.h"
#include "TTree.h"


bool DEBUG_ = false;


int main() {

  TFile* file = TFile::Open("qgMiniTuple.root");
  TTree* tree = (TTree*)file->Get("qgMiniTupleAK4chs/qgMiniTuple");

  int nPixCh;
  tree->SetBranchAddress( "nPixCh", &nPixCh );
  float drMaxCh;
  tree->SetBranchAddress( "drMaxCh", &drMaxCh );
  float pixelSizeCh;
  tree->SetBranchAddress( "pixelSizeCh", &pixelSizeCh );
  float chImage[99999];
  tree->SetBranchAddress( "chImage", chImage );
  int partonId;
  tree->SetBranchAddress( "partonId", &partonId );
  float pt;
  tree->SetBranchAddress( "pt", &pt );
  float eta;
  tree->SetBranchAddress( "eta", &eta );
  float qgl;
  tree->SetBranchAddress( "qgl", &qgl );

  tree->GetEntry(0);
  int nPixCh_1D = sqrt(nPixCh);



  TFile* newFile = TFile::Open("qgMiniTupleLite.root", "recreate");
  TTree* newTree = new TTree("qgLite", "");

  int isQuark;
  newTree->Branch("isQuark", &isQuark, "isQuark/I");

  newTree->Branch("qgl", &qgl, "qgl/F");

  int NPIX = 60;  // pixels of 0.005 from 0 to 0.3 in dR

  // use the following for flat branches:
  float chImageLite[NPIX]; 
  for( unsigned i=0; i<NPIX; ++i )
    newTree->Branch( Form("chImageLite_%d", i), &chImageLite[i], Form("chImageLite_%d/F", i) );


//// use the following for array branches:
//newTree->Branch("NPIX", &NPIX, "NPIX/I" );
//float chImageLite[NPIX]; 
//newTree->Branch( "chImageLite", chImageLite, "chImageLite[NPIX]/F");


  int nentries = tree->GetEntries();
  if( DEBUG_ ) 
    nentries = 1000;


  for( unsigned iEntry=0; iEntry<nentries; iEntry++ ) {

    if( iEntry % 10000 == 0 ) std::cout << "Entry: " << iEntry << " / " << nentries << std::endl;

    tree->GetEntry(iEntry);

    if( partonId==21 ) {
      isQuark=false;
    } else if( abs(partonId)<4 ) {
      isQuark=true;
    } else {
      continue;
    }


    if( pt<300. || pt>400. ) continue;
    if( fabs(eta)>2. ) continue;

    // initialize
    for( unsigned i=0; i<NPIX; i++ ) chImageLite[i]=0.;


    for( unsigned i=0; i<nPixCh; ++i ) {

      if( chImage[i]==0. ) continue;

      int etaBin = i % nPixCh_1D;
      int phiBin = floor((float)i/((float)nPixCh_1D));  

      float dEta = -0.3 + 0.005*(etaBin-1.) + 0.000001;
      float dPhi = -0.3 + 0.005*(phiBin-1.) + 0.000001;

      float dR = sqrt( dEta*dEta + dPhi*dPhi );

      if( DEBUG_ ) {
        std::cout << "--------------------" << std::endl;
        std::cout << "dR: " << dR << std::endl;
      }

      if( dR>0.3 ) continue;

      int dRbin = int( dR/0.005 );

      if( DEBUG_ ) {
        std::cout << "dRbin: " << dRbin << std::endl;
        std::cout << "chImage[i]: " << chImage[i] << std::endl;
      }

      chImageLite[dRbin] = chImage[i];

      //int etaBinNew = etaBin - (nPixCh_1D-NP1D)/2;
      //int phiBinNew = phiBin - (nPixCh_1D-NP1D)/2;

      //if( etaBinNew<0 || etaBinNew>=NP1D ) continue;
      //if( phiBinNew<0 || phiBinNew>=NP1D ) continue;

      //int newIndex = etaBinNew*NP1D + phiBinNew;
      //chImageLite[newIndex] = chImage[i];
  
    }

    if( DEBUG_ ) {

      std::cout << "chImage: ";

      for( unsigned i=0; i<NPIX; ++i ) std::cout << chImageLite[i] << " ";
      std::cout << std::endl;
      std::cout << "^ the above is event n: " << newTree->GetEntries() << std::endl;

    }

    newTree->Fill();

  }

  newFile->cd();
  newTree->Write();
  newFile->Close();

  return 0;

}
