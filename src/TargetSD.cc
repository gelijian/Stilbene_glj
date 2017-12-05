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

#include "stdio.h"
#include "TargetSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TargetSD::TargetSD(const G4String& name,
                         const G4String& hitsCollectionName) 
 : G4VSensitiveDetector(name),
   fHitsCollection(NULL)
{
  collectionName.insert(hitsCollectionName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TargetSD::~TargetSD() 
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TargetSD::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection
	fHitsCollection  = new TargetHitsCollection(SensitiveDetectorName, collectionName[0]); 

  // Add this collection in hce
	G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
	hce->AddHitsCollection( hcID, fHitsCollection ); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool TargetSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{  
    
	// negelet the wall effect(assuming all the energy of secondary particles desipoed in the detector)
    G4int ParentID = aStep->GetTrack()->GetParentID();
    G4int StepID = aStep->GetTrack()->GetCurrentStepNumber();
    G4String particleName = aStep->GetTrack()->GetDefinition()->GetParticleName();
    if (ParentID != 1 || StepID != 1 || particleName == "neutron" || particleName == "gamma" || particleName == "opticalphoton")
        return false;

    G4int trackID = aStep->GetTrack()->GetTrackID();
    G4double edep = aStep->GetPreStepPoint()->GetKineticEnergy() / keV;
    G4ThreeVector momentumDirection =  aStep->GetPreStepPoint()->GetMomentumDirection();
    
    TargetHit* newHit = new TargetHit();
    newHit->SetParticleName(particleName);
	newHit->SetTrackID(trackID);
	newHit->SetEdep(edep);
	newHit->SetMomentumDirection(momentumDirection);

	fHitsCollection->insert(newHit);
    return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TargetSD::EndOfEvent(G4HCofThisEvent*)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
