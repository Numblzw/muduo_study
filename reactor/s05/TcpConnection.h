//
// Created by jxq on 19-8-29.
//

#ifndef MYMUDUO_TCPCONNECTION_H
#define MYMUDUO_TCPCONNECTION_H

#include <boost/core/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/scoped_ptr.hpp>
#include "../s04/InetAddress.h"
#include "Callbacks.h"
#include "../s00/EventLoop.h"
#include "../s04/Socket.h"
#include "../s01/Channel.h"

namespace muduo
{

class Channel;
class EventLoop;
class Socket;

///
/// TCP connection, for both client and server usage.
///
class TcpConnection : boost::noncopyable,
                      public boost::enable_shared_from_this<TcpConnection>
{
public:
    /// Constructs a TcpConnection with a connected sockfd
    ///
    /// User should not create this object.
    TcpConnection(EventLoop* loop,
            const std::string& name,
            int sockfd,
            const InetAddress& localAddr,
            const InetAddress& peerAddr);

    ~TcpConnection();

    EventLoop* getLoop() const
    {
        return loop_;
    }

    const std::string& name() const
    {
        return name_;
    }

    const InetAddress& localAddress()
    {
        return localAddr_;
    }

    const InetAddress& peerAddress()
    {
        return peerAddr_;
    }

    bool connected() const
    {
        return state_ == kConnected;
    }

    void setConnectionCallback(const ConnectionCallback& cb)
    {
        connectionCallback_ = cb;
    }

    void setMessageCallback(const MessageCallback& cb)
    {
        messageCallback_ = cb;
    }

    /// Internal use only.

    // called when TcpServer accepts a new connection
    void connectEstablished();   // should be called only once

private:
    enum StateE {kConnecting, kConnected, };

    void setState(StateE s)
    {
        state_ = s;
    }

    void handleRead();

    EventLoop* loop_;
    std::string name_;
    StateE state_;  // FIXME: use atomic variable
    // we don't expose those classes to client
    boost::scoped_ptr<Socket> socket_;
    boost::scoped_ptr<Channel> channel_;
    InetAddress localAddr_;
    InetAddress peerAddr_;
    ConnectionCallback connectionCallback_;
    MessageCallback messageCallback_;

};

typedef boost::shared_ptr<TcpConnection> TcpConnectionPtr;

}


#endif //MYMUDUO_TCPCONNECTION_H