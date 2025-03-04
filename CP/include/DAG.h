#ifndef DAG_H
#define DAG_H

#include "Job.h"
#include <map>
#include <queue>
#include <string>
#include <fstream>
#include <sstream>

class DAG {
public:
    std::map<std::string, Job> jobs;

    DAG(const std::string& ini_file_path);
    bool is_valid() const;
    void execute(int max_concurrent_jobs);

private:
    bool has_cycle(const std::string& job_id, std::map<std::string, bool>& visited, std::map<std::string, bool>& rec_stack) const;
    bool is_connected() const;
    bool has_start_and_end_jobs() const;
    void load_from_ini(const std::string& ini_file_path);
};

#endif
