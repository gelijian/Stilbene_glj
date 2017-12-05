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

#include "TargetHit.hh"
#include "G4UnitsTable.hh"
#include <iomanip>

G4ThreadLocal G4Allocator<TargetHit>* TargetHitAllocator = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TargetHit::TargetHit()
 :  G4VHit(),
    particleName("undefined"),
    trackID(-1),
    edep(0.),
    momentumDirection(0.0, 0.0, 0.0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TargetHit::~TargetHit() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TargetHit::TargetHit(const TargetHit& right)
  : G4VHit()
{
    particleName = right.particleName;
    trackID = right.trackID;
    edep = right.edep;
    momentumDirection = right.momentumDirection;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const TargetHit& TargetHit::operator=(const TargetHit& right)
{
    particleName = right.particleName;
    trackID = right.trackID;
    edep = right.edep;
    momentumDirection = right.momentumDirection;
    
    return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int TargetHit::operator==(const TargetHit& right) const
{
    return ( this == &right ) ? 1 : 0;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
