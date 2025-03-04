#include <controller.h>
#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include <thread>

#include <tools.h>
#include <worker.h>

TEST(insertNode, test)
{
    auto id = 1;
    std::shared_ptr<Node> root = nullptr;

    auto ok = InsertNode(root, id);

    ASSERT_TRUE(ok);
    ASSERT_EQ(root->id, id);

    auto ok1 = InsertNode(root, id + 1);

    ASSERT_TRUE(ok1);
    ASSERT_FALSE(root->right == nullptr);
    ASSERT_EQ(root->right->id, id + 1);

    ASSERT_TRUE(root->left == nullptr);

    auto bad = InsertNode(root, id);
    ASSERT_FALSE(bad);
}

TEST(findNode, test)
{
    auto id = 1;
    auto pid = 234;
    std::shared_ptr<Node> root = nullptr;

    InsertNode(root, id);
    InsertNode(root, id + 1);

    auto first = FindNode(root, id);
    auto second = FindNode(root, id + 1);
    auto bad = FindNode(root, id - 1);

    ASSERT_EQ(first->id, id);
    ASSERT_EQ(second->id, id + 1);
    ASSERT_TRUE(bad == nullptr);
}

TEST(pinAndDestroyNodes, test)
{
    auto expected_size = 0;
    auto id1 = 1111;
    auto id2 = 2222;
    std::shared_ptr<Node> root = nullptr;

    InsertNode(root, id1);
    InsertNode(root, id2);

    std::unordered_set<int> unavailable_nodes;

    PingNodes(root, unavailable_nodes);

    EXPECT_EQ(unavailable_nodes.size(), expected_size);
    TerminateNodes(root);
}


TEST(ControllerTest, TimerStartStopTest) {
    std::stringstream stream;
    bool test = true;  // Включаем режим теста
    Controller(stream, test);

    // Вставляем команду для создания узла
    std::string create_command = "create 1\n";
    stream.str(create_command);
    Controller(stream, test);

    // Запуск таймера
    std::string start_command = "exec 1 start\n";
    stream.str(start_command);
    Controller(stream, test);

    std::shared_ptr<Node> node = FindNode(root, 1);
    ASSERT_NE(node, nullptr);  // Проверяем, что узел существует
    ASSERT_TRUE(node->start_time.has_value());  // Таймер должен быть запущен

    // Остановка таймера
    std::string stop_command = "exec 1 stop\n";
    stream.str(stop_command);
    Controller(stream, test);

    ASSERT_FALSE(node->start_time.has_value());  // Таймер должен быть остановлен
}


TEST(ControllerTest, TimerGetTimeTest) {
    std::stringstream stream;
    bool test = true;
    Controller(stream, test);

    // Создание узла и запуск таймера
    std::string create_command = "create 1\n";
    stream.str(create_command);
    Controller(stream, test);

    std::string start_command = "exec 1 start\n";
    stream.str(start_command);
    Controller(stream, test);

    std::shared_ptr<Node> node = FindNode(root, 1);
    ASSERT_NE(node, nullptr);  // Проверяем, что узел существует

    // Симуляция времени
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // Останавливаем таймер
    std::string stop_command = "exec 1 stop\n";
    stream.str(stop_command);
    Controller(stream, test);

    // Получаем время
    std::string time_command = "exec 1 time\n";
    stream.str(time_command);
    Controller(stream, test);

    auto elapsed = GetElapsedTime(node);
    std::cout << "Elapsed time: " << elapsed.count() << "ms\n";  // Можно также вывести в консоль

    // Проверяем, что время больше 0 (прошло какое-то время)
    ASSERT_GT(elapsed.count(), 0);
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}