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

#pragma once

#include <QObject>
#include <QList>
#include <QHash>
#include <QSharedPointer>
#include <QThread>
#include "ComputeEngine.h"
#include "OscEngine.h"
#include "Generator.h"
#include "GeneratorFacade.h"
#include "GeneratorModel.h"

class AppModel : public QObject {
    Q_OBJECT
public:
    static AppModel& getInstance() {            // get singleton instance
        static AppModel instance;
        return instance;
    }
    void start();
    QThread* getComputeThread() const;          // needed to exit the thread at app quit. we can only connect this from the main.
    QThread* getOscThread() const;              // needed to exit the thread at app quit. we can only connect this from the main.
    ComputeEngine* getComputeEngine() const;    // needed for connections
    OscEngine* getOscEngine() const;            // needed for connections
    Q_INVOKABLE void createGenerator();
    Q_INVOKABLE void deleteGenerator(int id);
    Q_INVOKABLE bool validateNewGeneratorName(QString name);
    Q_INVOKABLE Generator* getGenerator(int id) const;              // this isn't thread safe?
    Q_INVOKABLE GeneratorFacade* getGeneratorFacade(int id) const;  // this should be thread safe from the GUI thread?
    Q_INVOKABLE GeneratorModel* getGeneratorModel() const;          // this should be thread safe from the GUI thread?
private:
    AppModel();                                 // prevent instanciation
    AppModel(AppModel const&) = delete;         // prevent copy
    void operator=(AppModel const&) = delete;   // prevent assignment

    // data (lists)
    QSharedPointer<QList<QSharedPointer<Generator>>> generators;
    QSharedPointer<QList<QSharedPointer<GeneratorFacade>>> generatorFacades;

    // data (unique elements)
    QSharedPointer<GeneratorModel> generatorModel;

    // engines
    QSharedPointer<ComputeEngine> computeEngine;
    QSharedPointer<OscEngine> oscEngine;

    // threads
    QSharedPointer<QThread> computeThread;
    QSharedPointer<QThread> oscThread;

    // utility variables
    bool started = false;
    bool flagDebug = false;
};