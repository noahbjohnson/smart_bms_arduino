#include <command.h>
#include <unity.h>

// void setUp(void) {
// // set stuff up here
// }

// void tearDown(void) {
// // clean stuff up here
// }

int main(int argc, char **argv) {
    UNITY_BEGIN();
    UnityPrint("Testing Command Class");

    Command command;

    UNITY_END();
    return 0;
}