#include <iostream>
#include <../mavlink/v2.0/common/mavlink.h>
#include <string>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    std::string serialPortName = "/dev/ttyACM0";
    int baudRate = B57600;

    int serialPort = open(serialPortName.c_str(), O_RDWR);
    if (serialPort == -1) {
        std::cerr << "Error opening serial port" << std::endl;
        return 1;
    }

    struct termios tty;
    tcgetattr(serialPort, &tty);
    cfsetospeed(&tty, baudRate);
    cfsetispeed(&tty, baudRate);
    tcsetattr(serialPort, TCSANOW, &tty);

    mavlink_message_t msg;
    mavlink_status_t status;

    while (true) {
        uint8_t byte;
        if (read(serialPort, &byte, 1) == 1) {
            if (mavlink_parse_char(MAVLINK_COMM_0, byte, &msg, &status)) {
                if (msg.msgid == MAVLINK_MSG_ID_GLOBAL_POSITION_INT) {
                    mavlink_global_position_int_t gps;
                    mavlink_msg_global_position_int_decode(&msg, &gps);

                    double latitude = gps.lat / 1e7;
                    double longitude = gps.lon / 1e7;

                    std::cout << "Latitude: " << latitude << " | Longitude: " << longitude << std::endl;
                }
            }
        }
    }

    close(serialPort);
    return 0;
}
