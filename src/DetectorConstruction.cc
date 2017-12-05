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

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4GeometryTolerance.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SystemOfUnits.hh"

#include "DetectorConstruction.hh"
#include "TargetSD.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
    :G4VUserDetectorConstruction(), 
    expHallLV(NULL),
    targetLV(NULL),
    shellsideLV(NULL),
    shellfrontLV(NULL),
    pyrexWindowLV(NULL),
    phcathLV(NULL),
    expHallPV(NULL),
    targetPV(NULL),
    shellsidePV(NULL),
    shellfrontPV(NULL),
    pyrexWindowPV(NULL),
    phcathPV(NULL),
    ftargetMaterial(NULL), 
    fworldMaterial(NULL),
    fshellMaterial(NULL), 
    fpyrexWindowMaterial(NULL),
    fphcathMaterial(NULL),
    fCheckOverlaps(true)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
DetectorConstruction::~DetectorConstruction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
G4VPhysicalVolume* DetectorConstruction::Construct()
{
    // Define materials
    DefineMaterials();
    
    // Define volumes
    return DefineVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineMaterials()
{
    // Material definition 
    G4NistManager* nistManager = G4NistManager::Instance();
    nistManager->SetVerbose(1);
    G4double density;
    G4int nel;
    
    // Element
    G4Element* H = nistManager->FindOrBuildElement("H");
    G4Element* C = nistManager->FindOrBuildElement("C");
    G4Element* B = nistManager->FindOrBuildElement("B");
    G4Element* O = nistManager->FindOrBuildElement("O");
    G4Element* K = nistManager->FindOrBuildElement("K");
    G4Element* Na = nistManager->FindOrBuildElement("Na");
    G4Element* Al = nistManager->FindOrBuildElement("Al");
    G4Element* Si = nistManager->FindOrBuildElement("Si");

    //Air
    G4Material* fairMaterial = nistManager->FindOrBuildMaterial("G4_AIR");

    // Al
    G4Material* fAlMaterial = nistManager->FindOrBuildMaterial("G4_Al");
    
    // EJ301
    G4Material* fEJ301Material = new G4Material("EJ301Material", density = 0.874*g/cm3, nel = 2, kStateLiquid);
    fEJ301Material->AddElement(H, 8.476 * perCent);
    fEJ301Material->AddElement(C, 91.524 * perCent);
    
    // Stilbene Crystal
    G4Material* fstilbeneMaterial = new G4Material("stilbeneMaterial", density= 1.22*g/cm3, nel=2);
    fstilbeneMaterial->AddElement(H, 6.711*perCent);
    fstilbeneMaterial->AddElement(C, 93.289*perCent);

    // Borosilicate glass
    G4Material* fBSiGlassMaterial = new G4Material("BorosilicateGlassMaterial",density= 2.23*g/cm3, nel=6);
    fBSiGlassMaterial->AddElement(B,  0.040064);
    fBSiGlassMaterial->AddElement(O,  0.539561);
    fBSiGlassMaterial->AddElement(Na, 0.028191);
    fBSiGlassMaterial->AddElement(Al, 0.011644);    
    fBSiGlassMaterial->AddElement(Si, 0.377219);
    fBSiGlassMaterial->AddElement(K,  0.003321);

    // Photocathode glas
    G4Material* fphcathGlassMaterial = new G4Material("PhcathodeGlassMaterial",density= 2.23*g/cm3, nel=6);
    fphcathGlassMaterial->AddElement(B,  0.040064);
    fphcathGlassMaterial->AddElement(O,  0.539561);
    fphcathGlassMaterial->AddElement(Na, 0.028191);
    fphcathGlassMaterial->AddElement(Al, 0.011644); 
    fphcathGlassMaterial->AddElement(Si, 0.377219);
    fphcathGlassMaterial->AddElement(K,  0.003321);

    //Define Material
    fworldMaterial = fairMaterial;
    ftargetMaterial = fstilbeneMaterial;
    fshellMaterial = fAlMaterial;
    fpyrexWindowMaterial = fBSiGlassMaterial;
    fphcathMaterial = fphcathGlassMaterial;
    
  // Print all the materials defined.
    G4cout << G4endl << "The materials defined are : " << G4endl << G4endl;
    G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::DefineVolumes()
{
    //expHall as world
    G4double expHall_x = 50.0 * cm;
    G4double expHall_y = 50.0 * cm;
    G4double expHall_z = 50.0 * cm;
    G4Box* expHallSV = new G4Box("expHallSV", expHall_x, expHall_y, expHall_z);
    expHallLV = new G4LogicalVolume(expHallSV, fworldMaterial, "expHallLV");
    expHallPV = new G4PVPlacement(0,
                                  G4ThreeVector(),
                                  expHallLV,
                                  "expHallPV",
                                  0,
                                  false,
                                  0,
                                  fCheckOverlaps);
    
    // define the size of the scintillation detector
    G4double targetRadius = 2.5 * cm;
    G4double targetLength = 4.0 * cm;
    G4Tubs* targetSV = new G4Tubs("targetSV", 0 * cm, targetRadius, targetLength / 2.0, 0 * deg, 360 * deg);
    targetLV = new G4LogicalVolume(targetSV, ftargetMaterial, "targetLV");
    targetPV = new G4PVPlacement(0,
                                 G4ThreeVector(0 * cm, 0 * cm, 0 * cm),
                                 targetLV,
                                 "targetPV",
                                 expHallLV,
                                 false,
                                 0,
                                 fCheckOverlaps);
    
                                    
    //Al shell including two parts(side and front)
    G4double shellThickness = 1.5 * mm;
    G4double shellRadius = targetRadius + shellThickness;
    G4double shellLength = targetLength;
    
    //side shell
    G4Tubs * shellsideSV = new G4Tubs("shellsideSV", targetRadius, shellRadius, shellLength / 2, 0 * deg, 360 * deg);
    shellsideLV = new G4LogicalVolume(shellsideSV, fshellMaterial, "shellsideLV");
    shellsidePV = new G4PVPlacement(0,
                                    G4ThreeVector(0 * cm, 0 * cm, 0 * cm),
                                    shellsideLV,
                                    "shellsidePV",
                                    expHallLV,
                                    false,
                                    0,
                                    fCheckOverlaps);
    
    //front shell
    G4Tubs * shellfrontSV = new G4Tubs("shellfrontSV", 0 * mm, shellRadius, shellThickness / 2, 0 * deg, 360 * deg);
    G4double shellfontPosition = -(shellLength + shellThickness) / 2;
    shellfrontLV = new G4LogicalVolume(shellfrontSV, fshellMaterial, "shellfrontLV");
    shellfrontPV = new G4PVPlacement(0,
                                     G4ThreeVector(0 * cm, 0 * cm, shellfontPosition),
                                     shellfrontLV,
                                     "shellfrontPV",
                                     expHallLV,
                                     false,
                                     0,
                                     fCheckOverlaps);
    
    
    // Pyrex Glass windows, to PMT
    G4double pyrexWindowRadius = shellRadius;
    G4double pyrexWindowLength = 1.02 * cm;
    G4double pyrexWindowPosition = (targetLength + pyrexWindowLength) / 2;
    G4Tubs* pyrexWindowSV = new G4Tubs("pyrexWindowSV", 0 * mm,  pyrexWindowRadius, pyrexWindowLength / 2, 0 * deg, 360 * deg);
    pyrexWindowLV = new G4LogicalVolume(pyrexWindowSV, fpyrexWindowMaterial, "PyrexWindowLV");
    pyrexWindowPV  = new G4PVPlacement(0,
                                       G4ThreeVector(0 * cm, 0 * cm, pyrexWindowPosition),
                                       pyrexWindowLV,
                                       "PyrexWindowPV",
                                       expHallLV,
                                       false,
                                       0,
                                       fCheckOverlaps);
    // Photocathode of PMT
    G4double phcathRadius = shellRadius;
    G4double phcathLength = 0.02 * cm;
    G4double phcathPosition = pyrexWindowPosition + (pyrexWindowLength + phcathLength) / 2;
    G4Tubs* phcathSV = new G4Tubs("phcathSV", 0 * mm, phcathRadius, phcathLength / 2, 0 * deg, 360 * deg);
    phcathLV = new G4LogicalVolume(phcathSV, fphcathMaterial, "phcathLV");
    phcathPV = new G4PVPlacement(0,
                                 G4ThreeVector(0 * cm, 0 * cm, phcathPosition),
                                 phcathLV,
                                 "phcathPV",
                                 expHallLV,
                                 false,
                                 0,
                                 fCheckOverlaps);
    
    //--------- Visualization attributes -------------------------------
    //G4Colour
    G4Colour white (1.0,1.0,1.0);
    G4Colour grey (0.5,0.5,0.5);
    //G4Colour black (0.0,0.0,0.0);
    G4Colour red (1.0,0.0,0.0);
    G4Colour blue (0.0,0.0,1.0);
    G4Colour green (0.0,1.0,0.0);
    //G4Colour cyan (0.0,1.0,1.0);
    G4Colour magenta (1.0,0.0,1.0);
    //G4Colour yellow(1.0,1.0,0.0);
    G4VisAttributes* expHallVisAtt= new G4VisAttributes(G4VisAttributes::Invisible);
    expHallLV  ->SetVisAttributes(expHallVisAtt);  
    
    G4VisAttributes* targetVisAtt = new G4VisAttributes(red);
    targetLV->SetVisAttributes(targetVisAtt);
    
    G4VisAttributes* shellsideVisAtt = new G4VisAttributes(grey);
    shellsideLV->SetVisAttributes(shellsideVisAtt);
    
    G4VisAttributes* shellfrontVisAtt = new G4VisAttributes(blue);
    shellfrontLV->SetVisAttributes(shellfrontVisAtt);
    
    G4VisAttributes* pyrexWindowVisAtt = new G4VisAttributes(green);
    pyrexWindowLV->SetVisAttributes(pyrexWindowVisAtt);
    
    G4VisAttributes* phcathVisAtt = new G4VisAttributes(magenta);
    phcathLV->SetVisAttributes(phcathVisAtt);
    
    return expHallPV;
}
 
void DetectorConstruction::ConstructSDandField()
{
    // Sensitive detectors
    G4String targetName = "target";
    TargetSD* targetSD = new TargetSD(targetName, "targetHitsCollection");
    SetSensitiveDetector(targetLV, targetSD);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
