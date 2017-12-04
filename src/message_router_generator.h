/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2017 jwellbelove

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

/*[[[cog
import cog
cog.outl("#if 0")
]]]*/
/*[[[end]]]*/
#error THIS HEADER IS A GENERATOR. DO NOT INCLUDE.
/*[[[cog
import cog
cog.outl("#endif")
]]]*/
/*[[[end]]]*/

/*[[[cog
import cog
cog.outl("//***************************************************************************")
cog.outl("// This file has been auto generated. Do not edit this file.")
cog.outl("//***************************************************************************")
]]]*/
/*[[[end]]]*/

//***************************************************************************
// To generate to header file, run this at the command line.
// Note: You will need Python and COG installed.
//
// python -m cogapp -d -e -omessage_router.h -DHandlers=<n> message_router_generator.h 
// Where <n> is the number of messages to support.
//
// e.g.
// To generate handlers for up to 16 messages...
// python -m cogapp -d -e -omessage_router.h -DHandlers=16 message_router_generator.h 
//
// See generate.bat
//***************************************************************************

#ifndef __ETL_MESSAGE_ROUTER__
#define __ETL_MESSAGE_ROUTER__

#include <stdint.h>

#include "platform.h"
#include "message.h"
#include "message_types.h"
#include "alignment.h"
#include "error_handler.h"
#include "exception.h"
#include "largest.h"
#include "nullptr.h"

#undef ETL_FILE
#define ETL_FILE "35"

namespace etl
{
  //***************************************************************************
  /// Base exception class for message router
  //***************************************************************************
  class message_router_exception : public etl::exception
  {
  public:

