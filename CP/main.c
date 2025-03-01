#include "include/cp.h"

#include <iostream>
#include <fstream>
#include <yaml-cpp/yaml.h>
#include <unordered_set>
#include <sstream>
#include <map>

std::string GetConfigFileName() {
    std::string configFile;
    std::cout << "Enter the path to the configuration file: ";
    std::getline(std::cin, configFile);
    return configFile;
}

int main(int argc, char* argv[]) {
    std::string configFile;
    int maxParallelJobs;
    std::string mustBreakInput;

    if (argc >= 2) {
        configFile = argv[1];
    } else {
        configFile = GetConfigFileName();
    }

    if (argc >= 3) {
        try {
            maxParallelJobs = std::stoi(argv[2]);
        } catch (std::invalid_argument&) {
            std::cerr << "Error occured: Incorrect args types" << std::endl;
            return 1;
        }
        if (argc >= 4) {
            mustBreakInput = argv[3];
        }
    } else {
        std::string input;
        std::cout << "Enter the maximum number of parallel jobs: ";
        std::getline(std::cin, input);

        std::istringstream iss(input);
        iss >> maxParallelJobs >> mustBreakInput;
    }

    if (!mustBreakInput.empty()) {
        mustBreak = mustBreakInput;
    }

    std::ifstream file(configFile);
    if (!file.is_open()) {
        std::cerr << "Error occured: Cannot open file " << configFile << std::endl;
        return 1;
    }

    YAML::Node config = YAML::Load(file);
    file.close();

    std::unordered_set<int> ids;
    std::map<std::string, int> barrierUsage;

    for (const auto& job : config["jobs"]) {
        int id;
        try {
            id = job["id"].as<int>();

            if (ids.find(id) != ids.end()) {
                std::cerr << "Error occured: Graph contains duplicated ids" << std::endl;
                return 1;
            }

            ids.insert(id);
            std::string name = job["name"].as<std::string>();
            jobs[id] = {name, job["dependencies"].as<std::vector<int>>(), job["barrier"].as<std::string>(), job["time"].as<int>()};
        } catch (YAML::Exception&) {
            std::cerr << "Error occured: Parsing YAML failed" << std::endl;
            return 1;
        }
        graph[id] = job["dependencies"].as<std::vector<int>>();

        if (job["barrier"]) {
            const std::string barrier_name = job["barrier"].as<std::string>();
            barrierUsage[barrier_name]++;
        }
    }

    for (const auto& job : config["jobs"]) {
        if (job["barrier"]) {
            const std::string barrier_name = job["barrier"].as<std::string>();
            int barrier_count = barrierUsage[barrier_name];

            if (job["barrier_count"]) {
                barrier_count = job["barrier_count"].as<int>();
                if (barrier_count > barrierUsage[barrier_name]) {
                    std::cerr << "Error occured: barrier_count for " << barrier_name << " is greater than the number of jobs using it" << std::endl;
                    return 1;
                }
                if (barrier_count <= 0) {
                    std::cerr << "Error occured: barrier_count for " << barrier_name << " must be greater than 0" << std::endl;
                    return 1;
                }
            }

            if (barriers.find(barrier_name) == barriers.end()) {
                pthread_barrier_t barrier;
                pthread_barrier_init(&barrier, nullptr, barrier_count);
                barriers.emplace(barrier_name, barrier);
            }
        }
    }

    if (!check_graph(graph)) {
        return 1;
    }

    for (const auto& [job, deps] : graph) {
        inDegree[job] = deps.size();
        if (deps.empty()) {
            readyJobs.push(job);
        }
    }

    std::vector<pthread_t> workers;
    for (int i = 0; i < maxParallelJobs; ++i) {
        pthread_t thread;
        pthread_create(&thread, nullptr, thread_process, nullptr);
        workers.emplace_back(thread);
    }

    pthread_cond_broadcast(&queueCV);

    for (auto& worker : workers) {
        pthread_join(worker, nullptr);
    }

    for (auto& [_, barrier] : barriers) {
        pthread_barrier_destroy(&barrier);
    }

    std::cout << (errorFlag ? "Failure" : "Success") << std::endl;

    return 0;
}