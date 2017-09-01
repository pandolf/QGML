#include <iostream>

#include "QGLikelihood/interface/Bins.h"

#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TH1D.h"



void createPDFs( TFile* file, TTree* tree, const std::string& name );



int main() {

  TChain* treeDY = new TChain("tlite");
  treeDY->Add("treeLite_DYJetsToLL_M50_HT100to200.root");
  treeDY->Add("treeLite_DYJetsToLL_M50_HT200to400.root");
  treeDY->Add("treeLite_DYJetsToLL_M50_HT400to600.root");
  treeDY->Add("treeLite_DYJetsToLL_M50_HT600to800.root");

  TChain* treeQCD = new TChain("tlite");
  treeQCD->Add("treeLite_QCD_HT100to200.root");
  treeQCD->Add("treeLite_QCD_HT200to300.root");
  treeQCD->Add("treeLite_QCD_HT300to500.root");
  treeQCD->Add("treeLite_QCD_HT500to700.root");
  treeQCD->Add("treeLite_QCD_HT700to1000.root");
  //treeQCD->Add("treeLite_QCD_HT1000to1500.root");
  //treeQCD->Add("treeLite_QCD_HT1500to2000.root");
  //treeQCD->Add("treeLite_QCD_HT2000toInf.root");

  std::vector<float> ptBins;
  ptBins.push_back(30.);

  TFile* pdfFile = TFile::Open("pdfMixed.root", "recreate" );

  createPDFs( pdfFile, treeDY ,  "dy" );
  createPDFs( pdfFile, treeQCD, "qcd" );

  pdfFile->Close();

  return 0;

}


