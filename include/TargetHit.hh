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


#ifndef TargetHit_h
#define TargetHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"


class TargetHit : public G4VHit
{
    public:
        TargetHit();
        TargetHit(const TargetHit&);
        virtual ~TargetHit();

        // operators
        const TargetHit& operator=(const TargetHit&);
        G4int operator==(const TargetHit&) const;

        inline void* operator new(size_t);
        inline void  operator delete(void*);

        // Set methods
        void SetTrackID (G4int data) {trackID = data;};
        void SetEdep (G4double data) {edep = data;};
        void SetParticleName (G4String data) {particleName = data;};
        void SetMomentumDirection (G4ThreeVector data) {momentumDirection = data; };

        // Get methods
        G4int GetTrackID() const {return trackID; };
        G4double GetEdep() const {return edep; };
        G4String GetParticleName() const {return particleName; };
        G4ThreeVector GetMomentumDirection() const {return momentumDirection; };
    private:
    
        G4String particleName;
        G4int trackID;
        G4double edep;
        G4ThreeVector momentumDirection;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef G4THitsCollection<TargetHit> TargetHitsCollection;

extern G4ThreadLocal G4Allocator<TargetHit>* TargetHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* TargetHit::operator new(size_t)
{
    if(!TargetHitAllocator)
        TargetHitAllocator = new G4Allocator<TargetHit>;
    return (void *) TargetHitAllocator->MallocSingle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void TargetHit::operator delete(void *hit)
{
  TargetHitAllocator->FreeSingle((TargetHit*) hit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
