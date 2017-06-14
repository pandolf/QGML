#include <iostream>
#include "math.h"

#include "TFile.h"
#include "TTree.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TStyle.h"
#include "TPaveText.h"
#include "TColor.h"


bool DEBUG_ = false;


void setStyle();
void setColors( int baseColor );
void drawDeltaR( TTree* tree, float ptMin, float ptMax, float etaMin, float etaMax, bool ptWeighted=true ) ;
std::string etaString( float eta );
void drawImages( TTree* tree );
void drawImage( TProfile2D* hp, const std::string& saveName, int baseColor );


int main() {

  setStyle();

  system( "mkdir -p figures/examples" );

  TFile* file = TFile::Open("qgMiniTuple.root", "read");
  TTree* tree = (TTree*)file->Get("qgMiniTupleAK4chs/qgMiniTuple");

  //drawDeltaR( tree, 300., 400., 0., 2., true );
  //drawDeltaR( tree, 300., 400., 0., 2., false );

  drawImages( tree );

  return 0;

}


void drawImages( TTree* tree ) {

  int nPixCh;
  tree->SetBranchAddress( "nPixCh", &nPixCh );
  float drMaxCh;
  tree->SetBranchAddress( "drMaxCh", &drMaxCh );
  float pixelSizeCh;
  tree->SetBranchAddress( "pixelSizeCh", &pixelSizeCh );
  float chImage[99999];
  TBranch *b_chImage;
  tree->SetBranchAddress( "chImage", chImage, &b_chImage );
  int partonId;
  tree->SetBranchAddress( "partonId", &partonId );
  float pt;
  tree->SetBranchAddress( "pt", &pt );
  float eta;
  tree->SetBranchAddress( "eta", &eta );

  tree->GetEntry(0);


  int nPixCh_1D = sqrt(nPixCh);

  int nPixLite = 60;
  float chImageLite[nPixLite];

  TH2D* h2_chImage = new TH2D( "chImage", "", nPixCh_1D, -drMaxCh, drMaxCh, nPixCh_1D, -drMaxCh, drMaxCh );
  TH2D* h2_chImageLite = new TH2D( "chImageLite", "", nPixLite, 0., drMaxCh, 1, 0., 1. );

  TProfile2D* hp_chImageLite_gluon = new TProfile2D( "chImageProfLite_gluon", "", nPixLite, 0., drMaxCh, 1, 0., 1. );
  TProfile2D* hp_chImageLite_quark = new TProfile2D( "chImageProfLite_quark", "", nPixLite, 0., drMaxCh, 1, 0., 1. );

  TProfile2D* hp_chImage_gluon = new TProfile2D( "chImageProf_gluon", "", nPixCh_1D, -drMaxCh, drMaxCh, nPixCh_1D, -drMaxCh, drMaxCh );
  TProfile2D* hp_chImage_quark = new TProfile2D( "chImageProf_quark", "", nPixCh_1D, -drMaxCh, drMaxCh, nPixCh_1D, -drMaxCh, drMaxCh );

  TProfile2D* hp_chImageZoom_gluon = new TProfile2D( "chImageProfZoom_gluon", "", 40, -0.1, 0.1, 40, -0.1, 0.1 );
  TProfile2D* hp_chImageZoom_quark = new TProfile2D( "chImageProfZoom_quark", "", 40, -0.1, 0.1, 40, -0.1, 0.1 );



  int nentries = tree->GetEntries();
  nentries = 50000;
  int drawnEvents = 0;


  for( unsigned iEntry=0; iEntry<nentries; iEntry++ ) {

    tree->GetEntry(iEntry);


    // initialize lite image
    for( unsigned i=0; i<nPixLite; i++ ) chImageLite[i]=0.;


    if( DEBUG_ ) {
      std::cout << "-----: " << std::endl;
      if( partonId==21 ) 
        std::cout << "Gluon Jet" << std::endl;
      else
        std::cout << "Quark Jet" << std::endl;
      std::cout << Form("p_{T} = %.0f GeV",pt) <<std::endl;
      std::cout << Form("#eta = %.1f",eta) <<std::endl;
    }

    // this loop on the chImage:
    for( unsigned i=0; i<nPixCh; ++i ) {

      int etaBin = i % nPixCh_1D;
      int phiBin = floor((float)i/((float)nPixCh_1D));  
      etaBin += 1;
      phiBin += 1;

      float dEta = -0.3 + 0.005*(etaBin-1.) + 0.00000001;
      float dPhi = -0.3 + 0.005*(phiBin-1.) + 0.00000001;

      float dR = sqrt( dEta*dEta + dPhi*dPhi );

      if( dR>0.3 ) continue; //std::cout << "This shouldn't be possible." << std::endl;

      int dRbin = int( dR/0.005 );
      chImageLite[dRbin] += chImage[i];

      if( DEBUG_ ) {
        if( chImage[i]>0. ) {
          std::cout << "chimage: " << chImage[i] << std::endl;
          std::cout << "etaBin: " << etaBin << std::endl;
          std::cout << "phiBin: " << phiBin << std::endl;
          std::cout << "deta: " << dEta << std::endl;
          std::cout << "dphi: " << dPhi << std::endl;
          std::cout << "dR: " << dR << std::endl;
          std::cout << "dRbin: " << dRbin << std::endl;
        }
      }


      h2_chImage->SetBinContent( etaBin, phiBin, chImage[i] );

      if( partonId==21 ) {

        hp_chImage_gluon->Fill( dEta, dPhi, chImage[i] );
        hp_chImageZoom_gluon->Fill( dEta, dPhi, chImage[i] );

      } else if ( abs(partonId)<4 ) {

        hp_chImage_quark->Fill( dEta, dPhi, chImage[i] );
        hp_chImageZoom_quark->Fill( dEta, dPhi, chImage[i] );


      }

    } // for nPixCh

    for(unsigned i=0; i<nPixLite; ++i ) {

      h2_chImageLite->SetBinContent( i, 1, chImageLite[i] );

      float dR = float(i)*0.005;

      if( partonId==21 ) {
        hp_chImageLite_gluon->Fill( dR, 0., chImageLite[i] );
      } else if ( abs(partonId)<4 ) {
        hp_chImageLite_quark->Fill( dR, 0., chImageLite[i] );
      }

    } // for nPixLite


    if( DEBUG_ ) {
      std::cout << std::endl << "chImageLite: " << std::endl;
      for(unsigned i=0; i<60; ++i ) 
        std::cout << i << " " << chImageLite[i] << std::endl;
    }


    if( iEntry%1000==0 && drawnEvents<100 ) {

      TCanvas* c1 = new TCanvas("c1", "", 600, 600);
      c1->cd();

      setColors(kRed);

      TH2D* h2_axes = new TH2D("axes", "", 10, -0.3, 0.3, 10, -0.3, 0.3);
      h2_axes->Draw("");

      TPaveText* label = new TPaveText(0.2, 0.75, 0.55, 0.9, "brNDC");
      label->SetTextSize(0.032);
      label->SetFillColor(0);
      label->SetTextAlign(11);
      if( partonId==21 ) 
        label->AddText("Gluon Jet");
      else
        label->AddText("Quark Jet");
      label->AddText( Form("p_{T} = %.0f GeV",pt) );
      label->AddText( Form("#eta = %.1f",eta) );
      label->Draw("same");

      h2_chImage->Draw("col z same");

      gPad->RedrawAxis();

      c1->SaveAs( Form("figures/examples/chImage_%d.pdf", iEntry) );
      c1->SaveAs( Form("figures/examples/chImage_%d.eps", iEntry) );

      TCanvas* cLite = new TCanvas("cLite", "", 600, 600);
      cLite->cd();

      TH2D* h2_axesLite = new TH2D("axesLite", "", 10, 0., 0.3, 10, 0., 2.);
      h2_axesLite->Draw();

      label->Draw("same");

      h2_chImageLite->Draw("col z same");

      gPad->RedrawAxis();

      cLite->SaveAs( Form("figures/examples/chImageLite_%d.pdf", iEntry) );
      cLite->SaveAs( Form("figures/examples/chImageLite_%d.eps", iEntry) );

      delete c1;
      delete cLite;
      delete h2_axes;
      delete h2_axesLite;
      delete label;

      drawnEvents++;

      if( DEBUG_ ) exit(1);

    }

  } // for entries


  drawImage( hp_chImage_gluon, "chImage_gluon", kRed );
  drawImage( hp_chImage_quark, "chImage_quark", kRed );

  drawImage( hp_chImageZoom_gluon, "chImageZoom_gluon", kRed );
  drawImage( hp_chImageZoom_quark, "chImageZoom_quark", kRed );

  drawImage( hp_chImageLite_gluon, "chImageLite_gluon", kRed );
  drawImage( hp_chImageLite_quark, "chImageLite_quark", kRed );

}


