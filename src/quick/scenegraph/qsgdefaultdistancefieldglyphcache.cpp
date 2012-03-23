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

#include "qsgdefaultdistancefieldglyphcache_p.h"

#include <QtGui/private/qdistancefield_p.h>
#include <QtQuick/private/qsgdistancefieldutil_p.h>
#include <qopenglfunctions.h>

QT_BEGIN_NAMESPACE


QSGDefaultDistanceFieldGlyphCache::QSGDefaultDistanceFieldGlyphCache(QSGDistanceFieldGlyphCacheManager *man, QOpenGLContext *c, const QRawFont &font)
    : QSGDistanceFieldGlyphCache(man, c, font)
    , m_maxTextureSize(0)
    , m_maxTextureCount(3)
    , m_fbo(0)
    , m_blitProgram(0)
{
    m_currentTexture = createTextureInfo();

    m_blitVertexCoordinateArray[0] = -1.0f;
    m_blitVertexCoordinateArray[1] = -1.0f;
    m_blitVertexCoordinateArray[2] =  1.0f;
    m_blitVertexCoordinateArray[3] = -1.0f;
    m_blitVertexCoordinateArray[4] =  1.0f;
    m_blitVertexCoordinateArray[5] =  1.0f;
    m_blitVertexCoordinateArray[6] = -1.0f;
    m_blitVertexCoordinateArray[7] =  1.0f;

    m_blitTextureCoordinateArray[0] = 0.0f;
    m_blitTextureCoordinateArray[1] = 0.0f;
    m_blitTextureCoordinateArray[2] = 1.0f;
    m_blitTextureCoordinateArray[3] = 0.0f;
    m_blitTextureCoordinateArray[4] = 1.0f;
    m_blitTextureCoordinateArray[5] = 1.0f;
    m_blitTextureCoordinateArray[6] = 0.0f;
    m_blitTextureCoordinateArray[7] = 1.0f;
}

QSGDefaultDistanceFieldGlyphCache::~QSGDefaultDistanceFieldGlyphCache()
{
    for (int i = 0; i < m_textures.count(); ++i)
        glDeleteTextures(1, &m_textures[i].texture);
    ctx->functions()->glDeleteFramebuffers(1, &m_fbo);
    delete m_blitProgram;
}

void QSGDefaultDistanceFieldGlyphCache::requestGlyphs(const QSet<glyph_t> &glyphs)
{
    QList<GlyphPosition> glyphPositions;
    QVector<glyph_t> glyphsToRender;

    for (QSet<glyph_t>::const_iterator it = glyphs.constBegin(); it != glyphs.constEnd() ; ++it) {
        glyph_t glyphIndex = *it;

        if (cacheIsFull() && m_unusedGlyphs.isEmpty())
            continue;

        if (textureIsFull(m_currentTexture) && m_textures.count() < m_maxTextureCount)
            m_currentTexture = createTextureInfo();

        m_unusedGlyphs.remove(glyphIndex);

        TextureInfo *tex = m_currentTexture;

        GlyphPosition p;
        p.glyph = glyphIndex;
        p.position = QPointF(tex->currX, tex->currY);

        if (!cacheIsFull()) {
            tex->currX += QT_DISTANCEFIELD_TILESIZE(doubleGlyphResolution());
            if (tex->currX >= maxTextureSize()) {
                tex->currX = 0;
                tex->currY += QT_DISTANCEFIELD_TILESIZE(doubleGlyphResolution());
            }
        } else {
            // Recycle glyphs
            if (!m_unusedGlyphs.isEmpty()) {
                glyph_t unusedGlyph = *m_unusedGlyphs.constBegin();
                TexCoord unusedCoord = glyphTexCoord(unusedGlyph);
                tex = m_glyphsTexture.value(unusedGlyph);
                p.position = QPointF(unusedCoord.x, unusedCoord.y);
                m_unusedGlyphs.remove(unusedGlyph);
                m_glyphsTexture.remove(unusedGlyph);
                removeGlyph(unusedGlyph);
            }
        }

        if (p.position.y() < maxTextureSize()) {
            glyphPositions.append(p);
            glyphsToRender.append(glyphIndex);
            m_glyphsTexture.insert(glyphIndex, tex);
        }
    }

    setGlyphsPosition(glyphPositions);
    markGlyphsToRender(glyphsToRender);
}

