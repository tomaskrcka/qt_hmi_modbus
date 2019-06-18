#ifndef CAMERAINTERFACE_H
#define CAMERAINTERFACE_H

#include <QFile>

class CameraInterface : public QObject
{
    Q_OBJECT
public:
    CameraInterface(QObject *parent = nullptr) : QObject(parent) {}
    virtual QString TakeSnapshot(QString postfix) = 0;
    bool IsLastPictureOK() {
        if (last_picture_path_ == 0)
            return true;

        if ((last_picture_path_.length()) > 0
                && QFile::exists(last_picture_path_)) {
            return true;
        }

        return false;
    }

    void ClearLastPicture() {
        last_picture_path_ = "";
    }
protected:
    QString last_picture_path_;
};

#endif // CAMERAINTERFACE_H
