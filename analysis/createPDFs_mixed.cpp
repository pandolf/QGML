#include <iostream>

#include "QGLikelihood/interface/Bins.h"

#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TH1D.h"



void createPDFs( TTree* tree, const std::string& name );



int main() {

  TChain* treeDY = new TChain("tlite");
  treeDY->Add("treeLite_DYJetsToLL_M50_HT100to200.root");
  treeDY->Add("treeLite_DYJetsToLL_M50_HT200to400.root");
  treeDY->Add("treeLite_DYJetsToLL_M50_HT400to600.root");
  treeDY->Add("treeLite_DYJetsToLL_M50_HT600to800.root");

  TChain* treeQCD = new TChain("tlite");
  treeQCD->Add("treeLite_QCD_Pt50to80.root");
  treeQCD->Add("treeLite_QCD_Pt80to120.root");
  treeQCD->Add("treeLite_QCD_Pt120to170.root");
  treeQCD->Add("treeLite_QCD_Pt170to300.root");
  //treeQCD->Add("treeLite_QCD_Pt300to470.root");

  std::vector<float> ptBins;
  ptBins.push_back(30.);

  createPDFs( treeDY ,  "dy" );
  createPDFs( treeQCD, "qcd" );

  return 0;

}


void createPDFs( TTree* tree, const std::string& name ) {

  std::cout << "-> Creating PDFs for: " << name << std::endl;

  float weight;
  float pt;
  float rho;
  float eta;
  float ptd;
  float axis2;
  int mult;

  tree->SetBranchAddress( "weight", &weight );
  tree->SetBranchAddress( "rho"   , &rho    );
  tree->SetBranchAddress( "pt"    , &pt     );
  tree->SetBranchAddress( "eta"   , &eta    );
  tree->SetBranchAddress( "ptd"   , &ptd    );
  tree->SetBranchAddress( "axis2" , &axis2  );
  tree->SetBranchAddress( "mult"  , &mult   );


  TFile* file = TFile::Open(Form("pdfsMixed_%s.root", name.c_str()), "recreate");
  file->cd();

  int nBins_pt=20;
  double ptBins[nBins_pt+1];
  Bins::getBins(ptBins, nBins_pt, 20., 2000., true ); 

  int nBins_rho=5;
  double rhoBins[nBins_rho+1];
  rhoBins[0] = 0.;
  rhoBins[1] = 8.;
  rhoBins[2] = 11.;
  rhoBins[3] = 14.;
  rhoBins[4] = 17.;
  rhoBins[5] = 9999.;

  
  std::vector<TH1D*> pdfs_ptd, pdfs_axis2, pdfs_mult;
  
  for( unsigned i_pt=0; i_pt<nBins_pt; ++i_pt ) {

    for( unsigned i_rho=0; i_rho<nBins_rho; ++i_rho ) {

      std::string histoName_ptd  (Form("pdf_ptd_pt%d_rho%d"  , i_pt, i_rho));
      std::string histoName_axis2(Form("pdf_axis2_pt%d_rho%d", i_pt, i_rho));
      std::string histoName_mult (Form("pdf_mult_pt%d_rho%d" , i_pt, i_rho));

      TH1D* newhist_ptd   = new TH1D( histoName_ptd.c_str()  , "", 100, 0., 1.0001 );
      TH1D* newhist_axis2 = new TH1D( histoName_axis2.c_str(), "", 100, 0., 8.     );
      TH1D* newhist_mult  = new TH1D( histoName_mult.c_str() , "", 140, 2.5, 142.5 );

      newhist_ptd   ->Sumw2();
      newhist_axis2 ->Sumw2();
      newhist_mult  ->Sumw2();

      pdfs_ptd   .push_back(newhist_ptd  );
      pdfs_axis2 .push_back(newhist_axis2);
      pdfs_mult  .push_back(newhist_mult );

    }

  }



  int nentries = tree->GetEntries();


  for( unsigned iEntry=0; iEntry<nentries; ++iEntry ) {

    if( iEntry % 100000 == 0 ) std::cout << "Entry: " << iEntry << " / " << nentries << std::endl;

    tree->GetEntry(iEntry);

    int i_pt  = Bins::getBin( nBins_pt ,  ptBins, pt  );
    int i_rho = Bins::getBin( nBins_rho, rhoBins, rho );

    if( !(i_pt>=0 && i_pt<=nBins_pt && i_rho>=0 && i_rho<=nBins_rho ) ) continue;

    std::string histoName_ptd  (Form("pdf_ptd_pt%d_rho%d"  , i_pt, i_rho));
    std::string histoName_axis2(Form("pdf_axis2_pt%d_rho%d", i_pt, i_rho));
    std::string histoName_mult (Form("pdf_mult_pt%d_rho%d" , i_pt, i_rho));


    for( unsigned i=0; i<pdfs_ptd.size(); ++i ) {
      std::string thisName(pdfs_ptd[i]->GetName());
      if( thisName==histoName_ptd ) {
        pdfs_ptd[i]->Fill( ptd, weight );
        break;
      }
    }
    for( unsigned i=0; i<pdfs_axis2.size(); ++i ) {
      std::string thisName(pdfs_axis2[i]->GetName());
      if( thisName==histoName_axis2 ) {
        pdfs_axis2[i]->Fill( axis2, weight );
        break;
      }
    }
    for( unsigned i=0; i<pdfs_mult.size(); ++i ) {
      std::string thisName(pdfs_mult[i]->GetName());
      if( thisName==histoName_mult ) {
        pdfs_mult[i]->Fill( mult, weight );
        break;
      }
    }

  }

  file->cd();

  for( unsigned i=0; i<pdfs_ptd  .size(); ++i ) pdfs_ptd  [i]->Write();
  for( unsigned i=0; i<pdfs_axis2.size(); ++i ) pdfs_axis2[i]->Write();
  for( unsigned i=0; i<pdfs_mult .size(); ++i ) pdfs_mult [i]->Write();
    

  file->Close();

}
