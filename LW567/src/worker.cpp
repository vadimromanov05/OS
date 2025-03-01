#include <worker.h>

#include <iostream>
#include <string>
#include <memory>
#include <sstream>
#include <thread>
#include <zmq.hpp>
#include <cstdlib>
#include <sys/types.h>
#include <csignal>
#include <sys/wait.h>

void Worker(int id, int sockId) {
    zmq::context_t context(1);
    zmq::socket_t socket(context, zmq::socket_type::rep);

    auto start_time = std::chrono::steady_clock::time_point();
    std::chrono::milliseconds elapsed_time(0);
    bool running = false;

    try {
        socket.bind("tcp://127.0.0.1:" + std::to_string(5555 + sockId));
    } catch (zmq::error_t&) {
        context.close();
        exit(-1);
    }

    while (true) {
        zmq::message_t request;
        if (socket.recv(request, zmq::recv_flags::none)) {
            std::string msg(static_cast<char*>(request.data()), request.size());

            std::istringstream iss(msg);
            std::string command;
            iss >> command;
            
            if (command == "ping") {
                socket.send(zmq::buffer("Ok"), zmq::send_flags::none);
            } else if (command == "start") {
                if (!running) {
                    start_time = std::chrono::steady_clock::now();
                    running = true;
                }
                socket.send(zmq::buffer("Ok"), zmq::send_flags::none);
            } else if (command == "stop") {
                if (running) {
                    auto now = std::chrono::steady_clock::now();
                    elapsed_time += std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time);
                    running = false;
                }
                socket.send(zmq::buffer("Ok"), zmq::send_flags::none);
            } else if (command == "time") {
                auto total_time = elapsed_time;
                if (running) {
                    auto now = std::chrono::steady_clock::now();
                    total_time += std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time);
                }
                socket.send(zmq::buffer(std::to_string(total_time.count())), zmq::send_flags::none);
            } else if (!std::strcmp(command.c_str(), "exit")) {
                socket.send(zmq::buffer("Ok"), zmq::send_flags::none);
                socket.close();
                context.close();
                exit(0);
            } else {
                socket.send(zmq::buffer("!" + command + "!"), zmq::send_flags::none);
            }
        }
    }
}