//
// Created by Ricardo on 6/30/2021.
//

#include "OPAsyncTask.hpp"

OPCPUAsyncTask::OPCPUAsyncTask(size_t workerCount, const OPCPUAsyncTask::WorkerCallback& task) {
    setTask(workerCount, task);
}

OPCPUAsyncTask::~OPCPUAsyncTask() {
    join();
}

OPCPUAsyncTask::OPCPUAsyncTask() {

}

void OPCPUAsyncTask::setTask(size_t workerCount, const OPCPUAsyncTask::WorkerCallback &task) {
    join();
    _workers.resize(workerCount);
    for (int i = 0; i < _workers.size(); i++){
        _workers[i] = std::make_shared<Worker>(i, task, this);
    }
}

void OPCPUAsyncTask::dispatch() {
    for (auto& worker : _workers) {
        worker->dispatch();
    }
}

void OPCPUAsyncTask::join() {
    std::unique_lock<std::mutex> lock(_workerMutex);
    workerFinished.wait(lock, [this](){ return idle(); });
}

bool OPCPUAsyncTask::idle() const {
    return std::all_of(_workers.begin(), _workers.end(), [](const std::shared_ptr<Worker>& worker) { return worker->idle(); });
}

OPCPUAsyncTask::Worker::Worker(size_t index, OPCPUAsyncTask::WorkerCallback callback, OPCPUAsyncTask* owner) :
    _index(index),
    _callback(std::move(callback)),
    _owner(owner) {

}

OPCPUAsyncTask::Worker::~Worker() {
    if (_thread.joinable()){
        _thread.join();
    }
}

bool OPCPUAsyncTask::Worker::idle() const {
    return _idle;
}

void OPCPUAsyncTask::Worker::dispatch() {
    if (_thread.joinable()){
        _thread.join();
    }
    {
        std::unique_lock<std::mutex> lock(_owner->_workerMutex);
        _idle = false;
    }
    _thread = std::thread([this]() {
        _callback(_index, _owner->_workers.size());

        {
            std::unique_lock<std::mutex> lock(_owner->_workerMutex);
            _idle = true;
        }
        _owner->workerFinished.notify_all();
    });
}

