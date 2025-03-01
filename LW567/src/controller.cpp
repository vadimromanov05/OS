#include <controller.h>
#include <tools.h>

#include <iostream>
#include <string>
#include <unordered_set>
#include <memory>
#include <sstream>
#include <thread>
#include <zmq.hpp>
#include <unordered_map>
#include <future>
#include <vector>

std::string Exec(const std::shared_ptr<Node>& node, const int id, const std::string& oss) {
    std::string result;
    try {
        zmq::message_t message(oss);
        node->socket.send(message, zmq::send_flags::none);

        zmq::message_t reply;
        if (node->socket.recv(reply, zmq::recv_flags::none)) {
            result = reply.to_string() + "\n";
        } else {
            result = "Error:" + std::to_string(id) + ": Node is invaluable\n";
        }
    } catch (zmq::error_t&) {
        result = "Error:" + std::to_string(id) + ": Node is invaluable\n";
    } catch (std::exception& e) {
        result = "Error:" + std::to_string(id) + ": " + e.what() + "\n";
    }
    return result;
}

void Controller(std::istream &stream, bool test) {
    std::vector<std::future<std::string>> futures;

    while (true) {
        if (!test) {
            std::cout << "command> ";
            std::cout.flush();
        }
        std::string command;
        if (std::getline(stream, command)) {
            std::istringstream iss(command);

            std::string cmdType;
            iss >> cmdType;

            if (cmdType == "create") {
                int id;
                iss >> id;

                if (!InsertNode(root, id)) {
                    std::cout << "Error: Already exists\n";
                    continue;
                } else {
                    std::cout << "Ok\n";
                }
                std::cout << "Ok: " << root->pid << "\n";
            } else if (cmdType == "exec") {
                int id;
                std::string subcommand;
                iss >> id >> subcommand;

                auto node = FindNode(root, id);
                if (!node) {
                    std::cout << "Error:" << id << ": Not found\n";
                    continue;
                }

                if (subcommand == "start") {
                    StartTimer(node);
                    std::cout << "Ok:" << id << "\n";
                } else if (subcommand == "stop") {
                    StopTimer(node);
                    std::cout << "Ok:" << id << "\n";
                } else if (subcommand == "time") {
                    auto elapsed = GetElapsedTime(node);
                    std::cout << "Ok:" << id << ": " << elapsed.count() << "\n";
                } else {
                    std::cout << "Error: Unknown subcommand\n";
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            } else if (cmdType == "pingall") {
                std::unordered_set<int> unavailableNodes;

                PingNodes(root, unavailableNodes);

                if (unavailableNodes.empty()) {
                    std::cout << "Ok: -1\n";
                } else {
                    auto e = unavailableNodes.begin();
                    std::cout << "Ok: " << *e;
                    ++e;
                    for (; e != unavailableNodes.end(); ++e) {
                        std::cout << ";" << *e;
                    }
                    std::cout << "\n";
                }
            } else if (cmdType == "exit") {
                TerminateNodes(root);
                globalContext.close();
                break;
            } else {
                std::cout << "Error: Unknown command\n";
            }
        }

        for (auto it = futures.begin(); it != futures.end();) {
            if (it->wait_for(std::chrono::milliseconds(0)) == std::future_status::ready) {
                std::cout << it->get();
                it = futures.erase(it);
            } else {
                ++it;
            }
        }
    }
}