void drawImage( TProfile2D* hp, const std::string& saveName, int baseColor ) {

  TCanvas* c1 = new TCanvas( "c1", "", 600, 600 );
  c1->cd();

  setColors( baseColor );

  hp->Draw("col z" );

  gPad->RedrawAxis();

  c1->SaveAs( Form("figures/%s.eps", saveName.c_str()) );
  c1->SaveAs( Form("figures/%s.pdf", saveName.c_str()) );

  delete c1;
  
}


void setColors( int baseColor ) {

  int colors[255];
  colors[0] = kWhite;
  colors[1] = baseColor-10;
  colors[2] = baseColor-7;
  colors[3] = baseColor-4;
  colors[4] = baseColor;
  colors[5] = baseColor+1;
  colors[6] = baseColor+2;
  colors[7] = baseColor+3;
  colors[8] = baseColor+4;
  colors[9] = kBlack;
  gStyle->SetPalette(10,colors);

}


void drawDeltaR( TTree* tree, float ptMin, float ptMax, float etaMin, float etaMax, bool ptWeighted ) {

  std::string suffix( Form("pt%.0f_%.0f", ptMin, ptMax ) );
  suffix = suffix + "_eta" + etaString(etaMin) + "_" + etaString(etaMax);
  if( ptWeighted ) suffix = suffix + "_ptW";

  int nBins = 44;
  float xMin = 0.;
  float xMax = 0.44;

  TH1D* h1_ch = new TH1D( Form("h1_ch_%s", suffix.c_str()), "", nBins, xMin, xMax );
  TH1D* h1_nh = new TH1D( Form("h1_nh_%s", suffix.c_str()), "", nBins, xMin, xMax );
  TH1D* h1_ph = new TH1D( Form("h1_ph_%s", suffix.c_str()), "", nBins, xMin, xMax );

  h1_ch->SetLineColor(46);
  h1_nh->SetLineColor(38);
  h1_ph->SetLineColor(30);

  h1_ch->SetLineWidth(2);
  h1_nh->SetLineWidth(2);
  h1_ph->SetLineWidth(2);

  int nentries = tree->GetEntries();


  float pt;
  tree->SetBranchAddress( "pt", &pt );
  float eta;
  tree->SetBranchAddress( "eta", &eta );
  int nDRch;
  tree->SetBranchAddress( "nDRch", &nDRch );
  int nDRnh;
  tree->SetBranchAddress( "nDRnh", &nDRnh );
  int nDRph;
  tree->SetBranchAddress( "nDRph", &nDRph );
  float deltaRch[500];
  tree->SetBranchAddress( "deltaRch", deltaRch );
  float deltaRnh[500];
  tree->SetBranchAddress( "deltaRnh", deltaRnh );
  float deltaRph[500];
  tree->SetBranchAddress( "deltaRph", deltaRph );
  float ptch[500];
  tree->SetBranchAddress( "ptch", ptch );
  float ptnh[500];
  tree->SetBranchAddress( "ptnh", ptnh );
  float ptph[500];
  tree->SetBranchAddress( "ptph", ptph );

  for( unsigned iEntry=0; iEntry<nentries; ++iEntry ) {

    if( iEntry % 10000 == 0 ) std::cout << "Entry: " << iEntry << " / " << nentries << std::endl;

    tree->GetEntry(iEntry);

    if( pt < ptMin ) continue;
    if( pt > ptMax ) continue;
    if( fabs(eta) < etaMin ) continue;
    if( fabs(eta) > etaMax ) continue;

    if( ptWeighted ) {

      for( unsigned i=0; i<nDRch; ++i )
        h1_ch->Fill( deltaRch[i], ptch[i]/pt );
      for( unsigned i=0; i<nDRnh; ++i )
        h1_nh->Fill( deltaRnh[i], ptnh[i]/pt );
      for( unsigned i=0; i<nDRph; ++i )
        h1_ph->Fill( deltaRph[i], ptph[i]/pt );

    } else {

      for( unsigned i=0; i<nDRch; ++i )
        h1_ch->Fill( deltaRch[i] );
      for( unsigned i=0; i<nDRnh; ++i )
        h1_nh->Fill( deltaRnh[i] );
      for( unsigned i=0; i<nDRph; ++i )
        h1_ph->Fill( deltaRph[i] );

    }

  }

  TCanvas *c1 = new TCanvas("c1", "", 600, 600);
  c1->cd();

  TH2D* h2_axes = new TH2D("axes", "", 10, xMin, xMax, 10, 0., 0.3 );
  h2_axes->SetXTitle("#DeltaR(candidate-jet)");
  h2_axes->SetYTitle("Normalized to Unity");
  h2_axes->Draw();

  TLegend* legend = new TLegend( 0.55, 0.7, 0.9, 0.93 );
  legend->SetFillColor(0);
  legend->SetTextSize(0.035);
  legend->AddEntry( h1_ch, "Charged hadrons", "L" );
  legend->AddEntry( h1_nh, "Neutral hadrons", "L" );
  legend->AddEntry( h1_ph, "Photons", "L" );
  legend->Draw("same");

  TPaveText* label_pt = new TPaveText( 0.19, 0.85, 0.5, 0.91, "brNDC" );
  label_pt->SetTextSize(0.035);
  label_pt->SetFillColor(0);
  label_pt->SetTextAlign(11);
  label_pt->AddText( Form("%.0f < p_{T} < %.0f GeV", ptMin, ptMax) );
  label_pt->Draw("same");

  TPaveText* label_eta = new TPaveText( 0.19, 0.80, 0.5, 0.85, "brNDC" );
  label_eta->SetTextSize(0.035);
  label_eta->SetFillColor(0);
  label_eta->SetTextAlign(11);
  label_eta->AddText( Form("%.1f < |#eta| < %.1f", etaMin, etaMax) );
  label_eta->Draw("same");

  h1_ch->DrawNormalized("same");
  h1_nh->DrawNormalized("same");
  h1_ph->DrawNormalized("same");

  c1->SaveAs( Form("figures/profile_%s.eps", suffix.c_str()) );
  c1->SaveAs( Form("figures/profile_%s.pdf", suffix.c_str()) );

  TCanvas* c1_log = new TCanvas("c1_log", "", 600, 600);
  c1_log->cd();
  c1_log->SetLogy();

  TH2D* h2_axes_log = new TH2D("axes_log", "", 10, xMin, xMax, 10, 0.000001, 2. );
  h2_axes_log->SetXTitle("#DeltaR(candidate-jet)");
  h2_axes_log->SetYTitle("Normalized to Unity");
  h2_axes_log->Draw();

  legend->Draw("same");

  h1_ch->DrawNormalized("same");
  h1_nh->DrawNormalized("same");
  h1_ph->DrawNormalized("same");

  c1_log->SaveAs( Form("figures/profile_%s_log.eps", suffix.c_str()) );
  c1_log->SaveAs( Form("figures/profile_%s_log.pdf", suffix.c_str()) );


  delete c1;
  delete c1_log;
  delete legend;
  delete h2_axes;
  delete h2_axes_log;
  delete h1_ch;
  delete h1_nh;
  delete h1_ph;

}


