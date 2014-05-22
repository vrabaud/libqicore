#include <string>

#include <qi/types.hpp>

#include <qitype/anyobject.hpp>

#include <qicore/logmessage.hpp>
#include <qicore/logmanager.hpp>
#include <qicore/loglistener.hpp>

namespace qi
{
  class LogManagerProxy : public qi::Proxy, public LogManager
  {
  public:
    LogManagerProxy(qi::AnyObject obj)
      : qi::Proxy(obj)
    {
    }

    void log(const LogMessage& p0)
    {
      _obj.call<void>("log", p0);
    }

    LogListenerPtr getListener()
    {
      return _obj.call<LogListenerPtr>("getListener");
    }

    int addProvider(Object<LogProvider> p0)
    {
      return _obj.call<int>("addProvider", p0);
    }

    void removeProvider(int p0)
    {
      _obj.call<void>("removeProvider", p0);
    }
  };

  QI_REGISTER_PROXY_INTERFACE(LogManagerProxy, LogManager);
} // !qi