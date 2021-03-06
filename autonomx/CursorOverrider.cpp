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

#include "CursorOverrider.h"
#include <QDebug>
#include <QPixmap>

CursorOverrider::CursorOverrider(QGuiApplication* app, QObject* parent) : QObject(parent=nullptr)
{
    this->app = app;
    this->crossbeam = QCursor(QPixmap{":/assets/images/crossbeam.png"});
}

void CursorOverrider::overrideWithBeam()
{
    app->setOverrideCursor(crossbeam);
}

void CursorOverrider::overrideCursor(Qt::CursorShape shape)
{
    app->setOverrideCursor(QCursor(shape));
    // qDebug() << "override";
}

void CursorOverrider::restoreCursor()
{
    app->restoreOverrideCursor();
    // qDebug() << "restore";
}

void CursorOverrider::sendPress(QObject *object, QPointF pos, int button)
{
    Qt::MouseButton type = Qt::MouseButton(button);
    auto down = new QMouseEvent(QMouseEvent::Type::MouseButtonPress, pos, type, type, Qt::KeyboardModifier::NoModifier);
    app->postEvent(object, down);
}

void CursorOverrider::deAlt(QObject* tf)
{
    QKeyEvent releaseEvent = QKeyEvent(QEvent::KeyRelease, Qt::Key_Alt, Qt::NoModifier);
    QCoreApplication::sendEvent(tf, &releaseEvent);

    // qDebug() << "key released";
}
