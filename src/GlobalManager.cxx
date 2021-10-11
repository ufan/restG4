//
// Created by lobis on 10/11/2021.
//

#include "GlobalManager.h"

#include <TRestGeant4Metadata.h>
#include <TRestGeant4PhysicsLists.h>
#include <TRestRun.h>

#include <G4RunManager.hh>
#include <G4Threading.hh>

using namespace std;

GlobalManager* GlobalManager::pinstance_ = nullptr;

GlobalManager* GlobalManager::Instance() {
    if (!pinstance_) {
        pinstance_ = new GlobalManager();
    }
    return pinstance_;
}

GlobalManager::GlobalManager()
    : fRestGeant4Metadata(nullptr), fRestRun(nullptr), fRestGeant4PhysicsLists(nullptr) {
    // Master thread should create the GlobalManager, worker threads should spawn after the manager is created
    // by the master thread
    if (!G4Threading::IsMasterThread()) {
        cout
            << "GlobalManager::GlobalManager - ERROR GlobalManager should never be created by a worker thread"
            << endl;
        exit(1);
    }
}

GlobalManager::~GlobalManager() {
    cout << "GlobalManager::~GlobalManager (Destructor)" << endl;
    // delete fRun;
}

void GlobalManager::InitializeFromConfigFile(const TString& rmlFile) {
    if (fRestGeant4Metadata || fRestRun || fRestGeant4PhysicsLists) {
        cout << "GlobalManager::InitializeRestGeant4Metadata - ERROR rest classes should not be initialized "
                "twice"
             << endl;
        exit(1);
    }

    fInputConfigFile = rmlFile;

    InitializeRestGeant4Metadata(fInputConfigFile);
    CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
    CLHEP::HepRandom::setTheSeed(fRestGeant4Metadata->GetSeed());

    InitializeRestGeant4PhysicsLists(fInputConfigFile);

    InitializeRestRun(fInputConfigFile);
}

void GlobalManager::InitializeRestGeant4Metadata(const TString& rmlFile) {
    fRestGeant4Metadata = new TRestGeant4Metadata(const_cast<char*>(rmlFile.Data()));

    string geant4Version = TRestTools::Execute("geant4-config --version");
    fRestGeant4Metadata->SetGeant4Version(geant4Version);
}

void GlobalManager::InitializeRestRun(const TString& rmlFile) {
    fRestRun = new TRestRun();
    fRestRun->LoadConfigFromFile(const_cast<char*>(rmlFile.Data()));
    TString runTag = fRestRun->GetRunTag();
    if (runTag == "Null" || runTag == "") {
        fRestRun->SetRunTag(fRestGeant4Metadata->GetTitle());
    }

    fRestRun->SetRunType("restG4");

    fRestRun->AddMetadata(fRestGeant4Metadata);
    fRestRun->AddMetadata(fRestGeant4PhysicsLists);

    fRestRun->PrintMetadata();

    fRestRun->FormOutputFile();
}

void GlobalManager::InitializeRestGeant4PhysicsLists(const TString& rmlFile) {
    fRestGeant4PhysicsLists = new TRestGeant4PhysicsLists(const_cast<char*>(rmlFile.Data()));
}

size_t GlobalManager::InsertEvent(std::unique_ptr<TRestGeant4Event>& event) {
    fEventContainerMutex.lock();
    fEventContainer.push(std::move(event));
    auto size = fEventContainer.size();
    fEventContainerMutex.unlock();
    return size;
}
