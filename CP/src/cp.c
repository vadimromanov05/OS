#include "../include/cp.h"

#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <unordered_set>
#include <yaml-cpp/yaml.h>

bool is_cyclic_util(const int node, std::unordered_map<int, std::vector<int>>& graph,
        std::unordered_map<int, bool>& visited, std::unordered_map<int, bool>& recStack) {
    if (!visited[node]) {
        visited[node] = true;
        recStack[node] = true;

        for (const auto& neighbor : graph[node]) {
            if (!visited[neighbor] && is_cyclic_util(neighbor, graph, visited, recStack) || recStack[neighbor])
                return true;
        }
    }

    recStack[node] = false;
    return false;
}

bool is_cyclic(std::unordered_map<int, std::vector<int>>& graph) {
    std::unordered_map<int, bool> visited, recStack;

    for (const auto& [fst, _] : graph) {
        if (is_cyclic_util(fst, graph, visited, recStack))
            return true;
    }

    return false;
}

bool check_graph(std::unordered_map<int, std::vector<int>>& graph) {
    std::unordered_set<int> ids;

    for (auto& [fst, _] : graph) {
        ids.insert(fst);
    }

    for (auto& [_, snd] : graph) {
        for (auto id : snd) {
            if (ids.find(id) == ids.end()) {
                std::cerr << "Error occured. Graph has undefined ids" << std::endl;
                return false;
            }
        }
    }

    std::vector<int> startJobs, endJobs;

    if (is_cyclic(graph)) {
        std::cerr << "Error occured. Graph is cyclic" << std::endl;
        return false;
    }

    for (const auto&[fst, snd] : graph) {
        auto end = true;
        for (const auto&[fst, snd] : graph) {
            if (std::find(snd.begin(), snd.end(), fst) != snd.end()) {
                end = false;
                break;
            }
        }
        if (end) {
            endJobs.push_back(fst);
        }
    }

    if (endJobs.empty()) {
        std::cerr << "Error occured. No end jobs found" << std::endl;
        return false;
    }

    for (const auto& [job, deps] : graph) {
        if (deps.empty()) {
            startJobs.push_back(job);
        }
    }

    if (startJobs.empty()) {
        std::cerr << "Error occured. No start jobs found" << std::endl;
        return false;
    }

    std::unordered_set<int> visited;
    std::queue<int> q;

    if (!endJobs.empty()) {
        q.push(endJobs[0]);
        visited.insert(endJobs[0]);
    }

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        for (const auto& neighbor : graph[current]) {
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                q.push(neighbor);
            }
        }
    }

    if (visited.size() != graph.size()) {
        std::cerr << "Error occured. Graph isn't a single connected component" << std::endl;
        return false;
    }

    return true;
}

void exec(const std::string& jobName, int jobId, pthread_barrier_t* barrier, std::atomic<bool>& errorFlag, const int execTime = 2) {
    if (mustBreak==jobName) {
        errorFlag=true;
    }

    if (barrier) {
        pthread_barrier_wait(barrier);
    }

    pthread_mutex_lock(&stdoutMutex);

    std::cout << jobName << " (" << jobId << ") starts, it's id = " << jobId << ";" << std::endl;

    pthread_mutex_unlock(&stdoutMutex);

    std::this_thread::sleep_for(std::chrono::seconds(execTime));

    pthread_mutex_lock(&stdoutMutex);

    if (mustBreak == jobName) {
        std::cerr << jobName << " (" << jobId << ") went wrong" << std::endl;
        pthread_cond_broadcast(&queueCV);
    } else {
        std::cout << jobName << " (" << jobId << ") finished;" << std::endl;
    }

    pthread_mutex_unlock(&stdoutMutex);
}

void *thread_process(void*) {
    while (true) {
        int currentJob = -1;

        pthread_mutex_lock(&queueMutex);

        while (readyJobs.empty() && !done && !errorFlag) {
            pthread_cond_wait(&queueCV, &queueMutex);
        }

        if (done || errorFlag) {
            pthread_mutex_unlock(&queueMutex);
            break;
        }

        if (!readyJobs.empty()) {
            currentJob = readyJobs.front();
            readyJobs.pop();
            ++activeJobs;
        }

        pthread_mutex_unlock(&queueMutex);

        if (currentJob != -1) {
            pthread_barrier_t *barrier = nullptr;

            if (!jobs[currentJob].barrierName.empty()) {
                barrier = &barriers[jobs[currentJob].barrierName];
            }
            exec(jobs[currentJob].name, currentJob, barrier, errorFlag, jobs[currentJob].time);

            pthread_mutex_lock(&queueMutex);

            --activeJobs;

            for (const auto &[job, deps]: graph) {
                if (auto it = std::find(deps.begin(), deps.end(), currentJob);
                        it != deps.end()) {
                    inDegree[job]--;

                    if (inDegree[job] == 0) {
                        readyJobs.push(job);
                        pthread_cond_signal(&queueCV);
                    }
                }
            }

            if (activeJobs == 0 && readyJobs.empty()) {
                done = true;
                pthread_cond_broadcast(&queueCV);
            }

            pthread_mutex_unlock(&queueMutex);
        }
    }

    return nullptr;
}