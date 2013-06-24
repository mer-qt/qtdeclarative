/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtQml module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QV4UNWINDHELPER_PDW2_H
#define QV4UNWINDHELPER_PDW2_H

#include "qv4unwindhelper_p.h"
#include "qv4functionobject_p.h"
#include "qv4function_p.h"
#include <wtf/Platform.h>
#include <wtf/PageAllocation.h>
#include <ExecutableAllocator.h>

#include <QMap>
#include <QMutex>

#define __USE_GNU
#include <dlfcn.h>

namespace QV4 {

namespace {
#if CPU(X86_64)
// Generated by fdegen
static const unsigned char cie_fde_data[] = {
    0x10, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x1, 0x0, 0x8, 0x78, 0x10, 0xc, 0x7, 0x8,
    0x90, 0x1, 0x0, 0x0, 0x20, 0x0, 0x0, 0x0,
    0x18, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x41, 0x13, 0x7e, 0x86,
    0x2, 0x43, 0xd, 0x6, 0x8c, 0x3, 0x8e, 0x4,
    0x0, 0x0, 0x0, 0x0
};
static const int fde_offset = 20;
static const int initial_location_offset = 28;
static const int address_range_offset = 36;
#elif CPU(X86) && OS(LINUX)
static const unsigned char cie_fde_data[] = {
    0x10, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x1, 0x0, 0x4, 0x7c, 0x8, 0xc, 0x4, 0x4,
    0x88, 0x1, 0x0, 0x0, 0x20, 0x0, 0x0, 0x0,
    0x18, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x41, 0x13, 0x7e, 0x85,
    0x2, 0x43, 0xd, 0x5, 0x86, 0x3, 0x87, 0x4,
    0x0, 0x0, 0x0, 0x0,
};
static const int fde_offset = 20;
static const int initial_location_offset = 28;
static const int address_range_offset = 32;
#endif

void writeIntPtrValue(unsigned char *addr, intptr_t val)
{
    addr[0] = (val >>  0) & 0xff;
    addr[1] = (val >>  8) & 0xff;
    addr[2] = (val >> 16) & 0xff;
    addr[3] = (val >> 24) & 0xff;
#if QT_POINTER_SIZE == 8
    addr[4] = (val >> 32) & 0xff;
    addr[5] = (val >> 40) & 0xff;
    addr[6] = (val >> 48) & 0xff;
    addr[7] = (val >> 56) & 0xff;
#endif
}
} // anonymous namespace

extern "C" void __register_frame(void *fde);
extern "C" void __deregister_frame(void *fde);

struct UnwindInfo : public ExecutableAllocator::PlatformUnwindInfo
{
    UnwindInfo(const QByteArray &cieFde);
    virtual ~UnwindInfo();
    QByteArray data;
};

UnwindInfo::UnwindInfo(const QByteArray &cieFde)
    : data(cieFde)
{
    __register_frame(data.data() + fde_offset);
}

UnwindInfo::~UnwindInfo()
{
    __deregister_frame(data.data() + fde_offset);
}

static void ensureUnwindInfo(Function *f)
{
    if (!f->codeRef)
        return; // Not a JIT generated function

    JSC::ExecutableMemoryHandle *handle = f->codeRef.executableMemory();
    if (!handle)
        return;
    ExecutableAllocator::ChunkOfPages *chunk = handle->chunk();

    // Already registered?
    if (chunk->unwindInfo)
        return;

    QByteArray info;
    info.resize(sizeof(cie_fde_data));

    unsigned char *cie_and_fde = reinterpret_cast<unsigned char *>(info.data());
    memcpy(cie_and_fde, cie_fde_data, sizeof(cie_fde_data));

    intptr_t ptr = static_cast<char *>(chunk->pages->base()) - static_cast<char *>(0);
    writeIntPtrValue(cie_and_fde + initial_location_offset, ptr);

    writeIntPtrValue(cie_and_fde + address_range_offset, chunk->pages->size());

    chunk->unwindInfo = new UnwindInfo(info);
}

void UnwindHelper::prepareForUnwind(ExecutionContext *context)
{
    for (ExecutionContext *ctx = context; ctx; ctx = ctx->parent) {
        if (CallContext *callCtx = ctx->asCallContext())
            if (FunctionObject *fobj = callCtx->function)
                if (Function *fun = fobj->function)
                    ensureUnwindInfo(fun);
        for (ExecutionContext::EvalCode *code = ctx->currentEvalCode;
             code; code = code->next)
            ensureUnwindInfo(code->function);
    }

    if (context->engine->globalCode)
        ensureUnwindInfo(context->engine->globalCode);
}

void UnwindHelper::registerFunction(Function *)
{
}

void UnwindHelper::registerFunctions(const QVector<Function *>&)
{
}

void UnwindHelper::deregisterFunction(Function *)
{
}

void UnwindHelper::deregisterFunctions(const QVector<Function *> &)
{
}

}

#endif // QV4UNWINDHELPER_PDW2_H
