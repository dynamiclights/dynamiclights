// Copyright 2020, Xmodal
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <QQmlEngine>
#include <QJsonObject>
#include <QDebug>

#include "GeneratorRegion.h"

GeneratorRegion::GeneratorRegion(QRect rect, double intensity, int type) {
    // set ownership of the GeneratorRegion to C++ so that QML doesn't try to take over garbage collection duties, resulting in a double free
    // this is necessary because GeneratorModel will return pointers to GeneratorRegion
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);

    this->rect = rect;
    this->intensity = intensity;
    this->type = type;
}

GeneratorRegion::GeneratorRegion(const GeneratorRegion& region) {
    // set ownership of the GeneratorRegion to C++ so that QML doesn't try to take over garbage collection duties, resulting in a double free
    // this is necessary because GeneratorModel will return pointers to GeneratorRegion
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
    this->rect = region.getRect();
    this->intensity = region.getIntensity();
    this->type = region.getType();
}

QRect GeneratorRegion::getRect() const {
    return rect;
}

double GeneratorRegion::getIntensity() const {
    return intensity;
}

int GeneratorRegion::getType() const {
    return type;
}

void GeneratorRegion::readJson(const QJsonObject &json)
{
    // create new rect from JSON
    QRect newRect(json["x"].toInt(), json["y"].toInt(), json["w"].toInt(), json["h"].toInt());
    // write + signal to GeneratorRegionSet
    writeRect(newRect);
}

void GeneratorRegion::writeJson(QJsonObject &json) const
{
    // write coords to object
    json["x"] = rect.x();
    json["y"] = rect.y();
    json["w"] = rect.width();
    json["h"] = rect.height();
}

void GeneratorRegion::writeRect(QRect rect) {
    if(this->rect == rect) {
        return;
    }

    this->rect = rect;
    emit rectChanged(rect);
    emit valueChanged("rect", QVariant(rect));
}

void GeneratorRegion::writeIntensity(double intensity) {
    if(this->intensity == intensity) {
        return;
    }

    this->intensity = intensity;
    emit intensityChanged(intensity);
    emit valueChanged("intensity", QVariant(intensity));
}

void GeneratorRegion::writeType(int type) {
    if(this->type == type) {
        return;
    }

    this->type = type;
    emit typeChanged(type);
    emit valueChanged("type", QVariant(type));
}
