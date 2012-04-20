/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the test suite of the Qt Toolkit.
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
import QtTest 1.0

Item {
    id: top

    Text {
        id: emptyText
    }

    Text {
        id: txtfamily
        text: "Hello world!"
        font.family: "Courier"
    }

    Text {
        id: txtcolor
        text: "Hello world!"
        color: "red"
    }

    Text {
        id: txtelide
        text: "Hello world!"
        elide: Text.ElideRight
    }

    Text {
        property string first: "Hello world!"
        property string second: "Hello\nworld!"
        property string third: "Hello\nworld\n!"

        id: txtlinecount
        text: first
        width: 100
        wrapMode: Text.WrapAnywhere
        font.pixelSize: 16
    }

    TestCase {
        name: "Text"

        function test_empty() {
            compare(emptyText.text, "")
        }

        function test_family() {
            compare(txtfamily.font.family, "Courier")
            txtfamily.font.family = "Helvetica";
            compare(txtfamily.font.family, "Helvetica")
        }

        function test_color() {
            compare(txtcolor.color, "#ff0000")
            txtcolor.color = "blue";
            compare(txtcolor.color, "#0000ff")
        }

        function test_elide() {
            compare(txtelide.elide, Text.ElideRight)
            txtelide.elide = Text.ElideLeft;
            compare(txtelide.elide, Text.ElideLeft)
            txtelide.elide = Text.ElideMiddle;
            compare(txtelide.elide, Text.ElideMiddle)
        }

        function test_linecount() {
            compare(txtlinecount.lineCount, 1)
            txtlinecount.text = txtlinecount.second;
            compare(txtlinecount.lineCount, 2)
            txtlinecount.text = txtlinecount.third;
            compare(txtlinecount.lineCount, 3)
            txtlinecount.text = txtlinecount.first;
            compare(txtlinecount.lineCount, 1)
            txtlinecount.width = 50;
            compare(txtlinecount.lineCount, 3)
        }

    }
}
