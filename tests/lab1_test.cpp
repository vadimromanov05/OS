#include <gtest/gtest.h>
#include "/home/vadim/Рабочий стол/OS/LW1/include/utils.h"
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>


TEST(CommonTest, WriteToPipe) {
    std::cout << "Wo sind Sie?\n";
    int pipefd[2];
    ASSERT_EQ(pipe(pipefd), 0);

    const char *test_string = "1 2 3";
    write_to_pipe(pipefd[1], test_string);

    char buffer[MAX_SIZE];
    close(pipefd[1]);

    ssize_t bytes_read = read(pipefd[0], buffer, MAX_SIZE);
    buffer[bytes_read] = '\0';

    ASSERT_STREQ(buffer, test_string);
    close(pipefd[0]);
}


TEST(CommonTest, ProcessInputFromPipe) {
    const char *temp_filename = "results.txt";
    remove(temp_filename);
    std::cout << "Der erstes Fall ist gemacht!\n";

    int pipefd[2];
    ASSERT_EQ(pipe(pipefd), 0);

    const char *input_string = "4 2 0 1";
    write_to_pipe(pipefd[1], input_string);
    close(pipefd[1]);
    std::cout << "Der zweites Fall ist gemacht!\n";


    if (fork() == 0) {
        process_input_from_pipe(pipefd[0], pipefd[1]);
        exit(0);
    }

    close(pipefd[0]);
    std::cout << "Der drittes Fall ist gemacht!\n";
    wait(NULL);


    std::ifstream file(temp_filename);
    //std::cout << "How can I do it?\n";
    ASSERT_TRUE(file.is_open());
    std::cout << "Der viertes Fall ist gemacht!\n";

    std::string line;
    //std::cout << "Was ist das?!1\n";
    bool found_error_message = false;
    //std::cout << "Was ist das?!2\n";
    while (std::getline(file, line)) {
        //std::cout << "Iteration_wieder!\n";
        //std::cout << line << '\n';
        if (line.find("Деление на ноль невозможно при делении 4.000000 / 0.000000.") != std::string::npos) {
            found_error_message = true;
        }
    }
    std::cout << "Der funftes Fall ist gemacht!\n";

    file.close();
    //std::cout << "Du bist ein Mensch!\n";
    ASSERT_TRUE(found_error_message);
    std::cout << "Der sechstes Fall ist gemacht!\n";
    remove(temp_filename);
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
