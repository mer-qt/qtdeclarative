/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
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
** $QT_END_LICENSE$
**
****************************************************************************/
#include "model.h"

#include <QtCore/QUrl>
#include <QtCore/QDate>
#include <QtCore/QList>
#include <QtCore/QStringList>
#include <QtCore/QDebug>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

StockModel::StockModel(QObject *parent)
    : QAbstractListModel(parent)
    , _startDate(QDate(1995, 4, 25))
    , _endDate(QDate::currentDate())
    , _dataCycle(StockModel::Daily)
    , _manager(0)
    , _updating(false)
{
    QHash<int, QByteArray> roles;
    roles[StockModel::DateRole] = "date";
    roles[StockModel::SectionRole] = "year";
    roles[StockModel::OpenPriceRole] = "openPrice";
    roles[StockModel::ClosePriceRole] = "closePrice";
    roles[StockModel::HighPriceRole] = "highPrice";
    roles[StockModel::LowPriceRole] = "lowPrice";
    roles[StockModel::VolumeRole] = "volume";
    roles[StockModel::AdjustedPriceRole] = "adjustedPrice";
    setRoleNames(roles);

    connect(this, SIGNAL(stockNameChanged()), SLOT(requestData()));
    connect(this, SIGNAL(startDateChanged()), SLOT(requestData()));
    connect(this, SIGNAL(endDateChanged()), SLOT(requestData()));
    connect(this, SIGNAL(dataCycleChanged()), SLOT(requestData()));

    _manager = new QNetworkAccessManager(this);
    connect(_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(update(QNetworkReply*)));

}

int StockModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent);
    return _prices.count();
}

void StockModel::requestData()
{
    if (!_updating) {
        _updating = true;
        QMetaObject::invokeMethod(this, "doRequest", Qt::QueuedConnection);
    }
}

void StockModel::doRequest()
{
    /*
        Fetch stock data from yahoo finance:
         url: http://ichart.finance.yahoo.com/table.csv?s=NOK&a=5&b=11&c=2010&d=7&e=23&f=2010&g=d&ignore=.csv
            s:stock name/id, a:start day, b:start month, c:start year  default: 25 April 1995, oldest c= 1962
            d:end day, e:end month, f:end year, default:today  (data only available 3 days before today)
            g:data cycle(d daily,  w weekly, m monthly, v Dividend)
      */
    if (_manager && !_stockName.isEmpty() && _endDate > _startDate) {
        QString query("http://ichart.finance.yahoo.com/table.csv?s=%1&a=%2&b=%3&c=%4&d=%5&e=%6&f=%7&g=%8&ignore=.csv");
        query = query.arg(_stockName)
                     .arg(_startDate.day()).arg(_startDate.month()).arg(_startDate.year())
                     .arg(_endDate.day()).arg(_endDate.month()).arg(_endDate.year())
                     .arg(dataCycleString());

        qDebug() << "request stock data:" << query;
         _manager->get(QNetworkRequest(QUrl(query)));
    }
}

void StockModel::update(QNetworkReply *reply)
{
    _updating = false;

    if (reply) {
         if (reply->error() == QNetworkReply::NoError) {
            beginResetModel();
            _prices.clear();

            while (!reply->atEnd()) {
                QString line = reply->readLine();
                QStringList fields = line.split(',');

                //data format:Date,Open,High,Low,Close,Volume,Adjusted close price
                //example: 2011-06-24,6.03,6.04,5.88,5.88,20465200,5.88
                if (fields.size() == 7) {
                    StockPrice price;
                    price.date = QDate::fromString(fields[0], Qt::ISODate);
                    price.openPrice = fields[1].toFloat();
                    price.highPrice = fields[2].toFloat();
                    price.lowPrice = fields[3].toFloat();
                    price.closePrice = fields[4].toFloat();
                    price.volume = fields[5].toInt();
                    price.adjustedPrice = fields[6].toFloat();
                    _prices.prepend(price);
                }
            }
            qDebug() << "get stock data successfully, total:" << _prices.count() << "records.";
         } else {
            qDebug() << "get stock data failed:" << reply->errorString();
         }
         reply->deleteLater();
         endResetModel();
    }
}

QVariant StockModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() > _prices.count())
        return QVariant();

    const StockPrice &price = _prices[index.row()];
    if (role == StockModel::DateRole)
        return price.date;
    else if (role == StockModel::OpenPriceRole)
        return price.openPrice;
    else if (role == StockModel::ClosePriceRole)
        return price.closePrice;
    else if (role == StockModel::HighPriceRole)
        return price.highPrice;
    else if (role == StockModel::LowPriceRole)
        return price.lowPrice;
    else if (role == StockModel::AdjustedPriceRole)
        return price.adjustedPrice;
    else if (role == StockModel::VolumeRole)
        return price.volume;
    else if (role == StockModel::SectionRole)
        return price.date.year();
    return QVariant();
}

QString StockModel::stockName() const
{
    return _stockName;
}
void StockModel::setStockName(const QString& name)
{
    if (_stockName != name) {
        _stockName = name;
        emit stockNameChanged();
    }
}

QDate StockModel::startDate() const
{
    return _startDate;
}
void StockModel::setStartDate(const QDate& date)
{
    if (_startDate.isValid() && _startDate != date) {
        _startDate = date;
        emit startDateChanged();
    }
}

QDate StockModel::endDate() const
{
    return _endDate;
}
void StockModel::setEndDate(const QDate& date)
{
    if (_endDate.isValid() && _endDate != date) {
        _endDate = date;
        emit endDateChanged();
    }
}

StockModel::StockDataCycle StockModel::dataCycle() const
{
    return _dataCycle;
}

QString StockModel::dataCycleString() const
{
    switch (_dataCycle) {
    case StockModel::Daily:
        return QString('d');
        break;
    case StockModel::Weekly:
        return QString('w');
    case StockModel::Monthly:
        return QString('m');
    case StockModel::Dividend:
        return QString('v');
    }

    return QString('d');
}


void StockModel::setDataCycle(StockModel::StockDataCycle cycle)
{
    if (_dataCycle != cycle) {
        _dataCycle = cycle;
        emit dataCycleChanged();
    }
}
