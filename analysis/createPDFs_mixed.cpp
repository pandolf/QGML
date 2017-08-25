#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TH1D.h"



void createPDFs( TFile* file, TTree* tree, const std::string& name, float ptMin, float ptMax );



int main() {

  TFile* fileDY = TFile::Open("treelite_DYJetsToLL_M50_LO.root");
  TTree* treeDY = (TTree*)fileDY->Get("tlite");

  TChain* treeQCD = new TChain("tlite");
  treeQCD->Add("treelite_QCD_Pt50to80.root");
  treeQCD->Add("treelite_QCD_Pt80to120.root");
  treeQCD->Add("treelite_QCD_Pt120to170.root");
  treeQCD->Add("treelite_QCD_Pt170to300.root");

  TFile* pdfFile = TFile::Open("pdfsMixed.root", "recreate");

  createPDFs( pdfFile, treeDY ,  "dy", 100., 120. );
  createPDFs( pdfFile, treeQCD, "qcd", 100., 120. );

  return 0;

}


void createPDFs( TFile* file, TTree* tree, const std::string& name, float ptMin, float ptMax ) {

  float weight;
  float pt;
  float eta;
  float ptd;
  float axis2;
  int mult;

  tree->SetBranchAddress( "weight", &weight );
  tree->SetBranchAddress( "pt", &pt );
  tree->SetBranchAddress( "eta", &eta );
  tree->SetBranchAddress( "ptd", &ptd );
  tree->SetBranchAddress( "axis2", &axis2 );
  tree->SetBranchAddress( "mult", &mult );


  file->cd();
  
  TH1D* h1_ptd   = new TH1D(Form("%s_pdf_ptd", name.c_str()), "", 30, 0., 1.0001 );
  h1_ptd->Sumw2();
  TH1D* h1_axis2 = new TH1D(Form("%s_pdf_axis2", name.c_str()), "", 30, 0., 10. );
  h1_axis2->Sumw2();
  TH1D* h1_mult  = new TH1D(Form("%s_pdf_mult", name.c_str()), "", 50, 0., 50. );
  h1_mult->Sumw2();

  int nentries = tree->GetEntries();


  for( unsigned iEntry=0; iEntry<nentries; ++iEntry ) {

    if( pt<ptMin || pt>ptMax ) continue;

    h1_ptd  ->Fill( ptd  , weight );
    h1_axis2->Fill( axis2, weight );
    h1_mult ->Fill( mult , weight );

  }

  file->cd();

  h1_ptd  ->Write();
  h1_axis2->Write();
  h1_mult ->Write();


}
