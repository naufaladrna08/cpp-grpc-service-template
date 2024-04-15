#ifndef AUTH_HPP
#define AUTH_HPP

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include "auth.grpc.pb.h"

/* MongoDB */
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>

#include <jwt-cpp/jwt.h>
#include <bcrypt.h>

class AuthImpl: public Auth::Service {
  public:
    AuthImpl(mongocxx::client* client);
    grpc::Status HelloWorld(grpc::ServerContext* context, const HelloWorldRequest* request, HelloWorldResponse* response) override;
    grpc::Status Login(grpc::ServerContext* context, const LoginRequest* request, LoginResponse* response) override;
    grpc::Status Register(grpc::ServerContext* context, const RegisterRequest* request, RegisterResponse* response) override;

  private:
    mongocxx::client* m_client;
};

#endif