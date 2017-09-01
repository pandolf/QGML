#include <iostream>

#include "../interface/QGMLCommon.h"
#include "QGLikelihood/interface/Bins.h"


#include "TCanvas.h"
#include "TLegend.h"
#include "TTree.h"
#include "TFile.h"
#include "TH2D.h"
#include "TH1D.h"
#include "THStack.h"
#include "TChain.h"




void drawPDFs( const std::string& outdir, const std::string& var, const std::string& axisName, TFile* fileDY, TFile* fileQCD, int nBinsPt, double* ptBins, int nBinsRho, double* rhoBins );



int main( int argc, char* argv[] ) {
 
  QGMLCommon::setStyle();


  TFile* fileDY  = TFile::Open( "pdfsMixed_dy.root"  );
  TFile* fileQCD = TFile::Open( "pdfsMixed_qcd.root" );

  double ptBins[Bins::nPtBins+1];
  Bins::getPtBins(ptBins);
  //std::cout << "-> Using these pt bins:" << std::endl;
  //for( unsigned i=0; i<Bins::nPtBins+1; ++i )
  //  std::cout << "  " << ptBins[i] << std::endl;
  //std::cout << std::endl << std::endl;

  double rhoBins[Bins::nRhoBins+1];
  Bins::getRhoBins(rhoBins);
  //std::cout << "-> Using these rho bins:" << std::endl;
  //for( unsigned i=0; i<Bins::nRhoBins+1; ++i )
  //  std::cout << "  " << rhoBins[i] << std::endl;
  //std::cout << std::endl << std::endl;

  std::string outdir("pdfPlots");
  system( Form("mkdir -p %s", outdir.c_str()) );

  drawPDFs( outdir, "ptd", "p_{T,D}", fileDY, fileQCD, Bins::nPtBins, ptBins, Bins::nRhoBins, rhoBins );





}


