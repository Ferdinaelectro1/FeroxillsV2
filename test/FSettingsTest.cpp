//
// Created by ferdinand on 14/03/2026.
//

#include <qobject.h>
#include "../src/core/FSettings.h"
#include <QTest>

class FSettingsTest final : public  QObject {
    Q_OBJECT
private slots:
    void testSettings() {
        FSettings::instance()->setCh1VoltDiv(4);
        QCOMPARE(4,FSettings::instance()->getCh1VoltDiv());
        FSettings::instance()->setTimeDiv(0.001);
        QCOMPARE(0.001,FSettings::instance()->getTimeDiv());
    }

};

QTEST_MAIN(FSettingsTest)
#include "FSettingsTest.moc"