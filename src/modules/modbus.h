#ifndef MODBUS_H
#define MODBUS_H

#include <QString>
#include <QObject>
#include <QModbusDataUnit>
#include <QModbusDevice>
#include <QTimer>
#include <QSet>

class QModbusClient;
class QModbusReply;

class Modbus : public QObject
{
    Q_OBJECT
public:
    Modbus(QString pclAddr, quint32 port);
    ~Modbus();
    bool InitModule();
    void addRegisterCheck(int addr);
    void addBitRegisterCheck(int addr, quint8 bit);
    // void readRegister(int address);
    bool writeRegister(int addr, quint16 value);
    bool writeRegisterBit(int addr, quint8 bit, bool value);
    bool toggleRegisterBit(int addr, quint8 bit, int timeout);
    bool connected() { return connected_; }

signals:
    void registerChanged(int addr, quint16 value);
    void registerBitChanged(int addr, quint8 bit, bool value);
    void registerWriteRepl(int addr, bool err);
    void errorModbusOccurred();

private:
    bool getBitValue(ushort data, ushort bit);
    void readRegister(int address, int num = 1);

private:
    QString pclAddr_;
    quint32 port_;
    QModbusClient *modbusDevice;
    QTimer * timer_;
    QSet<int> addressesCheck_;
    QMap<int, QSet<int>> bitsCheck_;
    QMap<int, quint16> modbusValues_;
    QMap<int, quint16> modbusWriteValues_;
    int minAddr;
    int maxAddr;
    bool connected_;

private slots:
    void onStateChanged(int state);
    void errorOccurred(QModbusDevice::Error error);
    void readReady();

    // Timer
    void update();
};

#endif // MODBUS_H
