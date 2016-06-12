#include <QtQml>
#include <QtQml/QQmlContext>

#include "bible.h"
#include "biblechapter.h"
#include "biblemanager.h"
// #include "searchtask.h"
#include "progress.h"

class BibleRegisterHelper {

public:
    BibleRegisterHelper(const char *uri) {
        // @uri uBible
        qmlRegisterType<BibleChapter>(uri, 0, 1, "BibleChapter");
        // qmlRegisterType<SearchTask>(uri, 1, 0, "SearchTask");
        qmlRegisterSingletonType<BibleManager>(uri, 0, 1, "BibleManager", BibleManager::qmlSingleton);
        qmlRegisterType<Bible>(uri, 0, 1, "Bible");
        qmlRegisterUncreatableType<Progress>(uri, 0, 1, "Progress", "Will be returned by Bible methods");
    }
};

// When using QPM, the C++ plugin is not used and the QML types must be registered manually
#ifdef QPM_INIT
    static BibleRegisterHelper registerHelper("Bible");
#endif