void drawPDFs( const std::string& outdir, const std::string& var, const std::string& axisName, TFile* fileDY, TFile* fileQCD, int nBinsPt, double* ptBins, int nBinsRho, double* rhoBins ) {

  std::cout << "-> Starting to draw PDFs for " << var << std::endl;

  for( unsigned i_pt=0; i_pt<nBinsPt; ++i_pt ) {

    for( unsigned i_rho=0; i_rho<nBinsRho; ++i_rho ) {

      float ptMin  =  ptBins [i_pt   ];
      float ptMax  =  ptBins [i_pt+1 ];
      float rhoMin =  rhoBins[i_rho  ];
      float rhoMax =  rhoBins[i_rho+1];

      std::string histoName_pdf  (Form(  "pdf_ptd_pt%d_rho%d"  , i_pt, i_rho));
      std::string histoName_quark(Form("quark_ptd_pt%d_rho%d"  , i_pt, i_rho));
      std::string histoName_gluon(Form("gluon_ptd_pt%d_rho%d"  , i_pt, i_rho));
      std::string histoName_undef(Form("undef_ptd_pt%d_rho%d"  , i_pt, i_rho));

      TH1D* h1_pdf_dy    = (TH1D*)fileDY ->Get(histoName_pdf  .c_str());
      TH1D* h1_quark_dy  = (TH1D*)fileDY ->Get(histoName_quark.c_str());
      TH1D* h1_gluon_dy  = (TH1D*)fileDY ->Get(histoName_gluon.c_str());
      TH1D* h1_undef_dy  = (TH1D*)fileDY ->Get(histoName_undef.c_str());

      TH1D* h1_pdf_qcd   = (TH1D*)fileQCD->Get(histoName_pdf  .c_str());
      TH1D* h1_quark_qcd = (TH1D*)fileQCD->Get(histoName_quark.c_str());
      TH1D* h1_gluon_qcd = (TH1D*)fileQCD->Get(histoName_gluon.c_str());
      TH1D* h1_undef_qcd = (TH1D*)fileQCD->Get(histoName_undef.c_str());


      if( h1_pdf_qcd->GetEntries()==0 ) continue;
      if( h1_pdf_dy ->GetEntries()==0 ) continue;


      TCanvas* c1 = new TCanvas( "c1", "", 600, 600 );
      c1->cd();

      float q_dy = h1_quark_dy->Integral();
      float g_dy = h1_gluon_dy->Integral();
      float u_dy = h1_undef_dy->Integral();

      float qfrac_dy = q_dy / ( q_dy+g_dy+u_dy );
      float gfrac_dy = g_dy / ( q_dy+g_dy+u_dy );
      float ufrac_dy = u_dy / ( q_dy+g_dy+u_dy );

      float q_qcd = h1_quark_qcd->Integral();
      float g_qcd = h1_gluon_qcd->Integral();
      float u_qcd = h1_undef_qcd->Integral();

      float qfrac_qcd = q_qcd / ( q_qcd+g_qcd+u_qcd );
      float gfrac_qcd = g_qcd / ( q_qcd+g_qcd+u_qcd );
      float ufrac_qcd = u_qcd / ( q_qcd+g_qcd+u_qcd );

      float xMin = h1_pdf_dy->GetXaxis()->GetXmin();
      float xMax = h1_pdf_dy->GetXaxis()->GetXmax();
      float yMax_dy  = h1_pdf_dy ->GetMaximum()/h1_pdf_dy ->Integral();
      float yMax_qcd = h1_pdf_qcd->GetMaximum()/h1_pdf_qcd->Integral();
      float yMax = (yMax_dy>yMax_qcd) ? yMax_dy : yMax_qcd;

      h1_pdf_dy ->SetLineColor(38);
      h1_pdf_qcd->SetLineColor(46);

      h1_pdf_dy ->SetFillColor(38);
      h1_pdf_qcd->SetFillColor(46);

      h1_pdf_dy ->SetLineWidth(2);
      h1_pdf_qcd->SetLineWidth(2);

      h1_pdf_dy ->SetFillStyle(3004);
      h1_pdf_qcd->SetFillStyle(3005);


      TH2D* h2_axes = new TH2D( "axes", "", 10, xMin, xMax, 10, 0., yMax*1.6 );
      h2_axes->SetXTitle( axisName.c_str() );
      h2_axes->SetYTitle( "Normalized to Unity" );
      h2_axes->Draw();

      TLegend* legend = new TLegend( 0.25, 0.74, 0.6, 0.94 );
      legend->SetFillColor(0);
      legend->SetTextSize(0.035);
      legend->SetHeader(Form("%.0f < p_{T} < %.0f GeV, |#eta|<1.3, %.0f < #rho < %.0f GeV", ptMin, ptMax, rhoMin, rhoMax));
      legend->AddEntry( h1_pdf_dy , Form( "DY (%.1f%% q / %.1f%% g / %.1f%% u)" , 100.*qfrac_dy , 100.*gfrac_dy , 100.*ufrac_dy ), "L" );
      legend->AddEntry( h1_pdf_qcd, Form("QCD (%.1f%% q / %.1f%% g / %.1f%% u)" , 100.*qfrac_qcd, 100.*gfrac_qcd, 100.*ufrac_qcd), "L" );
      legend->Draw("same");

      h1_pdf_dy ->DrawNormalized("same"); 
      h1_pdf_qcd->DrawNormalized("same"); 

      TPaveText* labelTop = QGMLCommon::get_labelTop();
      TPaveText* labelCMS = QGMLCommon::get_labelCMS();

      labelTop->Draw("same");
      labelCMS->Draw("same");

      gPad->RedrawAxis();

      c1->SaveAs( Form("%s/pdfs_pt%d_rho%d.eps", outdir.c_str(), i_pt, i_rho) );
      c1->SaveAs( Form("%s/pdfs_pt%d_rho%d.pdf", outdir.c_str(), i_pt, i_rho) );

      delete c1;
      delete h2_axes;

    } // i pt
 
  } // i rho

}

