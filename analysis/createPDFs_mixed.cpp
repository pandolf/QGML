#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TH1D.h"



void createPDFs( TTree* tree, const std::string& name, float ptMin, float ptMax );



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

  createPDFs( treeDY ,  "dy", 100., 120. );
  createPDFs( treeQCD, "qcd", 100., 120. );

  return 0;

}


void createPDFs( TTree* tree, const std::string& name, float ptMin, float ptMax ) {

  std::cout << "-> Creating PDFs for: " << name << std::endl;

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


  TFile* file = TFile::Open(Form("pdfsMixed_%s.root", name.c_str()), "recreate");
  file->cd();
  
  TH1D* h1_ptd   = new TH1D("pdf_ptd", "", 30, 0., 1.0001 );
  h1_ptd->Sumw2();
  TH1D* h1_axis2 = new TH1D("pdf_axis2", "", 30, 0., 10. );
  h1_axis2->Sumw2();
  TH1D* h1_mult  = new TH1D("pdf_mult", "", 50, 0., 50. );
  h1_mult->Sumw2();

  int nentries = tree->GetEntries();


  for( unsigned iEntry=0; iEntry<nentries; ++iEntry ) {

    if( iEntry % 100000 == 0 ) std::cout << "Entry: " << iEntry << " / " << nentries << std::endl;

    tree->GetEntry(iEntry);

    if( pt<ptMin || pt>ptMax ) continue;

    h1_ptd  ->Fill( ptd  , weight );
    h1_axis2->Fill( axis2, weight );
    h1_mult ->Fill( mult , weight );

  }

  file->cd();

  h1_ptd  ->Write();
  h1_axis2->Write();
  h1_mult ->Write();

  file->Close();

}
