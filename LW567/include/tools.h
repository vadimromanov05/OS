#pragma once

#include <memory>
#include <unordered_set>
#include <chrono>
#include <optional>
#include <zmq.hpp>

inline zmq::context_t globalContext(1);

struct Node {
    int id;
    int pid;
    int sockId;
    zmq::socket_t socket;

    std::shared_ptr<Node> left = nullptr;
    std::shared_ptr<Node> right = nullptr;

    std::optional<std::chrono::steady_clock::time_point> start_time;
    std::chrono::milliseconds elapsed_time = std::chrono::milliseconds(0);

    Node(int id);
};

inline std::shared_ptr<Node> root = nullptr;

std::shared_ptr<Node> FindNode(std::shared_ptr<Node> root, int id);

bool InsertNode(std::shared_ptr<Node>& root, int id);

void PingNodes(const std::shared_ptr<Node>& node, std::unordered_set<int> &unavailable_nodes);

void TerminateNodes(const std::shared_ptr<Node>& node);

void StartTimer(std::shared_ptr<Node>& node);

void StopTimer(std::shared_ptr<Node>& node);

std::chrono::milliseconds GetElapsedTime(std::shared_ptr<Node>& node);