std::string etaString( float eta ) {

  double intpart;
  return std::string( Form("%.0fp%.0f", eta, 10.*modf(eta, &intpart)) );

}



void setStyle() {

  TStyle* style = new TStyle("DrawBaseStyle", "");
  style->SetCanvasColor(0);
  style->SetPadColor(0);
  style->SetFrameFillColor(0);
  style->SetStatColor(0);
  style->SetOptStat(0);
  style->SetTitleFillColor(0);
  style->SetCanvasBorderMode(0);
  style->SetPadBorderMode(0);
  style->SetFrameBorderMode(0);
  style->SetPadBottomMargin(0.12);
  style->SetPadLeftMargin(0.12);
  style->cd();
  // For the canvas:
  style->SetCanvasBorderMode(0);
  style->SetCanvasColor(kWhite);
  style->SetCanvasDefH(600); //Height of canvas
  style->SetCanvasDefW(600); //Width of canvas
  style->SetCanvasDefX(0); //POsition on screen
  style->SetCanvasDefY(0);
  // For the Pad:
  style->SetPadBorderMode(0);
  style->SetPadColor(kWhite);
  style->SetPadGridX(false);
  style->SetPadGridY(false);
  style->SetGridColor(0);
  style->SetGridStyle(3);
  style->SetGridWidth(1);
  // For the frame:
  style->SetFrameBorderMode(0);
  style->SetFrameBorderSize(1);
  style->SetFrameFillColor(0);
  style->SetFrameFillStyle(0);
  style->SetFrameLineColor(1);
  style->SetFrameLineStyle(1);
  style->SetFrameLineWidth(1);
  // Margins:
  style->SetPadTopMargin(0.05);
  style->SetPadBottomMargin(0.15);//0.13);
  style->SetPadLeftMargin(0.15);//0.16);
  style->SetPadRightMargin(0.05);//0.02);
  // For the Global title:
  style->SetOptTitle(0);
  style->SetTitleFont(42);
  style->SetTitleColor(1);
  style->SetTitleTextColor(1);
  style->SetTitleFillColor(10);
  style->SetTitleFontSize(0.05);
  // For the axis titles:
  style->SetTitleColor(1, "XYZ");
  style->SetTitleFont(42, "XYZ");
  style->SetTitleSize(0.05, "XYZ");
  style->SetTitleXOffset(1.15);//0.9);
  style->SetTitleYOffset(1.5); // => 1.15 if exponents
  // For the axis labels:
  style->SetLabelColor(1, "XYZ");
  style->SetLabelFont(42, "XYZ");
  style->SetLabelOffset(0.007, "XYZ");
  style->SetLabelSize(0.045, "XYZ");
  // For the axis:
  style->SetAxisColor(1, "XYZ");
  style->SetStripDecimals(kTRUE);
  style->SetTickLength(0.03, "XYZ");
  style->SetNdivisions(510, "XYZ");
  //  style->SetPadTickX(1); // To get tick marks on the opposite side of the frame
  style->SetPadTickY(1);
  // for histograms:
  style->SetHistLineColor(1);
  // for the palette
  Double_t stops[5] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  Double_t red  [5] = { 0.00, 0.00, 0.87, 1.00, 0.51 };
  Double_t green[5] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
  Double_t blue [5] = { 0.51, 1.00, 0.12, 0.00, 0.00 };
  TColor::CreateGradientColorTable(5, stops, red, green, blue, 100);
  style->SetNumberContours(100);

  style->cd();

}
