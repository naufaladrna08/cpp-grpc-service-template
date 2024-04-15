
#ifndef AUTH_INTERCEPTOR_HPP
#define AUTH_INTERCEPTOR_HPP

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include "auth.grpc.pb.h"

using grpc::experimental::InterceptorBatchMethods;
using grpc::experimental::InterceptionHookPoints;

class AuthInterceptor : public grpc::experimental::Interceptor {
  public:
    void Intercept(InterceptorBatchMethods* methods) override;
};

class AuthInterceptorFactory : public grpc::experimental::ServerInterceptorFactoryInterface {
  public:
    grpc::experimental::Interceptor* CreateServerInterceptor(grpc::experimental::ServerRpcInfo* info) override;
};

#endif