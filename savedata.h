#ifndef SAVEDATA_H
#define SAVEDATA_H

#include <QFile>

class SaveData
{
public:
    SaveData(int i, QFile *save);
    void writeback(QFile *save);
    void showInfo();
    int no;
    int highScore;
    int unlocked[3];
};

#endif // SAVEDATA_H
