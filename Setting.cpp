#include "setting.h"

Setting::Setting () {
}

Setting::Setting (
        const QString& value,
        bool enable,
        int index)
    : value (value),
      enable (enable),
      index (index) {
}

Setting::Setting (
        const Setting& setting)
    : value (setting.value),
      enable (setting.enable),
      index (setting.index) {
}

Setting&
Setting::operator = (
        const Setting& setting) {

    if (this == &setting) return *this;

    this->value = setting.value;
    this->enable = setting.enable;
    this->index = setting.index;

    return *this;
}

bool
Setting::operator == (
        const Setting& setting) const {

    if (this->value != setting.value) {
        return false;
    }

    if (this->enable != setting.enable) {
        return false;
    }

    if (this->index != setting.index) {
        return false;
    }

    return true;
}

QTextStream&
Setting::operator << (
        QTextStream& textStream) const {

    textStream << this->index;
    textStream << this->value;
    textStream << static_cast<int> (this->enable);

    return textStream;
}

QTextStream&
Setting::operator >> (
        QTextStream& textStream) {

    textStream >> this->index;
    textStream >> this->value;

    int integer = 0;
    textStream >> integer;
    this->enable = (1 == integer);

    return textStream;
}

int
Setting::getIndex () const {

    return this->index;
}

QString
Setting::getValue () const {

    return this->value;
}

bool
Setting::getEnable () const {

    return this->enable;
}

bool
Setting::setIndex (
        int index) {

    this->index = index;
    return true;
}

bool
Setting::setValue (
        const QString& value) {

    this->value = value;
    return true;
}

bool
Setting::setEnable (
        bool enable) {

    this->enable = enable;
    return true;
}
