#include "HW4-1.h"

int main()
{
    FILES * command = new FILES("./MT_command.txt");

    cout << command->filelist[2][1] << endl;

    for (int i = 0; i < command->num; i++) {
        POLYGON * p = new POLYGON(command->filelist[i][0]);
        p->get_distance();
        p->get_length();
        p->get_chords();

        p->write(string("./")+command->filelist[i][1]);

        delete p;
    }

    delete command;
}

