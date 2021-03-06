#include "Validation/HcalHits/interface/HcalSimHitStudy.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"

#include "FWCore/Utilities/interface/Exception.h"

HcalSimHitStudy::HcalSimHitStudy(const edm::ParameterSet& ps) {

  g4Label  = ps.getUntrackedParameter<std::string>("moduleLabel","g4SimHits");
  hcalHits = ps.getUntrackedParameter<std::string>("HitCollection","HcalHits");
  outFile_ = ps.getUntrackedParameter<std::string>("outputFile", "hcHit.root");
  verbose_ = ps.getUntrackedParameter<bool>("Verbose", false);
  checkHit_= true;

  tok_hits_ = consumes<edm::PCaloHitContainer>(edm::InputTag(g4Label,hcalHits));

  edm::LogInfo("HcalSim") << "Module Label: " << g4Label << "   Hits: "
			  << hcalHits << " / "<< checkHit_ 
			  << "   Output: " << outFile_;

  dbe_ = edm::Service<DQMStore>().operator->();
  if (dbe_) {
    if (verbose_) {
      dbe_->setVerbose(1);
      sleep (3);
      dbe_->showDirStructure();
    } else {
      dbe_->setVerbose(0);
    }
  }
}

HcalSimHitStudy::~HcalSimHitStudy() {}

void HcalSimHitStudy::beginJob() {

  if (dbe_) {
    dbe_->setCurrentFolder("HcalHitsV/HcalSimHitsTask");

    //Histograms for Hits
    if (checkHit_) {
      meAllNHit_  = dbe_->book1D("Hit01","Number of Hits in HCal",1000,0.,1000.);
      meBadDetHit_= dbe_->book1D("Hit02","Hits with wrong Det",   100,0.,100.);
      meBadSubHit_= dbe_->book1D("Hit03","Hits with wrong Subdet",100,0.,100.);
      meBadIdHit_ = dbe_->book1D("Hit04","Hits with wrong ID",    100,0.,100.);
      meHBNHit_   = dbe_->book1D("Hit05","Number of Hits in HB",1000,0.,1000.);
      meHENHit_   = dbe_->book1D("Hit06","Number of Hits in HE",1000,0.,1000.);
      meHONHit_   = dbe_->book1D("Hit07","Number of Hits in HO",1000,0.,1000.);
      meHFNHit_   = dbe_->book1D("Hit08","Number of Hits in HF",1000,0.,1000.);
      meDetectHit_= dbe_->book1D("Hit09","Detector ID",           50,0.,50.);
      meSubdetHit_= dbe_->book1D("Hit10","Subdetectors in HCal",  50,0.,50.);
      meDepthHit_ = dbe_->book1D("Hit11","Depths in HCal",        20,0.,20.);
      meEtaHit_   = dbe_->book1D("Hit12","Eta in HCal",          100,-50.,50.);
      mePhiHit_   = dbe_->book1D("Hit13","Phi in HCal",          100,0.,100.);
      meEnergyHit_= dbe_->book1D("Hit14","Energy in HCal",       100,0.,1.);
      meTimeHit_  = dbe_->book1D("Hit15","Time in HCal",         100,0.,400.);
      meTimeWHit_ = dbe_->book1D("Hit16","Time in HCal (E wtd)", 100,0.,400.);
      meHBDepHit_ = dbe_->book1D("Hit17","Depths in HB",          20,0.,20.);
      meHEDepHit_ = dbe_->book1D("Hit18","Depths in HE",          20,0.,20.);
      meHODepHit_ = dbe_->book1D("Hit19","Depths in HO",          20,0.,20.);
      meHFDepHit_ = dbe_->book1D("Hit20","Depths in HF",          20,0.,20.);
      meHBEtaHit_ = dbe_->book1D("Hit21","Eta in HB",            100,-50.,50.);
      meHEEtaHit_ = dbe_->book1D("Hit22","Eta in HE",            100,-50.,50.);
      meHOEtaHit_ = dbe_->book1D("Hit23","Eta in HO",            100,-50.,50.);
      meHFEtaHit_ = dbe_->book1D("Hit24","Eta in HF",            100,-50.,50.);
      meHBPhiHit_ = dbe_->book1D("Hit25","Phi in HB",            100,0.,100.);
      meHEPhiHit_ = dbe_->book1D("Hit26","Phi in HE",            100,0.,100.);
      meHOPhiHit_ = dbe_->book1D("Hit27","Phi in HO",            100,0.,100.);
      meHFPhiHit_ = dbe_->book1D("Hit28","Phi in HF",            100,0.,100.);
      meHBEneHit_ = dbe_->book1D("Hit29","Energy in HB",         100,0.,1.);
      meHEEneHit_ = dbe_->book1D("Hit30","Energy in HE",         100,0.,1.);
      meHOEneHit_ = dbe_->book1D("Hit31","Energy in HO",         100,0.,1.);
      meHFEneHit_ = dbe_->book1D("Hit32","Energy in HF",         100,0.,100.);
      meHBTimHit_ = dbe_->book1D("Hit33","Time in HB",           100,0.,400.);
      meHETimHit_ = dbe_->book1D("Hit34","Time in HE",           100,0.,400.);
      meHOTimHit_ = dbe_->book1D("Hit35","Time in HO",           100,0.,400.);
      meHFTimHit_ = dbe_->book1D("Hit36","Time in HF",           100,0.,400.);
      meHBEneHit2_ = dbe_->book1D("Hit37","Energy in HB 2",         100,0.,0.0001);
      meHEEneHit2_ = dbe_->book1D("Hit38","Energy in HE 2",         100,0.,0.0001);
      meHOEneHit2_ = dbe_->book1D("Hit39","Energy in HO 2",         100,0.,0.0001);
      meHFEneHit2_ = dbe_->book1D("Hit40","Energy in HF 2",         100,0.,0.0001);
      meHBL10Ene_ = dbe_->book1D("Hit41","Log10Energy in HB", 140, -10., 4. );
      meHEL10Ene_ = dbe_->book1D("Hit42","Log10Energy in HE", 140, -10., 4. );
      meHFL10Ene_ = dbe_->book1D("Hit43","Log10Energy in HF", 140, -10., 4. );
      meHOL10Ene_ = dbe_->book1D("Hit44","Log10Energy in HO", 140, -10., 4. );
      meHBL10EneP_ = dbe_->bookProfile("Hit45","Log10Energy in HB vs Hit contribution", 140, -10., 4., 100, 0., 1. );
      meHEL10EneP_ = dbe_->bookProfile("Hit46","Log10Energy in HE vs Hit contribution", 140, -10., 4., 100, 0., 1. );
      meHFL10EneP_ = dbe_->bookProfile("Hit47","Log10Energy in HF vs Hit contribution", 140, -10., 4., 100, 0., 1. );
      meHOL10EneP_ = dbe_->bookProfile("Hit48","Log10Energy in HO vs Hit contribution", 140, -10., 4., 100, 0., 1. );
    }
  }
}

