#ifndef CP_H
#define CP_H


#include <atomic>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>
#include <pthread.h>

struct Job {
    std::string name;
    std::vector<int> dependencies;
    std::string barrierName;
    int time;
};

inline pthread_mutex_t stdoutMutex = PTHREAD_MUTEX_INITIALIZER;
inline pthread_mutex_t queueMutex = PTHREAD_MUTEX_INITIALIZER;

inline pthread_cond_t queueCV = PTHREAD_COND_INITIALIZER;

inline std::atomic<bool> errorFlag(false);
inline std::atomic<bool> done(false);
inline std::atomic<int> activeJobs(0);

inline std::string mustBreak;

inline std::queue<int> readyJobs;

inline std::unordered_map<int, Job> jobs;
inline std::unordered_map<int, std::vector<int>> graph;
inline std::unordered_map<int, unsigned long> inDegree;
inline std::unordered_map<std::string, pthread_barrier_t> barriers;

bool is_cyclic_util(int node, std::unordered_map<int, std::vector<int>>& graph,
        std::unordered_map<int, bool>& visited, std::unordered_map<int, bool>& recStack);

bool is_cyclic(std::unordered_map<int, std::vector<int>>& graph);

bool check_graph(std::unordered_map<int, std::vector<int>>& graph);

void exec(const std::string& jobName, int jobId, pthread_barrier_t* barrier, std::atomic<bool>& errorFlag, int execTime);

void *thread_process(void*);

#endif