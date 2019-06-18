#include "modbus.h"

#include <QModbusTcpClient>
#include <QModbusRtuSerialMaster>
#include <QDebug>

Modbus::Modbus(QString pclAddr, quint32 port)
    : pclAddr_(pclAddr), port_(port), timer_(nullptr)
{
    minAddr = 0xffff;
    maxAddr = -1;
}

Modbus::~Modbus() {
    if (timer_) {
        timer_->stop();
        delete timer_;
    }

    if (modbusDevice) {
        modbusDevice->disconnectDevice();
        delete modbusDevice;
    }
}

bool Modbus::InitModule() {
   modbusDevice = new QModbusTcpClient(this);
   if (!modbusDevice)
       return false;

   connect(modbusDevice, &QModbusClient::stateChanged,
               this, &Modbus::onStateChanged);

   connect(modbusDevice, &QModbusClient::errorOccurred, this, &Modbus::errorOccurred);

   if (modbusDevice->state() == QModbusDevice::ConnectedState)
       return false;

   modbusDevice->setConnectionParameter(QModbusDevice::NetworkPortParameter, port_);
   modbusDevice->setConnectionParameter(QModbusDevice::NetworkAddressParameter, pclAddr_);
   modbusDevice->setTimeout(1000);
   modbusDevice->setNumberOfRetries(3);
   if (!modbusDevice->connectDevice())
       return false;

   // update();
   timer_ = new QTimer(this);
   connect(timer_, SIGNAL(timeout()), this, SLOT(update()));
   timer_->start(100);

   return true;
}

void Modbus::onStateChanged(int state)
{

    qInfo() << "state change: " << state;
    if ((state == QModbusDevice::ConnectedState) || (state == QModbusDevice::ConnectingState)) {
        connected_ = true;
    } else {
        connected_ = false;
    }

    /*bool connected = (state != QModbusDevice::UnconnectedState);

    if (state == QModbusDevice::UnconnectedState)
        ui->connectButton->setText(tr("Connect"));
    else if (state == QModbusDevice::ConnectedState)
        ui->connectButton->setText(tr("Disconnect"));*/
}

void Modbus::errorOccurred(QModbusDevice::Error error) {
    qWarning() << "errorOccurred: " << error;
    emit errorModbusOccurred();
}

void Modbus::update() {
    if (!connected_)
        return;

    if (maxAddr > -1) {
        readRegister(minAddr, (maxAddr - minAddr) + 1);
    }
}

void Modbus::addRegisterCheck(int addr) {
    if (addr < minAddr) {
        minAddr = addr;
    }

    if (addr > maxAddr) {
        maxAddr = addr;
    }
    addressesCheck_.insert(addr);
}

void Modbus::addBitRegisterCheck(int addr, quint8 bit) {
    addRegisterCheck(addr);
    auto it = bitsCheck_.find(addr);
    if (it == bitsCheck_.end()) {
        QSet<int> bits;
        bits.insert(bit);
        bitsCheck_.insert(addr, bits);
    } else {
        it.value().insert(bit);
    }
}

bool Modbus::writeRegisterBit(int addr, quint8 bit, bool value) {
    ushort actualValue = 0;

    auto it = modbusWriteValues_.find(addr);

    if (it != modbusWriteValues_.end()) {
        actualValue = it.value();
    }

    ushort newValue = (ushort) (1 << bit);
    ushort result = 0;

    // bigEndian
    result |= (ushort)((newValue & 0x00ff) << 8);
    result |= (ushort)((newValue & 0xff00) >> 8);

    // littleendian
    // result = newValue;

    if (value)
    {
        actualValue |= result;
    }
    else
    {
        actualValue &= (ushort)(~result);
    }

    modbusWriteValues_.insert(addr, actualValue);

    return writeRegister(addr, actualValue);
}

void Modbus::readRegister(int address, int num)
{
    if (!modbusDevice)
        return;

    auto request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters,
                                   address, num);

    if (auto *reply = modbusDevice->sendReadRequest(request, 1)) {
        if (!reply->isFinished())
            connect(reply, &QModbusReply::finished, this, &Modbus::readReady);
        else
            delete reply; // broadcast replies return immediately
    } else {
        qWarning() << "readRegister error";
        emit errorModbusOccurred();
    }
}

bool Modbus::getBitValue(ushort data, ushort bit)
{
    ushort result = 0;
    result |= (ushort)((data & 0x00ff) << 8);
    result |= (ushort)((data & 0xff00) >> 8);

    return ((((result >> bit) & 1) == 1) ? true : false);
}

void Modbus::readReady()
{
    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
        return;

    if (reply->error() == QModbusDevice::NoError) {
        const QModbusDataUnit unit = reply->result();
        for (uint i = 0; i < unit.valueCount(); i++) {
            if (addressesCheck_.find(unit.startAddress() + i) ==
                    addressesCheck_.end())
                continue;

            const auto it = modbusValues_.find(unit.startAddress() + i);
            bool empty = (it == modbusValues_.end());
            if (empty || (it.value() != unit.value(i))) {
                emit registerChanged(unit.startAddress() + i, unit.value(i));
                const auto itbit = bitsCheck_.find(unit.startAddress() + i);
                if (itbit != bitsCheck_.end()) {
                    foreach (auto bit, bitsCheck_.find(unit.startAddress() + i).value()) {
                        auto bitvalue = getBitValue(unit.value(i), bit);
                        if (empty || (getBitValue(it.value(), bit)
                                      != bitvalue)) {
                            emit registerBitChanged(unit.startAddress() + i, bit, bitvalue);
                        }
                    }
                }
                modbusValues_.insert(unit.startAddress() + i, unit.value(i));
            }
        }
    } else if (reply->error() == QModbusDevice::ProtocolError) {
        qCritical() << tr("Read response error: %1 (Mobus exception: 0x%2)").
                       arg(reply->errorString()).
                       arg(reply->rawResult().exceptionCode(), -1, 16);
        emit errorModbusOccurred();
    } else {
        qCritical() << tr("Read response error: %1 (code: 0x%2)").
                                    arg(reply->errorString()).
                                    arg(reply->error(), -1, 16);
        emit errorModbusOccurred();
    }

    delete reply;
    // reply->deleteLater();
}

bool Modbus::writeRegister(int addr, quint16 value)
{
    if ((!modbusDevice) || (!connected_))
        return false;

    auto writeUnit = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, addr, 1);
    writeUnit.setValue(0, value);

    if (auto *reply = modbusDevice->sendWriteRequest(writeUnit, 1)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, this, [this, reply]() {
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    emit registerWriteRepl(unit.startAddress(), unit.value(0));
                } else if (reply->error() == QModbusDevice::ProtocolError) {
                    qCritical() << tr("Write response error: %1 (Mobus exception: 0x%2)")
                        .arg(reply->errorString()).arg(reply->rawResult().exceptionCode(), -1, 16);
                    emit errorModbusOccurred();
                } else if (reply->error() != QModbusDevice::NoError) {
                    qCritical() << tr("Write response error: %1 (code: 0x%2)").
                        arg(reply->errorString()).arg(reply->error(), -1, 16);
                    emit errorModbusOccurred();
                }
                reply->deleteLater();
            });
        } else {
            // broadcast replies return immediately
            reply->deleteLater();
        }
    }

    return true;
}

