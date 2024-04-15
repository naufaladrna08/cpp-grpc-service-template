#include <authInterceptor.hpp>

void AuthInterceptor::Intercept(InterceptorBatchMethods* methods) {
  if (methods->QueryInterceptionHookPoint(InterceptionHookPoints::PRE_SEND_INITIAL_METADATA)) {
    auto* metadata = methods->GetSendInitialMetadata();
    metadata->insert(std::make_pair("authorization", "Bearer 123456"));

    std::cout << "Added authorization header" << std::endl;
  }

  methods->Proceed();
}

grpc::experimental::Interceptor* AuthInterceptorFactory::CreateServerInterceptor(grpc::experimental::ServerRpcInfo* info) {
  return new AuthInterceptor();
}