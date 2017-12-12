//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
#include <fstream>
#include <cmath>

#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4ios.hh"
#include "G4ParticleGun.hh"
#include "G4UnitsTable.hh"


#include "LOPutil.hh"
#include "EventAction.hh"
#include "TargetHit.hh"
#include "TargetSD.hh"
#include "PrimaryGeneratorAction.hh"


using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction()
: G4UserEventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event* event)
{
    G4int eventID = event->GetEventID();
    if (eventID % 1000 == 0)
    {
        G4cout << "eventID: " << eventID  << G4endl;
    }
    G4cout << "eventID: " << eventID << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* event)
{
    // get analysis manager
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    
    // Fill the histogram of  response
    G4int HCID = G4SDManager::GetSDMpointer()->GetCollectionID("targetHitsCollection");
    TargetHitsCollection* HC = static_cast<TargetHitsCollection*>(event->GetHCofThisEvent()->GetHC(HCID));
    G4int nofHits = HC->entries();
    if (nofHits == 0)
        return;
        
    G4double threshold = 10;
    G4double Eee = 0.0;
    G4int NumProtonHits = 0;
    G4int NumCarbonHits = 0;
    for (int i = 0; i < nofHits; i++)
    {
        G4String particleName = (*HC)[i]->GetParticleName();
        G4double edep = (*HC)[i]->GetEdep();
        G4ThreeVector momentumDirection =  (*HC)[i]->GetMomentumDirection();
        G4double momentumZ = momentumDirection.getZ();
        
        //  Count the hits on different particles
        if (particleName == "proton")
        {
            NumProtonHits += 1;
        }
        if (particleName == "C12" || particleName == "C13")
        {
            NumCarbonHits += 1;
        }
        
        //  Calculate the pulse height (in unit: keV)
        G4double factorEnergy = 0.0;
        G4double factorAngle = 0.0;
        G4double alpha = 2.0;
        if (particleName != "e+" && particleName != "e-")
        {
            factorEnergy = 0.2947 - 0.00906 * edep / 1000.0;
            factorAngle = pow((1 - momentumZ * momentumZ), alpha / 2.0);
        }
        G4double EeeRaw = LOPutil::GetInstance() -> GetEee(edep, particleName);
        Eee += EeeRaw * (1 + factorEnergy * factorAngle);
    }
    if (Eee > threshold)
    {
        analysisManager->FillNtupleDColumn(0, Eee);
        analysisManager->FillNtupleIColumn(1, NumProtonHits);
        analysisManager->FillNtupleIColumn(2, NumCarbonHits);
        analysisManager->AddNtupleRow();
    }
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
