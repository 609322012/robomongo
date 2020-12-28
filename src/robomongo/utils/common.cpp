#include "robomongo/utils/common.h"

#include <QFileInfo>
#include <QSettings>

#include "robomongo/core/events/MongoEvents.h"
#include "robomongo/core/utils/Logger.h"
#include "robomongo/core/EventBus.h"
#include "robomongo/utils/StringOperations.h"

namespace Robomongo
{
    void genericEventErrorHandler(Event *event, const std::string &userFriendlyMessage, EventBus* bus, 
                                  QObject* sender)
    {
        if (!event->isError())
            return;

        std::string errMsg = captilizeFirstChar(event->error().errorMessage());
        LOG_MSG(userFriendlyMessage + " " + errMsg, mongo::logger::LogSeverity::Error());

        if (bus && sender)
            bus->publish(new OperationFailedEvent(sender, errMsg, userFriendlyMessage));
        else
            LOG_MSG("Failed to publish OperationFailedEvent.", mongo::logger::LogSeverity::Error());
    }

    bool fileExists(const QString &filePath)
    {
        QFileInfo fileInfo(filePath);
        return fileInfo.exists() && fileInfo.isFile();
    }

    template<typename T>
    bool vectorContains(std::vector<T> const& vec, T const& value) {
        return find(vec.cbegin(), vec.cend(), value) != vec.cend();
    }

    QVariant getSetting(QString const& key) {
        return QSettings("3T", "Robomongo").value(key).toSize();
    }

    void saveSetting(QString const& key, QVariant const& value) {
        QSettings("3T", "Robomongo").setValue(key, value);
    }

}   // end of name space Robomongo
