//
// Created by lobis on 10/11/2021.
//

#include "OutputManager.h"

#include <TRestGeant4DataEvent.h>
#include <TRestGeant4DataSteps.h>
#include <TRestGeant4DataTrack.h>
#include <spdlog/spdlog.h>

#include <G4RunManager.hh>
#include <G4Step.hh>
#include <G4Threading.hh>
#include <G4Track.hh>
#include <G4UserEventAction.hh>

#include "GlobalManager.h"

using namespace std;

thread_local OutputManager* OutputManager::pinstance_ = nullptr;

OutputManager* OutputManager::Instance() {
    if (G4Threading::IsMasterThread() && G4Threading::IsMultithreadedApplication()) {
        cout << "OutputManager::Instance() - Thread local instance should never be invoked from master "
                "thread in "
                "a MT application"
             << endl;
    }
    if (pinstance_ == nullptr) {
        pinstance_ = new OutputManager();
    }
    return pinstance_;
}

void OutputManager::UpdateEvent() {
    auto event = G4EventManager::GetEventManager()->GetConstCurrentEvent();
    fEvent = make_unique<TRestGeant4DataEvent>(event);
}

void OutputManager::FinishAndSubmitEvent() {
    spdlog::debug("OutputManager::FinishAndSubmitEvent");

    spdlog::info("OutputManager::FinishAndSubmitEvent ---> Finished event ID {} with {} tracks and {} steps",
                 fEvent->GetEventID(), fEvent->GetNumberOfTracks(), fEvent->GetNumberOfSteps());

    if (IsEmptyEvent()) {
        return;
    }

    //  print useful end of event info
    /*
    spdlog::debug(
        "OutputManager::FinishAndSubmitEvent - EventID {} - SubEventID {} - Sensitive volume energy: "
        "{:03.2f}",
        fEvent->fEventID, fEvent->fSubEventID, fEvent->fSensitiveVolumeEnergy);
    */
    if (IsValidEvent()) {
        spdlog::info(
            "OutputManager::FinishAndSubmitEvent - Added valid event with ID {} and sensitive volume energy "
            "{}",
            fEvent->GetEventID(), fEvent->GetSensitiveVolumeEnergy());
        GlobalManager::Instance()->InsertEvent(fEvent);
    }
    UpdateEvent();
}

void OutputManager::RecordStep(const G4Step* step) { fEvent->InsertStep(step); }

/*!
 * Inserts track into current event.
 * This should be called from tracking action at the start of tracking
 * The track can be updated afterwards i.e. with secondary information
 */
void OutputManager::RecordTrack(const G4Track* track) {
    // spdlog::debug("OutputManager::RecordTrack - Track ID {}", track->GetTrackID());
    fEvent->InsertTrack(track);
}

/*!
 * Updates latest track with never information only available at the end of tracking, such as secondaries
 * It should be called at the end of tracking action
 */
void OutputManager::UpdateTrack(const G4Track* track) { fEvent->UpdateTrack(track); }

bool OutputManager::IsEmptyEvent() const { return !fEvent || fEvent->IsEmpty(); }

bool OutputManager::IsValidEvent() const {
    if (IsEmptyEvent()) return false;
    if (GlobalManager::Instance()->GetSaveAllEventsFlag()) return true;
    if (fEvent->GetSensitiveVolumeEnergy() <= 0) return false;
    return true;
}