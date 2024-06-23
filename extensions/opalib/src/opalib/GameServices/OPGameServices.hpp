//
// Created by Ricardo on 31/01/22.
//

#ifndef OP_GAMESERVICES_HPP
#define OP_GAMESERVICES_HPP

#include <functional>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <memory>

class OPGameServices {
public:
    enum ResultCode {
        SUCCESS = 0,
        FAILED_UNKNOWN = 1,
        FAILED_NOT_INITIALIZED = 2,
        FAILED_ALREADY_SIGNING_IN = 3,
        FAILED_NOT_SIGNED_IN = 4,
        FAILED_ALREADY_SIGNED_IN = 5,
        FAILED_SNAPSHOT_NAME_NOT_FOUND = 6
    };

    class Snapshot {
    public:
        Snapshot(std::string name, long timestamp) :
        _name(name), _timestamp(timestamp) {}
    private:
        std::string _name;
        long _timestamp;
    };

    class Account {
    public:

        Account(std::string id, std::string userName) :
                _id(id), _userName(userName){}

        void addSnapshot(std::string name, long timestamp){
            _snapshots[name] = std::unique_ptr<Snapshot>(new Snapshot(name, timestamp));
        }

    private:
        std::string _id;
        std::string _userName;
        std::unordered_map<std::string, std::unique_ptr<Snapshot>> _snapshots;
    };

    typedef std::function<void(ResultCode, Account*)> SignInCallback;
    typedef std::function<void(ResultCode)> LoadSnapshotMetadataCallback;
    typedef std::function<void(ResultCode)> SaveSnapshotContentCallback;
    typedef std::function<void(ResultCode, void*, size_t)> LoadSnapshotContentCallback;

    static OPGameServices* shared();

    virtual ~OPGameServices() = default;

    virtual void initialize() = 0;

    virtual void signIn(const SignInCallback& callback) = 0;

    virtual void loadSnapshotMetadata(const LoadSnapshotMetadataCallback& callback) = 0;

    virtual void saveSnapshotContent(const std::string &snapshotName, void* data, size_t size, const SaveSnapshotContentCallback& callback) = 0;

    virtual void loadSnapshotContent(const std::string &snapshotName, const LoadSnapshotContentCallback& callback) = 0;

protected:
    std::unique_ptr<Account> _signedAccount;
};


#endif //OP_GAMESERVICES_HPP
