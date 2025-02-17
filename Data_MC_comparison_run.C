// ------------ Data and MC comparison (eta, phi and pT distributions) + ratios MC/data(A02D level)
#include <iostream>
#include <TAttLine.h>
#include <TAxis.h>
#include <TCanvas.h>
#include <TChain.h>
#include <TDirectoryFile.h>
#include <TF1.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <THashList.h>
#include <TLatex.h>
#include <TLine.h>
#include <TLegend.h>
#include <TList.h>
#include <TMath.h>
#include <TMatrix.h>
#include <TVirtualPad.h>
#include <TStopwatch.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TTree.h>
#include <string>
#include <TROOT.h>
#include <fstream>

using namespace std;

void Data_MC_comparison_run(string MC_prod, string period, string apass, int run){
  
  cout << "MC_prod: " << MC_prod << endl;
  cout << "period: " << period << endl;
  cout << "apass: " << apass << endl;
  cout << "run: " << run << endl;  
    
  gROOT->SetBatch(kTRUE);
  gStyle->SetOptStat(0);
  
  //----------Input and Output directory - same directory of the config file
  char output_dir_name[1][200] = {"/home/luca/cernbox/assegnoTorino/MIDQC/aQC-studies/pp13TeV2023_apass4"}; //same directory of the config file
  
  Long_t *dummy1 = 0, *dummy2 = 0, *dummy3 = 0, *dummy4 = 0;
    
  TFile *file_in_data = 0x0;
  TFile *file_in_MC = 0x0;
  
  //---------- Read data file
  if(gSystem->GetPathInfo(Form("%s/%i/AnalysisResultsData_run_%i.root",output_dir_name[0],run,run),dummy1,dummy2,dummy3,dummy4) != 0){
    cout << "File Not Found! Try again" << endl;  
    return;
  }
  file_in_data = new TFile(Form("%s/%i/AnalysisResultsData_run_%i.root",output_dir_name[0],run,run),"read");
  printf("Open File data: %s\n",file_in_data->GetName());
  
  //---------- Read MC file
  if(gSystem->GetPathInfo(Form("%s/%i/AnalysisResultsMC_run_%i.root",output_dir_name[0],run,run),dummy1,dummy2,dummy3,dummy4) != 0){
    cout << "File Not Found! Try again" << endl;    
    return;
  }
  file_in_MC = new TFile(Form("%s/%i/AnalysisResultsMC_run_%i.root",output_dir_name[0],run,run),"read");
  printf("Open File data: %s\n",file_in_MC->GetName());

  //---------- Create .txt file for good/bad/la runs
  ofstream hOutGood;
  hOutGood.open(Form("%s/goodRuns.txt",output_dir_name[0]), std::ios_base::app);

  ofstream hOutBad;
  hOutBad.open(Form("%s/badRuns.txt",output_dir_name[0]), std::ios_base::app);
  
  //---------- Interesting objects: pT, eta, phi distributions
  string objPath = "analysis-muon-selection/output";
  
  //--- Matched tracks MCH-MID
  THashList *output_data_matched = (THashList *)file_in_data->Get(objPath.c_str());
  TList *TrackMuon_matchedMchMid_data = (TList*) output_data_matched->FindObject("TrackMuon_matchedMchMid");
  TH1F *hPt_matchedMchMid_data = 0x0;
  hPt_matchedMchMid_data = (TH1F*) TrackMuon_matchedMchMid_data->FindObject("Pt");
  hPt_matchedMchMid_data->SetName("hPt_matchedMchMid_data");
  hPt_matchedMchMid_data->Scale(1./hPt_matchedMchMid_data->Integral());
  
  TH1F *hEta_matchedMchMid_data = 0x0;
  hEta_matchedMchMid_data = (TH1F*) TrackMuon_matchedMchMid_data->FindObject("Eta");
  hEta_matchedMchMid_data->SetName("hEta_matchedMchMid_data");
  hEta_matchedMchMid_data->Scale(1./hEta_matchedMchMid_data->Integral());
  
  TH1F *hPhi_matchedMchMid_data = 0x0;
  hPhi_matchedMchMid_data = (TH1F*) TrackMuon_matchedMchMid_data->FindObject("Phi");
  hPhi_matchedMchMid_data->SetName("hPhi_matchedMchMid_data");
  hPhi_matchedMchMid_data->Scale(1./hPhi_matchedMchMid_data->Integral());
  
  
  THashList *output_MC_matched = (THashList *)file_in_MC->Get(objPath.c_str());
  TList *TrackMuon_matchedMchMid_MC = (TList*) output_MC_matched->FindObject("TrackMuon_matchedMchMid");
  TH1F *hPt_matchedMchMid_MC = 0x0;
  hPt_matchedMchMid_MC = (TH1F*) TrackMuon_matchedMchMid_MC->FindObject("Pt");
  hPt_matchedMchMid_MC->SetName("hPt_matchedMchMid_MC");
  hPt_matchedMchMid_MC->Scale(1./hPt_matchedMchMid_MC->Integral());
  
  TH1F *hEta_matchedMchMid_MC = 0x0;
  hEta_matchedMchMid_MC = (TH1F*) TrackMuon_matchedMchMid_MC->FindObject("Eta");
  hEta_matchedMchMid_MC->SetName("hEta_matchedMchMid_MC");
  hEta_matchedMchMid_MC->Scale(1./hEta_matchedMchMid_MC->Integral());
  
  TH1F *hPhi_matchedMchMid_MC = 0x0;
  hPhi_matchedMchMid_MC = (TH1F*) TrackMuon_matchedMchMid_MC->FindObject("Phi");
  hPhi_matchedMchMid_MC->SetName("hPhi_matchedMchMid_MC");
  hPhi_matchedMchMid_MC->Scale(1./hPhi_matchedMchMid_MC->Integral());
   
  TLatex lat_cuts;
  lat_cuts.SetTextSize(0.03); 
  lat_cuts.SetTextFont(42);
  lat_cuts.SetTextColor(kBlack);
  
  //--- Matched tracks MCH-MID + pt>0.7 
  THashList *output_data_matched_ptsel = (THashList *)file_in_data->Get(objPath.c_str());
  TList *TrackMuon_matchedMchMid_ptsel_data = (TList*) output_data_matched_ptsel->FindObject("TrackMuon_MuonLow2023");
  TH1F *hPt_matchedMchMid_ptsel_data = 0x0;
  hPt_matchedMchMid_ptsel_data = (TH1F*) TrackMuon_matchedMchMid_ptsel_data->FindObject("Pt");
  hPt_matchedMchMid_ptsel_data->SetName("hPt_matchedMchMid_ptsel_data");
  hPt_matchedMchMid_ptsel_data->Scale(1./hPt_matchedMchMid_ptsel_data->Integral());
  
  TH1F *hEta_matchedMchMid_ptsel_data = 0x0;
  hEta_matchedMchMid_ptsel_data = (TH1F*) TrackMuon_matchedMchMid_ptsel_data->FindObject("Eta");
  hEta_matchedMchMid_ptsel_data->SetName("hEta_matchedMchMid_ptsel_data");
  hEta_matchedMchMid_ptsel_data->Scale(1./hEta_matchedMchMid_ptsel_data->Integral());
  
  TH1F *hPhi_matchedMchMid_ptsel_data = 0x0;
  hPhi_matchedMchMid_ptsel_data = (TH1F*) TrackMuon_matchedMchMid_ptsel_data->FindObject("Phi");
  hPhi_matchedMchMid_ptsel_data->SetName("hPhi_matchedMchMid_ptsel_data");
  hPhi_matchedMchMid_ptsel_data->Scale(1./hPhi_matchedMchMid_ptsel_data->Integral());
  
  THashList *output_MC_matched_ptsel = (THashList *)file_in_MC->Get(objPath.c_str());
  TList *TrackMuon_matchedMchMid_ptsel_MC = (TList*) output_MC_matched_ptsel->FindObject("TrackMuon_MuonLow2023");
  TH1F *hPt_matchedMchMid_ptsel_MC = 0x0;
  hPt_matchedMchMid_ptsel_MC = (TH1F*) TrackMuon_matchedMchMid_ptsel_MC->FindObject("Pt");
  hPt_matchedMchMid_ptsel_MC->SetName("hPt_matchedMchMid_ptsel_MC");
  hPt_matchedMchMid_ptsel_MC->Scale(1./hPt_matchedMchMid_ptsel_MC->Integral());
  
  TH1F *hEta_matchedMchMid_ptsel_MC = 0x0;
  hEta_matchedMchMid_ptsel_MC = (TH1F*) TrackMuon_matchedMchMid_ptsel_MC->FindObject("Eta");
  hEta_matchedMchMid_ptsel_MC->SetName("hEta_matchedMchMid_ptsel_MC");
  hEta_matchedMchMid_ptsel_MC->Scale(1./hEta_matchedMchMid_ptsel_MC->Integral());
  
  TH1F *hPhi_matchedMchMid_ptsel_MC = 0x0;
  hPhi_matchedMchMid_ptsel_MC = (TH1F*) TrackMuon_matchedMchMid_ptsel_MC->FindObject("Phi");
  hPhi_matchedMchMid_ptsel_MC->SetName("hPhi_matchedMchMid_ptsel_MC");
  hPhi_matchedMchMid_ptsel_MC->Scale(1./hPhi_matchedMchMid_ptsel_MC->Integral());
  
  TLatex lat_cuts_ptsel;
  lat_cuts_ptsel.SetTextSize(0.03); 
  lat_cuts_ptsel.SetTextFont(42);
  lat_cuts_ptsel.SetTextColor(kBlack);
  
  //---- ratios MC/data
  TH1F *hRatio_Pt_matchedMchMid_MC_data = (TH1F*) hPt_matchedMchMid_MC->Clone("hRatio_Pt_matchedMchMid_MC_data");
  hRatio_Pt_matchedMchMid_MC_data->Divide(hPt_matchedMchMid_data);
  TH1F *hRatio_Eta_matchedMchMid_MC_data = (TH1F*) hEta_matchedMchMid_MC->Clone("hRatio_Eta_matchedMchMid_MC_data");
  hRatio_Eta_matchedMchMid_MC_data->Divide(hEta_matchedMchMid_data);
  TH1F *hRatio_Phi_matchedMchMid_MC_data = (TH1F*) hPhi_matchedMchMid_MC->Clone("hRatio_Phi_matchedMchMid_MC_data");
  hRatio_Phi_matchedMchMid_MC_data->Divide(hPhi_matchedMchMid_data);
  
  TH1F *hRatio_Pt_matchedMchMid_ptsel_MC_data = (TH1F*) hPt_matchedMchMid_ptsel_MC->Clone("hRatio_Pt_matchedMchMid_ptsel_MC_data");
  hRatio_Pt_matchedMchMid_ptsel_MC_data->Divide(hPt_matchedMchMid_ptsel_data);
  TH1F *hRatio_Eta_matchedMchMid_ptsel_MC_data = (TH1F*) hEta_matchedMchMid_ptsel_MC->Clone("hRatio_Eta_matchedMchMid_ptsel_MC_data");
  hRatio_Eta_matchedMchMid_ptsel_MC_data->Divide(hEta_matchedMchMid_ptsel_data);
  TH1F *hRatio_Phi_matchedMchMid_ptsel_MC_data = (TH1F*) hPhi_matchedMchMid_ptsel_MC->Clone("hRatio_Phi_matchedMchMid_ptsel_MC_data");
  hRatio_Phi_matchedMchMid_ptsel_MC_data->Divide(hPhi_matchedMchMid_ptsel_data);

  double lowLimitEtaPhi = 0.9, highLimitEtaPhi = 1.1; //+- 10% from unity in the ratio of eta and phi
  double lowLimitPt = 0.8, highLimitPt = 1.2; //+- 20% from unity in the ratio of p_{T} in the range 0-2 GeV/c as suggested by il Nutellaro
  int nBinsEta = 0, nBinsPhi = 0, nBinsPt = 0;
  nBinsEta = hRatio_Eta_matchedMchMid_ptsel_MC_data->GetNbinsX();
  nBinsPhi = hRatio_Phi_matchedMchMid_ptsel_MC_data->GetNbinsX();
  nBinsPt = hRatio_Pt_matchedMchMid_ptsel_MC_data->GetNbinsX();
  
  int nBinsBadEta = 0, nBinsBadPhi = 0, nBinsBadPt = 0;
  int nBinsGoodEta = 0, nBinsGoodPhi = 0, nBinsGoodPt = 0;

  bool isGoodEta = 0, isGoodPhi = 0, isGoodPt = 0;

  //-- loop through the bins of the eta histo MC/data ratio and check quality of comparison (ratio within limits)
  for (int iEta = 0; iEta < nBinsEta; iEta++) {
    double getBinContentEta = 0.;
    
    getBinContentEta = hRatio_Eta_matchedMchMid_ptsel_MC_data->GetBinContent(iEta+1);
    //Add condition to skip the bump at eta -3.7
    if (getBinContentEta > 1e-6) { //Tehre is something in the bin
      nBinsGoodEta++;

      if (getBinContentEta <= highLimitEtaPhi && getBinContentEta >= lowLimitEtaPhi) { //Good
        //cout << "Ratio of bin " << iEta+1 << ": " << getBinContentEta << endl;
        continue;
      }

      else { //Bad
        //cout << "Bin " << iEta+1 << " is bad, ratio: " << getBinContentEta << endl;
        nBinsBadEta++;
      }

    }
  }

  //-- ratio of "bad" bins over total number of bins
  double badRatioEta = 0.;
  badRatioEta = (nBinsBadEta/(double)nBinsGoodEta)*100;
  cout << "For run: " << run << " the number of bad bins is: " << nBinsBadEta << " and the number of good bins is: " << nBinsGoodEta << " the ratio bad/good bins for Eta is: " << badRatioEta << endl;
  if (badRatioEta <= 10) { //Number of bins with bad ratio <= 10% of the total number of bins -> run is good for eta
    //cout << "Run is good for eta" << endl;
    isGoodEta = true;
  }

  else {  //Number of bins with bad ratio > 10% of the total number of bins -> run is bad for eta
    //cout << "Run is bad for eta" << endl;
    isGoodEta = false;
  }

  //-- loop through the bins of the phi histo MC/data ratio and check quality of comparison (ratio within limits)
  for (int iPhi = 0; iPhi < nBinsPhi; iPhi++) {
    double getBinContentPhi = 0.;
    
    getBinContentPhi = hRatio_Phi_matchedMchMid_ptsel_MC_data->GetBinContent(iPhi+1);
    //Add condition to skip the bump at Phi -3.7
    if (getBinContentPhi > 1e-6) { //Tehre is something in the bin
      nBinsGoodPhi++;

      if (getBinContentPhi <= highLimitEtaPhi && getBinContentPhi >= lowLimitEtaPhi) { //Good
        //cout << "Ratio of bin " << iPhi+1 << ": " << getBinContentPhi << endl;
        continue;
      }

      else { //Bad
        //cout << "Bin " << iPhi+1 << " is bad, ratio: " << getBinContentPhi << endl;
        nBinsBadPhi++;
      }

    }
  }

  //-- ratio of "bad" bins over total number of bins
  double badRatioPhi = 0.;
  badRatioPhi = (nBinsBadPhi/(double)nBinsGoodPhi)*100;
  cout << "For run: " << run << " the number of bad bins is: " << nBinsBadPhi << " and the number of good bins is: " << nBinsGoodPhi << " the ratio bad/good bins for Phi is: " << badRatioPhi << endl;
  if (badRatioPhi <= 10) { //Number of bins with bad ratio <= 10% of the total number of bins -> run is good for Phi
    //cout << "Run is good for Phi" << endl;
    isGoodPhi = true;
  }

  else {  //Number of bins with bad ratio > 10% of the total number of bins -> run is bad for Phi
    //cout << "Run is bad for Phi" << endl;
    isGoodPhi = false;
  }

  //-- loop through the bins of the Pt histo MC/data ratio and check quality of comparison (ratio within limits)
  for (int iPt = 0; iPt < nBinsPt; iPt++) {
    double getBinContentPt = 0., getBinLimitPt = 0.;
    
    getBinContentPt = hRatio_Pt_matchedMchMid_ptsel_MC_data->GetBinContent(iPt+1);
    getBinLimitPt = hRatio_Pt_matchedMchMid_ptsel_MC_data->GetBinLowEdge(iPt+1);
    //Add condition to skip the bump at Pt -3.7
    if (getBinContentPt > 1e-6 && ((int)getBinLimitPt <= 2)) { //Tehre is something in the bin
      nBinsGoodPt++;

      if (getBinContentPt <= highLimitPt && getBinContentPt >= lowLimitPt) { //Good
        //cout << "Ratio of bin " << iPt+1 << ": " << getBinContentPt << endl;
        continue;
      }

      else { //Bad
        //cout << "Bin " << iPt+1 << " is bad, ratio: " << getBinContentPt << endl;
        nBinsBadPt++;
      }

    }
  }

  //-- ratio of "bad" bins over total number of bins
  double badRatioPt = 0.;
  badRatioPt = (nBinsBadPt/(double)nBinsGoodPt)*100;
  cout << "For run: " << run << " the number of bad bins is: " << nBinsBadPt << " and the number of good bins is: " << nBinsGoodPt << " the ratio bad/good bins for Pt is: " << badRatioPt << endl;
  if (badRatioPt <= 10) { //Number of bins with bad ratio <= 10% of the total number of bins -> run is good for Pt
    //cout << "Run is good for Pt" << endl;
    isGoodPt = true;
  }

  else {  //Number of bins with bad ratio > 10% of the total number of bins -> run is bad for Pt
    //cout << "Run is bad for Pt" << endl;
    isGoodPt = false;
  }

  //-- write to QC.txt file
  if (isGoodEta && isGoodPhi) { //Good for eta and phi -> run is good
    hOutGood << run << "\t" << isGoodEta << "\t" << isGoodPhi<< "\t" << isGoodPt << "\n";
  }

  else {
    hOutBad << run << "\t" << isGoodEta << "\t" << isGoodPhi<< "\t" << isGoodPt << "\n";
  }

  //-- draw Eta distrib Matched tracks MCH-MID + ratios 
  TCanvas *can_etadistrib_comparison;
  TLegend *leg_etadistrib_comparison[2];
  TLine *line_etadistrib_comparison;
  TH1F *frame_etadistrib_comparison[2];
  TPad* pad_etadistrib_comparison[2];
  
  can_etadistrib_comparison = new TCanvas("Etadistrib_comparison_data_MC","Etadistrib_comparison_data_MC",1600,800);
  can_etadistrib_comparison->Divide(2,1); 
  for(int i=0;i<2;i++){
   can_etadistrib_comparison->cd(i+1);
   pad_etadistrib_comparison[i] = new TPad("padetadistrib_comparison","",0,0,1,1);
   pad_etadistrib_comparison[i]->SetMargin(0.15,0.1,0.15,0.1); //sx,dx,basso,alto
   pad_etadistrib_comparison[i]->Draw();
   can_etadistrib_comparison->cd(i+1);
   pad_etadistrib_comparison[i]->cd();
   //pad_etadistrib_comparison[i]->SetLogz();
   
   if(i==0){
    pad_etadistrib_comparison[i]->SetLogy(); 
    frame_etadistrib_comparison[i] = new TH1F("frame_etadistrib_comparison","",1,-4,-2.5); 
    frame_etadistrib_comparison[i]->SetTitle(Form("#eta distribution - run %i",run));
    frame_etadistrib_comparison[i]->GetXaxis()->SetTitle("#eta");
    frame_etadistrib_comparison[i]->GetYaxis()->SetTitle("Normalized counts");
    frame_etadistrib_comparison[i]->GetXaxis()->SetTitleSize(0.05);
    frame_etadistrib_comparison[i]->GetYaxis()->SetTitleSize(0.05);
    frame_etadistrib_comparison[i]->GetXaxis()->SetLabelSize(0.05);
    frame_etadistrib_comparison[i]->GetYaxis()->SetLabelSize(0.05);
    frame_etadistrib_comparison[i]->GetXaxis()->SetTitleOffset(1.3); 
    frame_etadistrib_comparison[i]->GetYaxis()->SetTitleOffset(1.5);
    frame_etadistrib_comparison[i]->GetXaxis()->SetRangeUser(-4,-2.5);
    //frame_etadistrib_comparison->GetYaxis()->SetRangeUser(1e-5,1e-1);//1,1e8);
    frame_etadistrib_comparison[i]->Draw("");
    
    hEta_matchedMchMid_data->SetTitle(Form("#eta distribution - run %i",run));
    hEta_matchedMchMid_data->GetXaxis()->SetTitle("#eta");
    hEta_matchedMchMid_data->GetYaxis()->SetTitle("Normalized counts");
    hEta_matchedMchMid_data->GetXaxis()->SetTitleSize(0.05);
    hEta_matchedMchMid_data->GetYaxis()->SetTitleSize(0.05);
    hEta_matchedMchMid_data->GetXaxis()->SetLabelSize(0.05);
    hEta_matchedMchMid_data->GetYaxis()->SetLabelSize(0.05);
    hEta_matchedMchMid_data->GetXaxis()->SetTitleOffset(1.3);
    hEta_matchedMchMid_data->GetYaxis()->SetTitleOffset(1.5);
    hEta_matchedMchMid_data->GetXaxis()->SetRangeUser(-4,-2.5);
    //hEta_matchedMchMid_data->GetYaxis()->SetRangeUser(1e-3,1e-2);//1,1e8);
    hEta_matchedMchMid_data->SetLineColor(kRed+2);
    hEta_matchedMchMid_data->SetMarkerColor(kRed+2);
    hEta_matchedMchMid_data->SetMarkerStyle(8);
    hEta_matchedMchMid_data->SetMarkerSize(1.5);
    //hEta_matchedMchMid_data->SetLineWidth(2);
    hEta_matchedMchMid_data->Draw("EP");
    hEta_matchedMchMid_MC->SetLineColor(kBlue+2);
    hEta_matchedMchMid_MC->SetMarkerColor(kBlue+2);
    hEta_matchedMchMid_MC->SetMarkerStyle(8);
    hEta_matchedMchMid_MC->SetMarkerSize(1.5);
    //hEta_matchedMchMid_MC->SetLineWidth(2);
    hEta_matchedMchMid_MC->Draw("EPSAME");
    
    leg_etadistrib_comparison[i] = new TLegend(0.18,0.78,0.4,0.88);
    //leg_etadistrib_comparison[i]->SetHeader("Matched Tracks MCH-MID","");  
    leg_etadistrib_comparison[i]->SetTextSize(0.03);
    leg_etadistrib_comparison[i]->SetTextFont(42);
    leg_etadistrib_comparison[i]->SetBorderSize(0);
    leg_etadistrib_comparison[i]->AddEntry(hEta_matchedMchMid_data,Form("%s %s (Data)",period.c_str(),apass.c_str() ),"lp");
    leg_etadistrib_comparison[i]->AddEntry(hEta_matchedMchMid_MC,Form("%s (MC)",MC_prod.c_str() ),"lp");
    leg_etadistrib_comparison[i]->Draw("SAME");
    lat_cuts.DrawLatexNDC(0.56,0.85,"#bf{Selections}");
    lat_cuts.DrawLatexNDC(0.56,0.81,"|z| < 10 cm");
    lat_cuts.DrawLatexNDC(0.56,0.77,"MuonQualityCuts");
    lat_cuts.DrawLatexNDC(0.56,0.73,"Matched Tracks MCH-MID");
   }
   else{
    frame_etadistrib_comparison[i] = new TH1F("frame_ratio_etadistrib","",1,-4,-2.5); 
    frame_etadistrib_comparison[i]->SetTitle(Form("Ratio MC/data #eta distribution - run %i",run));
    frame_etadistrib_comparison[i]->GetXaxis()->SetTitle("#eta");
    frame_etadistrib_comparison[i]->GetYaxis()->SetTitle("MC/data");
    frame_etadistrib_comparison[i]->GetXaxis()->SetTitleSize(0.05);
    frame_etadistrib_comparison[i]->GetYaxis()->SetTitleSize(0.05);
    frame_etadistrib_comparison[i]->GetXaxis()->SetLabelSize(0.05);
    frame_etadistrib_comparison[i]->GetYaxis()->SetLabelSize(0.05);
    frame_etadistrib_comparison[i]->GetXaxis()->SetTitleOffset(1.3); 
    frame_etadistrib_comparison[i]->GetYaxis()->SetTitleOffset(1.3);
    frame_etadistrib_comparison[i]->GetXaxis()->SetRangeUser(-4,-2.5);
    frame_etadistrib_comparison[i]->GetYaxis()->SetRangeUser(0,2);
    frame_etadistrib_comparison[i]->Draw("");
   
    hRatio_Eta_matchedMchMid_MC_data->SetTitle(Form("Ratio MC/data #eta distribution - run %i",run));
    hRatio_Eta_matchedMchMid_MC_data->GetXaxis()->SetTitle("#eta");
    hRatio_Eta_matchedMchMid_MC_data->GetYaxis()->SetTitle("MC/data");
    hRatio_Eta_matchedMchMid_MC_data->GetXaxis()->SetTitleSize(0.05);
    hRatio_Eta_matchedMchMid_MC_data->GetYaxis()->SetTitleSize(0.05);
    hRatio_Eta_matchedMchMid_MC_data->GetXaxis()->SetLabelSize(0.05);
    hRatio_Eta_matchedMchMid_MC_data->GetYaxis()->SetLabelSize(0.05);
    hRatio_Eta_matchedMchMid_MC_data->GetXaxis()->SetTitleOffset(1.3);
    hRatio_Eta_matchedMchMid_MC_data->GetYaxis()->SetTitleOffset(1.3);
    hRatio_Eta_matchedMchMid_MC_data->GetXaxis()->SetRangeUser(-4,-2.5);
    hRatio_Eta_matchedMchMid_MC_data->GetYaxis()->SetRangeUser(0,2);
    hRatio_Eta_matchedMchMid_MC_data->SetLineColor(kBlack);
    hRatio_Eta_matchedMchMid_MC_data->SetMarkerColor(kBlack);
    hRatio_Eta_matchedMchMid_MC_data->SetMarkerStyle(8);
    hRatio_Eta_matchedMchMid_MC_data->SetMarkerSize(1.5);
    //hRatio_Eta_matchedMchMid_MC_data->SetLineWidth(2);
    hRatio_Eta_matchedMchMid_MC_data->Draw("EP");
    line_etadistrib_comparison = new TLine(-4,1,-2.5,1);
    line_etadistrib_comparison->SetLineColor(kGreen+3);
    line_etadistrib_comparison->Draw("L");
   }
  }
  
  //-- draw Phi distrib Matched tracks MCH-MID + ratios
  TCanvas *can_phidistrib_comparison;
  TLegend *leg_phidistrib_comparison[2];
  TLine *line_phidistrib_comparison;
  TH1F *frame_phidistrib_comparison[2];
  TPad* pad_phidistrib_comparison[2];
  
  can_phidistrib_comparison = new TCanvas("Phidistrib_comparison_data_MC","Phidistrib_comparison_data_MC",1600,800);
  can_phidistrib_comparison->Divide(2,1); 
  for(int i=0;i<2;i++){
   can_phidistrib_comparison->cd(i+1);
   pad_phidistrib_comparison[i] = new TPad("padphidistrib_comparison","",0,0,1,1);
   pad_phidistrib_comparison[i]->SetMargin(0.15,0.1,0.15,0.1); //sx,dx,basso,alto
   pad_phidistrib_comparison[i]->Draw();
   can_phidistrib_comparison->cd(i+1);
   pad_phidistrib_comparison[i]->cd();
   //pad_phidistrib_comparison[i]->SetLogz();
   
   if(i==0){
    pad_phidistrib_comparison[i]->SetLogy(); 
    frame_phidistrib_comparison[i] = new TH1F("frame_phidistrib_comparison","",1,-2.5,2.5); 
    frame_phidistrib_comparison[i]->SetTitle(Form("#phi distribution - run %i",run));
    frame_phidistrib_comparison[i]->GetXaxis()->SetTitle("#phi (rad.)");
    frame_phidistrib_comparison[i]->GetYaxis()->SetTitle("Normalized counts");
    frame_phidistrib_comparison[i]->GetXaxis()->SetTitleSize(0.05);
    frame_phidistrib_comparison[i]->GetYaxis()->SetTitleSize(0.05);
    frame_phidistrib_comparison[i]->GetXaxis()->SetLabelSize(0.05);
    frame_phidistrib_comparison[i]->GetYaxis()->SetLabelSize(0.05);
    frame_phidistrib_comparison[i]->GetXaxis()->SetTitleOffset(1.3); 
    frame_phidistrib_comparison[i]->GetYaxis()->SetTitleOffset(1.5);
    frame_phidistrib_comparison[i]->GetXaxis()->SetRangeUser(-2.5,2.5);
    frame_phidistrib_comparison[i]->Draw("");
    
    hPhi_matchedMchMid_data->SetTitle(Form("#phi distribution - run %i",run));
    hPhi_matchedMchMid_data->GetXaxis()->SetTitle("#phi (rad.)");
    hPhi_matchedMchMid_data->GetYaxis()->SetTitle("Normalized counts");
    hPhi_matchedMchMid_data->GetXaxis()->SetTitleSize(0.05);
    hPhi_matchedMchMid_data->GetYaxis()->SetTitleSize(0.05);
    hPhi_matchedMchMid_data->GetXaxis()->SetLabelSize(0.05);
    hPhi_matchedMchMid_data->GetYaxis()->SetLabelSize(0.05);
    hPhi_matchedMchMid_data->GetXaxis()->SetTitleOffset(1.3);
    hPhi_matchedMchMid_data->GetYaxis()->SetTitleOffset(1.5);
    hPhi_matchedMchMid_data->GetXaxis()->SetRangeUser(-2.5,2.5);
    hPhi_matchedMchMid_data->SetLineColor(kRed+2);
    hPhi_matchedMchMid_data->SetMarkerColor(kRed+2);
    hPhi_matchedMchMid_data->SetMarkerStyle(8);
    hPhi_matchedMchMid_data->SetMarkerSize(1.5);
    //hPhi_matchedMchMid_data->SetLineWidth(2);
    hPhi_matchedMchMid_data->Draw("EP");
    hPhi_matchedMchMid_MC->SetLineColor(kBlue+2);
    hPhi_matchedMchMid_MC->SetMarkerColor(kBlue+2);
    hPhi_matchedMchMid_MC->SetMarkerStyle(8);
    hPhi_matchedMchMid_MC->SetMarkerSize(1.5);
    //hPhi_matchedMchMid_MC->SetLineWidth(2);
    hPhi_matchedMchMid_MC->Draw("EPSAME");
    
    leg_phidistrib_comparison[i] = new TLegend(0.18,0.78,0.4,0.88);//(0.2,0.75,0.8,0.88);
    //leg_phidistrib_comparison[i]->SetHeader("Matched Tracks MCH-MID","");  
    leg_phidistrib_comparison[i]->SetTextSize(0.03);
    leg_phidistrib_comparison[i]->SetTextFont(42);
    leg_phidistrib_comparison[i]->SetBorderSize(0);
    leg_phidistrib_comparison[i]->AddEntry(hPhi_matchedMchMid_data,Form("%s %s (Data)",period.c_str(),apass.c_str() ),"lp");
    leg_phidistrib_comparison[i]->AddEntry(hPhi_matchedMchMid_MC,Form("%s (MC)",MC_prod.c_str() ),"lp");
    leg_phidistrib_comparison[i]->Draw("SAME");
    lat_cuts.DrawLatexNDC(0.56,0.85,"#bf{Selections}");
    lat_cuts.DrawLatexNDC(0.56,0.81,"|z| < 10 cm");
    lat_cuts.DrawLatexNDC(0.56,0.77,"MuonQualityCuts");
    lat_cuts.DrawLatexNDC(0.56,0.73,"Matched Tracks MCH-MID");
   }
   else{
    frame_phidistrib_comparison[i] = new TH1F("frame_ratio_phidistrib","",1,-2.5,2.5); 
    frame_phidistrib_comparison[i]->SetTitle(Form("Ratio MC/data #phi distribution - run %i",run));
    frame_phidistrib_comparison[i]->GetXaxis()->SetTitle("#phi (rad.)");
    frame_phidistrib_comparison[i]->GetYaxis()->SetTitle("MC/data");
    frame_phidistrib_comparison[i]->GetXaxis()->SetTitleSize(0.05);
    frame_phidistrib_comparison[i]->GetYaxis()->SetTitleSize(0.05);
    frame_phidistrib_comparison[i]->GetXaxis()->SetLabelSize(0.05);
    frame_phidistrib_comparison[i]->GetYaxis()->SetLabelSize(0.05);
    frame_phidistrib_comparison[i]->GetXaxis()->SetTitleOffset(1.3); 
    frame_phidistrib_comparison[i]->GetYaxis()->SetTitleOffset(1.3);
    frame_phidistrib_comparison[i]->GetXaxis()->SetRangeUser(-2.5,2.5);
    frame_phidistrib_comparison[i]->GetYaxis()->SetRangeUser(0,2);
    frame_phidistrib_comparison[i]->Draw("");
   
    hRatio_Phi_matchedMchMid_MC_data->SetTitle(Form("Ratio MC/data #phi distribution - run %i",run));
    hRatio_Phi_matchedMchMid_MC_data->GetXaxis()->SetTitle("#phi (rad.)");
    hRatio_Phi_matchedMchMid_MC_data->GetYaxis()->SetTitle("MC/data");
    hRatio_Phi_matchedMchMid_MC_data->GetXaxis()->SetTitleSize(0.05);
    hRatio_Phi_matchedMchMid_MC_data->GetYaxis()->SetTitleSize(0.05);
    hRatio_Phi_matchedMchMid_MC_data->GetXaxis()->SetLabelSize(0.05);
    hRatio_Phi_matchedMchMid_MC_data->GetYaxis()->SetLabelSize(0.05);
    hRatio_Phi_matchedMchMid_MC_data->GetXaxis()->SetTitleOffset(1.3);
    hRatio_Phi_matchedMchMid_MC_data->GetYaxis()->SetTitleOffset(1.3);
    hRatio_Phi_matchedMchMid_MC_data->GetXaxis()->SetRangeUser(-2.5,2.5);
    hRatio_Phi_matchedMchMid_MC_data->GetYaxis()->SetRangeUser(0,2);
    hRatio_Phi_matchedMchMid_MC_data->SetLineColor(kBlack);
    hRatio_Phi_matchedMchMid_MC_data->SetMarkerColor(kBlack);
    hRatio_Phi_matchedMchMid_MC_data->SetMarkerStyle(8);
    hRatio_Phi_matchedMchMid_MC_data->SetMarkerSize(1.5);
    //hRatio_Phi_matchedMchMid_MC_data->SetLineWidth(2);
    hRatio_Phi_matchedMchMid_MC_data->Draw("EP");
    line_phidistrib_comparison = new TLine(-2.5,1,2.5,1);
    line_phidistrib_comparison->SetLineColor(kGreen+3);
    line_phidistrib_comparison->Draw("L");
  
   }
  }
  
   
  //-- draw PT distrib Matched tracks MCH-MID + ratios
  TCanvas *can_ptdistrib_comparison;
  TLegend *leg_ptdistrib_comparison[2];
  TLine *line_ptdistrib_comparison;
  TH1F *frame_ptdistrib_comparison[2];
  TPad* pad_ptdistrib_comparison[2];
  
  can_ptdistrib_comparison = new TCanvas("Ptdistrib_comparison_data_MC","Ptdistrib_comparison_data_MC",1600,800);
  can_ptdistrib_comparison->Divide(2,1); 
  for(int i=0;i<2;i++){
   can_ptdistrib_comparison->cd(i+1);
   pad_ptdistrib_comparison[i] = new TPad("padptdistrib_comparison","",0,0,1,1);
   pad_ptdistrib_comparison[i]->SetMargin(0.15,0.1,0.15,0.1); //sx,dx,basso,alto
   pad_ptdistrib_comparison[i]->Draw();
   can_ptdistrib_comparison->cd(i+1);
   pad_ptdistrib_comparison[i]->cd();
   //pad_ptdistrib_comparison[i]->SetLogz();
   
   if(i==0){
    pad_ptdistrib_comparison[i]->SetLogy(); 
    frame_ptdistrib_comparison[i] = new TH1F("frame_ptdistrib_comparison","",1,0,5); 
    frame_ptdistrib_comparison[i]->SetTitle(Form("#it{p}_{T} distribution - run %i",run));
    frame_ptdistrib_comparison[i]->GetXaxis()->SetTitle("#it{p}_{T} (GeV/#it{c})");
    frame_ptdistrib_comparison[i]->GetYaxis()->SetTitle("Normalized counts");
    frame_ptdistrib_comparison[i]->GetXaxis()->SetTitleSize(0.05);
    frame_ptdistrib_comparison[i]->GetYaxis()->SetTitleSize(0.05);
    frame_ptdistrib_comparison[i]->GetXaxis()->SetLabelSize(0.05);
    frame_ptdistrib_comparison[i]->GetYaxis()->SetLabelSize(0.05);
    frame_ptdistrib_comparison[i]->GetXaxis()->SetTitleOffset(1.3); 
    frame_ptdistrib_comparison[i]->GetYaxis()->SetTitleOffset(1.5);
    frame_ptdistrib_comparison[i]->GetXaxis()->SetRangeUser(0,5);
    frame_ptdistrib_comparison[i]->Draw("");
    
    hPt_matchedMchMid_data->SetTitle(Form("#it{p}_{T} distribution - run %i",run));
    hPt_matchedMchMid_data->GetXaxis()->SetTitle("#it{p}_{T} (GeV/#it{c})");
    hPt_matchedMchMid_data->GetYaxis()->SetTitle("Normalized counts");
    hPt_matchedMchMid_data->GetXaxis()->SetTitleSize(0.05);
    hPt_matchedMchMid_data->GetYaxis()->SetTitleSize(0.05);
    hPt_matchedMchMid_data->GetXaxis()->SetLabelSize(0.05);
    hPt_matchedMchMid_data->GetYaxis()->SetLabelSize(0.05);
    hPt_matchedMchMid_data->GetXaxis()->SetTitleOffset(1.3);
    hPt_matchedMchMid_data->GetYaxis()->SetTitleOffset(1.5);
    hPt_matchedMchMid_data->GetXaxis()->SetRangeUser(0,5);
    hPt_matchedMchMid_data->SetLineColor(kRed+2);
    hPt_matchedMchMid_data->SetMarkerColor(kRed+2);
    hPt_matchedMchMid_data->SetMarkerStyle(8);
    hPt_matchedMchMid_data->SetMarkerSize(1.5);
    //hPt_matchedMchMid_data->SetLineWidth(2);
    hPt_matchedMchMid_data->Draw("EP");
    hPt_matchedMchMid_MC->SetLineColor(kBlue+2);
    hPt_matchedMchMid_MC->SetMarkerColor(kBlue+2);
    hPt_matchedMchMid_MC->SetMarkerStyle(8);
    hPt_matchedMchMid_MC->SetMarkerSize(1.5);
    //hPt_matchedMchMid_MC->SetLineWidth(2);
    hPt_matchedMchMid_MC->Draw("EPSAME");
    
    leg_ptdistrib_comparison[i] = new TLegend(0.18,0.78,0.4,0.88);//(0.2,0.75,0.8,0.88);
    //leg_ptdistrib_comparison[i]->SetHeader("Matched Tracks MCH-MID","");  
    leg_ptdistrib_comparison[i]->SetTextSize(0.03);
    leg_ptdistrib_comparison[i]->SetTextFont(42);
    leg_ptdistrib_comparison[i]->SetBorderSize(0);
    leg_ptdistrib_comparison[i]->AddEntry(hPt_matchedMchMid_data,Form("%s %s (Data)",period.c_str(),apass.c_str() ),"lp");
    leg_ptdistrib_comparison[i]->AddEntry(hPt_matchedMchMid_MC,Form("%s (MC)",MC_prod.c_str() ),"lp");
    leg_ptdistrib_comparison[i]->Draw("SAME");
    lat_cuts.DrawLatexNDC(0.56,0.85,"#bf{Selections}");
    lat_cuts.DrawLatexNDC(0.56,0.81,"|z| < 10 cm");
    lat_cuts.DrawLatexNDC(0.56,0.77,"MuonQualityCuts");
    lat_cuts.DrawLatexNDC(0.56,0.73,"Matched Tracks MCH-MID");
   }
   else{
    frame_ptdistrib_comparison[i] = new TH1F("frame_ratio_ptdistrib","",1,0,5); 
    frame_ptdistrib_comparison[i]->SetTitle(Form("Ratio MC/data #it{p}_{T} distribution - run %i",run));
    frame_ptdistrib_comparison[i]->GetXaxis()->SetTitle("#it{p}_{T} (GeV/#it{c})");
    frame_ptdistrib_comparison[i]->GetYaxis()->SetTitle("MC/data");
    frame_ptdistrib_comparison[i]->GetXaxis()->SetTitleSize(0.05);
    frame_ptdistrib_comparison[i]->GetYaxis()->SetTitleSize(0.05);
    frame_ptdistrib_comparison[i]->GetXaxis()->SetLabelSize(0.05);
    frame_ptdistrib_comparison[i]->GetYaxis()->SetLabelSize(0.05);
    frame_ptdistrib_comparison[i]->GetXaxis()->SetTitleOffset(1.3); 
    frame_ptdistrib_comparison[i]->GetYaxis()->SetTitleOffset(1.3);
    frame_ptdistrib_comparison[i]->GetXaxis()->SetRangeUser(0,5);
    frame_ptdistrib_comparison[i]->GetYaxis()->SetRangeUser(0,2);
    frame_ptdistrib_comparison[i]->Draw("");
   
    hRatio_Pt_matchedMchMid_MC_data->SetTitle(Form("Ratio MC/data #it{p}_{T} distribution - run %i",run));
    hRatio_Pt_matchedMchMid_MC_data->GetXaxis()->SetTitle("#it{p}_{T} (GeV/#it{c})");
    hRatio_Pt_matchedMchMid_MC_data->GetYaxis()->SetTitle("MC/data");
    hRatio_Pt_matchedMchMid_MC_data->GetXaxis()->SetTitleSize(0.05);
    hRatio_Pt_matchedMchMid_MC_data->GetYaxis()->SetTitleSize(0.05);
    hRatio_Pt_matchedMchMid_MC_data->GetXaxis()->SetLabelSize(0.05);
    hRatio_Pt_matchedMchMid_MC_data->GetYaxis()->SetLabelSize(0.05);
    hRatio_Pt_matchedMchMid_MC_data->GetXaxis()->SetTitleOffset(1.3);
    hRatio_Pt_matchedMchMid_MC_data->GetYaxis()->SetTitleOffset(1.3);
    hRatio_Pt_matchedMchMid_MC_data->GetXaxis()->SetRangeUser(0,5);
    hRatio_Pt_matchedMchMid_MC_data->GetYaxis()->SetRangeUser(0,2);
    hRatio_Pt_matchedMchMid_MC_data->SetLineColor(kBlack);
    hRatio_Pt_matchedMchMid_MC_data->SetMarkerColor(kBlack);
    hRatio_Pt_matchedMchMid_MC_data->SetMarkerStyle(8);
    hRatio_Pt_matchedMchMid_MC_data->SetMarkerSize(1.5);
    //hRatio_Pt_matchedMchMid_MC_data->SetLineWidth(2);
    hRatio_Pt_matchedMchMid_MC_data->Draw("EP");
    line_ptdistrib_comparison = new TLine(0,1,15,1);
    line_ptdistrib_comparison->SetLineColor(kGreen+3);
    line_ptdistrib_comparison->Draw("L");
   }
  }
  
  
  
  //-- draw Eta distrib Matched tracks MCH-MID + pt >0.7 + ratios 
  TCanvas *can_etadistrib_comparison_ptsel;
  TLegend *leg_etadistrib_comparison_ptsel[2];
  TLine *line_etadistrib_comparison_ptsel;
  TH1F *frame_etadistrib_comparison_ptsel[2];
  TPad* pad_etadistrib_comparison_ptsel[2];
  
  can_etadistrib_comparison_ptsel = new TCanvas("Etadistrib_comparison_ptsel_data_MC","Etadistrib_comparison_ptsel_data_MC",1600,800);
  can_etadistrib_comparison_ptsel->Divide(2,1); 
  for(int i=0;i<2;i++){
   can_etadistrib_comparison_ptsel->cd(i+1);
   pad_etadistrib_comparison_ptsel[i] = new TPad("padetadistrib_comparison_ptsel","",0,0,1,1);
   pad_etadistrib_comparison_ptsel[i]->SetMargin(0.15,0.1,0.15,0.1); //sx,dx,basso,alto
   pad_etadistrib_comparison_ptsel[i]->Draw();
   can_etadistrib_comparison_ptsel->cd(i+1);
   pad_etadistrib_comparison_ptsel[i]->cd();
   //pad_etadistrib_comparison_ptsel[i]->SetLogz();
   
   if(i==0){
    pad_etadistrib_comparison_ptsel[i]->SetLogy(); 
    frame_etadistrib_comparison_ptsel[i] = new TH1F("frame_etadistrib_comparison_ptsel","",1,-4,-2.5); 
    frame_etadistrib_comparison_ptsel[i]->SetTitle(Form("#eta distribution - run %i",run));
    frame_etadistrib_comparison_ptsel[i]->GetXaxis()->SetTitle("#eta");
    frame_etadistrib_comparison_ptsel[i]->GetYaxis()->SetTitle("Normalized counts");
    frame_etadistrib_comparison_ptsel[i]->GetXaxis()->SetTitleSize(0.05);
    frame_etadistrib_comparison_ptsel[i]->GetYaxis()->SetTitleSize(0.05);
    frame_etadistrib_comparison_ptsel[i]->GetXaxis()->SetLabelSize(0.05);
    frame_etadistrib_comparison_ptsel[i]->GetYaxis()->SetLabelSize(0.05);
    frame_etadistrib_comparison_ptsel[i]->GetXaxis()->SetTitleOffset(1.3); 
    frame_etadistrib_comparison_ptsel[i]->GetYaxis()->SetTitleOffset(1.5);
    frame_etadistrib_comparison_ptsel[i]->GetXaxis()->SetRangeUser(-4,-2.5);
    //frame_etadistrib_comparison_ptsel->GetYaxis()->SetRangeUser(1e-5,1e-1);//1,1e8);
    frame_etadistrib_comparison_ptsel[i]->Draw("");
    
    hEta_matchedMchMid_ptsel_data->SetTitle(Form("#eta distribution - run %i",run));
    hEta_matchedMchMid_ptsel_data->GetXaxis()->SetTitle("#eta");
    hEta_matchedMchMid_ptsel_data->GetYaxis()->SetTitle("Normalized counts");
    hEta_matchedMchMid_ptsel_data->GetXaxis()->SetTitleSize(0.05);
    hEta_matchedMchMid_ptsel_data->GetYaxis()->SetTitleSize(0.05);
    hEta_matchedMchMid_ptsel_data->GetXaxis()->SetLabelSize(0.05);
    hEta_matchedMchMid_ptsel_data->GetYaxis()->SetLabelSize(0.05);
    hEta_matchedMchMid_ptsel_data->GetXaxis()->SetTitleOffset(1.3);
    hEta_matchedMchMid_ptsel_data->GetYaxis()->SetTitleOffset(1.5);
    hEta_matchedMchMid_ptsel_data->GetXaxis()->SetRangeUser(-4,-2.5);
    //hEta_matchedMchMid_ptsel_data->GetYaxis()->SetRangeUser(1e-5,1e-1);//1,1e8);
    hEta_matchedMchMid_ptsel_data->SetLineColor(kRed+2);
    hEta_matchedMchMid_ptsel_data->SetMarkerColor(kRed+2);
    hEta_matchedMchMid_ptsel_data->SetMarkerStyle(8);
    hEta_matchedMchMid_ptsel_data->SetMarkerSize(1.5);
    //hEta_matchedMchMid_ptsel_data->SetLineWidth(2);
    hEta_matchedMchMid_ptsel_data->Draw("EP");
    hEta_matchedMchMid_ptsel_MC->SetLineColor(kBlue+2);
    hEta_matchedMchMid_ptsel_MC->SetMarkerColor(kBlue+2);
    hEta_matchedMchMid_ptsel_MC->SetMarkerStyle(8);
    hEta_matchedMchMid_ptsel_MC->SetMarkerSize(1.5);
    //hEta_matchedMchMid_ptsel_MC->SetLineWidth(2);
    hEta_matchedMchMid_ptsel_MC->Draw("EPSAME");
    
    leg_etadistrib_comparison_ptsel[i] = new TLegend(0.18,0.78,0.4,0.88);//(0.2,0.75,0.8,0.88);
    //leg_etadistrib_comparison_ptsel[i]->SetHeader("Matched Tracks MCH-MID","");  
    leg_etadistrib_comparison_ptsel[i]->SetTextSize(0.03);
    leg_etadistrib_comparison_ptsel[i]->SetTextFont(42);
    leg_etadistrib_comparison_ptsel[i]->SetBorderSize(0);
    leg_etadistrib_comparison_ptsel[i]->AddEntry(hEta_matchedMchMid_ptsel_data,Form("%s %s (Data)",period.c_str(),apass.c_str() ),"lp");
    leg_etadistrib_comparison_ptsel[i]->AddEntry(hEta_matchedMchMid_ptsel_MC,Form("%s (MC)",MC_prod.c_str() ),"lp");
    leg_etadistrib_comparison_ptsel[i]->Draw("SAME");
    lat_cuts_ptsel.DrawLatexNDC(0.18,0.75,"#bf{Selections}");
    lat_cuts_ptsel.DrawLatexNDC(0.18,0.71,"|z| < 10 cm");
    lat_cuts_ptsel.DrawLatexNDC(0.18,0.67,"MuonQualityCuts");
    lat_cuts_ptsel.DrawLatexNDC(0.18,0.63,"Matched Tracks MCH-MID");
    lat_cuts_ptsel.DrawLatexNDC(0.18,0.59,"#it{p}_{T} > 0.7 GeV/#it{c}");
   }
   else{
    frame_etadistrib_comparison_ptsel[i] = new TH1F("frame_ratio_etadistrib_ptsel","",1,-4,-2.5); 
    frame_etadistrib_comparison_ptsel[i]->SetTitle(Form("Ratio MC/data #eta distribution - run %i",run));
    frame_etadistrib_comparison_ptsel[i]->GetXaxis()->SetTitle("#eta");
    frame_etadistrib_comparison_ptsel[i]->GetYaxis()->SetTitle("MC/data");
    frame_etadistrib_comparison_ptsel[i]->GetXaxis()->SetTitleSize(0.05);
    frame_etadistrib_comparison_ptsel[i]->GetYaxis()->SetTitleSize(0.05);
    frame_etadistrib_comparison_ptsel[i]->GetXaxis()->SetLabelSize(0.05);
    frame_etadistrib_comparison_ptsel[i]->GetYaxis()->SetLabelSize(0.05);
    frame_etadistrib_comparison_ptsel[i]->GetXaxis()->SetTitleOffset(1.3); 
    frame_etadistrib_comparison_ptsel[i]->GetYaxis()->SetTitleOffset(1.3);
    frame_etadistrib_comparison_ptsel[i]->GetXaxis()->SetRangeUser(-4,-2.5);
    frame_etadistrib_comparison_ptsel[i]->GetYaxis()->SetRangeUser(0,2);
    frame_etadistrib_comparison_ptsel[i]->Draw("");
   
    hRatio_Eta_matchedMchMid_ptsel_MC_data->SetTitle(Form("Ratio MC/data #eta distribution - run %i",run));
    hRatio_Eta_matchedMchMid_ptsel_MC_data->GetXaxis()->SetTitle("#eta");
    hRatio_Eta_matchedMchMid_ptsel_MC_data->GetYaxis()->SetTitle("MC/data");
    hRatio_Eta_matchedMchMid_ptsel_MC_data->GetXaxis()->SetTitleSize(0.05);
    hRatio_Eta_matchedMchMid_ptsel_MC_data->GetYaxis()->SetTitleSize(0.05);
    hRatio_Eta_matchedMchMid_ptsel_MC_data->GetXaxis()->SetLabelSize(0.05);
    hRatio_Eta_matchedMchMid_ptsel_MC_data->GetYaxis()->SetLabelSize(0.05);
    hRatio_Eta_matchedMchMid_ptsel_MC_data->GetXaxis()->SetTitleOffset(1.3);
    hRatio_Eta_matchedMchMid_ptsel_MC_data->GetYaxis()->SetTitleOffset(1.3);
    hRatio_Eta_matchedMchMid_ptsel_MC_data->GetXaxis()->SetRangeUser(-4,-2.5);
    hRatio_Eta_matchedMchMid_ptsel_MC_data->GetYaxis()->SetRangeUser(0,2);
    hRatio_Eta_matchedMchMid_ptsel_MC_data->SetLineColor(kBlack);
    hRatio_Eta_matchedMchMid_ptsel_MC_data->SetMarkerColor(kBlack);
    hRatio_Eta_matchedMchMid_ptsel_MC_data->SetMarkerStyle(8);
    hRatio_Eta_matchedMchMid_ptsel_MC_data->SetMarkerSize(1.5);
    //hRatio_Eta_matchedMchMid_ptsel_MC_data->SetLineWidth(2);
    hRatio_Eta_matchedMchMid_ptsel_MC_data->Draw("EP");
    line_etadistrib_comparison_ptsel = new TLine(-4,1,-2.5,1);
    line_etadistrib_comparison_ptsel->SetLineColor(kGreen+3);
    line_etadistrib_comparison_ptsel->Draw("L");
   }
  }
  
  //-- draw Phi distrib Matched tracks MCH-MID + pt >0.7 + ratios
  TCanvas *can_phidistrib_comparison_ptsel;
  TLegend *leg_phidistrib_comparison_ptsel[2];
  TLine *line_phidistrib_comparison_ptsel;
  TH1F *frame_phidistrib_comparison_ptsel[2];
  TPad* pad_phidistrib_comparison_ptsel[2];
  
  can_phidistrib_comparison_ptsel = new TCanvas("Phidistrib_comparison_ptsel_data_MC","Phidistrib_comparison_ptsel_data_MC",1600,800);
  can_phidistrib_comparison_ptsel->Divide(2,1); 
  for(int i=0;i<2;i++){
   can_phidistrib_comparison_ptsel->cd(i+1);
   pad_phidistrib_comparison_ptsel[i] = new TPad("padphidistrib_comparison_ptsel","",0,0,1,1);
   pad_phidistrib_comparison_ptsel[i]->SetMargin(0.15,0.1,0.15,0.1); //sx,dx,basso,alto
   pad_phidistrib_comparison_ptsel[i]->Draw();
   can_phidistrib_comparison_ptsel->cd(i+1);
   pad_phidistrib_comparison_ptsel[i]->cd();
   //pad_phidistrib_comparison_ptsel[i]->SetLogz();
   
   if(i==0){
    pad_phidistrib_comparison_ptsel[i]->SetLogy(); 
    frame_phidistrib_comparison_ptsel[i] = new TH1F("frame_phidistrib_comparison_ptsel","",1,-2.5,2.5); 
    frame_phidistrib_comparison_ptsel[i]->SetTitle(Form("#phi distribution - run %i",run));
    frame_phidistrib_comparison_ptsel[i]->GetXaxis()->SetTitle("#phi (rad.)");
    frame_phidistrib_comparison_ptsel[i]->GetYaxis()->SetTitle("Normalized counts");
    frame_phidistrib_comparison_ptsel[i]->GetXaxis()->SetTitleSize(0.05);
    frame_phidistrib_comparison_ptsel[i]->GetYaxis()->SetTitleSize(0.05);
    frame_phidistrib_comparison_ptsel[i]->GetXaxis()->SetLabelSize(0.05);
    frame_phidistrib_comparison_ptsel[i]->GetYaxis()->SetLabelSize(0.05);
    frame_phidistrib_comparison_ptsel[i]->GetXaxis()->SetTitleOffset(1.3); 
    frame_phidistrib_comparison_ptsel[i]->GetYaxis()->SetTitleOffset(1.5);
    frame_phidistrib_comparison_ptsel[i]->GetXaxis()->SetRangeUser(-2.5,2.5);
    frame_phidistrib_comparison_ptsel[i]->Draw("");
    
    hPhi_matchedMchMid_ptsel_data->SetTitle(Form("#phi distribution - run %i",run));
    hPhi_matchedMchMid_ptsel_data->GetXaxis()->SetTitle("#phi (rad.)");
    hPhi_matchedMchMid_ptsel_data->GetYaxis()->SetTitle("Normalized counts");
    hPhi_matchedMchMid_ptsel_data->GetXaxis()->SetTitleSize(0.05);
    hPhi_matchedMchMid_ptsel_data->GetYaxis()->SetTitleSize(0.05);
    hPhi_matchedMchMid_ptsel_data->GetXaxis()->SetLabelSize(0.05);
    hPhi_matchedMchMid_ptsel_data->GetYaxis()->SetLabelSize(0.05);
    hPhi_matchedMchMid_ptsel_data->GetXaxis()->SetTitleOffset(1.3);
    hPhi_matchedMchMid_ptsel_data->GetYaxis()->SetTitleOffset(1.5);
    hPhi_matchedMchMid_ptsel_data->GetXaxis()->SetRangeUser(-2.5,2.5);
    hPhi_matchedMchMid_ptsel_data->SetLineColor(kRed+2);
    hPhi_matchedMchMid_ptsel_data->SetMarkerColor(kRed+2);
    hPhi_matchedMchMid_ptsel_data->SetMarkerStyle(8);
    hPhi_matchedMchMid_ptsel_data->SetMarkerSize(1.5);
    //hPhi_matchedMchMid_ptsel_data->SetLineWidth(2);
    hPhi_matchedMchMid_ptsel_data->Draw("EP");
    hPhi_matchedMchMid_ptsel_MC->SetLineColor(kBlue+2);
    hPhi_matchedMchMid_ptsel_MC->SetMarkerColor(kBlue+2);
    hPhi_matchedMchMid_ptsel_MC->SetMarkerStyle(8);
    hPhi_matchedMchMid_ptsel_MC->SetMarkerSize(1.5);
    //hPhi_matchedMchMid_ptsel_MC->SetLineWidth(2);
    hPhi_matchedMchMid_ptsel_MC->Draw("EPSAME");
    
    leg_phidistrib_comparison_ptsel[i] = new TLegend(0.18,0.78,0.4,0.88);//(0.2,0.75,0.8,0.88);
    //leg_phidistrib_comparison_ptsel[i]->SetHeader("Matched Tracks MCH-MID, #it{p}_{T} > 0.7 GeV/#it{c}","");  
    leg_phidistrib_comparison_ptsel[i]->SetTextSize(0.03);
    leg_phidistrib_comparison_ptsel[i]->SetTextFont(42);
    leg_phidistrib_comparison_ptsel[i]->SetBorderSize(0);
    leg_phidistrib_comparison_ptsel[i]->AddEntry(hPhi_matchedMchMid_ptsel_data,Form("%s %s (Data)",period.c_str(),apass.c_str() ),"lp");
    leg_phidistrib_comparison_ptsel[i]->AddEntry(hPhi_matchedMchMid_ptsel_MC,Form("%s (MC)",MC_prod.c_str() ),"lp");
    leg_phidistrib_comparison_ptsel[i]->Draw("SAME");
    lat_cuts_ptsel.DrawLatexNDC(0.56,0.85,"#bf{Selections}");
    lat_cuts_ptsel.DrawLatexNDC(0.56,0.81,"|z| < 10 cm");
    lat_cuts_ptsel.DrawLatexNDC(0.56,0.77,"MuonQualityCuts");
    lat_cuts_ptsel.DrawLatexNDC(0.56,0.73,"Matched Tracks MCH-MID");
    lat_cuts_ptsel.DrawLatexNDC(0.56,0.69,"#it{p}_{T} > 0.7 GeV/#it{c}");
   }
   else{
    frame_phidistrib_comparison_ptsel[i] = new TH1F("frame_ratio_phidistrib_ptsel","",1,-2.5,2.5); 
    frame_phidistrib_comparison_ptsel[i]->SetTitle(Form("Ratio MC/data #phi distribution - run %i",run));
    frame_phidistrib_comparison_ptsel[i]->GetXaxis()->SetTitle("#phi (rad.)");
    frame_phidistrib_comparison_ptsel[i]->GetYaxis()->SetTitle("MC/data");
    frame_phidistrib_comparison_ptsel[i]->GetXaxis()->SetTitleSize(0.05);
    frame_phidistrib_comparison_ptsel[i]->GetYaxis()->SetTitleSize(0.05);
    frame_phidistrib_comparison_ptsel[i]->GetXaxis()->SetLabelSize(0.05);
    frame_phidistrib_comparison_ptsel[i]->GetYaxis()->SetLabelSize(0.05);
    frame_phidistrib_comparison_ptsel[i]->GetXaxis()->SetTitleOffset(1.3); 
    frame_phidistrib_comparison_ptsel[i]->GetYaxis()->SetTitleOffset(1.3);
    frame_phidistrib_comparison_ptsel[i]->GetXaxis()->SetRangeUser(-2.5,2.5);
    frame_phidistrib_comparison_ptsel[i]->GetYaxis()->SetRangeUser(0,2);
    frame_phidistrib_comparison_ptsel[i]->Draw("");
   
    hRatio_Phi_matchedMchMid_ptsel_MC_data->SetTitle(Form("Ratio MC/data #phi distribution - run %i",run));
    hRatio_Phi_matchedMchMid_ptsel_MC_data->GetXaxis()->SetTitle("#phi (rad.)");
    hRatio_Phi_matchedMchMid_ptsel_MC_data->GetYaxis()->SetTitle("MC/data");
    hRatio_Phi_matchedMchMid_ptsel_MC_data->GetXaxis()->SetTitleSize(0.05);
    hRatio_Phi_matchedMchMid_ptsel_MC_data->GetYaxis()->SetTitleSize(0.05);
    hRatio_Phi_matchedMchMid_ptsel_MC_data->GetXaxis()->SetLabelSize(0.05);
    hRatio_Phi_matchedMchMid_ptsel_MC_data->GetYaxis()->SetLabelSize(0.05);
    hRatio_Phi_matchedMchMid_ptsel_MC_data->GetXaxis()->SetTitleOffset(1.3);
    hRatio_Phi_matchedMchMid_ptsel_MC_data->GetYaxis()->SetTitleOffset(1.3);
    hRatio_Phi_matchedMchMid_ptsel_MC_data->GetXaxis()->SetRangeUser(-2.5,2.5);
    hRatio_Phi_matchedMchMid_ptsel_MC_data->GetYaxis()->SetRangeUser(0,2);
    hRatio_Phi_matchedMchMid_ptsel_MC_data->SetLineColor(kBlack);
    hRatio_Phi_matchedMchMid_ptsel_MC_data->SetMarkerColor(kBlack);
    hRatio_Phi_matchedMchMid_ptsel_MC_data->SetMarkerStyle(8);
    hRatio_Phi_matchedMchMid_ptsel_MC_data->SetMarkerSize(1.5);
    //hRatio_Phi_matchedMchMid_ptsel_MC_data->SetLineWidth(2);
    hRatio_Phi_matchedMchMid_ptsel_MC_data->Draw("EP");
    line_phidistrib_comparison_ptsel = new TLine(-2.5,1,2.5,1);
    line_phidistrib_comparison_ptsel->SetLineColor(kGreen+3);
    line_phidistrib_comparison_ptsel->Draw("L");
   }
  }
  
   
  //-- draw PT distrib Matched tracks MCH-MID + pt >0.7 + ratios
  TCanvas *can_ptdistrib_comparison_ptsel;
  TLegend *leg_ptdistrib_comparison_ptsel[2];
  TLine *line_ptdistrib_comparison_ptsel;
  TH1F *frame_ptdistrib_comparison_ptsel[2];
  TPad* pad_ptdistrib_comparison_ptsel[2];
  
  can_ptdistrib_comparison_ptsel = new TCanvas("Ptdistrib_comparison_ptsel_data_MC","Ptdistrib_comparison_ptsel_data_MC",1600,800);
  can_ptdistrib_comparison_ptsel->Divide(2,1); 
  for(int i=0;i<2;i++){
   can_ptdistrib_comparison_ptsel->cd(i+1);
   pad_ptdistrib_comparison_ptsel[i] = new TPad("padptdistrib_comparison_ptsel","",0,0,1,1);
   pad_ptdistrib_comparison_ptsel[i]->SetMargin(0.15,0.1,0.15,0.1); //sx,dx,basso,alto
   pad_ptdistrib_comparison_ptsel[i]->Draw();
   can_ptdistrib_comparison_ptsel->cd(i+1);
   pad_ptdistrib_comparison_ptsel[i]->cd();
   //pad_ptdistrib_comparison_ptsel[i]->SetLogz();
   
   if(i==0){
    pad_ptdistrib_comparison_ptsel[i]->SetLogy(); 
    frame_ptdistrib_comparison_ptsel[i] = new TH1F("frame_ptdistrib_comparison_ptsel","",1,0,5); 
    frame_ptdistrib_comparison_ptsel[i]->SetTitle(Form("#it{p}_{T} distribution - run %i",run));
    frame_ptdistrib_comparison_ptsel[i]->GetXaxis()->SetTitle("#it{p}_{T} (GeV/#it{c})");
    frame_ptdistrib_comparison_ptsel[i]->GetYaxis()->SetTitle("Normalized counts");
    frame_ptdistrib_comparison_ptsel[i]->GetXaxis()->SetTitleSize(0.05);
    frame_ptdistrib_comparison_ptsel[i]->GetYaxis()->SetTitleSize(0.05);
    frame_ptdistrib_comparison_ptsel[i]->GetXaxis()->SetLabelSize(0.05);
    frame_ptdistrib_comparison_ptsel[i]->GetYaxis()->SetLabelSize(0.05);
    frame_ptdistrib_comparison_ptsel[i]->GetXaxis()->SetTitleOffset(1.3); 
    frame_ptdistrib_comparison_ptsel[i]->GetYaxis()->SetTitleOffset(1.5);
    frame_ptdistrib_comparison_ptsel[i]->GetXaxis()->SetRangeUser(0,5);
    frame_ptdistrib_comparison_ptsel[i]->Draw("");
    
    hPt_matchedMchMid_ptsel_data->SetTitle(Form("#it{p}_{T} distribution - run %i",run));
    hPt_matchedMchMid_ptsel_data->GetXaxis()->SetTitle("#it{p}_{T} (GeV/#it{c})");
    hPt_matchedMchMid_ptsel_data->GetYaxis()->SetTitle("Normalized counts");
    hPt_matchedMchMid_ptsel_data->GetXaxis()->SetTitleSize(0.05);
    hPt_matchedMchMid_ptsel_data->GetYaxis()->SetTitleSize(0.05);
    hPt_matchedMchMid_ptsel_data->GetXaxis()->SetLabelSize(0.05);
    hPt_matchedMchMid_ptsel_data->GetYaxis()->SetLabelSize(0.05);
    hPt_matchedMchMid_ptsel_data->GetXaxis()->SetTitleOffset(1.3);
    hPt_matchedMchMid_ptsel_data->GetYaxis()->SetTitleOffset(1.5);
    hPt_matchedMchMid_ptsel_data->GetXaxis()->SetRangeUser(0,5);
    hPt_matchedMchMid_ptsel_data->SetLineColor(kRed+2);
    hPt_matchedMchMid_ptsel_data->SetMarkerColor(kRed+2);
    hPt_matchedMchMid_ptsel_data->SetMarkerStyle(8);
    hPt_matchedMchMid_ptsel_data->SetMarkerSize(1.5);
    //hPt_matchedMchMid_ptsel_data->SetLineWidth(2);
    hPt_matchedMchMid_ptsel_data->Draw("EP");
    hPt_matchedMchMid_ptsel_MC->SetLineColor(kBlue+2);
    hPt_matchedMchMid_ptsel_MC->SetMarkerColor(kBlue+2);
    hPt_matchedMchMid_ptsel_MC->SetMarkerStyle(8);
    hPt_matchedMchMid_ptsel_MC->SetMarkerSize(1.5);
    //hPt_matchedMchMid_ptsel_MC->SetLineWidth(2);
    hPt_matchedMchMid_ptsel_MC->Draw("EPSAME");
    
    leg_ptdistrib_comparison_ptsel[i] = new TLegend(0.18,0.78,0.4,0.88);//(0.2,0.75,0.8,0.88);
    //leg_ptdistrib_comparison_ptsel[i]->SetHeader("Matched Tracks MCH-MID, #it{p}_{T} > 0.7 GeV/#it{c}","");  
    leg_ptdistrib_comparison_ptsel[i]->SetTextSize(0.03);
    leg_ptdistrib_comparison_ptsel[i]->SetTextFont(42);
    leg_ptdistrib_comparison_ptsel[i]->SetBorderSize(0);
    leg_ptdistrib_comparison_ptsel[i]->AddEntry(hPt_matchedMchMid_ptsel_data,Form("%s %s (Data)",period.c_str(),apass.c_str() ),"lp");
    leg_ptdistrib_comparison_ptsel[i]->AddEntry(hPt_matchedMchMid_ptsel_MC,Form("%s (MC)",MC_prod.c_str() ),"lp");
    leg_ptdistrib_comparison_ptsel[i]->Draw("SAME");
    lat_cuts_ptsel.DrawLatexNDC(0.56,0.85,"#bf{Selections}");
    lat_cuts_ptsel.DrawLatexNDC(0.56,0.81,"|z| < 10 cm");
    lat_cuts_ptsel.DrawLatexNDC(0.56,0.77,"MuonQualityCuts");
    lat_cuts_ptsel.DrawLatexNDC(0.56,0.73,"Matched Tracks MCH-MID");
    lat_cuts_ptsel.DrawLatexNDC(0.56,0.69,"#it{p}_{T} > 0.7 GeV/#it{c}");
   }
   else{
    frame_ptdistrib_comparison_ptsel[i] = new TH1F("frame_ratio_ptdistrib_ptsel","",1,0,5); 
    frame_ptdistrib_comparison_ptsel[i]->SetTitle(Form("Ratio MC/data #it{p}_{T} distribution - run %i",run));
    frame_ptdistrib_comparison_ptsel[i]->GetXaxis()->SetTitle("#it{p}_{T} (GeV/#it{c})");
    frame_ptdistrib_comparison_ptsel[i]->GetYaxis()->SetTitle("MC/data");
    frame_ptdistrib_comparison_ptsel[i]->GetXaxis()->SetTitleSize(0.05);
    frame_ptdistrib_comparison_ptsel[i]->GetYaxis()->SetTitleSize(0.05);
    frame_ptdistrib_comparison_ptsel[i]->GetXaxis()->SetLabelSize(0.05);
    frame_ptdistrib_comparison_ptsel[i]->GetYaxis()->SetLabelSize(0.05);
    frame_ptdistrib_comparison_ptsel[i]->GetXaxis()->SetTitleOffset(1.3); 
    frame_ptdistrib_comparison_ptsel[i]->GetYaxis()->SetTitleOffset(1.3);
    frame_ptdistrib_comparison_ptsel[i]->GetXaxis()->SetRangeUser(0,5);
    frame_ptdistrib_comparison_ptsel[i]->GetYaxis()->SetRangeUser(0,2);
    frame_ptdistrib_comparison_ptsel[i]->Draw("");
   
    hRatio_Pt_matchedMchMid_ptsel_MC_data->SetTitle(Form("Ratio MC/data #it{p}_{T} distribution - run %i",run));
    hRatio_Pt_matchedMchMid_ptsel_MC_data->GetXaxis()->SetTitle("#it{p}_{T} (GeV/#it{c})");
    hRatio_Pt_matchedMchMid_ptsel_MC_data->GetYaxis()->SetTitle("MC/data");
    hRatio_Pt_matchedMchMid_ptsel_MC_data->GetXaxis()->SetTitleSize(0.05);
    hRatio_Pt_matchedMchMid_ptsel_MC_data->GetYaxis()->SetTitleSize(0.05);
    hRatio_Pt_matchedMchMid_ptsel_MC_data->GetXaxis()->SetLabelSize(0.05);
    hRatio_Pt_matchedMchMid_ptsel_MC_data->GetYaxis()->SetLabelSize(0.05);
    hRatio_Pt_matchedMchMid_ptsel_MC_data->GetXaxis()->SetTitleOffset(1.3);
    hRatio_Pt_matchedMchMid_ptsel_MC_data->GetYaxis()->SetTitleOffset(1.3);
    hRatio_Pt_matchedMchMid_ptsel_MC_data->GetXaxis()->SetRangeUser(0,5);
    hRatio_Pt_matchedMchMid_ptsel_MC_data->GetYaxis()->SetRangeUser(0,2);
    hRatio_Pt_matchedMchMid_ptsel_MC_data->SetLineColor(kBlack);
    hRatio_Pt_matchedMchMid_ptsel_MC_data->SetMarkerColor(kBlack);
    hRatio_Pt_matchedMchMid_ptsel_MC_data->SetMarkerStyle(8);
    hRatio_Pt_matchedMchMid_ptsel_MC_data->SetMarkerSize(1.5);
    //hRatio_Pt_matchedMchMid_ptsel_MC_data->SetLineWidth(2);
    hRatio_Pt_matchedMchMid_ptsel_MC_data->Draw("EP");
    line_ptdistrib_comparison_ptsel = new TLine(0,1,15,1);
    line_ptdistrib_comparison_ptsel->SetLineColor(kGreen+3);
    line_ptdistrib_comparison_ptsel->Draw("L");
   }
  }
  
  /*
  can_etadistrib_comparison->Print(Form("%s/%i/MCdata_comparison_MC_%s_data_%s_%s_run_%i.pdf[",output_dir_name[0],run,MC_prod.c_str(),period.c_str(),apass.c_str(),run));
  can_etadistrib_comparison->Print(Form("%s/%i/MCdata_comparison_MC_%s_data_%s_%s_run_%i.pdf",output_dir_name[0],run,MC_prod.c_str(),period.c_str(),apass.c_str(),run));
  can_phidistrib_comparison->Print(Form("%s/%i/MCdata_comparison_MC_%s_data_%s_%s_run_%i.pdf",output_dir_name[0],run,MC_prod.c_str(),period.c_str(),apass.c_str(),run));
  can_ptdistrib_comparison->Print(Form("%s/%i/MCdata_comparison_MC_%s_data_%s_%s_run_%i.pdf",output_dir_name[0],run,MC_prod.c_str(),period.c_str(),apass.c_str(),run));
  can_etadistrib_comparison_ptsel->Print(Form("%s/%i/MCdata_comparison_MC_%s_data_%s_%s_run_%i.pdf",output_dir_name[0],run,MC_prod.c_str(),period.c_str(),apass.c_str(),run));
  can_phidistrib_comparison_ptsel->Print(Form("%s/%i/MCdata_comparison_MC_%s_data_%s_%s_run_%i.pdf",output_dir_name[0],run,MC_prod.c_str(),period.c_str(),apass.c_str(),run));
  can_ptdistrib_comparison_ptsel->Print(Form("%s/%i/MCdata_comparison_MC_%s_data_%s_%s_run_%i.pdf",output_dir_name[0],run,MC_prod.c_str(),period.c_str(),apass.c_str(),run));
  can_ptdistrib_comparison_ptsel->Print(Form("%s/%i/MCdata_comparison_MC_%s_data_%s_%s_run_%i.pdf]",output_dir_name[0],run,MC_prod.c_str(),period.c_str(),apass.c_str(),run));
  */
  
}
