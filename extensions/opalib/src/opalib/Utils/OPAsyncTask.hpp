//
// Created by Ricardo on 6/30/2021.
//

#ifndef OPASYNCTASK_HPP
#define OPASYNCTASK_HPP

#include <functional>
#include <thread>
#include <vector>
#include <memory>

class OPBaseAsyncTask {
public:
    virtual ~OPBaseAsyncTask() = default;
    virtual void dispatch() = 0;
    virtual void join() = 0;
};

class OPCPUAsyncTask : public OPBaseAsyncTask {
public:
    typedef std::function<void(size_t workerIndex, size_t workerCount)> WorkerCallback;
public:
    OPCPUAsyncTask();
    OPCPUAsyncTask(size_t workerCount, const WorkerCallback& callback);
    ~OPCPUAsyncTask() override;

    void setTask(size_t workerCount, const WorkerCallback& task);

    void dispatch() override;
    void join() override;
    bool idle() const;
    size_t workerCount() const { return _workers.size(); }
private:

    class Worker {
    public:
        Worker(size_t index, WorkerCallback callback, OPCPUAsyncTask* owner);
        ~Worker();
        bool idle() const;
        void dispatch();
    private:
        friend OPCPUAsyncTask;
        size_t _index;
        WorkerCallback _callback;
        OPCPUAsyncTask* _owner;
        std::thread _thread;
        bool _idle = true;
    };

private:
    friend Worker;
    std::vector<std::shared_ptr<Worker>> _workers;
    std::mutex _workerMutex;
    std::condition_variable workerFinished;
};


//soon tm
//class OPGPUAsyncTask : public OPBaseAsyncTask {
//
//};


#endif //OPASYNCTASK_HPP
