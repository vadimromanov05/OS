#include "../include/tools.h"
#include <iostream>
#include <fstream>
#include <unordered_set>
#include <sstream>
#include <map>
#include "../inih/cpp/INIReader.h"


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

    // Используем INIReader для чтения конфигурации
    INIReader reader(configFile);
    if (reader.ParseError() < 0) {
        std::cerr << "Error occured: Cannot open file " << configFile << std::endl;
        return 1;
    }

    std::unordered_set<int> ids;
    std::map<std::string, int> barrierUsage;

    // Чтение данных о заданиях из INI-файла
    int jobCount = reader.GetInteger("jobs", "count", 0);
    for (int i = 0; i < jobCount; ++i) {
        std::string section = "job" + std::to_string(i);
        
        int id = reader.GetInteger(section, "id", -1);
        if (id == -1) {
            std::cerr << "Error occured: required field 'id' is missing in section " << section << std::endl;
            return 1;
        }

        if (ids.find(id) != ids.end()) {
            std::cerr << "Error occured: Graph contains duplicated ids" << std::endl;
            return 1;
        }

        ids.insert(id);
        std::string name = reader.Get(section, "name", "");
        std::string dependenciesStr = reader.Get(section, "dependencies", "");
        std::vector<int> dependencies;
        std::istringstream depStream(dependenciesStr);
        int depId;
        while (depStream >> depId) {
            dependencies.push_back(depId);
            if (depStream.peek() == ',') depStream.ignore();
        }
        std::string barrier = reader.Get(section, "barrier", "");
        int time = reader.GetInteger(section, "time", -1);

        // Здесь нужно создать структуры, которые у вас были до этого
        jobs[id] = {name, dependencies, barrier, time};
        graph[id] = dependencies;

        if (!barrier.empty()) {
            barrierUsage[barrier]++;
        }
    }

    // Проверяем инициализацию барьеров и связи
    for (const auto& job : graph) {
        const std::string& barrier_name = job.barriers;
        int barrier_count = barrierUsage[barrier_name];

        if (barrier_count > 0) {
            // логика инициализации барьеров
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