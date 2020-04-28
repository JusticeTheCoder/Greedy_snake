#include "savedata.h"
#include <QTextStream>
#include <QDebug>
SaveData::SaveData(int i, QFile *save)
{
    no = i;
    save->open(QIODevice::ReadOnly|QIODevice::Text);
    QTextStream in(save);
    in >> highScore;
    in >> unlocked[0] >>unlocked[1] >> unlocked[2];
    save->close();
}


void SaveData::writeback(QFile *save)
{
    save->open(QIODevice::ReadWrite|QIODevice::Text|QIODevice::Truncate);
    QTextStream out(save);
    out << highScore << " " << unlocked[0] << " " << unlocked[1] << " " << unlocked[2] << " " << endl;
    save->close();
}

void SaveData::showInfo()
{
    qDebug() << "最高分：" << highScore << endl;
    qDebug() << "解锁情况：" << unlocked[0] << " " << unlocked[1] << " " << unlocked[2] << " " << endl;
}
