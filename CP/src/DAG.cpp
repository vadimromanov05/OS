#include "../include/DAG.h"
#include "../include/ThreadPool.h"
#include "../include/Barrier.h"
#include <iostream>
#include <queue>
#include <valarray>
#include <future>

DAG::DAG(const std::string& ini_file_path) {
    load_from_ini(ini_file_path);
    //std::cout << "dag_load\n";
}

void DAG::load_from_ini(const std::string& ini_file_path) {
    std::ifstream file(ini_file_path);
    std::string line;
    std::string current_job_id;

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == ';') {
            continue; // пропустить пустые строки и комментарии
        }

        if (line.front() == '[' && line.back() == ']') {
            current_job_id = line.substr(1, line.size() - 2); // удаляем скобки
            jobs[current_job_id] = Job(current_job_id, {});   // создаем новый Job без зависимостей
        } else if (current_job_id.size() > 0 && line.find("dependencies") != std::string::npos) {
            std::string deps = line.substr(line.find('=') + 1);
            std::stringstream ss(deps);
            std::string dependency;
            
            //std::cout << "Wir sind in Parser\n";
            //std::cout << deps << '\n';
            while (std::getline(ss, dependency, ',')) {
                //std::cout << dependency << "1\n";
                if (dependency != " "){
                    jobs[current_job_id].dependencies.push_back(dependency);
                }
            }
        }
    }
    //std::cout << "dag_parcer\n";
}


bool DAG::is_valid() const {
    for (const auto& [id, job] : jobs) {
        //std::cout << id << "\nIteration(in valid)!\n";
        std::map<std::string, bool> visited;
        std::map<std::string, bool> rec_stack;
        //std::cout << "Du bist hier(in valid)!\n";
        if (has_cycle(id, visited, rec_stack)) {
            return false;
        }
    }
    //std::cout << "dag_check1\n";

    if (!is_connected()) {
        return false;
    }
    //std::cout << "dag_connection\n";

    if (!has_start_and_end_jobs()) {
        return false;
    }
    //std::cout << "dag_jobs\n";

    return true;
}

bool DAG::has_cycle(const std::string& job_id, std::map<std::string, bool>& visited, std::map<std::string, bool>& rec_stack) const {
    //std::cout << "Start cycle check\n";
    if (!visited[job_id] && job_id != " ") {
        visited[job_id] = true;
        rec_stack[job_id] = true;
        //std::cout << "Sehr gut!\n";
        //std::cout << job_id << '\n';
        if (job_id[0] == ' '){
            std::string buf = job_id.substr(1, job_id.size());
            while (buf[0] == ' '){
                buf = buf.substr(1, buf.size());
            }
            //std::cout << buf << '\n';
            //std::cout << jobs.at(buf).dependencies.empty() << '\n';
            //std::cout << "If has started!\n";
            for (const auto& dep : jobs.at(buf).dependencies) {
                //std::cout << dep << '\n';
                if (!visited[dep] && has_cycle(dep, visited, rec_stack)) {
                    return true;
                } else if (rec_stack[dep]) {
                    return true;
                }
            }

        } else {
            //std::cout << "Verschiss dieser Fehler!\n";
            //std::cout << jobs.at(job_id).dependencies.empty() << '\n';
            //std::cout << "If has started!\n";
            for (const auto& dep : jobs.at(job_id).dependencies) {
                //std::cout << dep << '\n';
                if (!visited[dep] && has_cycle(dep, visited, rec_stack)) {
                    return true;
                } else if (rec_stack[dep]) {
                    return true;
                }
            }
        } 
    }
    //std::cout << "dag_cycle\n";
    rec_stack[job_id] = false;
    return false;
}

bool DAG::is_connected() const {
    if (jobs.empty()) {
        return true;
    }
    //std::cout << "dag_full\n";

    std::map<std::string, bool> visited;

    for (const auto& [id, job] : jobs) {
        //std::cout << "Check_size: " << id << ' ' << job.dependencies.size() << '\n';
        if (job.dependencies.empty()) {
            std::queue<std::string> queue;
            queue.push(id);
            visited[id] = true;

            while (!queue.empty()) {
                std::string current_job_id = " " + queue.front();
                queue.pop();

                for (const auto& [other_id, other_job] : jobs) {
                    //std::cout << "Seh Id: " << other_id << " " << current_job_id << '\n';
                    if (std::find(other_job.dependencies.begin(), other_job.dependencies.end(), current_job_id) != other_job.dependencies.end()) {
                        //std::cout << "Seh If_Id: " << other_id << " " << current_job_id << '\n';
                        if (!visited[other_id]) {
                            queue.push(other_id);
                            visited[other_id] = true;
                        }
                    }
                }
            }
        }
    }
    //std::cout << "dag_connect1\n";

    //Das ist ein Test
    //for (const auto& [other_id, other_job] : jobs){
        //std::cout << other_id << ' ' << visited[other_id] << '\n';
    //}

    for (const auto& [id, job] : jobs) {
        //std::cout << "Check jobs for bugs" << '\n';
        if (!visited[id]) {
            std::cout << "Alles ist schlecht!\n";
            return false;
        }
    }
    //std::cout << "dag_is_connected\n";

    return true;
}

bool DAG::has_start_and_end_jobs() const {
    bool has_start = false;
    bool has_end = false;

    for (const auto& [id, job] : jobs) {
        if (job.dependencies.empty()) {
            has_start = true;
        }

        bool is_end = true;
        for (const auto& [_, other_job] : jobs) {
            if (std::find(other_job.dependencies.begin(), other_job.dependencies.end(), id) != other_job.dependencies.end()) {
                is_end = false;
                break;
            }
        }
        if (is_end) {
            has_end = true;
        }
    }
    //std::cout << "dag_start_end\n";

    return has_start && has_end;
}

void DAG::execute(int max_concurrent_jobs) {
    ThreadPool pool(max_concurrent_jobs);
    Barrier barrier(jobs.size());
    std::vector<std::future<void>> futures;
    //std::cout << "dag_execute_init" << jobs.size() << "\n";
    for (auto& [id, job] : jobs) {
        futures.push_back(std::async(std::launch::async, [&job] {
            job.execute();
        }));
    }
    
    // Ожидание завершения всех заданий
    for (auto& future : futures) {
        future.get(); // Получаем результат выполнения для блокировки до завершения
    }
    //std::cout << "dag_has_executed\n";
}