void HcalSimHitStudy::endJob() {
  if (dbe_ && outFile_.size() > 0) dbe_->save(outFile_);
}

void HcalSimHitStudy::analyze(const edm::Event& e, const edm::EventSetup& ) {

  LogDebug("HcalSim") << "Run = " << e.id().run() << " Event = " 
		      << e.id().event();

  std::vector<PCaloHit>               caloHits;
  edm::Handle<edm::PCaloHitContainer> hitsHcal;

  bool getHits = false;
  if (checkHit_) {
    e.getByToken(tok_hits_,hitsHcal); 
    if (hitsHcal.isValid()) getHits = true;
  }

  LogDebug("HcalSim") << "HcalValidation: Input flags Hits " << getHits;

  if (getHits) {
    caloHits.insert(caloHits.end(),hitsHcal->begin(),hitsHcal->end());
    LogDebug("HcalSim") << "HcalValidation: Hit buffer " 
			<< caloHits.size(); 
    analyzeHits (caloHits);
  }
}

void HcalSimHitStudy::analyzeHits (std::vector<PCaloHit>& hits) {

  int nHit = hits.size();
  int nHB=0, nHE=0, nHO=0, nHF=0, nBad1=0, nBad2=0, nBad=0;
  std::vector<double> encontHB(140, 0.);
  std::vector<double> encontHE(140, 0.);
  std::vector<double> encontHF(140, 0.);
  std::vector<double> encontHO(140, 0.);
  double entotHB = 0, entotHE = 0, entotHF = 0, entotHO = 0; 

  for (int i=0; i<nHit; i++) {
    double energy    = hits[i].energy();
    double log10en   = log10(energy);
    int log10i       = int( (log10en+10.)*10. );
    double time      = hits[i].time();
    unsigned int id_ = hits[i].id();
    HcalDetId id     = HcalDetId(id_);
    int det          = id.det();
    int subdet       = id.subdet();
    int depth        = id.depth();
    int eta          = id.ieta();
    int phi          = id.iphi();
    LogDebug("HcalSim") << "Hit[" << i << "] ID " << std::hex << id_ 
			<< std::dec << " Det " << det << " Sub " 
			<< subdet << " depth " << depth << " Eta " << eta
			<< " Phi " << phi << " E " << energy << " time " 
			<< time;
    if (det ==  4) { // Check DetId.h
      if      (subdet == static_cast<int>(HcalBarrel))  nHB++;
      else if (subdet == static_cast<int>(HcalEndcap))  nHE++;
      else if (subdet == static_cast<int>(HcalOuter))   nHO++;
      else if (subdet == static_cast<int>(HcalForward)) nHF++;
      else    { nBad++;  nBad2++;}
    } else    { nBad++;  nBad1++;}
    if (dbe_) {
      meDetectHit_->Fill(double(det));
      if (det ==  4) {
	meSubdetHit_->Fill(double(subdet));
	meDepthHit_->Fill(double(depth));
	meEtaHit_->Fill(double(eta));
	mePhiHit_->Fill(double(phi));
	meEnergyHit_->Fill(energy);
	meTimeHit_->Fill(time);
	meTimeWHit_->Fill(double(time),energy);
	if      (subdet == static_cast<int>(HcalBarrel)) {
	  meHBDepHit_->Fill(double(depth));
	  meHBEtaHit_->Fill(double(eta));
	  meHBPhiHit_->Fill(double(phi));
	  meHBEneHit_->Fill(energy);
	  meHBEneHit2_->Fill(energy);
	  meHBTimHit_->Fill(time);
	  meHBL10Ene_->Fill(log10en);
	  if( log10i >=0 && log10i < 140 ) encontHB[log10i] += energy;
	  entotHB += energy;
	} else if (subdet == static_cast<int>(HcalEndcap)) {
	  meHEDepHit_->Fill(double(depth));
	  meHEEtaHit_->Fill(double(eta));
	  meHEPhiHit_->Fill(double(phi));
	  meHEEneHit_->Fill(energy);
	  meHEEneHit2_->Fill(energy);
	  meHETimHit_->Fill(time);
	  meHEL10Ene_->Fill(log10en);
	  if( log10i >=0 && log10i < 140 ) encontHE[log10i] += energy;
	  entotHE += energy;
	} else if (subdet == static_cast<int>(HcalOuter)) {
	  meHODepHit_->Fill(double(depth));
	  meHOEtaHit_->Fill(double(eta));
	  meHOPhiHit_->Fill(double(phi));
	  meHOEneHit_->Fill(energy);
	  meHOEneHit2_->Fill(energy);
	  meHOTimHit_->Fill(time);
	  meHOL10Ene_->Fill(log10en);
	  if( log10i >=0 && log10i < 140 ) encontHO[log10i] += energy;
	  entotHO += energy;
	} else if (subdet == static_cast<int>(HcalForward)) {
	  meHFDepHit_->Fill(double(depth));
	  meHFEtaHit_->Fill(double(eta));
	  meHFPhiHit_->Fill(double(phi));
	  meHFEneHit_->Fill(energy);
	  meHFEneHit2_->Fill(energy);
	  meHFTimHit_->Fill(time);
	  meHFL10Ene_->Fill(log10en);
	  if( log10i >=0 && log10i < 140 ) encontHF[log10i] += energy;
	  entotHF += energy;
	}
      }
    }
  }
  if( entotHB != 0 ) for( int i=0; i<140; i++ ) meHBL10EneP_->Fill( -10.+(float(i)+0.5)/10., encontHB[i]/entotHB );
  if( entotHE != 0 ) for( int i=0; i<140; i++ ) meHEL10EneP_->Fill( -10.+(float(i)+0.5)/10., encontHE[i]/entotHE );
  if( entotHF != 0 ) for( int i=0; i<140; i++ ) meHFL10EneP_->Fill( -10.+(float(i)+0.5)/10., encontHF[i]/entotHF );
  if( entotHO != 0 ) for( int i=0; i<140; i++ ) meHOL10EneP_->Fill( -10.+(float(i)+0.5)/10., encontHO[i]/entotHO );

  if (dbe_) {
    meAllNHit_->Fill(double(nHit));
    meBadDetHit_->Fill(double(nBad1));
    meBadSubHit_->Fill(double(nBad2));
    meBadIdHit_->Fill(double(nBad));
    meHBNHit_->Fill(double(nHB));
    meHENHit_->Fill(double(nHE));
    meHONHit_->Fill(double(nHO));
    meHFNHit_->Fill(double(nHF));
  }
  LogDebug("HcalSim") << "HcalSimHitStudy::analyzeHits: HB " << nHB 
		      << " HE " << nHE << " HO " << nHO << " HF " << nHF 
		      << " Bad " << nBad << " All " << nHit;

}