void createPDFs( TFile* file, TTree* tree, const std::string& name ) {

  std::cout << "-> Creating PDFs for: " << name << std::endl;

  float weight;
  float pt;
  float rho;
  float eta;
  float ptd;
  float axis2;
  int mult;
  int partonId;

  tree->SetBranchAddress( "weight"    , &weight     );
  tree->SetBranchAddress( "rho"       , &rho        );
  tree->SetBranchAddress( "pt"        , &pt         );
  tree->SetBranchAddress( "eta"       , &eta        );
  tree->SetBranchAddress( "ptd"       , &ptd        );
  tree->SetBranchAddress( "axis2"     , &axis2      );
  tree->SetBranchAddress( "mult"      , &mult       );
  tree->SetBranchAddress( "partonId"  , &partonId   );


  //TFile* file = TFile::Open(Form("pdfsMixed_%s.root", name.c_str()), "recreate");
  file->cd();

  double ptBins[Bins::nPtBins+1];
  Bins::getPtBins(ptBins);
  //Bins::getBins(ptBins, nBins_pt, 20., 2000., true ); 
  std::cout << "-> Using these pt bins:" << std::endl;
  for( unsigned i=0; i<Bins::nPtBins+1; ++i )
    std::cout << "  " << ptBins[i] << std::endl;
  std::cout << std::endl << std::endl;

  double rhoBins[Bins::nRhoBins+1];
  Bins::getRhoBins(rhoBins);
  std::cout << "-> Using these rho bins:" << std::endl;
  for( unsigned i=0; i<Bins::nRhoBins+1; ++i )
    std::cout << "  " << rhoBins[i] << std::endl;
  //rhoBins[0] = 0.;
  //rhoBins[1] = 8.;
  //rhoBins[2] = 11.;
  //rhoBins[3] = 14.;
  //rhoBins[4] = 17.;
  //rhoBins[5] = 9999.;

  
  std::vector<TH1D*>  pdfs_ptd,  pdfs_axis2,  pdfs_mult;
  std::vector<TH1D*> gluon_ptd, gluon_axis2, gluon_mult;
  std::vector<TH1D*> quark_ptd, quark_axis2, quark_mult;
  std::vector<TH1D*> undef_ptd, undef_axis2, undef_mult;
  
  for( unsigned i_pt=0; i_pt<Bins::nPtBins; ++i_pt ) {

    for( unsigned i_rho=0; i_rho<Bins::nRhoBins; ++i_rho ) {

      std::string histoName_ptd  (Form("pdf_%s_ptd_pt%d_rho%d"  , name.c_str(), i_pt, i_rho));
      std::string histoName_axis2(Form("pdf_%s_axis2_pt%d_rho%d", name.c_str(), i_pt, i_rho));
      std::string histoName_mult (Form("pdf_%s_mult_pt%d_rho%d" , name.c_str(), i_pt, i_rho));

      TH1D* newhist_ptd   = new TH1D( histoName_ptd.c_str()  , "", 100, 0., 1.0001 );
      TH1D* newhist_axis2 = new TH1D( histoName_axis2.c_str(), "", 100, 0., 8.     );
      TH1D* newhist_mult  = new TH1D( histoName_mult.c_str() , "", 140, 2.5, 142.5 );

      newhist_ptd   ->Sumw2();
      newhist_axis2 ->Sumw2();
      newhist_mult  ->Sumw2();

      pdfs_ptd   .push_back(newhist_ptd  );
      pdfs_axis2 .push_back(newhist_axis2);
      pdfs_mult  .push_back(newhist_mult );

      std::string histoName_gluon_ptd  (Form("gluon_%s_ptd_pt%d_rho%d"  , name.c_str(), i_pt, i_rho));
      std::string histoName_gluon_axis2(Form("gluon_%s_axis2_pt%d_rho%d", name.c_str(), i_pt, i_rho));
      std::string histoName_gluon_mult (Form("gluon_%s_mult_pt%d_rho%d" , name.c_str(), i_pt, i_rho));

      TH1D* newhist_gluon_ptd   = new TH1D( histoName_gluon_ptd.c_str()  , "", 100, 0., 1.0001 );
      TH1D* newhist_gluon_axis2 = new TH1D( histoName_gluon_axis2.c_str(), "", 100, 0., 8.     );
      TH1D* newhist_gluon_mult  = new TH1D( histoName_gluon_mult.c_str() , "", 140, 2.5, 142.5 );

      newhist_gluon_ptd   ->Sumw2();
      newhist_gluon_axis2 ->Sumw2();
      newhist_gluon_mult  ->Sumw2();

      gluon_ptd   .push_back(newhist_gluon_ptd  );
      gluon_axis2 .push_back(newhist_gluon_axis2);
      gluon_mult  .push_back(newhist_gluon_mult );

      std::string histoName_quark_ptd  (Form("quark_%s_ptd_pt%d_rho%d"  , name.c_str(), i_pt, i_rho));
      std::string histoName_quark_axis2(Form("quark_%s_axis2_pt%d_rho%d", name.c_str(), i_pt, i_rho));
      std::string histoName_quark_mult (Form("quark_%s_mult_pt%d_rho%d" , name.c_str(), i_pt, i_rho));

      TH1D* newhist_quark_ptd   = new TH1D( histoName_quark_ptd.c_str()  , "", 100, 0., 1.0001 );
      TH1D* newhist_quark_axis2 = new TH1D( histoName_quark_axis2.c_str(), "", 100, 0., 8.     );
      TH1D* newhist_quark_mult  = new TH1D( histoName_quark_mult.c_str() , "", 140, 2.5, 142.5 );

      newhist_quark_ptd   ->Sumw2();
      newhist_quark_axis2 ->Sumw2();
      newhist_quark_mult  ->Sumw2();

      quark_ptd   .push_back(newhist_quark_ptd  );
      quark_axis2 .push_back(newhist_quark_axis2);
      quark_mult  .push_back(newhist_quark_mult );

      std::string histoName_undef_ptd  (Form("undef_%s_ptd_pt%d_rho%d"  , name.c_str(), i_pt, i_rho));
      std::string histoName_undef_axis2(Form("undef_%s_axis2_pt%d_rho%d", name.c_str(), i_pt, i_rho));
      std::string histoName_undef_mult (Form("undef_%s_mult_pt%d_rho%d" , name.c_str(), i_pt, i_rho));

      TH1D* newhist_undef_ptd   = new TH1D( histoName_undef_ptd.c_str()  , "", 100, 0., 1.0001 );
      TH1D* newhist_undef_axis2 = new TH1D( histoName_undef_axis2.c_str(), "", 100, 0., 8.     );
      TH1D* newhist_undef_mult  = new TH1D( histoName_undef_mult.c_str() , "", 140, 2.5, 142.5 );

      newhist_undef_ptd   ->Sumw2();
      newhist_undef_axis2 ->Sumw2();
      newhist_undef_mult  ->Sumw2();

      undef_ptd   .push_back(newhist_undef_ptd  );
      undef_axis2 .push_back(newhist_undef_axis2);
      undef_mult  .push_back(newhist_undef_mult );

    }

  }



  int nentries = tree->GetEntries();


  for( unsigned iEntry=0; iEntry<nentries; ++iEntry ) {

    if( iEntry % 100000 == 0 ) std::cout << "Entry: " << iEntry << " / " << nentries << std::endl;

    tree->GetEntry(iEntry);

    if( pt>2000 ) continue;
    int i_pt  = Bins::getBin( Bins::nPtBins ,  ptBins, pt  );
    int i_rho = Bins::getBin( Bins::nRhoBins, rhoBins, rho );


    if( i_pt <0 ) return;
    if( i_rho<0 ) return;
    if( i_pt  >= Bins::nPtBins  ) continue;
    if( i_rho >= Bins::nRhoBins ) continue;

  

    //if( !(i_pt>=0 && i_pt<=Bins::nPtBins && i_rho>=0 && i_rho<=Bins::nRhoBins ) ) continue;

    std::string histoName_ptd  (Form("pdf_%s_ptd_pt%d_rho%d"  , name.c_str(), i_pt, i_rho));
    std::string histoName_axis2(Form("pdf_%s_axis2_pt%d_rho%d", name.c_str(), i_pt, i_rho));
    std::string histoName_mult (Form("pdf_%s_mult_pt%d_rho%d" , name.c_str(), i_pt, i_rho));


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


    if( partonId==21 ) {

      std::string histoName_gluon_ptd  (Form("gluon_%s_ptd_pt%d_rho%d"  , name.c_str(), i_pt, i_rho));
      std::string histoName_gluon_axis2(Form("gluon_%s_axis2_pt%d_rho%d", name.c_str(), i_pt, i_rho));
      std::string histoName_gluon_mult (Form("gluon_%s_mult_pt%d_rho%d" , name.c_str(), i_pt, i_rho));


      for( unsigned i=0; i<gluon_ptd.size(); ++i ) {
        std::string thisName(gluon_ptd[i]->GetName());
        if( thisName==histoName_gluon_ptd ) {
          gluon_ptd[i]->Fill( ptd, weight );
          break;
        }
      }
      for( unsigned i=0; i<gluon_axis2.size(); ++i ) {
        std::string thisName(gluon_axis2[i]->GetName());
        if( thisName==histoName_gluon_axis2 ) {
          gluon_axis2[i]->Fill( axis2, weight );
          break;
        }
      }
      for( unsigned i=0; i<gluon_mult.size(); ++i ) {
        std::string thisName(gluon_mult[i]->GetName());
        if( thisName==histoName_gluon_mult ) {
          gluon_mult[i]->Fill( mult, weight );
          break;
        }
      }

    } else if( abs(partonId)>0 && abs(partonId)<6  ) {

      std::string histoName_quark_ptd  (Form("quark_%s_ptd_pt%d_rho%d"  , name.c_str(), i_pt, i_rho));
      std::string histoName_quark_axis2(Form("quark_%s_axis2_pt%d_rho%d", name.c_str(), i_pt, i_rho));
      std::string histoName_quark_mult (Form("quark_%s_mult_pt%d_rho%d" , name.c_str(), i_pt, i_rho));


      for( unsigned i=0; i<quark_ptd.size(); ++i ) {
        std::string thisName(quark_ptd[i]->GetName());
        if( thisName==histoName_quark_ptd ) {
          quark_ptd[i]->Fill( ptd, weight );
          break;
        }
      }
      for( unsigned i=0; i<quark_axis2.size(); ++i ) {
        std::string thisName(quark_axis2[i]->GetName());
        if( thisName==histoName_quark_axis2 ) {
          quark_axis2[i]->Fill( axis2, weight );
          break;
        }
      }
      for( unsigned i=0; i<quark_mult.size(); ++i ) {
        std::string thisName(quark_mult[i]->GetName());
        if( thisName==histoName_quark_mult ) {
          quark_mult[i]->Fill( mult, weight );
          break;
        }
      }

    } else {

      //fillHisto( undef_ptd, "ptd", i_pt, i_rho );

      std::string histoName_undef_ptd  (Form("undef_%s_ptd_pt%d_rho%d"  , name.c_str(), i_pt, i_rho));
      std::string histoName_undef_axis2(Form("undef_%s_axis2_pt%d_rho%d", name.c_str(), i_pt, i_rho));
      std::string histoName_undef_mult (Form("undef_%s_mult_pt%d_rho%d" , name.c_str(), i_pt, i_rho));


      for( unsigned i=0; i<undef_ptd.size(); ++i ) {
        std::string thisName(undef_ptd[i]->GetName());
        if( thisName==histoName_undef_ptd ) {
          undef_ptd[i]->Fill( ptd, weight );
          break;
        }
      }
      for( unsigned i=0; i<undef_axis2.size(); ++i ) {
        std::string thisName(undef_axis2[i]->GetName());
        if( thisName==histoName_undef_axis2 ) {
          undef_axis2[i]->Fill( axis2, weight );
          break;
        }
      }
      for( unsigned i=0; i<undef_mult.size(); ++i ) {
        std::string thisName(undef_mult[i]->GetName());
        if( thisName==histoName_undef_mult ) {
          undef_mult[i]->Fill( mult, weight );
          break;
        }
      }

    }

  }

  file->cd();

  for( unsigned i=0; i<pdfs_ptd  .size(); ++i ) pdfs_ptd  [i]->Write();
  for( unsigned i=0; i<pdfs_axis2.size(); ++i ) pdfs_axis2[i]->Write();
  for( unsigned i=0; i<pdfs_mult .size(); ++i ) pdfs_mult [i]->Write();
    
  for( unsigned i=0; i<gluon_ptd  .size(); ++i ) gluon_ptd  [i]->Write();
  for( unsigned i=0; i<gluon_axis2.size(); ++i ) gluon_axis2[i]->Write();
  for( unsigned i=0; i<gluon_mult .size(); ++i ) gluon_mult [i]->Write();
    
  for( unsigned i=0; i<quark_ptd  .size(); ++i ) quark_ptd  [i]->Write();
  for( unsigned i=0; i<quark_axis2.size(); ++i ) quark_axis2[i]->Write();
  for( unsigned i=0; i<quark_mult .size(); ++i ) quark_mult [i]->Write();
    
  for( unsigned i=0; i<undef_ptd  .size(); ++i ) undef_ptd  [i]->Write();
  for( unsigned i=0; i<undef_axis2.size(); ++i ) undef_axis2[i]->Write();
  for( unsigned i=0; i<undef_mult .size(); ++i ) undef_mult [i]->Write();
    

}

