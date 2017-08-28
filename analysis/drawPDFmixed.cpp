#include <iostream>

#include "../interface/QGMLCommon.h"

#include "TCanvas.h"
#include "TLegend.h"
#include "TTree.h"
#include "TFile.h"
#include "TH2D.h"
#include "TH1D.h"
#include "THStack.h"
#include "TChain.h"



void drawVar( const std::string& outdir, TTree* tree, const std::string& var, const std::string& sel, int nBins, float xMin, float xMax, const std::string& axisName, int legendQuadrant );




int main( int argc, char* argv[] ) {
 
  QGMLCommon::setStyle();

  std::string datasetType = "DY";

  if( argc>1 ) {

    datasetType = std::string(argv[1]);

    if( datasetType!="DY" && datasetType!="QCD" ) {
      std::cout << "-> Only 'QCD' and 'DY' are accepted arguments." << std::endl;
      exit(1);
    }

  }

  std::cout << " -> Dataset: " << datasetType << std::endl;

  std::string outdir( Form("plots%s", datasetType.c_str()) );
  system( Form("mkdir -p %s", outdir.c_str()) );

  TChain* tree = new TChain("tlite");

  if( datasetType=="DY" ) {
    tree->Add("treeLite_DYJetsToLL_M50_HT100to200.root");
    tree->Add("treeLite_DYJetsToLL_M50_HT200to400.root");
    tree->Add("treeLite_DYJetsToLL_M50_HT400to600.root");
    //tree->Add("treeLite_DYJetsToLL_M50_HT600to800.root");
    //tree->Add("treeLite_DYJetsToLL_M50_HT800to1200.root");
    //tree->Add("treeLite_DYJetsToLL_M50_HT1200to2500.root");
    //tree->Add("treeLite_DYJetsToLL_M50_HT2500toInf.root");
  } else if( datasetType=="QCD" ) {
    tree->Add("treeLite_QCD_Pt15to30.root");
    tree->Add("treeLite_QCD_Pt30to50.root");
    tree->Add("treeLite_QCD_Pt50to80.root");
    tree->Add("treeLite_QCD_Pt80to120.root");
    tree->Add("treeLite_QCD_Pt120to170.root");
    tree->Add("treeLite_QCD_Pt170to300.root");
    //tree->Add("treeLite_QCD_Pt300to470.root");
    //tree->Add("treeLite_QCD_Pt470to600.root");
    //tree->Add("treeLite_QCD_Pt600to800.root");
    //tree->Add("treeLite_QCD_Pt800to1000.root");
    //tree->Add("treeLite_QCD_Pt1000to1400.root");
    //tree->Add("treeLite_QCD_Pt1400to1800.root");
    //tree->Add("treeLite_QCD_Pt1800to2400.root");
    //tree->Add("treeLite_QCD_Pt2400to3200.root");
    //tree->Add("treeLite_QCD_Pt3200.root");
  }
  
  drawVar( outdir, tree, "pt" , ""                  , 50, 100.,   200., "p_{T} [GeV]", 2 );
  drawVar( outdir, tree, "eta", "pt>120. && pt<200.", 50,  -2.,     2., "#eta"       , 0 );
  drawVar( outdir, tree, "qgl", "pt>120. && pt<200.", 50,   0., 1.0001, "QG LD"      , 0 );




}


void drawVar( const std::string& outdir, TTree* tree, const std::string& var, const std::string& sel, int nBins, float xMin, float xMax, const std::string& axisName, int legendQuadrant ) {

  std::string hname_q( Form("h1_q_%s", var.c_str()) );
  std::string hname_g( Form("h1_g_%s", var.c_str()) );

  TH1D* h1_q = new TH1D( hname_q.c_str(), "", nBins, xMin, xMax );
  h1_q->Sumw2();
  TH1D* h1_g = new TH1D( hname_g.c_str(), "", nBins, xMin, xMax );
  h1_g->Sumw2();

  tree->Project( hname_q.c_str(), var.c_str(), Form("w*(abs(partonId)<5 && %s)", sel.c_str()) );
  tree->Project( hname_g.c_str(), var.c_str(), Form("w*(partonId==21    && %s)", sel.c_str()) );

  float hint_q = h1_q->Integral();
  float hint_g = h1_g->Integral();
  
  float yMax = 0.;

  for( unsigned i=0; i<nBins; i++ ) {

    h1_q->SetBinContent( i+1, h1_q->GetBinContent(i+1)/hint_q );
    h1_g->SetBinContent( i+1, h1_g->GetBinContent(i+1)/hint_g );
    h1_q->SetBinError( i+1, 0. );
    h1_g->SetBinError( i+1, 0. );

    if( h1_q->GetBinContent(i+1)+h1_g->GetBinContent(i+1) > yMax )
      yMax = h1_q->GetBinContent(i+1)+h1_g->GetBinContent(i+1);

  }

  h1_q->SetFillColor( 38 );
  h1_g->SetFillColor( 46 );

  THStack* stack = new THStack();
  stack->Add( h1_q );
  stack->Add( h1_g );

  TH2D* h2_axes = new TH2D( "axes", "", 10, xMin, xMax, 10, 0., 1.35*yMax );
  h2_axes->SetXTitle( axisName.c_str() );
  h2_axes->SetYTitle( "Normalized to Unity" );


  TCanvas* c1 = new TCanvas("c1", "", 600, 600 );
  c1->cd();

  h2_axes->Draw();


  float xMin_leg, xMax_leg, yMin_leg, yMax_leg;

  if( legendQuadrant==2 ) {
    xMin_leg = 0.67;
    xMax_leg = 0.9;
    yMin_leg = 0.73;
    yMax_leg = 0.9;
  } else if( legendQuadrant==0 ) {
    xMin_leg = 0.39;
    xMax_leg = 0.61;
    yMin_leg = 0.73;
    yMax_leg = 0.9;
  }
  
  TLegend* legend = new TLegend( xMin_leg, yMin_leg, xMax_leg, yMax_leg );
  legend->SetTextSize ( 0.038 );
  legend->SetFillColor( 0 );
  legend->AddEntry( h1_q, "Quarks", "F" );
  legend->AddEntry( h1_g, "Gluons", "F" );
  legend->Draw("same");

  stack->Draw("same");

  TPaveText* labelTop = QGMLCommon::get_labelTop();
  TPaveText* labelCMS = QGMLCommon::get_labelCMS();

  labelTop->Draw("same");
  labelCMS->Draw("same");

  gPad->RedrawAxis();

  c1->SaveAs( Form("%s/%s.eps", outdir.c_str(), var.c_str()) );
  c1->SaveAs( Form("%s/%s.pdf", outdir.c_str(), var.c_str()) );

  delete c1;
  delete h2_axes;
  delete h1_q;
  delete h1_g;
  delete stack;

}
