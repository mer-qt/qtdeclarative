/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtQml module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QSGCONTEXT_H
#define QSGCONTEXT_H

#include <QtCore/QObject>
#include <QtCore/qabstractanimation.h>

#include <QtGui/QImage>
#include <QtGui/QSurfaceFormat>

#include <private/qrawfont_p.h>

#include <QtQuick/qsgnode.h>
#include <QtQuick/private/qsgdepthstencilbuffer_p.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE


class QSGContextPrivate;
class QSGRectangleNode;
class QSGImageNode;
class QSGGlyphNode;
class QSGRenderer;
class QSGDistanceFieldGlyphCache;
class QQuickCanvas;
class QSGTexture;
class QSGMaterial;
class QSGMaterialShader;
class QSGEngine;
class QQuickWindowManager;

class QOpenGLContext;
class QOpenGLFramebufferObject;

class QQuickTextureFactory;

class Q_QUICK_EXPORT QSGContext : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QSGContext)

public:
    explicit QSGContext(QObject *parent = 0);
    ~QSGContext();

    virtual void initialize(QOpenGLContext *context);
    virtual void invalidate();

    QOpenGLContext *glContext() const;

    bool isReady() const;

    QSGMaterialShader *prepareMaterial(QSGMaterial *material);

    virtual void renderNextFrame(QSGRenderer *renderer, GLuint fboId);

    virtual QSGDistanceFieldGlyphCache *distanceFieldGlyphCache(const QRawFont &font);

    virtual QSGRectangleNode *createRectangleNode();
    virtual QSGImageNode *createImageNode();
    virtual QSGGlyphNode *createGlyphNode();
    virtual QSGRenderer *createRenderer();

    virtual QSGTexture *createTexture(const QImage &image = QImage()) const;
    virtual QSize minimumFBOSize() const;
    virtual QSharedPointer<QSGDepthStencilBuffer> depthStencilBufferForFbo(QOpenGLFramebufferObject *fbo);
    QSGDepthStencilBufferManager *depthStencilBufferManager();

    virtual QSurfaceFormat defaultSurfaceFormat() const;

    QSGTexture *textureForFactory(QQuickTextureFactory *factory, QQuickCanvas *canvas);

    static QSGContext *createDefaultContext();

    void setFlashModeEnabled(bool enabled);
    bool isFlashModeEnabled() const;

    void setRenderAlpha(qreal renderAlpha);
    qreal renderAlpha() const;

    void setDistanceFieldEnabled(bool enabled);
    bool isDistanceFieldEnabled() const;

    virtual QAnimationDriver *createAnimationDriver(QObject *parent);

    static QQuickTextureFactory *createTextureFactoryFromImage(const QImage &image);
    static QQuickWindowManager *createWindowManager();


public slots:
    void textureFactoryDestroyed(QObject *o);

signals:
    void initialized();
    void invalidated();
};

QT_END_NAMESPACE

QT_END_HEADER

#endif // QSGCONTEXT_H
