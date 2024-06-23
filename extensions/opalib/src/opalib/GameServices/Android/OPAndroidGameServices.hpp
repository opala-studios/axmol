//
// Created by Ricardo on 01/02/22.
//

#ifndef OPANDROID_GAME_SERVICES_HPP
#define OPANDROID_GAME_SERVICES_HPP

#include <opalib/GameServices/OPGameServices.hpp>

class OPAndroidGameServices : public OPGameServices {
public:
    OPAndroidGameServices();

    void initialize() override;

    void signIn(const OPGameServices::SignInCallback &callback) override;

    void loadSnapshotMetadata(const OPGameServices::LoadSnapshotMetadataCallback& callback) override;

    void saveSnapshotContent(const std::string& snapshotName, void* data, size_t size, const SaveSnapshotContentCallback& callback) override;

    void loadSnapshotContent(const std::string &snapshotName,
                             const LoadSnapshotContentCallback &callback) override;

    void onSignInResult(ResultCode resultCode, OPGameServices::Account* account);
    void onSnapshotContentLoaded(ResultCode code, void *bytes, size_t byteCount);
    void onSnapshotMetadataLoaded(const std::string& snapshotName, long timestamp);
    void onSnapshotMetadataLoadedCompleted(ResultCode code);
    void onSnapshotContentSaved(ResultCode resultCode);
private:
    SignInCallback _signInCallback;
    LoadSnapshotMetadataCallback  _loadSnapshotMetadataCallback;
    SaveSnapshotContentCallback _saveSnapshotContentCallback;
    LoadSnapshotContentCallback _loadSnapshotContentCallback;
    bool _signingIn = false;
    bool _initialized = false;
};


#endif //OPANDROID_GAME_SERVICES_HPP