    message_router_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : etl::exception(reason_, file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// Router id is out of the legal range.
  //***************************************************************************
  class message_router_illegal_id : public etl::message_router_exception
  {
  public:

    message_router_illegal_id(string_type file_name_, numeric_type line_number_)
      : message_router_exception(ETL_ERROR_TEXT("message router:illegal id", ETL_FILE"A"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  class imessage_router
  {
  public:

    virtual ~imessage_router() {}
    virtual void receive(const etl::imessage& message) = 0;
    virtual void receive(imessage_router& source, const etl::imessage& message) = 0;
    virtual bool accepts(etl::message_id_t id) const = 0;

    //********************************************
    bool accepts(const etl::imessage& msg) const
    {
      return accepts(msg.message_id);
    }

    //********************************************
    etl::message_router_id_t get_message_router_id() const
    {
      return message_router_id;
    }

    //********************************************
    bool is_null_router() const
    {
      return (message_router_id == NULL_MESSAGE_ROUTER);
    }

    //********************************************
    bool is_bus() const
    {
      return (message_router_id == MESSAGE_BUS);
    }

    //********************************************
    void set_successor(imessage_router& successor_)
    {
      successor = &successor_;
    }

    //********************************************
    imessage_router& get_successor() const
    {
      return *successor;
    }

    //********************************************
    bool has_successor() const
    {
      return (has_successor());
    }

    enum
    {
      NULL_MESSAGE_ROUTER = 255,
      MESSAGE_BUS         = 254,
      ALL_MESSAGE_ROUTERS = 253,
      MAX_MESSAGE_ROUTER  = 249
    };

  protected:

    imessage_router(etl::message_router_id_t id_)
      : successor(nullptr),
        message_router_id(id_)
    {
    }

    imessage_router(etl::message_router_id_t id_, 
                    imessage_router&         successor_)
      : successor(&successor_),
        message_router_id(id_)        
    {
    }

  private:

    // Disabled.
    imessage_router(const imessage_router&);
    imessage_router& operator =(const imessage_router&);

    etl::imessage_router* successor;

    etl::message_router_id_t  message_router_id;
  };

  //***************************************************************************
  /// This router can be used either as a sink for messages
  /// or as a producer-only of messages such an interrupt routine.
  //***************************************************************************
  class null_message_router : public imessage_router
  {
  public:

    null_message_router()
      : imessage_router(imessage_router::NULL_MESSAGE_ROUTER)
    {
    }

    //********************************************
    void receive(const etl::imessage&)
    {
    }

    //********************************************
    void receive(etl::imessage_router&, const etl::imessage&)
    {
    }

    //********************************************
    bool accepts(etl::message_id_t) const
    {
      return false;
    }

    //********************************************
    static null_message_router& instance()
    {
      static null_message_router nmr;
      return nmr;
    }
  };

  //***************************************************************************
  /// Send a message to a router.
  /// Sets the 'sender' to etl::null_message_router type.
  //***************************************************************************
  inline static void send_message(etl::imessage_router& destination, 
                                  const etl::imessage&  message)
  {
    destination.receive(message);
  }

  //***************************************************************************
  /// Send a message to a router.
  //***************************************************************************
  inline static void send_message(etl::imessage_router& source, 
                                  etl::imessage_router& destination, 
                                  const etl::imessage&  message)
  {
    destination.receive(source, message);
  }
  
  /*[[[cog
      import cog
      ################################################
      # The first definition for all of the messages.
      ################################################
      cog.outl("//***************************************************************************")
      cog.outl("// The definition for all %s message types." % Handlers)
      cog.outl("//***************************************************************************")
      cog.outl("template <typename TDerived,")
      cog.out("          ")
      cog.out("typename T1, ")
      for n in range(2, int(Handlers)):
          cog.out("typename T%s = void, " % n)
          if n % 4 == 0:
              cog.outl("")
              cog.out("          ")
      cog.outl("typename T%s = void>" % int(Handlers))
      cog.out("class message_router")
      cog.outl("  : public imessage_router")
      cog.outl("{")
      cog.outl("public:")
      cog.outl("")
      cog.outl("  //**********************************************")
      cog.outl("  class message_packet")
      cog.outl("  {")
      cog.outl("  public:")
      cog.outl("")
      cog.outl("    //********************************************")
      cog.outl("    explicit message_packet(const etl::imessage& msg)")
      cog.outl("    {")
      cog.outl("      const size_t id = msg.message_id;")
      cog.outl("")
      cog.outl("      void* p = data;")
      cog.outl("")
      cog.outl("      switch (id)")
      cog.outl("      {")
      for n in range(1, int(Handlers) + 1):
          cog.outl("        case T%s::ID: ::new (p) T%s(static_cast<const T%s&>(msg)); break;" % (n, n, n))
      cog.outl("        default: ETL_ASSERT(false, ETL_ERROR(unhandled_message_exception)); break;")
      cog.outl("      }")
      cog.outl("    }")
      cog.outl("")      
      cog.outl("    //********************************************")
      cog.outl("    template <typename T>")
      cog.outl("    explicit message_packet(const T& msg)")
      cog.outl("    {")
      cog.out("      STATIC_ASSERT((etl::is_one_of<T, ")
      for n in range(1, int(Handlers)):
          cog.out("T%s, " % n)
          if n % 16 == 0:
              cog.outl("")
              cog.out("                                       ")
      cog.outl("""T%s>::value), "Unsupported type for this message packet");""" % int(Handlers))
      cog.outl("")
      cog.outl("      void* p = data;")
      cog.outl("      ::new (p) T(static_cast<const T&>(msg));")
      cog.outl("    }")
      cog.outl("")
      cog.outl("    //********************************************")
      cog.outl("    ~message_packet()")
      cog.outl("    {")
      cog.outl("      static_cast<etl::imessage*>(data)->~imessage();")
      cog.outl("    }")
      cog.outl("")
      cog.outl("    //********************************************")
      cog.outl("    etl::imessage& get()")
      cog.outl("    {")
      cog.outl("      return *static_cast<etl::imessage*>(data);")
      cog.outl("    }")
      cog.outl("")
      cog.outl("    //********************************************")
      cog.outl("    const etl::imessage& get() const")
      cog.outl("    {")
      cog.outl("      return *static_cast<const etl::imessage*>(data);")
      cog.outl("    }")
      cog.outl("")
      cog.outl("    enum")
      cog.outl("    {")
      cog.out("      SIZE      = etl::largest<")
      for n in range(1, int(Handlers)):
          cog.out("T%s, " % n)
      cog.outl("T%s>::size," % int(Handlers))
      cog.out("      ALIGNMENT = etl::largest<")
      for n in range(1, int(Handlers)):
          cog.out("T%s, " % n)
      cog.outl("T%s>::alignment" % int(Handlers))
      cog.outl("    };")
      cog.outl("")
      cog.outl("  private:")
      cog.outl("")
      cog.outl("    typename etl::aligned_storage<SIZE, ALIGNMENT>::type data;")
      cog.outl("  };")
      cog.outl("")
      cog.outl("  //**********************************************")
      cog.outl("  message_router(etl::message_router_id_t id_)")
      cog.outl("    : imessage_router(id_)")
      cog.outl("  {")
      cog.outl("    ETL_ASSERT(id_ <= etl::imessage_router::MAX_MESSAGE_ROUTER, ETL_ERROR(etl::message_router_illegal_id));")
      cog.outl("  }")
      cog.outl("")
      cog.outl("  //**********************************************")
      cog.outl("  message_router(etl::message_router_id_t id_, etl::imessage_router& successor_)")
      cog.outl("    : imessage_router(id_, successor_)")
      cog.outl("  {")
      cog.outl("    ETL_ASSERT(id_ <= etl::imessage_router::MAX_MESSAGE_ROUTER, ETL_ERROR(etl::message_router_illegal_id));")
      cog.outl("  }")
      cog.outl("")
      cog.outl("  //**********************************************")
      cog.outl("  void receive(const etl::imessage& msg)")
      cog.outl("  {")
      cog.outl("    receive(etl::null_message_router::instance(), msg);")
      cog.outl("  }")
      cog.outl("")
      cog.outl("  //**********************************************")
      cog.outl("  void receive(etl::imessage_router& source, const etl::imessage& msg)")
      cog.outl("  {")
      cog.outl("    const etl::message_id_t id = msg.message_id;")
      cog.outl("")
      cog.outl("    switch (id)")
      cog.outl("    {")
      for n in range(1, int(Handlers) + 1):
          cog.out("      case T%d::ID:" % n)
          cog.out(" static_cast<TDerived*>(this)->on_receive(source, static_cast<const T%d&>(msg));" % n)
          cog.outl(" break;")
      cog.outl("      default:")
      cog.outl("      {")
      cog.outl("         if (has_successor())")
      cog.outl("         {")
      cog.outl("           get_successor().receive(source, msg);")
      cog.outl("         }")
      cog.outl("         else")
      cog.outl("         {")
      cog.outl("           static_cast<TDerived*>(this)->on_receive_unknown(source, msg);")
      cog.outl("         }")
      cog.outl("         break;")
      cog.outl("      }")
      cog.outl("    }")
      cog.outl("  }")
      cog.outl("")
      cog.outl("  using imessage_router::accepts;")
      cog.outl("")
      cog.outl("  //**********************************************")
      cog.outl("  bool accepts(etl::message_id_t id) const")
      cog.outl("  {")
      cog.outl("    switch (id)")
      cog.outl("    {")
      cog.out("      ")
      for n in range(1, int(Handlers) + 1):
          cog.out("case T%d::ID: " % n)
          if n % 8 == 0:
              cog.outl("")
              cog.out("      ")
      cog.outl("  return true; break;")
      cog.outl("      default:")
      cog.outl("        return false; break;")
      cog.outl("    }")
      cog.outl("  }")
      cog.outl("};")

      ####################################
      # All of the other specialisations.
      ####################################
      for n in range(int(Handlers) - 1, 0, -1):
          cog.outl("")
          cog.outl("//***************************************************************************")
          if n == 1:
              cog.outl("// Specialisation for %d message type." % n)
          else:
              cog.outl("// Specialisation for %d message types." % n)
          cog.outl("//***************************************************************************")
          cog.outl("template <typename TDerived, ")
          cog.out("          ")
          for t in range(1, n):
              cog.out("typename T%d, " % t)
              if t % 4 == 0:
                  cog.outl("")
                  cog.out("          ")
          cog.outl("typename T%d>" % n)
          cog.out("class message_router<TDerived, ")
          for t in range(1, n + 1):
              cog.out("T%d, " % t)
              if t % 16 == 0:
                  cog.outl("")
                  cog.out("               ")
          for t in range(n + 1, int(Handlers)):
              cog.out("void, ")
              if t % 16 == 0:
                  cog.outl("")
                  cog.out("               ")
          cog.outl("void>")
          cog.outl(" : public imessage_router")
          cog.outl("{")
          cog.outl("public:")
          cog.outl("")
          cog.outl("  //**********************************************")
          cog.outl("  class message_packet")
          cog.outl("  {")
          cog.outl("  public:")
          cog.outl("")
          cog.outl("    //********************************************")
          cog.outl("    explicit message_packet(const etl::imessage& msg)")
          cog.outl("    {")
          cog.outl("      const size_t id = msg.message_id;")
          cog.outl("")
          cog.outl("      void* p = data;")
          cog.outl("")
          cog.outl("      switch (id)")
          cog.outl("      {")
          for t in range(1, n + 1):
              cog.outl("        case T%s::ID: ::new (p) T%s(static_cast<const T%s&>(msg)); break;" % (t, t, t))
          cog.outl("        default: ETL_ASSERT(false, ETL_ERROR(unhandled_message_exception)); break;")
          cog.outl("      }")
          cog.outl("    }")
          cog.outl("")
          cog.outl("    //********************************************")
          cog.outl("    template <typename T>")
          cog.outl("    explicit message_packet(const T& msg)")
          cog.outl("    {")
          cog.out("      STATIC_ASSERT((etl::is_one_of<T, ")
          for t in range(1, n):
              cog.out("T%s, " % t)
              if t % 16 == 0:
                  cog.outl("")
                  cog.out("                                       ")
          cog.outl("""T%s>::value), "Unsupported type for this message packet");""" % n)
          cog.outl("")
          cog.outl("      void* p = data;")
          cog.outl("      ::new (p) T(static_cast<const T&>(msg));")
          cog.outl("    }")
          cog.outl("")
          cog.outl("    //********************************************")
          cog.outl("    ~message_packet()")
          cog.outl("    {")
          cog.outl("      static_cast<etl::imessage*>(data)->~imessage();")
          cog.outl("    }")
          cog.outl("")
          cog.outl("    //********************************************")
          cog.outl("    etl::imessage& get()")
          cog.outl("    {")
          cog.outl("      return *static_cast<etl::imessage*>(data);")
          cog.outl("    }")
          cog.outl("")
          cog.outl("    //********************************************")
          cog.outl("    const etl::imessage& get() const")
          cog.outl("    {")
          cog.outl("      return *static_cast<const etl::imessage*>(data);")
          cog.outl("    }")
          cog.outl("")
          cog.outl("    enum")
          cog.outl("    {")
          cog.out("      SIZE      = etl::largest<")
          for t in range(1, n):
              cog.out("T%s, " % t)
          cog.outl("T%s>::size," % n)
          cog.out("      ALIGNMENT = etl::largest<")
          for t in range(1, n):
              cog.out("T%s, " % t)
          cog.outl("T%s>::alignment" % n)
          cog.outl("    };")
          cog.outl("")
          cog.outl("  private:")
          cog.outl("")
          cog.outl("    typename etl::aligned_storage<SIZE, ALIGNMENT>::type data;")
          cog.outl("  };")
          cog.outl("")
          cog.outl("  //**********************************************")
          cog.outl("  message_router(etl::message_router_id_t id_)")
          cog.outl("    : imessage_router(id_)")
          cog.outl("  {")
          cog.outl("    ETL_ASSERT(id_ <= etl::imessage_router::MAX_MESSAGE_ROUTER, ETL_ERROR(etl::message_router_illegal_id));")
          cog.outl("  }")
          cog.outl("")
          cog.outl("  //**********************************************")
          cog.outl("  message_router(etl::message_router_id_t id_, etl::imessage_router& successor_)")
          cog.outl("    : imessage_router(id_, successor_)")
          cog.outl("  {")
          cog.outl("    ETL_ASSERT(id_ <= etl::imessage_router::MAX_MESSAGE_ROUTER, ETL_ERROR(etl::message_router_illegal_id));")
          cog.outl("  }")
          cog.outl("")
          cog.outl("  //**********************************************")
          cog.outl("  void receive(const etl::imessage& msg)")
          cog.outl("  {")
          cog.outl("    receive(etl::null_message_router::instance(), msg);")
          cog.outl("  }")
          cog.outl("")
          cog.outl("  //**********************************************")
          cog.outl("  void receive(etl::imessage_router& source, const etl::imessage& msg)")
          cog.outl("  {")
          cog.outl("    const size_t id = msg.message_id;")
          cog.outl("")
          cog.outl("    switch (id)")
          cog.outl("    {")
          for t in range(1, n + 1):
              cog.out("      case T%d::ID:" % t)
              cog.out(" static_cast<TDerived*>(this)->on_receive(source, static_cast<const T%d&>(msg));" % t)
              cog.outl(" break;")
          cog.outl("      default:")
          cog.outl("      {")
          cog.outl("         if (has_successor())")
          cog.outl("         {")
          cog.outl("           get_successor().receive(source, msg);")
          cog.outl("         }")
          cog.outl("         else")
          cog.outl("         {")
          cog.outl("           static_cast<TDerived*>(this)->on_receive_unknown(source, msg);")
          cog.outl("         }")
          cog.outl("         break;")
          cog.outl("      }")
          cog.outl("    }")
          cog.outl("  }")
          cog.outl("")
          cog.outl("  using imessage_router::accepts;")
          cog.outl("")
          cog.outl("  //**********************************************")
          cog.outl("  bool accepts(etl::message_id_t id) const")
          cog.outl("  {")
          cog.outl("    switch (id)")
          cog.outl("    {")
          cog.out("      ")
          for t in range(1, n + 1):
              cog.out("case T%d::ID: " % t)
              if t % 8 == 0:
                  cog.outl("")
                  cog.out("      ")
          cog.outl("")
          cog.outl("        return true; break;")
          cog.outl("      default:")
          cog.outl("        return false; break;")
          cog.outl("    }")
          cog.outl("  }")
          cog.outl("};")
  ]]]*/
  /*[[[end]]]*/
}

#undef ETL_FILE

#endif
