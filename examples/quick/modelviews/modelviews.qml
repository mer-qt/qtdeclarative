/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.0
import "../../shared" as Examples

/*!
    \title QtQuick Examples - ModelViews
    \example quick/modelviews
    \brief This is a collection of QML drag and drop examples
    \image qml-modelviews-example.png

    This is a collection of small QML examples relating to model and view functionality.

    GridView and PathView demonstrate usage of these elements to display views.

    Dynamic List demonstrates runtime modification of a ListModel.

    Expanding Delegates demonstrates delegates that expand when activated.

    Highlight demonstrates adding a custom highlight to a ListView.

    Highlight Ranges shows the three different highlight range modes of ListView.

    Sections demonstrates the various section headers and footers available to ListView.

    Packages demonstrates using Packages to transition delegates between two views.

    VisualItemModel uses a VisualItemModel for the model instead of a ListModel.
*/

Item {
    height: 480
    width: 320
    Examples.LauncherList {
        id: ll
        anchors.fill: parent
        Component.onCompleted: {
            addExample("GridView", "A simple GridView", Qt.resolvedUrl("gridview/gridview-example.qml"))
            addExample("Dynamic List", "A dynamically alterable list", Qt.resolvedUrl("listview/dynamiclist.qml"))
            addExample("Expanding Delegates", "A ListView with delegates that expand", Qt.resolvedUrl("listview/expandingdelegates.qml"))
            addExample("Highlight", "A ListView with a custom highlight", Qt.resolvedUrl("listview/highlight.qml"))
            addExample("Highlight Ranges", "The three highlight ranges of ListView", Qt.resolvedUrl("listview/highlightranges.qml"))
            addExample("Sections", "ListView section headers and footers", Qt.resolvedUrl("listview/sections.qml"))
            addExample("Packages", "Transitions between a ListView and GridView", Qt.resolvedUrl("package/view.qml"))
            addExample("PathView", "A simple PathView", Qt.resolvedUrl("pathview/pathview-example.qml"))
            addExample("VisualItemModel", "Using a VisualItemModel", Qt.resolvedUrl("visualitemmodel/visualitemmodel.qml"))
        }
    }
}
