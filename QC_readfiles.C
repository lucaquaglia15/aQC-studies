#include <iostream>

#include <TAttLine.h>
#include <TAxis.h>
#include <TCanvas.h>
#include <TChain.h>
#include <TF1.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
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

using namespace std;
using namespace o2::quality_control::core;
using namespace o2::quality_control_modules::mid;

void QC_readfiles(string period, string apass, string run){
  
  gROOT->SetBatch(kTRUE);
  //char period_name = period.c_str();
  //char pass_name = apass.c_str();
  //char run.c_str() = run.c_str();

  Long_t *dummy1 = 0, *dummy2 = 0, *dummy3 = 0, *dummy4 = 0;
    
  TStopwatch time;
  time.Start();
  
  TFile *file_in = 0x0;
  
  
  if(gSystem->GetPathInfo(Form("QC_fullrun_%s_%s_%s.root",period.c_str(),apass.c_str(),run.c_str()),dummy1,dummy2,dummy3,dummy4) != 0) cout << "File Not Found! Try again" << endl;    
  file_in = new TFile(Form("QC_fullrun_%s_%s_%s.root",period.c_str(),apass.c_str(),run.c_str()),"read");
  
  printf("Open File: %s\n",file_in->GetName());
  
  //------------ Local Board Occupancy
  string objPath = "MID/Digits";
  if(apass == "apass4") objPath = "int/" + objPath;
  o2::quality_control::core::MonitorObjectCollection *Digits = static_cast<o2::quality_control::core::MonitorObjectCollection *>(file_in->Get(objPath.c_str())); //int/MID/Digits if apass4, else MID/Digits
  
  //printf("%s\n", Digits->FindObject("LocalBoardsMap11")->ClassName());
  o2::quality_control::core::MonitorObject *LocalBoardsMap11 = static_cast<o2::quality_control::core::MonitorObject *>(Digits->FindObject("LocalBoardsMap11"));
  TH2F *hhLocalBoardsMap11 = reinterpret_cast<TH2F*>(LocalBoardsMap11->getObject());
  hhLocalBoardsMap11->SetName("hhLocalBoardsMap11");
  //hhLocalBoardsMap11->Draw();
  
  o2::quality_control::core::MonitorObject *LocalBoardsMap12 = static_cast<o2::quality_control::core::MonitorObject *>(Digits->FindObject("LocalBoardsMap12"));
  TH2F *hhLocalBoardsMap12 = reinterpret_cast<TH2F*>(LocalBoardsMap12->getObject());
  hhLocalBoardsMap12->SetName("hhLocalBoardsMap12");
  //hhLocalBoardsMap12->Draw();
  
  o2::quality_control::core::MonitorObject *LocalBoardsMap21 = static_cast<o2::quality_control::core::MonitorObject *>(Digits->FindObject("LocalBoardsMap21"));
  TH2F *hhLocalBoardsMap21 = reinterpret_cast<TH2F*>(LocalBoardsMap21->getObject());
  hhLocalBoardsMap21->SetName("hhLocalBoardsMap21");
  //hhLocalBoardsMap21->Draw();
  
  o2::quality_control::core::MonitorObject *LocalBoardsMap22 = static_cast<o2::quality_control::core::MonitorObject *>(Digits->FindObject("LocalBoardsMap22"));
  TH2F *hhLocalBoardsMap22 = reinterpret_cast<TH2F*>(LocalBoardsMap22->getObject());
  hhLocalBoardsMap22->SetName("hhLocalBoardsMap22");
  //hhLocalBoardsMap22->Draw();
  
   //------------ Drawing LocalBoardsOccupancy
   gStyle->SetOptStat(0);
   char LocalBoardOccupancy_title[4][50] = {"Local boards Occupancy MapMT11", "Local boards Occupancy MapMT12", "Local boards Occupancy MapMT21", "Local boards Occupancy MapMT22"};
   TCanvas *can_lboccupancy = 0x0;
   TLegend *leg_lboccupancy;
   TLatex latex_lboccupancy;
   TH2F *frame_lboccupancy[4];
   TPad* pad_lboccupancy[4];
   
   can_lboccupancy = new TCanvas("can_lboccupancy","can_lboccupancy",1600,1600);
   can_lboccupancy->Divide(2,2); 

   for(int lmap=0;lmap<4;lmap++){
    can_lboccupancy->cd(lmap+1);
    //can_lboccupancy->cd(lmap+1)->SetMargin(0.10,0.03,0.10,0.05); 
    pad_lboccupancy[lmap] = new TPad(Form("padlboccupancy_%i",lmap),"",0,0,1,1);
    pad_lboccupancy[lmap]->SetMargin(0.1,0.1,0.1,0.1); //sx,dx,basso,alto
    pad_lboccupancy[lmap]->Draw();
    can_lboccupancy->cd(lmap+1);
    pad_lboccupancy[lmap]->cd();
    //pad_lboccupancy[lmap]->SetLogz();
    
    frame_lboccupancy[lmap] = new TH2F(Form("frame_%i",lmap),"",1,-7,7,1,0,9); 
    frame_lboccupancy[lmap]->SetTitle(Form("%s",LocalBoardOccupancy_title[lmap]));
    frame_lboccupancy[lmap]->GetXaxis()->SetTitle("Column");
    frame_lboccupancy[lmap]->GetYaxis()->SetTitle("Line");
    frame_lboccupancy[lmap]->GetXaxis()->SetTitleSize(0.05);
    frame_lboccupancy[lmap]->GetYaxis()->SetTitleSize(0.05);
    frame_lboccupancy[lmap]->GetXaxis()->SetLabelSize(0.05);
    frame_lboccupancy[lmap]->GetYaxis()->SetLabelSize(0.05);
    frame_lboccupancy[lmap]->GetXaxis()->SetTitleOffset(1.1); 
    frame_lboccupancy[lmap]->GetYaxis()->SetTitleOffset(1.1);
    frame_lboccupancy[lmap]->GetXaxis()->SetRangeUser(-7,7);
    frame_lboccupancy[lmap]->GetYaxis()->SetRangeUser(0,9);
    frame_lboccupancy[lmap]->Draw("");
    
    if(lmap==0) hhLocalBoardsMap11->Draw("COLZ");
    else if(lmap==1) hhLocalBoardsMap12->Draw("COLZ");
     else if(lmap==2) hhLocalBoardsMap21->Draw("COLZ");
      else hhLocalBoardsMap22->Draw("COLZ");
   }
   gSystem->mkdir("images");
   can_lboccupancy->SaveAs(Form("images/can_lboccupancy_period_%s_run_%s_pass_%s.png",period.c_str(),run.c_str(),apass.c_str()));
      
   
  //-------------Bending & Non-Bending ??
  
  //------------ Efficiency Plots
  objPath = "MID/Tracks";
  if(apass == "apass4") objPath = "int/" + objPath;
  o2::quality_control::core::MonitorObjectCollection *Tracks = static_cast<o2::quality_control::core::MonitorObjectCollection *>(file_in->Get(objPath.c_str())); //int/MID/Tracks if apass4, else MID/Tracks
  
  o2::quality_control::core::MonitorObject *EffRatio11 = static_cast<o2::quality_control::core::MonitorObject *>(Tracks->FindObject("TrackDetRatio44Map11"));
  TH2F *hhTrackDetRatio44Map11 = reinterpret_cast<TH2F*>(EffRatio11->getObject());
  hhTrackDetRatio44Map11->SetName("hhTrackDetRatio44Map11");
  //hhTrackDetRatio44Map11->Draw();
  
  o2::quality_control::core::MonitorObject *EffRatio12 = static_cast<o2::quality_control::core::MonitorObject *>(Tracks->FindObject("TrackDetRatio44Map12"));
  TH2F *hhTrackDetRatio44Map12 = reinterpret_cast<TH2F*>(EffRatio12->getObject());
  hhTrackDetRatio44Map12->SetName("hhTrackDetRatio44Map12");
  //hhTrackDetRatio44Map12->Draw();
  
  o2::quality_control::core::MonitorObject *EffRatio21 = static_cast<o2::quality_control::core::MonitorObject *>(Tracks->FindObject("TrackDetRatio44Map21"));
  TH2F *hhTrackDetRatio44Map21 = reinterpret_cast<TH2F*>(EffRatio21->getObject());
  hhTrackDetRatio44Map21->SetName("hhTrackDetRatio44Map21");
  //hhTrackDetRatio44Map21->Draw();
  
  o2::quality_control::core::MonitorObject *EffRatio22 = static_cast<o2::quality_control::core::MonitorObject *>(Tracks->FindObject("TrackDetRatio44Map22"));
  TH2F *hhTrackDetRatio44Map22 = reinterpret_cast<TH2F*>(EffRatio22->getObject());
  hhTrackDetRatio44Map22->SetName("hhTrackDetRatio44Map22");
  //hhTrackDetRatio44Map22->Draw();
  
   //------------ Drawing EfficiencyRatio
   char EfficiencyRatio_title[4][50] = {"MT11 Detector Track 44/all Map", "MT12 Detector Track 44/all Map", "MT21 Detector Track 44/all Map", "MT22 Detector Track 44/all Map"};
   TCanvas *can_effratio = 0x0;
   TLatex latex_effratio;
   TH2F *frame_effratio[4];
   TPad* pad_effratio[4];
   
   can_effratio = new TCanvas("can_effratio","can_effratio",1600,1600);
   can_effratio->Divide(2,2); 
   for(int lmap=0;lmap<4;lmap++){
    can_effratio->cd(lmap+1);
    //can_effratio->cd(lmap+1)->SetMargin(0.10,0.03,0.10,0.05); 
    pad_effratio[lmap] = new TPad(Form("padeffratio_%i",lmap),"",0,0,1,1);
    pad_effratio[lmap]->SetMargin(0.1,0.1,0.1,0.1); //sx,dx,basso,alto
    pad_effratio[lmap]->Draw();
    can_effratio->cd(lmap+1);
    pad_effratio[lmap]->cd();
    //pad_effratio[lmap]->SetLogz();
    
    frame_effratio[lmap] = new TH2F(Form("frame_eff_%i",lmap),"",1,-7,7,1,0,9); 
    frame_effratio[lmap]->SetTitle(Form("%s",EfficiencyRatio_title[lmap]));
    frame_effratio[lmap]->GetXaxis()->SetTitle("Column");
    frame_effratio[lmap]->GetYaxis()->SetTitle("Line");
    frame_effratio[lmap]->GetXaxis()->SetTitleSize(0.05);
    frame_effratio[lmap]->GetYaxis()->SetTitleSize(0.05);
    frame_effratio[lmap]->GetXaxis()->SetLabelSize(0.05);
    frame_effratio[lmap]->GetYaxis()->SetLabelSize(0.05);
    frame_effratio[lmap]->GetXaxis()->SetTitleOffset(1.1); 
    frame_effratio[lmap]->GetYaxis()->SetTitleOffset(1.1);
    frame_effratio[lmap]->GetXaxis()->SetRangeUser(-7,7);
    frame_effratio[lmap]->GetYaxis()->SetRangeUser(0,9);
    frame_effratio[lmap]->Draw("");
    
    if(lmap==0) hhTrackDetRatio44Map11->Draw("COLZ");
     else if(lmap==1) hhTrackDetRatio44Map12->Draw("COLZ");
      else if(lmap==2) hhTrackDetRatio44Map21->Draw("COLZ");
       else hhTrackDetRatio44Map22->Draw("COLZ");
   }
  
   can_effratio->SaveAs(Form("images/can_effratio_period_%s_run_%s_pass_%s.png",period.c_str(),run.c_str(),apass.c_str()));
  
  
  
  TFile *file_out = new TFile(Form("QC_fullrun_%s_%s_%s_output.root",period.c_str(),apass.c_str(),run.c_str()),"recreate");
  file_out->cd();
  hhLocalBoardsMap11->Write();
  hhLocalBoardsMap12->Write();
  hhLocalBoardsMap21->Write();
  hhLocalBoardsMap22->Write();
  hhTrackDetRatio44Map11->Write();
  hhTrackDetRatio44Map12->Write();
  hhTrackDetRatio44Map21->Write();
  hhTrackDetRatio44Map22->Write();
  
  
  time.Stop();
  time.Print();
  
}