void QSGDefaultDistanceFieldGlyphCache::storeGlyphs(const QHash<glyph_t, QImage> &glyphs)
{
    int requiredWidth = maxTextureSize();
    int rows = 128 / (requiredWidth / QT_DISTANCEFIELD_TILESIZE(doubleGlyphResolution())); // Enough rows to fill the latin1 set by default..

    QHash<TextureInfo *, QVector<glyph_t> > glyphTextures;

    QHash<glyph_t, QImage>::const_iterator it;
    for (it = glyphs.constBegin(); it != glyphs.constEnd(); ++it) {
        glyph_t glyphIndex = it.key();
        TexCoord c = glyphTexCoord(glyphIndex);
        TextureInfo *texInfo = m_glyphsTexture.value(glyphIndex);

        int requiredHeight = qMin(maxTextureSize(),
                                  qMax(texInfo->currY + QT_DISTANCEFIELD_TILESIZE(doubleGlyphResolution()),
                                       QT_DISTANCEFIELD_TILESIZE(doubleGlyphResolution()) * rows));

        resizeTexture(texInfo, requiredWidth, requiredHeight);
        glBindTexture(GL_TEXTURE_2D, texInfo->texture);

        glyphTextures[texInfo].append(glyphIndex);

        QImage glyph = it.value();

        if (useWorkaroundBrokenFBOReadback()) {
            uchar *inBits = glyph.scanLine(0);
            uchar *outBits = texInfo->image.scanLine(int(c.y)) + int(c.x);
            for (int y = 0; y < glyph.height(); ++y) {
                qMemCopy(outBits, inBits, glyph.width());
                inBits += glyph.bytesPerLine();
                outBits += texInfo->image.bytesPerLine();
            }
        }

        glTexSubImage2D(GL_TEXTURE_2D, 0, c.x, c.y, glyph.width(), glyph.height(), GL_ALPHA, GL_UNSIGNED_BYTE, glyph.constBits());
    }

    QHash<TextureInfo *, QVector<glyph_t> >::const_iterator i;
    for (i = glyphTextures.constBegin(); i != glyphTextures.constEnd(); ++i) {
        Texture t;
        t.textureId = i.key()->texture;
        t.size = i.key()->size;
        setGlyphsTexture(i.value(), t);
    }
}

void QSGDefaultDistanceFieldGlyphCache::referenceGlyphs(const QSet<glyph_t> &glyphs)
{
    m_unusedGlyphs -= glyphs;
}

void QSGDefaultDistanceFieldGlyphCache::releaseGlyphs(const QSet<glyph_t> &glyphs)
{
    m_unusedGlyphs += glyphs;
}

