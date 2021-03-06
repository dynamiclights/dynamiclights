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

#include <chrono>
#include <QThread>
#include <QDebug>
#include <QMetaProperty>
#include <QQmlEngine>

#include "GeneratorFacade.h"

GeneratorFacade::GeneratorFacade(Generator *generator) : QQmlPropertyMap(this, nullptr) {
    this->generator = generator;

    // set ownership of the GeneratorFacade to C++ so that QML doesn't try to take over garbage collection duties, resulting in a double free
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);

    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "constructor (GeneratorFacade)\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId();

    }

    // init properties
    const QMetaObject *metaObject = generator->metaObject();

    for (int i = 0; i < metaObject->propertyCount(); ++i) {
        QMetaProperty metaProperty = metaObject->property(i);
        const char *name = metaProperty.name();
        QVariant value = generator->property(name);
        // add key-value pair to facade
        insert(name, value);

        if(flagDebug) {
            qDebug() << "added property (GeneratorFacade): " << name;
        }
    }

    // init region models
    // no longer necessary because we reference GeneratorRegionSet instances in Generator directly now
//    inputRegionModel = QSharedPointer<GeneratorRegionModel>(new GeneratorRegionModel(0));
//    outputRegionModel = QSharedPointer<GeneratorRegionModel>(new GeneratorRegionModel(1));

    // connect generator changes to facade
    QObject::connect(generator, &Generator::valueChanged, this, &GeneratorFacade::updateValueFromAlias, Qt::QueuedConnection);
    // connect facade changes to generator
    QObject::connect(this, &GeneratorFacade::valueChanged, generator, &Generator::updateValue, Qt::QueuedConnection);

    // [DEPRECATED] connect facade region model changes to region set
    // NO LONGER NECESSARY! :) but i'll keep it here for reference in case things go to shit somehow
//    QObject::connect(inputRegionModel.data(), &GeneratorRegionModel::addRegionFromModel, generator->getInputRegionSet(), &GeneratorRegionSet::addRegion, Qt::QueuedConnection);
//    QObject::connect(outputRegionModel.data(), &GeneratorRegionModel::addRegionFromModel, generator->getOutputRegionSet(), &GeneratorRegionSet::addRegion, Qt::QueuedConnection);

//    QObject::connect(inputRegionModel.data(), &GeneratorRegionModel::deleteRegionFromModel, generator->getInputRegionSet(), &GeneratorRegionSet::deleteRegion, Qt::QueuedConnection);
//    QObject::connect(outputRegionModel.data(), &GeneratorRegionModel::deleteRegionFromModel, generator->getOutputRegionSet(), &GeneratorRegionSet::deleteRegion, Qt::QueuedConnection);

//    QObject::connect(inputRegionModel.data(), &GeneratorRegionModel::writeRegionFromModel, generator->getInputRegionSet(), &GeneratorRegionSet::writeRegion, Qt::QueuedConnection);
//    QObject::connect(outputRegionModel.data(), &GeneratorRegionModel::writeRegionFromModel, generator->getOutputRegionSet(), &GeneratorRegionSet::writeRegion, Qt::QueuedConnection);

//    // connect facade region set changes to region model
//    QObject::connect(generator->getInputRegionSet(), &GeneratorRegionSet::writeRegionFromSet, inputRegionModel.data(), &GeneratorRegionModel::writeRegion, Qt::QueuedConnection);
//    QObject::connect(generator->getOutputRegionSet(), &GeneratorRegionSet::writeRegionFromSet, outputRegionModel.data(), &GeneratorRegionModel::writeRegion, Qt::QueuedConnection);
}

GeneratorFacade::~GeneratorFacade() {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "destructor (GeneratorFacade)\t\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
    }
}


void GeneratorFacade::updateValueFromAlias(const QString &key, const QVariant &value) {
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "updateValueFromAlias (GeneratorFacade):\tt = " << now.count() << "\tid = " << QThread::currentThreadId() << "\tkey: " << key << "\tvalue: " << value;
    }

    // filter for unchanging updates
    QVariant currentValue = this->value(key);
    if(currentValue != value) {
        insert(key, value);
        emit valueChangedFromAlias(key, value);
    }
}

GeneratorRegionSet* GeneratorFacade::getInputRegionModel() {
    return generator->getInputRegionSet();
}

GeneratorRegionSet* GeneratorFacade::getOutputRegionModel() {
    return generator->getOutputRegionSet();
}

// i'll be quite frank... this is rather dumb
void GeneratorFacade::initialize()
{
    generator->initialize();
}

void GeneratorFacade::resetParameters()
{
    generator->resetParameters();
}

void GeneratorFacade::resetRegions()
{
    generator->resetRegions();
}
