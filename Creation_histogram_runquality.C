// ------------ Creation of pie charts saved in a PDF file showing the MC/data run quality, based on data from goodRuns.txt and badRuns.txt
#include <iostream>
#include <TAttLine.h>
#include <TAxis.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TFile.h>
#include <TH1.h>
#include <TLatex.h>
#include <TLine.h>
#include <TLegend.h>
#include <TList.h>
#include <TMath.h>
#include <TPie.h>
#include <TStopwatch.h>
#include <TStyle.h>
#include <TSystem.h>
#include <string>
#include <TROOT.h>
#include <fstream>

using namespace std;

void Creation_histogram_runquality(string MC_prod, string period, string apass){
  
  cout << "MC_prod: " << MC_prod << endl;
  cout << "period: " << period << endl;
  cout << "apass: " << apass << endl;
    
  gROOT->SetBatch(kTRUE);
  gStyle->SetOptStat(0);
  
  //----------Input and Output directory - same directory of the config file
  char output_dir_name[1][200] = {"/home/sarapc/Desktop/MID_QC/ComparisonDataMC_A02D/pp136TeV2023_apass4"}; //same directory of the config file

  Long_t *dummy1 = 0, *dummy2 = 0, *dummy3 = 0, *dummy4 = 0;

  //---------- Read .txt file for good runs
  ifstream fileGood (Form("%s/goodRuns.txt",output_dir_name[0])); //thresold
  ifstream fileGood_2sigmas (Form("%s/goodRuns_2sigmas.txt",output_dir_name[0])); //fit
  ifstream fileGood_chi2 (Form("%s/goodRuns_chi2.txt",output_dir_name[0])); //chi2
  ifstream fileGood_thresholds_sigmas (Form("%s/goodRuns_thresholds_sigmas.txt",output_dir_name[0])); //thresholds + sigmas
  
  if(!fileGood){
   cout << "File GoodRuns not Found!" << endl;  
   return;
  }  
  
  int rungood_column = 0, Etagood_column = 0, Phigood_column = 0, Ptgood_column = 0; //file_good_index
  int count_goodruns = 0, count_GoodEtaPhi = 0, count_GoodEtaPhiPt = 0; //Good runs
  
  while(fileGood >> rungood_column >> Etagood_column >> Phigood_column >> Ptgood_column){
    //counters for different conditions  
    count_goodruns++; 
    if(Etagood_column==1 && Phigood_column==1 && Ptgood_column==0) count_GoodEtaPhi++;
    if(Etagood_column==1 && Phigood_column==1 && Ptgood_column==1) count_GoodEtaPhiPt++;
  }
  
  cout << "Tot good runs: " << count_goodruns << " |etaphi: " << count_GoodEtaPhi << " |also pt: " << count_GoodEtaPhiPt << endl;
  double percentages_good[2] = {0.,0.};
  
  for(int i=0;i<2;i++){
   if(i==0) percentages_good[i] = (count_GoodEtaPhi/(double)count_goodruns) *100;
    else percentages_good[i] = (count_GoodEtaPhiPt/ (double)count_goodruns) *100;
   //cout << percentages_good[i] << endl; 
  }
  
  //---- TPie graph for good runs
  const char *labels_good[2] = {"#eta&&#phi GOOD", "#eta&&#phi&&p_{T} GOOD"};  // labels
  int colors_good[2] = {kGreen+2,kBlue};
  
  TCanvas *c_good = new TCanvas("c_good", "Grafico Good Runs", 800, 800);
  c_good->cd();
  TPie *pie_good = new TPie("pie_good", Form("Good Runs_thresholds %s", period.c_str()), 2, percentages_good, nullptr);
  for (int i = 0; i < 2; i++) {
   pie_good->SetEntryLabel(i, "");
   pie_good->SetTextSize(0.03);
   pie_good->SetEntryFillColor(i, colors_good[i]);
  }
  pie_good->SetLabelsOffset(0.02);
  pie_good->Draw("3d");
  TLegend * leg_good = new TLegend(0.25,0.75,0.95,0.9);
  for (int i = 0; i < 2; i++) {
   leg_good->SetTextSize(0.04);   
   leg_good->AddEntry(pie_good->GetSlice(i), Form("%s: %.1f%%", labels_good[i], percentages_good[i]), "f");
  }
  leg_good->Draw();
  
  c_good->Print(Form("%s/QualityHisto_MC_%s_data_%s_%s.pdf[",output_dir_name[0],MC_prod.c_str(),period.c_str(),apass.c_str()));
  c_good->Print(Form("%s/QualityHisto_MC_%s_data_%s_%s.pdf",output_dir_name[0],MC_prod.c_str(),period.c_str(),apass.c_str()));
  
  
  
  if(!fileGood_2sigmas){
   cout << "File Good_2sigmasRuns not Found!" << endl;  
   return;
  }  
  
  int rungood_2sigmas_column = 0, Etagood_2sigmas_column = 0, Phigood_2sigmas_column = 0, Ptgood_2sigmas_column = 0; //file_good_2sigmas_index
  int count_good_2sigmasruns = 0, count_Good_2sigmasEtaPhi = 0, count_Good_2sigmasEtaPhiPt = 0; //Good_2sigmas runs
  
  while(fileGood_2sigmas >> rungood_2sigmas_column >> Etagood_2sigmas_column >> Phigood_2sigmas_column >> Ptgood_2sigmas_column){
    //counters for different conditions  
    count_good_2sigmasruns++; 
    if(Etagood_2sigmas_column==1 && Phigood_2sigmas_column==1 && Ptgood_2sigmas_column==0) count_Good_2sigmasEtaPhi++;
    if(Etagood_2sigmas_column==1 && Phigood_2sigmas_column==1 && Ptgood_2sigmas_column==1) count_Good_2sigmasEtaPhiPt++;
  }
  
  cout << "Tot good_2sigmas runs: " << count_good_2sigmasruns << " |etaphi: " << count_Good_2sigmasEtaPhi << " |also pt: " << count_Good_2sigmasEtaPhiPt << endl;
  double percentages_good_2sigmas[2] = {0.,0.};
  
  for(int i=0;i<2;i++){
   if(i==0) percentages_good_2sigmas[i] = (count_Good_2sigmasEtaPhi/(double)count_good_2sigmasruns) *100;
    else percentages_good_2sigmas[i] = (count_Good_2sigmasEtaPhiPt/ (double)count_good_2sigmasruns) *100;
   //cout << percentages_good_2sigmas[i] << endl; 
  }
  
  //---- TPie graph for good_2sigmas runs
  const char *labels_good_2sigmas[2] = {"#eta&&#phi GOOD", "#eta&&#phi&&p_{T} GOOD"};  // labels
  int colors_good_2sigmas[2] = {kGreen+2,kBlue};
  
  TCanvas *c_good_2sigmas = new TCanvas("c_good_2sigmas", "Grafico Good_2sigmas Runs", 800, 800);
  c_good_2sigmas->cd();
  TPie *pie_good_2sigmas = new TPie("pie_good_2sigmas", Form("Good_2sigmas Runs %s", period.c_str()), 2, percentages_good_2sigmas, nullptr);
  for (int i = 0; i < 2; i++) {
   pie_good_2sigmas->SetEntryLabel(i, "");
   pie_good_2sigmas->SetTextSize(0.03);
   pie_good_2sigmas->SetEntryFillColor(i, colors_good_2sigmas[i]);
  }
  pie_good_2sigmas->SetLabelsOffset(0.02);
  pie_good_2sigmas->Draw("3d");
  TLegend * leg_good_2sigmas = new TLegend(0.25,0.75,0.95,0.9);
  for (int i = 0; i < 2; i++) {
   leg_good_2sigmas->SetTextSize(0.04);   
   leg_good_2sigmas->AddEntry(pie_good_2sigmas->GetSlice(i), Form("%s: %.1f%%", labels_good_2sigmas[i], percentages_good_2sigmas[i]), "f");
  }
  leg_good_2sigmas->Draw();
  
  //c_good_2sigmas->Print(Form("%s/QualityHisto_MC_%s_data_%s_%s.pdf[",output_dir_name[0],MC_prod.c_str(),period.c_str(),apass.c_str()));
  c_good_2sigmas->Print(Form("%s/QualityHisto_MC_%s_data_%s_%s.pdf",output_dir_name[0],MC_prod.c_str(),period.c_str(),apass.c_str()));
  
  
  if(!fileGood_chi2){
   cout << "File Good_chi2Runs not Found!" << endl;  
   return;
  }  
  
  int rungood_chi2_column = 0, Etagood_chi2_column = 0, Phigood_chi2_column = 0, Ptgood_chi2_column = 0; //file_good_chi2_index
  int count_good_chi2runs = 0, count_Good_chi2EtaPhi = 0, count_Good_chi2EtaPhiPt = 0; //Good_chi2 runs
  
  while(fileGood_chi2 >> rungood_chi2_column >> Etagood_chi2_column >> Phigood_chi2_column >> Ptgood_chi2_column){
    //counters for different conditions  
    count_good_chi2runs++; 
    if(Etagood_chi2_column==1 && Phigood_chi2_column==1 && Ptgood_chi2_column==0) count_Good_chi2EtaPhi++;
    if(Etagood_chi2_column==1 && Phigood_chi2_column==1 && Ptgood_chi2_column==1) count_Good_chi2EtaPhiPt++;
  }
  
  cout << "Tot good_chi2 runs: " << count_good_chi2runs << " |etaphi: " << count_Good_chi2EtaPhi << " |also pt: " << count_Good_chi2EtaPhiPt << endl;
  double percentages_good_chi2[2] = {0.,0.};
  
  for(int i=0;i<2;i++){
   if(i==0) percentages_good_chi2[i] = (count_Good_chi2EtaPhi/(double)count_good_chi2runs) *100;
    else percentages_good_chi2[i] = (count_Good_chi2EtaPhiPt/ (double)count_good_chi2runs) *100;
   //cout << percentages_good_chi2[i] << endl; 
  }
  
  //---- TPie graph for good_chi2 runs
  const char *labels_good_chi2[2] = {"#eta&&#phi GOOD", "#eta&&#phi&&p_{T} GOOD"};  // labels
  int colors_good_chi2[2] = {kGreen+2,kBlue};
  
  TCanvas *c_good_chi2 = new TCanvas("c_good_chi2", "Grafico Good_chi2 Runs", 800, 800);
  c_good_chi2->cd();
  TPie *pie_good_chi2 = new TPie("pie_good_chi2", Form("Good_chi2 Runs %s", period.c_str()), 2, percentages_good_chi2, nullptr);
  for (int i = 0; i < 2; i++) {
   pie_good_chi2->SetEntryLabel(i, "");
   pie_good_chi2->SetTextSize(0.03);
   pie_good_chi2->SetEntryFillColor(i, colors_good_chi2[i]);
  }
  pie_good_chi2->SetLabelsOffset(0.02);
  pie_good_chi2->Draw("3d");
  TLegend * leg_good_chi2 = new TLegend(0.25,0.75,0.95,0.9);
  for (int i = 0; i < 2; i++) {
   leg_good_chi2->SetTextSize(0.04);   
   leg_good_chi2->AddEntry(pie_good_chi2->GetSlice(i), Form("%s: %.1f%%", labels_good_chi2[i], percentages_good_chi2[i]), "f");
  }
  leg_good_chi2->Draw();
  
  //c_good_chi2->Print(Form("%s/QualityHisto_MC_%s_data_%s_%s.pdf[",output_dir_name[0],MC_prod.c_str(),period.c_str(),apass.c_str()));
  c_good_chi2->Print(Form("%s/QualityHisto_MC_%s_data_%s_%s.pdf",output_dir_name[0],MC_prod.c_str(),period.c_str(),apass.c_str()));
  
  
  if(!fileGood_thresholds_sigmas){
   cout << "File Good_thresholds_sigmasRuns not Found!" << endl;  
   return;
  }  
  
  int rungood_thresholds_sigmas_column = 0, Etagood_thresholds_sigmas_column = 0, Phigood_thresholds_sigmas_column = 0, Ptgood_thresholds_sigmas_column = 0; //file_good_thresholds_sigmas_index
  int count_good_thresholds_sigmasruns = 0, count_Good_thresholds_sigmasEtaPhi = 0, count_Good_thresholds_sigmasEtaPhiPt = 0; //Good_thresholds_sigmas runs
  
  while(fileGood_thresholds_sigmas >> rungood_thresholds_sigmas_column >> Etagood_thresholds_sigmas_column >> Phigood_thresholds_sigmas_column >> Ptgood_thresholds_sigmas_column){
    //counters for different conditions  
    count_good_thresholds_sigmasruns++; 
    if(Etagood_thresholds_sigmas_column==1 && Phigood_thresholds_sigmas_column==1 && Ptgood_thresholds_sigmas_column==0) count_Good_thresholds_sigmasEtaPhi++;
    if(Etagood_thresholds_sigmas_column==1 && Phigood_thresholds_sigmas_column==1 && Ptgood_thresholds_sigmas_column==1) count_Good_thresholds_sigmasEtaPhiPt++;
  }
  
  cout << "Tot good_thresholds_sigmas runs: " << count_good_thresholds_sigmasruns << " |etaphi: " << count_Good_thresholds_sigmasEtaPhi << " |also pt: " << count_Good_thresholds_sigmasEtaPhiPt << endl;
  double percentages_good_thresholds_sigmas[2] = {0.,0.};
  
  for(int i=0;i<2;i++){
   if(i==0) percentages_good_thresholds_sigmas[i] = (count_Good_thresholds_sigmasEtaPhi/(double)count_good_thresholds_sigmasruns) *100;
    else percentages_good_thresholds_sigmas[i] = (count_Good_thresholds_sigmasEtaPhiPt/ (double)count_good_thresholds_sigmasruns) *100;
   //cout << percentages_good_thresholds_sigmas[i] << endl; 
  }
  
  //---- TPie graph for good_thresholds_sigmas runs
  const char *labels_good_thresholds_sigmas[2] = {"#eta&&#phi GOOD", "#eta&&#phi&&p_{T} GOOD"};  // labels
  int colors_good_thresholds_sigmas[2] = {kGreen+2,kBlue};
  
  TCanvas *c_good_thresholds_sigmas = new TCanvas("c_good_thresholds_sigmas", "Grafico Good_thresholds_sigmas Runs", 800, 800);
  c_good_thresholds_sigmas->cd();
  TPie *pie_good_thresholds_sigmas = new TPie("pie_good_thresholds_sigmas", Form("Good_thresholds_sigmas Runs %s", period.c_str()), 2, percentages_good_thresholds_sigmas, nullptr);
  for (int i = 0; i < 2; i++) {
   pie_good_thresholds_sigmas->SetEntryLabel(i, "");
   pie_good_thresholds_sigmas->SetTextSize(0.03);
   pie_good_thresholds_sigmas->SetEntryFillColor(i, colors_good_thresholds_sigmas[i]);
  }
  pie_good_thresholds_sigmas->SetLabelsOffset(0.02);
  pie_good_thresholds_sigmas->Draw("3d");
  TLegend * leg_good_thresholds_sigmas = new TLegend(0.25,0.75,0.95,0.9);
  for (int i = 0; i < 2; i++) {
   leg_good_thresholds_sigmas->SetTextSize(0.04);   
   leg_good_thresholds_sigmas->AddEntry(pie_good_thresholds_sigmas->GetSlice(i), Form("%s: %.1f%%", labels_good_thresholds_sigmas[i], percentages_good_thresholds_sigmas[i]), "f");
  }
  leg_good_thresholds_sigmas->Draw();
  
  //c_good_thresholds_sigmas->Print(Form("%s/QualityHisto_MC_%s_data_%s_%s.pdf[",output_dir_name[0],MC_prod.c_str(),period.c_str(),apass.c_str()));
  c_good_thresholds_sigmas->Print(Form("%s/QualityHisto_MC_%s_data_%s_%s.pdf",output_dir_name[0],MC_prod.c_str(),period.c_str(),apass.c_str()));
  
  
  
  
  //---------- Read .txt file for bad runs
  ifstream fileBad (Form("%s/badRuns.txt",output_dir_name[0]));
  ifstream fileBad_2sigmas (Form("%s/badRuns_2sigmas.txt",output_dir_name[0])); //fit
  ifstream fileBad_chi2 (Form("%s/badRuns_chi2.txt",output_dir_name[0])); //chi2 
  ifstream fileBad_thresholds_sigmas (Form("%s/badRuns_thresholds_sigmas.txt",output_dir_name[0])); //thresholds + sigmas
  
  if(!fileBad){
   cout << "File BadRuns not Found!" << endl;  
   return;
  } 
  
  int runbad_column = 0, Etabad_column = 0, Phibad_column = 0, Ptbad_column = 0; //file_bad_index
  int count_badruns = 0, count_BadEta = 0, count_BadPhi = 0, count_BadEtaPhi = 0; //Bad runs
  
  while(fileBad >> runbad_column >> Etabad_column >> Phibad_column >> Ptbad_column){
    //counters for different conditions  
    count_badruns++;
    if(Etabad_column==0 && Phibad_column==1) count_BadEta++;
    if(Etabad_column==1 && Phibad_column==0) count_BadPhi++;
    if(Etabad_column==0 && Phibad_column==0) count_BadEtaPhi++;  
  }
  
  cout << "Tot bad runs: " << count_badruns << " |eta: " << count_BadEta << " |phi: " << count_BadPhi << " |etaphi: " << count_BadEtaPhi << endl;
  
  double percentages_bad[3] = {0.,0.,0.};
  
  for(int i=0;i<3;i++){
   if(i==0) percentages_bad[i] = (count_BadEta/ (double)count_badruns) *100;
   if(i==1) percentages_bad[i] = (count_BadPhi/ (double)count_badruns) *100;
   if(i==2) percentages_bad[i] = (count_BadEtaPhi/ (double)count_badruns) *100;
   //cout << percentages_bad[i] << endl;
  }

  //---- TPie graph for bad runs
  const char *labels_bad[3] = {"#eta BAD", "#phi BAD", "#eta&&#phi BAD"};
  int colors_bad[3] = {kOrange+7, kYellow+1, kRed+2};
  
  TCanvas *c_bad = new TCanvas("c_bad", "Grafico Bad Runs", 800, 800);
  c_bad->cd();
  TPie *pie_bad = new TPie("pie_bad", Form("Bad Runs_thresholds %s", period.c_str()), 3, percentages_bad, nullptr);
  for (int i = 0; i < 3; i++) {
   pie_bad->SetEntryLabel(i, "");
   pie_bad->SetTextSize(0.03);
   pie_bad->SetEntryFillColor(i, colors_bad[i]);
  }
  pie_bad->SetLabelsOffset(0.02);
  pie_bad->Draw("3d");
  TLegend * leg_bad = new TLegend(0.45,0.75,0.95,0.9);
  for (int i = 0; i < 3; i++) {
   leg_bad->SetTextSize(0.04);   
   leg_bad->AddEntry(pie_bad->GetSlice(i), Form("%s: %.1f%%", labels_bad[i], percentages_bad[i]), "f");
  }
  leg_bad->Draw();
  
  c_bad->Print(Form("%s/QualityHisto_MC_%s_data_%s_%s.pdf",output_dir_name[0],MC_prod.c_str(),period.c_str(),apass.c_str()));
  //c_bad->Print(Form("%s/QualityHisto_MC_%s_data_%s_%s.pdf]",output_dir_name[0],MC_prod.c_str(),period.c_str(),apass.c_str()));
  
  if(!fileBad_2sigmas){
   cout << "File BadRuns not Found!" << endl;  
   return;
  } 
  
  int runbad_2sigmas_column = 0, Etabad_2sigmas_column = 0, Phibad_2sigmas_column = 0, Ptbad_2sigmas_column = 0; //file_bad_2sigmas_index
  int count_bad_2sigmasruns = 0, count_Bad_2sigmasEta = 0, count_Bad_2sigmasPhi = 0, count_Bad_2sigmasEtaPhi = 0; //Bad_2sigmas runs
  
  while(fileBad_2sigmas >> runbad_2sigmas_column >> Etabad_2sigmas_column >> Phibad_2sigmas_column >> Ptbad_2sigmas_column){
    //counters for different conditions  
    count_bad_2sigmasruns++;
    if(Etabad_2sigmas_column==0 && Phibad_2sigmas_column==1) count_Bad_2sigmasEta++;
    if(Etabad_2sigmas_column==1 && Phibad_2sigmas_column==0) count_Bad_2sigmasPhi++;
    if(Etabad_2sigmas_column==0 && Phibad_2sigmas_column==0) count_Bad_2sigmasEtaPhi++;  
  }
  
  cout << "Tot bad_2sigmas runs: " << count_bad_2sigmasruns << " |eta: " << count_Bad_2sigmasEta << " |phi: " << count_Bad_2sigmasPhi << " |etaphi: " << count_Bad_2sigmasEtaPhi << endl;
  
  double percentages_bad_2sigmas[3] = {0.,0.,0.};
  
  for(int i=0;i<3;i++){
   if(i==0) percentages_bad_2sigmas[i] = (count_Bad_2sigmasEta/ (double)count_bad_2sigmasruns) *100;
   if(i==1) percentages_bad_2sigmas[i] = (count_Bad_2sigmasPhi/ (double)count_bad_2sigmasruns) *100;
   if(i==2) percentages_bad_2sigmas[i] = (count_Bad_2sigmasEtaPhi/ (double)count_bad_2sigmasruns) *100;
   //cout << percentages_bad_2sigmas[i] << endl;
  }
  
  //---- TPie graph for bad_2sigmas runs
  const char *labels_bad_2sigmas[3] = {"#eta BAD", "#phi BAD", "#eta&&#phi BAD"};
  int colors_bad_2sigmas[3] = {kOrange+7, kYellow+1, kRed+2};
  
  TCanvas *c_bad_2sigmas = new TCanvas("c_bad_2sigmas", "Grafico Bad_2sigmas Runs", 800, 800);
  c_bad_2sigmas->cd();
  TPie *pie_bad_2sigmas = new TPie("pie_bad_2sigmas", Form("Bad_2sigmas Runs %s", period.c_str()), 3, percentages_bad_2sigmas, nullptr);
  for (int i = 0; i < 3; i++) {
   pie_bad_2sigmas->SetEntryLabel(i, "");
   pie_bad_2sigmas->SetTextSize(0.03);
   pie_bad_2sigmas->SetEntryFillColor(i, colors_bad_2sigmas[i]);
  }
  pie_bad_2sigmas->SetLabelsOffset(0.02);
  pie_bad_2sigmas->Draw("3d");
  TLegend * leg_bad_2sigmas = new TLegend(0.45,0.75,0.95,0.9);
  for (int i = 0; i < 3; i++) {
   leg_bad_2sigmas->SetTextSize(0.04);   
   leg_bad_2sigmas->AddEntry(pie_bad_2sigmas->GetSlice(i), Form("%s: %.1f%%", labels_bad_2sigmas[i], percentages_bad_2sigmas[i]), "f");
  }
  leg_bad_2sigmas->Draw();
  
  c_bad_2sigmas->Print(Form("%s/QualityHisto_MC_%s_data_%s_%s.pdf",output_dir_name[0],MC_prod.c_str(),period.c_str(),apass.c_str()));
  //c_bad_2sigmas->Print(Form("%s/QualityHisto_MC_%s_data_%s_%s.pdf]",output_dir_name[0],MC_prod.c_str(),period.c_str(),apass.c_str()));
  
  
  if(!fileBad_chi2){
   cout << "File BadRuns not Found!" << endl;  
   return;
  } 
  
  int runbad_chi2_column = 0, Etabad_chi2_column = 0, Phibad_chi2_column = 0, Ptbad_chi2_column = 0; //file_bad_chi2_index
  int count_bad_chi2runs = 0, count_Bad_chi2Eta = 0, count_Bad_chi2Phi = 0, count_Bad_chi2EtaPhi = 0; //Bad_chi2 runs
  
  while(fileBad_chi2 >> runbad_chi2_column >> Etabad_chi2_column >> Phibad_chi2_column >> Ptbad_chi2_column){
    //counters for different conditions  
    count_bad_chi2runs++;
    if(Etabad_chi2_column==0 && Phibad_chi2_column==1) count_Bad_chi2Eta++;
    if(Etabad_chi2_column==1 && Phibad_chi2_column==0) count_Bad_chi2Phi++;
    if(Etabad_chi2_column==0 && Phibad_chi2_column==0) count_Bad_chi2EtaPhi++;  
  }
  
  cout << "Tot bad_chi2 runs: " << count_bad_chi2runs << " |eta: " << count_Bad_chi2Eta << " |phi: " << count_Bad_chi2Phi << " |etaphi: " << count_Bad_chi2EtaPhi << endl;
  
  double percentages_bad_chi2[3] = {0.,0.,0.};
  
  for(int i=0;i<3;i++){
   if(i==0) percentages_bad_chi2[i] = (count_Bad_chi2Eta/ (double)count_bad_chi2runs) *100;
   if(i==1) percentages_bad_chi2[i] = (count_Bad_chi2Phi/ (double)count_bad_chi2runs) *100;
   if(i==2) percentages_bad_chi2[i] = (count_Bad_chi2EtaPhi/ (double)count_bad_chi2runs) *100;
   //cout << percentages_bad_chi2[i] << endl;
  }
  
  //---- TPie graph for bad_chi2 runs
  const char *labels_bad_chi2[3] = {"#eta BAD", "#phi BAD", "#eta&&#phi BAD"};
  int colors_bad_chi2[3] = {kOrange+7, kYellow+1, kRed+2};
  
  TCanvas *c_bad_chi2 = new TCanvas("c_bad_chi2", "Grafico Bad_chi2 Runs", 800, 800);
  c_bad_chi2->cd();
  TPie *pie_bad_chi2 = new TPie("pie_bad_chi2", Form("Bad_chi2 Runs %s", period.c_str()), 3, percentages_bad_chi2, nullptr);
  for (int i = 0; i < 3; i++) {
   pie_bad_chi2->SetEntryLabel(i, "");
   pie_bad_chi2->SetTextSize(0.03);
   pie_bad_chi2->SetEntryFillColor(i, colors_bad_chi2[i]);
  }
  pie_bad_chi2->SetLabelsOffset(0.02);
  pie_bad_chi2->Draw("3d");
  TLegend * leg_bad_chi2 = new TLegend(0.45,0.75,0.95,0.9);
  for (int i = 0; i < 3; i++) {
   leg_bad_chi2->SetTextSize(0.04);   
   leg_bad_chi2->AddEntry(pie_bad_chi2->GetSlice(i), Form("%s: %.1f%%", labels_bad_chi2[i], percentages_bad_chi2[i]), "f");
  }
  leg_bad_chi2->Draw();
  
  c_bad_chi2->Print(Form("%s/QualityHisto_MC_%s_data_%s_%s.pdf",output_dir_name[0],MC_prod.c_str(),period.c_str(),apass.c_str()));
  //c_bad_chi2->Print(Form("%s/QualityHisto_MC_%s_data_%s_%s.pdf]",output_dir_name[0],MC_prod.c_str(),period.c_str(),apass.c_str()));
  
  
  if(!fileBad_thresholds_sigmas){
   cout << "File BadRuns not Found!" << endl;  
   return;
  } 
  
  int runbad_thresholds_sigmas_column = 0, Etabad_thresholds_sigmas_column = 0, Phibad_thresholds_sigmas_column = 0, Ptbad_thresholds_sigmas_column = 0; //file_bad_thresholds_sigmas_index
  int count_bad_thresholds_sigmasruns = 0, count_Bad_thresholds_sigmasEta = 0, count_Bad_thresholds_sigmasPhi = 0, count_Bad_thresholds_sigmasEtaPhi = 0; //Bad_thresholds_sigmas runs
  
  while(fileBad_thresholds_sigmas >> runbad_thresholds_sigmas_column >> Etabad_thresholds_sigmas_column >> Phibad_thresholds_sigmas_column >> Ptbad_thresholds_sigmas_column){
    //counters for different conditions  
    count_bad_thresholds_sigmasruns++;
    if(Etabad_thresholds_sigmas_column==0 && Phibad_thresholds_sigmas_column==1) count_Bad_thresholds_sigmasEta++;
    if(Etabad_thresholds_sigmas_column==1 && Phibad_thresholds_sigmas_column==0) count_Bad_thresholds_sigmasPhi++;
    if(Etabad_thresholds_sigmas_column==0 && Phibad_thresholds_sigmas_column==0) count_Bad_thresholds_sigmasEtaPhi++;  
  }
  
  cout << "Tot bad_thresholds_sigmas runs: " << count_bad_thresholds_sigmasruns << " |eta: " << count_Bad_thresholds_sigmasEta << " |phi: " << count_Bad_thresholds_sigmasPhi << " |etaphi: " << count_Bad_thresholds_sigmasEtaPhi << endl;
  
  double percentages_bad_thresholds_sigmas[3] = {0.,0.,0.};
  
  for(int i=0;i<3;i++){
   if(i==0) percentages_bad_thresholds_sigmas[i] = (count_Bad_thresholds_sigmasEta/ (double)count_bad_thresholds_sigmasruns) *100;
   if(i==1) percentages_bad_thresholds_sigmas[i] = (count_Bad_thresholds_sigmasPhi/ (double)count_bad_thresholds_sigmasruns) *100;
   if(i==2) percentages_bad_thresholds_sigmas[i] = (count_Bad_thresholds_sigmasEtaPhi/ (double)count_bad_thresholds_sigmasruns) *100;
   //cout << percentages_bad_thresholds_sigmas[i] << endl;
  }
  
  //---- TPie graph for bad_thresholds_sigmas runs
  const char *labels_bad_thresholds_sigmas[3] = {"#eta BAD", "#phi BAD", "#eta&&#phi BAD"};
  int colors_bad_thresholds_sigmas[3] = {kOrange+7, kYellow+1, kRed+2};
  
  TCanvas *c_bad_thresholds_sigmas = new TCanvas("c_bad_thresholds_sigmas", "Grafico Bad_thresholds_sigmas Runs", 800, 800);
  c_bad_thresholds_sigmas->cd();
  TPie *pie_bad_thresholds_sigmas = new TPie("pie_bad_thresholds_sigmas", Form("Bad_thresholds_sigmas Runs %s", period.c_str()), 3, percentages_bad_thresholds_sigmas, nullptr);
  for (int i = 0; i < 3; i++) {
   pie_bad_thresholds_sigmas->SetEntryLabel(i, "");
   pie_bad_thresholds_sigmas->SetTextSize(0.03);
   pie_bad_thresholds_sigmas->SetEntryFillColor(i, colors_bad_thresholds_sigmas[i]);
  }
  pie_bad_thresholds_sigmas->SetLabelsOffset(0.02);
  pie_bad_thresholds_sigmas->Draw("3d");
  TLegend * leg_bad_thresholds_sigmas = new TLegend(0.45,0.75,0.95,0.9);
  for (int i = 0; i < 3; i++) {
   leg_bad_thresholds_sigmas->SetTextSize(0.04);   
   leg_bad_thresholds_sigmas->AddEntry(pie_bad_thresholds_sigmas->GetSlice(i), Form("%s: %.1f%%", labels_bad_thresholds_sigmas[i], percentages_bad_thresholds_sigmas[i]), "f");
  }
  leg_bad_thresholds_sigmas->Draw();
  
  c_bad_thresholds_sigmas->Print(Form("%s/QualityHisto_MC_%s_data_%s_%s.pdf",output_dir_name[0],MC_prod.c_str(),period.c_str(),apass.c_str()));
  //c_bad_thresholds_sigmas->Print(Form("%s/QualityHisto_MC_%s_data_%s_%s.pdf]",output_dir_name[0],MC_prod.c_str(),period.c_str(),apass.c_str()));
  

  //Total number of runs
  int count_totRuns = count_goodruns + count_badruns; //is the same for all configurations

  const char *labels_qualityGeneral[2] = {"GOOD", "BAD"};
  int colors_qualityGeneral[2] = {kGreen+2, kRed+2};

  //Charts to show percentages of good/bad runs
  double percentages_goodBad[2] = {0.,0.};
  double percentages_goodBad_2sigma[2] = {0.,0.};
  double percentages_goodBad_chisquare[2] = {0.,0.};
  double percentages_goodBad_thresholds_sigmas[2] = {0.,0.};

  for(int i=0;i<2;i++){
    if(i==0) percentages_goodBad[i] = (count_goodruns/ (double)count_totRuns) *100;
    if(i==1) percentages_goodBad[i] = (count_badruns/ (double)count_totRuns) *100;
    //cout << percentages_goodBad[i] << endl;
  }

  for(int i=0;i<2;i++){
    if(i==0) percentages_goodBad_2sigma[i] = (count_good_2sigmasruns/ (double)count_totRuns) *100;
    if(i==1) percentages_goodBad_2sigma[i] = (count_bad_2sigmasruns/ (double)count_totRuns) *100;
    //cout << percentages_goodBad_2sigma[i] << endl;
  }

  for(int i=0;i<2;i++){
    if(i==0) percentages_goodBad_chisquare[i] = (count_good_chi2runs/ (double)count_totRuns) *100;
    if(i==1) percentages_goodBad_chisquare[i] = (count_bad_chi2runs/ (double)count_totRuns) *100;
    //cout << percentages_goodBad_chisquare[i] << endl;
  }
  
  for(int i=0;i<2;i++){
    if(i==0) percentages_goodBad_thresholds_sigmas[i] = (count_good_thresholds_sigmasruns/ (double)count_totRuns) *100;
    if(i==1) percentages_goodBad_thresholds_sigmas[i] = (count_bad_thresholds_sigmasruns/ (double)count_totRuns) *100;
    //cout << percentages_goodBad_chisquare[i] << endl;
  }

  //Good/bad thresholds
  TCanvas *c_goodbad = new TCanvas("c_goodbad", "Grafico Good/Bad Runs", 800, 800);
  c_goodbad->cd();
  TPie *pie_goodbad = new TPie("pie_goodbad", Form("Good Bad Runs_thresholds %s", period.c_str()), 2, percentages_goodBad, nullptr);
  for (int i = 0; i < 2; i++) {
   pie_goodbad->SetEntryLabel(i, "");
   pie_goodbad->SetTextSize(0.03);
   pie_goodbad->SetEntryFillColor(i, colors_qualityGeneral[i]);
  }
  pie_goodbad->SetLabelsOffset(0.02);
  pie_goodbad->Draw("3d");
  TLegend * leg_goodbad = new TLegend(0.45,0.75,0.95,0.9);
  for (int i = 0; i < 2; i++) {
   leg_goodbad->SetTextSize(0.04);   
   leg_goodbad->AddEntry(pie_goodbad->GetSlice(i), Form("%s: %.1f%%", labels_qualityGeneral[i], percentages_goodBad[i]), "f");
  }
  leg_goodbad->Draw();
  
  c_goodbad->Print(Form("%s/QualityHisto_MC_%s_data_%s_%s.pdf",output_dir_name[0],MC_prod.c_str(),period.c_str(),apass.c_str()));
  //c_goodbad->Print(Form("%s/GoodBadHisto_MC_%s_data_%s_%s.pdf]",output_dir_name[0],MC_prod.c_str(),period.c_str(),apass.c_str()));

  //Good/bad 2 sigma
  TCanvas *c_goodbad_2sigma = new TCanvas("c_goodbad_2sigma", "Grafico Good/Bad Runs 2 sigma", 800, 800);
  c_goodbad_2sigma->cd();
  TPie *pie_goodbad_2sigma = new TPie("pie_goodbad_2sigma", Form("Good Bad Runs_2sigma %s", period.c_str()), 2, percentages_goodBad_2sigma, nullptr);
  for (int i = 0; i < 2; i++) {
   pie_goodbad_2sigma->SetEntryLabel(i, "");
   pie_goodbad_2sigma->SetTextSize(0.03);
   pie_goodbad_2sigma->SetEntryFillColor(i, colors_qualityGeneral[i]);
  }
  pie_goodbad_2sigma->SetLabelsOffset(0.02);
  pie_goodbad_2sigma->Draw("3d");
  TLegend * leg_goodbad_2sigma = new TLegend(0.45,0.75,0.95,0.9);
  for (int i = 0; i < 2; i++) {
   leg_goodbad_2sigma->SetTextSize(0.04);   
   leg_goodbad_2sigma->AddEntry(pie_goodbad_2sigma->GetSlice(i), Form("%s: %.1f%%", labels_qualityGeneral[i], percentages_goodBad_2sigma[i]), "f");
  }
  leg_goodbad_2sigma->Draw();
  
  c_goodbad_2sigma->Print(Form("%s/QualityHisto_MC_%s_data_%s_%s.pdf",output_dir_name[0],MC_prod.c_str(),period.c_str(),apass.c_str()));
  //c_goodbad_2sigma->Print(Form("%s/GoodBadHisto_2sigma_MC_%s_data_%s_%s.pdf]",output_dir_name[0],MC_prod.c_str(),period.c_str(),apass.c_str()));

  //Good/bad chi2
  TCanvas *c_goodbad_chi2 = new TCanvas("c_goodbad_chi2", "Grafico Good/Bad Runs chi2", 800, 800);
  c_goodbad_chi2->cd();
  TPie *pie_goodbad_chi2 = new TPie("pie_goodbad_chi2", Form("Good Bad Runs_chi2 %s", period.c_str()), 2, percentages_goodBad_chisquare, nullptr);
  for (int i = 0; i < 2; i++) {
   pie_goodbad_chi2->SetEntryLabel(i, "");
   pie_goodbad_chi2->SetTextSize(0.03);
   pie_goodbad_chi2->SetEntryFillColor(i, colors_qualityGeneral[i]);
  }
  pie_goodbad_chi2->SetLabelsOffset(0.02);
  pie_goodbad_chi2->Draw("3d");
  TLegend * leg_goodbad_chi2 = new TLegend(0.45,0.75,0.95,0.9);
  for (int i = 0; i < 2; i++) {
   leg_goodbad_chi2->SetTextSize(0.04);   
   leg_goodbad_chi2->AddEntry(pie_goodbad_chi2->GetSlice(i), Form("%s: %.1f%%", labels_qualityGeneral[i], percentages_goodBad_chisquare[i]), "f");
  }
  leg_goodbad_chi2->Draw();
  
  c_goodbad_chi2->Print(Form("%s/QualityHisto_MC_%s_data_%s_%s.pdf",output_dir_name[0],MC_prod.c_str(),period.c_str(),apass.c_str()));
  //c_goodbad_chi2->Print(Form("%s/QualityHisto_MC_%s_data_%s_%s.pdf]",output_dir_name[0],MC_prod.c_str(),period.c_str(),apass.c_str()));
 
  
  //Good/bad thresholds + sigmas
  TCanvas *c_goodbad_thresholds_sigmas = new TCanvas("c_goodbad_thresholds_sigmas", "Grafico Good/Bad Runs thresholds_sigmas", 800, 800);
  c_goodbad_thresholds_sigmas->cd();
  TPie *pie_goodbad_thresholds_sigmas = new TPie("pie_goodbad_thresholds_sigmas", Form("Good Bad Runs_thresholds_sigmas %s", period.c_str()), 2, percentages_goodBad_thresholds_sigmas, nullptr);
  for (int i = 0; i < 2; i++) {
   pie_goodbad_thresholds_sigmas->SetEntryLabel(i, "");
   pie_goodbad_thresholds_sigmas->SetTextSize(0.03);
   pie_goodbad_thresholds_sigmas->SetEntryFillColor(i, colors_qualityGeneral[i]);
  }
  pie_goodbad_thresholds_sigmas->SetLabelsOffset(0.02);
  pie_goodbad_thresholds_sigmas->Draw("3d");
  TLegend * leg_goodbad_thresholds_sigmas = new TLegend(0.45,0.75,0.95,0.9);
  for (int i = 0; i < 2; i++) {
   leg_goodbad_thresholds_sigmas->SetTextSize(0.04);   
   leg_goodbad_thresholds_sigmas->AddEntry(pie_goodbad_thresholds_sigmas->GetSlice(i), Form("%s: %.1f%%", labels_qualityGeneral[i], percentages_goodBad_thresholds_sigmas[i]), "f");
  }
  leg_goodbad_thresholds_sigmas->Draw();
  
  c_goodbad_thresholds_sigmas->Print(Form("%s/QualityHisto_MC_%s_data_%s_%s.pdf",output_dir_name[0],MC_prod.c_str(),period.c_str(),apass.c_str()));
  c_goodbad_thresholds_sigmas->Print(Form("%s/QualityHisto_MC_%s_data_%s_%s.pdf]",output_dir_name[0],MC_prod.c_str(),period.c_str(),apass.c_str()));
  
  
}