void QSGDefaultDistanceFieldGlyphCache::createTexture(TextureInfo *texInfo, int width, int height)
{
    if (useWorkaroundBrokenFBOReadback() && texInfo->image.isNull())
        texInfo->image = QImage(width, height, QImage::Format_Indexed8);

    while (glGetError() != GL_NO_ERROR) { }

    glGenTextures(1, &texInfo->texture);
    glBindTexture(GL_TEXTURE_2D, texInfo->texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width, height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    texInfo->size = QSize(width, height);

    GLuint error = glGetError();
    if (error != GL_NO_ERROR) {
        glBindTexture(GL_TEXTURE_2D, 0);
        glDeleteTextures(1, &texInfo->texture);
        texInfo->texture = 0;
    }

}

void QSGDefaultDistanceFieldGlyphCache::resizeTexture(TextureInfo *texInfo, int width, int height)
{
    int oldWidth = texInfo->size.width();
    int oldHeight = texInfo->size.height();
    if (width == oldWidth && height == oldHeight)
        return;

    GLuint oldTexture = texInfo->texture;
    createTexture(texInfo, width, height);

    if (!oldTexture)
        return;

    updateTexture(oldTexture, texInfo->texture, texInfo->size);

    if (useWorkaroundBrokenFBOReadback()) {
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, oldWidth, oldHeight, GL_ALPHA, GL_UNSIGNED_BYTE, texInfo->image.constBits());
        texInfo->image = texInfo->image.copy(0, 0, width, height);
        glDeleteTextures(1, &oldTexture);
        return;
    }

    if (!m_blitProgram)
        createBlitProgram();

    Q_ASSERT(m_blitProgram);

    if (!m_fbo)
        ctx->functions()->glGenFramebuffers(1, &m_fbo);
    ctx->functions()->glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    GLuint tmp_texture;
    glGenTextures(1, &tmp_texture);
    glBindTexture(GL_TEXTURE_2D, tmp_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, oldWidth, oldHeight, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);
    ctx->functions()->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                             GL_TEXTURE_2D, tmp_texture, 0);

    ctx->functions()->glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, oldTexture);

    // save current render states
    GLboolean stencilTestEnabled;
    GLboolean depthTestEnabled;
    GLboolean scissorTestEnabled;
    GLboolean blendEnabled;
    GLint viewport[4];
    GLint oldProgram;
    glGetBooleanv(GL_STENCIL_TEST, &stencilTestEnabled);
    glGetBooleanv(GL_DEPTH_TEST, &depthTestEnabled);
    glGetBooleanv(GL_SCISSOR_TEST, &scissorTestEnabled);
    glGetBooleanv(GL_BLEND, &blendEnabled);
    glGetIntegerv(GL_VIEWPORT, &viewport[0]);
    glGetIntegerv(GL_CURRENT_PROGRAM, &oldProgram);

    glDisable(GL_STENCIL_TEST);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_SCISSOR_TEST);
    glDisable(GL_BLEND);

    glViewport(0, 0, oldWidth, oldHeight);

    ctx->functions()->glVertexAttribPointer(QT_VERTEX_COORDS_ATTR, 2, GL_FLOAT, GL_FALSE, 0, m_blitVertexCoordinateArray);
    ctx->functions()->glVertexAttribPointer(QT_TEXTURE_COORDS_ATTR, 2, GL_FLOAT, GL_FALSE, 0, m_blitTextureCoordinateArray);

    m_blitProgram->bind();
    m_blitProgram->enableAttributeArray(int(QT_VERTEX_COORDS_ATTR));
    m_blitProgram->enableAttributeArray(int(QT_TEXTURE_COORDS_ATTR));
    m_blitProgram->disableAttributeArray(int(QT_OPACITY_ATTR));
    m_blitProgram->setUniformValue("imageTexture", GLuint(0));

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glBindTexture(GL_TEXTURE_2D, texInfo->texture);

    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, oldWidth, oldHeight);

    ctx->functions()->glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                                GL_RENDERBUFFER, 0);
    glDeleteTextures(1, &tmp_texture);
    glDeleteTextures(1, &oldTexture);

    ctx->functions()->glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // restore render states
    if (stencilTestEnabled)
        glEnable(GL_STENCIL_TEST);
    if (depthTestEnabled)
        glEnable(GL_DEPTH_TEST);
    if (scissorTestEnabled)
        glEnable(GL_SCISSOR_TEST);
    if (blendEnabled)
        glEnable(GL_BLEND);
    glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
    ctx->functions()->glUseProgram(oldProgram);

    m_blitProgram->disableAttributeArray(int(QT_VERTEX_COORDS_ATTR));
    m_blitProgram->disableAttributeArray(int(QT_TEXTURE_COORDS_ATTR));
}

bool QSGDefaultDistanceFieldGlyphCache::useWorkaroundBrokenFBOReadback() const
{
    static bool set = false;
    static bool useWorkaround = false;
    if (!set) {
        QOpenGLContextPrivate *ctx_p = static_cast<QOpenGLContextPrivate *>(QOpenGLContextPrivate::get(ctx));
        useWorkaround = ctx_p->workaround_brokenFBOReadBack;
        set = true;
    }
    return useWorkaround;
}

int QSGDefaultDistanceFieldGlyphCache::maxTextureSize() const
{
    if (!m_maxTextureSize)
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &m_maxTextureSize);
    return m_maxTextureSize;
}

QT_END_NAMESPACE
