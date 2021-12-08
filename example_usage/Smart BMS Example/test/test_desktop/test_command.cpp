#include <common_arduino.h>
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
    UNITY_PRINT_EOL();

    Command my_command(WRITE);
    my_command.setRegister(ENTER_FACTORY_MODE);
    my_command.setData(ENTER_FACTORY_MODE_COMMAND[0], ENTER_FACTORY_MODE_COMMAND[1]);
    Generated_Command cmd = my_command.get_command();
    UnityPrintNumberHex(cmd.data[cmd.length-1], *" ");

    UNITY_END();
    return 0;
}